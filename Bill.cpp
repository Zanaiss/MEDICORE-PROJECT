#include "Bill.h"

Bill::Bill() {
    billID = patientID = appointmentID = 0;
    amount = 0.0f;
    status[0] = date[0] = '\0';
}

Bill::Bill(int bID, int pID, int aID, float amt, const char* st, const char* dt) {
    billID = bID;
    patientID = pID;
    appointmentID = aID;
    amount = amt;
    myStrCpy(status, st);
    myStrCpy(date, dt);
}

int Bill::getID() const { return billID; }
int Bill::getPatientID() const { return patientID; }
int Bill::getAppointmentID() const { return appointmentID; }
float Bill::getAmount() const { return amount; }
const char* Bill::getStatus() const { return status; }
const char* Bill::getDate() const { return date; }

void Bill::setStatus(const char* st) {
    myStrCpy(status, st);
}