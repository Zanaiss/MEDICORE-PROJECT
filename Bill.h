#ifndef BILL_H
#define BILL_H

#include "CustomStringHelper.h"

class Bill {
private:
    int billID;
    int patientID;
    int appointmentID;
    float amount;
    char status[20]; // unpaid, paid, cancelled
    char date[15];

public:
    Bill();
    Bill(int bID, int pID, int aID, float amt, const char* st, const char* dt);

    int getID() const;
    int getPatientID() const;
    int getAppointmentID() const;
    float getAmount() const;
    const char* getStatus() const;
    const char* getDate() const;

    void setStatus(const char* st);
};

#endif