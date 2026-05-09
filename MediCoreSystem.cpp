#define _CRT_SECURE_NO_WARNINGS
#include "MediCoreSystem.h"
#include "FileHandler.h"
#include "Exceptions.h"
#include "CustomStringHelper.h"
#include "Validator.h"
#include <fstream>
#include <iostream>
#include <ctime>
using namespace std;
MediCoreSystem::MediCoreSystem() {
    currentUserID = 0;
    currentUserRole = 0;
    failedLoginAttempts = 0;
    systemLocked = false;
}

MediCoreSystem::~MediCoreSystem() {
    try {
        saveData();
    }
    catch (...) {
        cout << "\n[SYSTEM NOTE] Data saved safely, but encountered a locked file on exit." << endl;
    }
}

void MediCoreSystem::loadData() {
    try {
        FileHandler::loadPatients("patients.txt", patients);
        FileHandler::loadDoctors("doctors.txt", doctors);
        FileHandler::loadAdmins("admin.txt", admins);
        FileHandler::loadAppointments("appointments.txt", appointments);
        FileHandler::loadBills("bills.txt", bills);
        FileHandler::loadPrescriptions("prescriptions.txt", prescriptions);
    }
    catch (const HospitalException& e) {
        cout << "[WARNING] " << e.what() << " (System will create it on save).\n";
    }
}

void MediCoreSystem::saveData() {
    FileHandler::savePatients("patients.txt", patients);
    FileHandler::saveDoctors("doctors.txt", doctors);
    FileHandler::saveAppointments("appointments.txt", appointments);
    FileHandler::saveBills("bills.txt", bills);
    FileHandler::savePrescriptions("prescriptions.txt", prescriptions);
}

void MediCoreSystem::getTodayDate(char* buffer) {
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    strftime(buffer, 15, "%d-%m-%Y", tm_info);
}

void MediCoreSystem::logSecurityEvent(const char* roleStr, int attemptedID, const char* result) {
    ofstream logFile("security_log.txt", ios::app);
    if (logFile.is_open()) {
        time_t now = time(0);
        char* dt = ctime(&now);
        dt[myStrLen(dt) - 1] = '\0';
        logFile << dt << "," << roleStr << "," << attemptedID << "," << result << "\n";
        logFile.close();
    }
}

bool MediCoreSystem::isLocked() const { return systemLocked; }

void MediCoreSystem::logout() {
    currentUserID = 0;
    currentUserRole = 0;
}

bool MediCoreSystem::login(int role, int id, const char* password) {
    if (systemLocked) throw HospitalException("Account locked. Contact admin.");

    bool success = false;
    const char* roleStr = "";

    if (role == 1) {
        roleStr = "Patient";
        Patient* p = patients.findByID(id);
        if (p != nullptr && myStrCmp(p->getPassword(), password) == 0) success = true;
    }
    else if (role == 2) {
        roleStr = "Doctor";
        Doctor* d = doctors.findByID(id);
        if (d != nullptr && myStrCmp(d->getPassword(), password) == 0) success = true;
    }
    else if (role == 3) {
        roleStr = "Admin";
        Admin* a = admins.findByID(id);
        if (a != nullptr && myStrCmp(a->getPassword(), password) == 0) success = true;
    }

    if (success) {
        currentUserID = id;
        currentUserRole = role;
        failedLoginAttempts = 0;
        logSecurityEvent(roleStr, id, "SUCCESS");
        return true;
    }
    else {
        failedLoginAttempts++;
        logSecurityEvent(roleStr, id, "FAILED");
        if (failedLoginAttempts >= 3) {
            systemLocked = true;
            throw HospitalException("Account locked. Contact admin.");
        }
        return false;
    }
}

// PATIENT LOGIC

void MediCoreSystem::searchDoctorBySpecialization(const char* spec) {
    Doctor* docs = doctors.getAll();
    bool found = false;
    cout << "\n--- Doctors specializing in " << spec << " ---\n";
    for (int i = 0; i < doctors.getSize(); i++) {
        if (myStrEqualsIgnoreCase(docs[i].getSpecialization(), spec)) {
            cout << docs[i] << "\n";
            found = true;
        }
    }
    if (!found) cout << "No doctors available for that specialization.\n";
}

