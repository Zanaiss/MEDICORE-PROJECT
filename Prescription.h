#ifndef PRESCRIPTION_H
#define PRESCRIPTION_H

#include "CustomStringHelper.h"

class Prescription {
private:
    int prescriptionID;
    int appointmentID;
    int patientID;
    int doctorID;
    char date[15];
    char medicines[500]; 
    char notes[300];    

public:
    Prescription();
    Prescription(int prID, int aID, int pID, int dID, const char* dt, const char* meds, const char* nts);

    int getID() const;
    int getAppointmentID() const;
    int getPatientID() const;
    int getDoctorID() const;
    const char* getDate() const;
    const char* getMedicines() const;
    const char* getNotes() const;
};

#endif