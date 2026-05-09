#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include "CustomStringHelper.h"
#include <iostream>

class Appointment {
private:
    int appointmentID;
    int patientID;
    int doctorID;
    char date[15];      // Format: DD-MM-YYYY
    char timeSlot[10];  // Format: HH:MM
    char status[20];    // pending, completed, cancelled, no-show

public:
    Appointment();
    Appointment(int aID, int pID, int dID, const char* dt, const char* ts, const char* st);

    int getID() const;
    int getPatientID() const;
    int getDoctorID() const;
    const char* getDate() const;
    const char* getTimeSlot() const;
    const char* getStatus() const;

    void setStatus(const char* st);

    bool operator==(const Appointment& other) const;
    friend std::ostream& operator<<(std::ostream& out, const Appointment& a);
};

#endif