void MediCoreSystem::viewAvailableTimeSlots(int doctorID, const char* date) {
    const char* allSlots[8] = { "09:00", "10:00", "11:00", "12:00", "13:00", "14:00", "15:00", "16:00" };
    cout << "\n--- Available slots for Doctor ID " << doctorID << " on " << date << " ---\n";
    Appointment* appts = appointments.getAll();

    for (int i = 0; i < 8; i++) {
        bool isBooked = false;
        Appointment tempCheck(0, 0, doctorID, date, allSlots[i], "pending");
        for (int j = 0; j < appointments.getSize(); j++) {
            if (appts[j] == tempCheck) {
                isBooked = true;
                break;
            }
        }
        if (!isBooked) cout << "[" << allSlots[i] << "]  ";
    }
    cout << "\n";
}

void MediCoreSystem::bookAppointment(int patientID, int doctorID, const char* date, const char* timeSlot) {
    if (!Validator::isValidDate(date)) throw InvalidInputException("Invalid date.");
    if (!Validator::isValidTimeSlot(timeSlot)) throw InvalidInputException("Invalid time slot.");

    Doctor* d = doctors.findByID(doctorID);
    Patient* p = patients.findByID(patientID);
    if (!d) throw InvalidInputException("Doctor not found.");
    if (!p) throw InvalidInputException("Patient not found.");

    Appointment tempCheck(0, 0, doctorID, date, timeSlot, "pending");
    Appointment* appts = appointments.getAll();
    for (int i = 0; i < appointments.getSize(); i++) {
        if (appts[i] == tempCheck) throw SlotUnavailableException();
    }

    if (p->getBalance() < d->getFee()) throw InsufficientFundsException();

    *p -= d->getFee(); // Deduct fee from patient

    int newApptID = appointments.getSize() > 0 ? appts[appointments.getSize() - 1].getID() + 1 : 1;
    appointments.add(Appointment(newApptID, patientID, doctorID, date, timeSlot, "pending"));

    int newBillID = bills.getSize() > 0 ? bills.getAll()[bills.getSize() - 1].getID() + 1 : 1;
    bills.add(Bill(newBillID, patientID, newApptID, d->getFee(), "unpaid", date));

    cout << "\nAppointment booked successfully. Appointment ID: " << newApptID << "\n";
}

void MediCoreSystem::cancelAppointment(int patientID, int appointmentID) {
    Appointment* appt = appointments.findByID(appointmentID);
    if (!appt || appt->getPatientID() != patientID || myStrCmp(appt->getStatus(), "pending") != 0) {
        throw InvalidInputException("Invalid appointment ID or status.");
    }

    appt->setStatus("cancelled");
    float refund = 0.0f;
    Bill* allBills = bills.getAll();
    for (int i = 0; i < bills.getSize(); i++) {
        if (allBills[i].getAppointmentID() == appointmentID) {
            allBills[i].setStatus("cancelled");
            refund = allBills[i].getAmount();
            break;
        }
    }

    Patient* p = patients.findByID(patientID);
    if (p) *p += refund;
    cout << "\nAppointment cancelled. PKR " << refund << " refunded.\n";
}

void MediCoreSystem::payBill(int patientID, int billID) {
    Bill* b = bills.findByID(billID);
    if (!b || b->getPatientID() != patientID || myStrCmp(b->getStatus(), "unpaid") != 0) {
        throw InvalidInputException("Invalid or already paid bill.");
    }

    Patient* p = patients.findByID(patientID);
    if (p->getBalance() < b->getAmount()) throw InsufficientFundsException();

    *p -= b->getAmount();
    b->setStatus("paid");
    cout << "\nBill paid successfully. Remaining balance: PKR " << p->getBalance() << "\n";
}

void MediCoreSystem::topUpBalance(int patientID, float amount) {
    if (amount <= 0) throw InvalidInputException("Amount must be > 0.");
    Patient* p = patients.findByID(patientID);
    if (p) {
        *p += amount;
        cout << "\nBalance updated. New balance: PKR " << p->getBalance() << "\n";
    }
}

void MediCoreSystem::viewMyAppointments(int patientID) {
    cout << "\n--- My Appointments ---\n";
    Appointment* appts = appointments.getAll();
    bool found = false;
    for (int i = 0; i < appointments.getSize(); i++) {
        if (appts[i].getPatientID() == patientID) {
            cout << appts[i] << "\n";
            found = true;
        }
    }
    if (!found) cout << "No appointments found.\n";
}

