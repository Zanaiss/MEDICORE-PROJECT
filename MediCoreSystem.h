#ifndef MEDICORE_SYSTEM_H
#define MEDICORE_SYSTEM_H

#include "Storage.h"
#include "Patient.h"
#include "Doctor.h"
#include "Admin.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"

class MediCoreSystem {
private:
    // Session State
    int currentUserID;
    int currentUserRole; // 1 = Patient, 2 = Doctor, 3 = Admin, 0 = Logged Out
    int failedLoginAttempts;
    bool systemLocked;

    void logSecurityEvent(const char* roleStr, int attemptedID, const char* result);
    void getTodayDate(char* buffer);

public:
    MediCoreSystem();
    ~MediCoreSystem();

    Storage<Patient> patients;
    Storage<Doctor> doctors;
    Storage<Admin> admins;
    Storage<Appointment> appointments;
    Storage<Bill> bills;
    Storage<Prescription> prescriptions;

    void loadData();
    void saveData();

    // Authentication
    bool login(int role, int id, const char* password);
    void logout();
    bool isLocked() const;

    // Patient Operations 
    void searchDoctorBySpecialization(const char* spec);
    void viewAvailableTimeSlots(int doctorID, const char* date);
    void bookAppointment(int patientID, int doctorID, const char* date, const char* timeSlot);
    void cancelAppointment(int patientID, int appointmentID);
    void payBill(int patientID, int billID);
    void topUpBalance(int patientID, float amount);
    void viewMyAppointments(int patientID);
    void viewMyBills(int patientID);
    void viewMyMedicalRecords(int patientID);

    // Doctor Operation
    void viewTodayAppointments(int doctorID);
    void markAppointmentComplete(int doctorID, int appointmentID);
    void markAppointmentNoShow(int doctorID, int appointmentID);
    void writePrescription(int doctorID, int appointmentID, const char* medicines, const char* notes);
    void viewPatientHistory(int doctorID, int patientID);

    // Admin Operations
    void dischargePatient(int patientID);
    void addPatient(const char* name, int age, char gender, const char* contact, const char* pass, float balance);
    void removePatient(int patientID);
    void addDoctor(const char* name, const char* spec, const char* contact, const char* pass, float fee);
    void removeDoctor(int doctorID);
    void viewAllPatients();
    void viewAllDoctors();
    void viewAllAppointments();
    void viewUnpaidBills();
    void viewSecurityLog();
    void generateDailyReport();
};

#endif