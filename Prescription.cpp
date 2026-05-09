#include "Prescription.h"

Prescription::Prescription() {
    prescriptionID = appointmentID = patientID = doctorID = 0;
    date[0] = medicines[0] = notes[0] = '\0';
}

Prescription::Prescription(int prID, int aID, int pID, int dID, const char* dt, const char* meds, const char* nts) {
    prescriptionID = prID;
    appointmentID = aID;
    patientID = pID;
    doctorID = dID;
    myStrCpy(date, dt);

    myStrCpy(medicines, meds);
    myStrCpy(notes, nts);
}

int Prescription::getID() const { return prescriptionID; }
int Prescription::getAppointmentID() const { return appointmentID; }
int Prescription::getPatientID() const { return patientID; }
int Prescription::getDoctorID() const { return doctorID; }
const char* Prescription::getDate() const { return date; }
const char* Prescription::getMedicines() const { return medicines; }
const char* Prescription::getNotes() const { return notes; }