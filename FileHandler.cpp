#include "FileHandler.h"
#include "Exceptions.h"
#include "CustomStringHelper.h"
#include <fstream>
#include <iostream>
using namespace std;

int FileHandler::parseCSVLine(const char* line, char tokens[][500]) {
    int tokenCount = 0;
    int charIndex = 0;

    for (int i = 0; line[i] != '\0'; i++) {
        if (line[i] == ',') {
            tokens[tokenCount][charIndex] = '\0'; 
            tokenCount++;
            charIndex = 0; 
        }
        else {

            if (line[i] != '\r' && line[i] != '\n') {
                tokens[tokenCount][charIndex] = line[i];
                charIndex++;
            }
        }
    }
 
    tokens[tokenCount][charIndex] = '\0';
    tokenCount++;

    return tokenCount;
}

// Patient I/O

void FileHandler::loadPatients(const char* filename, Storage<Patient>& storage) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw FileNotFoundException(filename);
    }

    char line[1000];
 
    while (file.getline(line, 1000)) {
        char tokens[10][500];
        int count = parseCSVLine(line, tokens);

        // A valid patient line in the format: id,name,age,gender,contact,password,balance
        if (count == 7) {
            int id = myAtoi(tokens[0]);
            int age = myAtoi(tokens[2]);
            char gender = tokens[3][0];
            float balance = myAtof(tokens[6]);

            Patient p(id, tokens[1], age, gender, tokens[4], tokens[5], balance);
            storage.add(p);
        }
    }
    file.close();
}

void FileHandler::savePatients(const char* filename, Storage<Patient>& storage) {
    ofstream file(filename, ios::trunc); 
    if (!file.is_open()) {
        throw FileNotFoundException(filename);
    }

    int size = storage.getSize();
    Patient* allPatients = storage.getAll();

    for (int i = 0; i < size; i++) {
        // Format: patient_id,name,age,gender,contact,password,balance
        file << allPatients[i].getID() << ","
            << allPatients[i].getName() << ","
            << allPatients[i].getAge() << ","
            << allPatients[i].getGender() << ","
            << allPatients[i].getContact() << ","
            << allPatients[i].getPassword() << ","
            << allPatients[i].getBalance() << "\n";
    }
    file.close();
}

// Doctor I/O

void FileHandler::loadDoctors(const char* filename, Storage<Doctor>& storage) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw FileNotFoundException(filename);
    }

    char line[1000];
    while (file.getline(line, 1000)) {
        char tokens[10][500];
        int count = parseCSVLine(line, tokens);

        // Format: doctor_id,name,specialization,contact,password,fee (6 tokens)
        if (count == 6) {
            int id = myAtoi(tokens[0]);
            float fee = myAtof(tokens[5]);

            Doctor d(id, tokens[1], tokens[2], tokens[3], tokens[4], fee);
            storage.add(d);
        }
    }
    file.close();
}

void FileHandler::saveDoctors(const char* filename, Storage<Doctor>& storage) {
    ofstream file(filename, ios::trunc);
    if (!file.is_open()) {
        throw FileNotFoundException(filename);
    }

    int size = storage.getSize();
    Doctor* allDoctors = storage.getAll();

    for (int i = 0; i < size; i++) {
        file << allDoctors[i].getID() << ","
            << allDoctors[i].getName() << ","
            << allDoctors[i].getSpecialization() << ","
            << allDoctors[i].getContact() << ","
            << allDoctors[i].getPassword() << ","
            << allDoctors[i].getFee() << "\n";
    }
    file.close();
}

// Admin I/O
void FileHandler::loadAdmins(const char* filename, Storage<Admin>& storage) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw FileNotFoundException(filename);
    }

    char line[1000];
    while (file.getline(line, 1000)) {
        char tokens[10][500];
        int count = parseCSVLine(line, tokens);

        // Format: admin_id,name,password (3 tokens)
        if (count == 3) {
            int id = myAtoi(tokens[0]);
            Admin a(id, tokens[1], tokens[2]);
            storage.add(a);
        }
    }
    file.close();
}

