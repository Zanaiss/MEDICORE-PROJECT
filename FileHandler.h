#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "Storage.h"
#include "Patient.h"
#include "Doctor.h"
#include "Admin.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"

class FileHandler {
private:
    static int parseCSVLine(const char* line, char tokens[][500]);

public:
    // Loaders
    static void loadPatients(const char* filename, Storage<Patient>& storage);
    static void loadDoctors(const char* filename, Storage<Doctor>& storage);
    static void loadAdmins(const char* filename, Storage<Admin>& storage);
    static void loadAppointments(const char* filename, Storage<Appointment>& storage);
    static void loadBills(const char* filename, Storage<Bill>& storage);
    static void loadPrescriptions(const char* filename, Storage<Prescription>& storage);

    // Savers
    static void savePatients(const char* filename, Storage<Patient>& storage);
    static void saveDoctors(const char* filename, Storage<Doctor>& storage);
    static void saveAppointments(const char* filename, Storage<Appointment>& storage);
    static void saveBills(const char* filename, Storage<Bill>& storage);
    static void savePrescriptions(const char* filename, Storage<Prescription>& storage);
};

#endif