void MediCoreSystem::viewMyBills(int patientID) {
    cout << "\n--- My Bills ---\n";
    Bill* b = bills.getAll();
    float owed = 0.0f;
    for (int i = 0; i < bills.getSize(); i++) {
        if (b[i].getPatientID() == patientID) {
            cout << "Bill ID: " << b[i].getID() << " | PKR " << b[i].getAmount()
                << " | Status: " << b[i].getStatus() << " | Date: " << b[i].getDate() << "\n";
            if (myStrCmp(b[i].getStatus(), "unpaid") == 0) owed += b[i].getAmount();
        }
    }
    cout << "Total Outstanding: PKR " << owed << "\n";
}

void MediCoreSystem::viewMyMedicalRecords(int patientID) {
    cout << "\n--- My Medical Records ---\n";
    Prescription* rx = prescriptions.getAll();
    for (int i = 0; i < prescriptions.getSize(); i++) {
        if (rx[i].getPatientID() == patientID) {
            cout << "Date: " << rx[i].getDate() << " | Meds: " << rx[i].getMedicines()
                << " | Notes: " << rx[i].getNotes() << "\n";
        }
    }
}

// DOCTOR LOGIC
void MediCoreSystem::viewTodayAppointments(int doctorID) {
    char today[15];
    getTodayDate(today);

    cout << "\n--- Appointments for Today (" << today << ") ---\n";
    Appointment* appts = appointments.getAll();
    bool found = false;
    for (int i = 0; i < appointments.getSize(); i++) {
        if (appts[i].getDoctorID() == doctorID && myStrCmp(appts[i].getDate(), today) == 0) {
            cout << appts[i] << "\n";
            found = true;
        }
    }
    if (!found) cout << "No appointments scheduled for today.\n";
}

void MediCoreSystem::markAppointmentComplete(int doctorID, int appointmentID) {
    Appointment* appt = appointments.findByID(appointmentID);

    if (!appt || appt->getDoctorID() != doctorID || myStrCmp(appt->getStatus(), "pending") != 0) {
        throw InvalidInputException("Invalid appt ID, not yours, or not pending.");
    }
    appt->setStatus("completed");
    std::cout << "\nAppointment marked as completed.\n";
}

void MediCoreSystem::markAppointmentNoShow(int doctorID, int appointmentID) {
    Appointment* appt = appointments.findByID(appointmentID);
    if (!appt || appt->getDoctorID() != doctorID /*  || myStrCmp(appt->getStatus(), "pending") != 0*/) {
        throw InvalidInputException("Invalid appt ID.");
    }
    appt->setStatus("no-show");

    Bill* bArr = bills.getAll();
    for (int i = 0; i < bills.getSize(); i++) {
        if (bArr[i].getAppointmentID() == appointmentID) {
            bArr[i].setStatus("cancelled");
            break;
        }
    }
    cout << "\nAppointment marked as no-show.\n";
}

void MediCoreSystem::writePrescription(int doctorID, int appointmentID, const char* medicines, const char* notes) {
    Appointment* appt = appointments.findByID(appointmentID);
    if (!appt || appt->getDoctorID() != doctorID || myStrCmp(appt->getStatus(), "completed") != 0) {
        throw InvalidInputException("Appointment must be completed before writing a prescription.");
    }

    Prescription* pArr = prescriptions.getAll();
    for (int i = 0; i < prescriptions.getSize(); i++) {
        if (pArr[i].getAppointmentID() == appointmentID) {
            throw InvalidInputException("Prescription already written for this appointment.");
        }
    }

    int newRxID = prescriptions.getSize() > 0 ? pArr[prescriptions.getSize() - 1].getID() + 1 : 1;
    prescriptions.add(Prescription(newRxID, appointmentID, appt->getPatientID(), doctorID, appt->getDate(), medicines, notes));
    cout << "\nPrescription saved.\n";
}

void MediCoreSystem::viewPatientHistory(int doctorID, int patientID) {
    Patient* p = patients.findByID(patientID);
    if (!p) throw InvalidInputException("Patient not found.");

    bool hasCompleted = false;
    Appointment* appts = appointments.getAll();
    for (int i = 0; i < appointments.getSize(); i++) {
        if (appts[i].getPatientID() == patientID && appts[i].getDoctorID() == doctorID && myStrCmp(appts[i].getStatus(), "completed") == 0) {
            hasCompleted = true;
            break;
        }
    }

    if (!hasCompleted) throw HospitalException("Access denied. You can only view records of your own patients.");

    cout << "\n--- History for Patient " << patientID << " ---\n";
    Prescription* rx = prescriptions.getAll();
    for (int i = 0; i < prescriptions.getSize(); i++) {
        if (rx[i].getPatientID() == patientID && rx[i].getDoctorID() == doctorID) {
            cout << "Date: " << rx[i].getDate() << " | Meds: " << rx[i].getMedicines() << "\n";
        }
    }
}