// Appointment I/O

void FileHandler::loadAppointments(const char* filename, Storage<Appointment>& storage) {
    ifstream file(filename);
    if (!file.is_open()) return; // It's okay if this is empty initially

    char line[1000];
    while (file.getline(line, 1000)) {
        char tokens[10][500];
        int count = parseCSVLine(line, tokens);

        // Format: appointment_id,patient_id,doctor_id,date,time_slot,status
        if (count == 6) {
            Appointment a(myAtoi(tokens[0]), myAtoi(tokens[1]), myAtoi(tokens[2]),
                tokens[3], tokens[4], tokens[5]);
            storage.add(a);
        }
    }
    file.close();
}

void FileHandler::saveAppointments(const char* filename, Storage<Appointment>& storage) {
    ofstream file(filename, ios::trunc);
    if (!file.is_open()) throw FileNotFoundException(filename);

    int size = storage.getSize();
    Appointment* arr = storage.getAll();
    for (int i = 0; i < size; i++) {
        file << arr[i].getID() << "," << arr[i].getPatientID() << ","
            << arr[i].getDoctorID() << "," << arr[i].getDate() << ","
            << arr[i].getTimeSlot() << "," << arr[i].getStatus() << "\n";
    }
    file.close();
}


// Bill I/O

void FileHandler::loadBills(const char* filename, Storage<Bill>& storage) {
    ifstream file(filename);
    if (!file.is_open()) return;

    char line[1000];
    while (file.getline(line, 1000)) {
        char tokens[10][500];
        int count = parseCSVLine(line, tokens);

        // Format: bill_id,patient_id,appointment_id,amount,status,date
        if (count == 6) {
            Bill b(myAtoi(tokens[0]), myAtoi(tokens[1]), myAtoi(tokens[2]),
                myAtof(tokens[3]), tokens[4], tokens[5]);
            storage.add(b);
        }
    }
    file.close();
}

void FileHandler::saveBills(const char* filename, Storage<Bill>& storage) {
    ofstream file(filename, ios::trunc);
    if (!file.is_open()) throw FileNotFoundException(filename);

    int size = storage.getSize();
    Bill* arr = storage.getAll();
    for (int i = 0; i < size; i++) {
        file << arr[i].getID() << "," << arr[i].getPatientID() << ","
            << arr[i].getAppointmentID() << "," << arr[i].getAmount() << ","
            << arr[i].getStatus() << "," << arr[i].getDate() << "\n";
    }
    file.close();
}

// Prescription I/O
void FileHandler::loadPrescriptions(const char* filename, Storage<Prescription>& storage) {
    ifstream file(filename);
    if (!file.is_open()) return;

    char line[1500]; 
    while (file.getline(line, 1500)) {
        char tokens[10][500];
        int count = parseCSVLine(line, tokens);

        // Format: prescription_id,appointment_id,patient_id,doctor_id,date,medicines,notes
        if (count == 7) {
            Prescription p(myAtoi(tokens[0]), myAtoi(tokens[1]), myAtoi(tokens[2]),
                myAtoi(tokens[3]), tokens[4], tokens[5], tokens[6]);
            storage.add(p);
        }
    }
    file.close();
}

void FileHandler::savePrescriptions(const char* filename, Storage<Prescription>& storage) {
    ofstream file(filename, ios::trunc);
    if (!file.is_open()) throw FileNotFoundException(filename);

    int size = storage.getSize();
    Prescription* arr = storage.getAll();
    for (int i = 0; i < size; i++) {
        file << arr[i].getID() << "," << arr[i].getAppointmentID() << ","
            << arr[i].getPatientID() << "," << arr[i].getDoctorID() << ","
            << arr[i].getDate() << "," << arr[i].getMedicines() << ","
            << arr[i].getNotes() << "\n";
    }
    file.close();
}