// ADMIN LOGIC 

void MediCoreSystem::addPatient(const char* name, int age, char gender, const char* contact, const char* pass, float balance) {
    if (!Validator::isValidContact(contact)) throw InvalidInputException("Contact must be 11 numeric digits.");
    if (!Validator::isValidPassword(pass)) throw InvalidInputException("Password too short.");

    int newID = patients.getSize() > 0 ? patients.getAll()[patients.getSize() - 1].getID() + 1 : 1;
    patients.add(Patient(newID, name, age, gender, contact, pass, balance));
    cout << "\nPatient added successfully. ID: " << newID << "\n";
}

void MediCoreSystem::removePatient(int patientID) {
    Patient* p = patients.findByID(patientID);
    if (!p) throw InvalidInputException("Patient not found.");

    Bill* bArr = bills.getAll();
    for (int i = 0; i < bills.getSize(); i++) {
        if (bArr[i].getPatientID() == patientID && myStrCmp(bArr[i].getStatus(), "unpaid") == 0) {
            throw HospitalException("Cannot remove patient with unpaid bills.");
        }
    }

    Appointment* appts = appointments.getAll();
    for (int i = 0; i < appointments.getSize(); i++) {
        if (appts[i].getPatientID() == patientID && myStrCmp(appts[i].getStatus(), "pending") == 0) {
            throw HospitalException("Cannot remove patient with pending appointments.");
        }
    }

    ofstream df("discharged.txt", ios::app);
    df << "REMOVED PATIENT," << p->getID() << "," << p->getName() << "," << p->getAge() << ","
        << p->getGender() << "," << p->getContact() << "," << p->getBalance() << "\n";
    df.close();

    patients.removeByID(patientID);
    cout << "\nPatient " << patientID << " removed and archived.\n";
}

void MediCoreSystem::addDoctor(const char* name, const char* spec, const char* contact, const char* pass, float fee) {
    if (!Validator::isValidContact(contact)) throw InvalidInputException("Contact must be 11 numeric digits.");
    if (!Validator::isValidPassword(pass)) throw InvalidInputException("Password too short.");

    int newID = doctors.getSize() > 0 ? doctors.getAll()[doctors.getSize() - 1].getID() + 1 : 1;
    doctors.add(Doctor(newID, name, spec, contact, pass, fee));
    cout << "\nDoctor added successfully. ID: " << newID << "\n";
}

void MediCoreSystem::removeDoctor(int doctorID) {
    Appointment* appts = appointments.getAll();
    for (int i = 0; i < appointments.getSize(); i++) {
        if (appts[i].getDoctorID() == doctorID && myStrCmp(appts[i].getStatus(), "pending") == 0) {
            throw HospitalException("Cannot remove doctor with pending appointments.");
        }
    }
    if (!doctors.removeByID(doctorID)) throw InvalidInputException("Doctor not found.");
    cout << "\nDoctor removed successfully.\n";
}

void MediCoreSystem::viewAllPatients() {
    cout << "\n--- All Patients ---\n";
    Patient* pArr = patients.getAll();
    for (int i = 0; i < patients.getSize(); i++) cout << pArr[i] << "\n";
}

void MediCoreSystem::viewAllDoctors() {
    cout << "\n--- All Doctors ---\n";
    Doctor* dArr = doctors.getAll();
    for (int i = 0; i < doctors.getSize(); i++) cout << dArr[i] << "\n";
}

void MediCoreSystem::viewAllAppointments() {
    cout << "\n--- All Appointments ---\n";
    Appointment* appt = appointments.getAll();
    for (int i = 0; i < appointments.getSize(); i++) cout << appt[i] << "\n";
}

void MediCoreSystem::viewUnpaidBills() {
    cout << "\n--- All Unpaid Bills ---\n";
    Bill* bArr = bills.getAll();
    for (int i = 0; i < bills.getSize(); i++) {
        if (myStrCmp(bArr[i].getStatus(), "unpaid") == 0) {
            cout << "Bill ID: " << bArr[i].getID() << " | Pat ID: " << bArr[i].getPatientID()
                << " | PKR " << bArr[i].getAmount() << " | Date: " << bArr[i].getDate() << "\n";
        }
    }
}

void MediCoreSystem::viewSecurityLog() {
    cout << "\n--- Security Log ---\n";
    ifstream file("security_log.txt");
    if (!file.is_open()) {
        cout << "No security events logged.\n";
        return;
    }
    char line[1000];
    while (file.getline(line, 1000)) cout << line << "\n";
    file.close();
}

void MediCoreSystem::dischargePatient(int patientID) {
    Patient* p = patients.findByID(patientID);
    if (!p) {
        throw InvalidInputException("Patient ID not found.");
    }

    Bill* bArr = bills.getAll();
    for (int i = 0; i < bills.getSize(); i++) {
        if (bArr[i].getPatientID() == patientID && myStrCmp(bArr[i].getStatus(), "unpaid") == 0) {
            throw HospitalException("Cannot discharge patient with unpaid bills.");
        }
    }

    Appointment* aArr = appointments.getAll();
    for (int i = 0; i < appointments.getSize(); i++) {
        if (aArr[i].getPatientID() == patientID && myStrCmp(aArr[i].getStatus(), "pending") == 0) {
            throw HospitalException("Cannot discharge patient with pending appointments.");
        }
    }

    ofstream outFile("discharged.txt", std::ios::app);
    if (outFile.is_open()) {

        outFile << p->getID() << "," << p->getName() << "," << p->getAge() << ","
            << p->getGender() << "," << p->getContact() << "," << p->getPassword() << ","
            << p->getBalance() << "\n";

        for (int i = 0; i < appointments.getSize(); i++) {
            if (aArr[i].getPatientID() == patientID) {
                outFile << "Appt," << aArr[i].getID() << "," << aArr[i].getPatientID() << ","
                    << aArr[i].getDoctorID() << "," << aArr[i].getDate() << ","
                    << aArr[i].getTimeSlot() << "," << aArr[i].getStatus() << "\n";
            }
        }

  
        Prescription* rxArr = prescriptions.getAll();
        for (int i = 0; i < prescriptions.getSize(); i++) {
            if (rxArr[i].getPatientID() == patientID) {
                outFile << "Rx," << rxArr[i].getID() << "," << rxArr[i].getAppointmentID() << ","
                    << rxArr[i].getPatientID() << "," << rxArr[i].getDoctorID() << ","
                    << rxArr[i].getDate() << "," << rxArr[i].getMedicines() << ","
                    << rxArr[i].getNotes() << "\n";
            }
        }


        for (int i = 0; i < bills.getSize(); i++) {
            if (bArr[i].getPatientID() == patientID) {
                outFile << "Bill," << bArr[i].getID() << "," << bArr[i].getPatientID() << ","
                    << bArr[i].getAppointmentID() << "," << bArr[i].getAmount() << ","
                    << bArr[i].getStatus() << "," << bArr[i].getDate() << "\n";
            }
        }
        outFile.close();
    }

    removePatient(patientID);
}

void MediCoreSystem::generateDailyReport() {
    char today[15];
    getTodayDate(today);

    int totAppt = 0, pend = 0, comp = 0, noShow = 0, canc = 0;
    float rev = 0.0f;

    Appointment* appts = appointments.getAll();
    for (int i = 0; i < appointments.getSize(); i++) {
        if (myStrCmp(appts[i].getDate(), today) == 0) {
            totAppt++;
            if (myStrCmp(appts[i].getStatus(), "pending") == 0) pend++;
            else if (myStrCmp(appts[i].getStatus(), "completed") == 0) comp++;
            else if (myStrCmp(appts[i].getStatus(), "no-show") == 0) noShow++;
            else if (myStrCmp(appts[i].getStatus(), "cancelled") == 0) canc++;
        }
    }

    Bill* bArr = bills.getAll();
    for (int i = 0; i < bills.getSize(); i++) {
        if (myStrCmp(bArr[i].getDate(), today) == 0 && myStrCmp(bArr[i].getStatus(), "paid") == 0) {
            rev += bArr[i].getAmount();
        }
    }

    cout << "\n=== DAILY REPORT (" << today << ") ===\n";
    cout << "Total Appointments: " << totAppt << " (Pending: " << pend << ", Completed: " << comp << ", No-show: " << noShow << ", Cancelled: " << canc << ")\n";
    cout << "Revenue Collected Today: PKR " << rev << "\n";
    cout << "======================================\n";
}

