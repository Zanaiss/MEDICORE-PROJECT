#include "Appointment.h"
using namespace std;
Appointment::Appointment() {
    appointmentID = 0;
    patientID = 0;
    doctorID = 0;
    date[0] = '\0';
    timeSlot[0] = '\0';
    status[0] = '\0';
}

Appointment::Appointment(int aID, int pID, int dID, const char* dt, const char* ts, const char* st) {
    appointmentID = aID;
    patientID = pID;
    doctorID = dID;
    myStrCpy(date, dt);
    myStrCpy(timeSlot, ts);
    myStrCpy(status, st);
}

int Appointment::getID() const { return appointmentID; }
int Appointment::getPatientID() const { return patientID; }
int Appointment::getDoctorID() const { return doctorID; }
const char* Appointment::getDate() const { return date; }
const char* Appointment::getTimeSlot() const { return timeSlot; }
const char* Appointment::getStatus() const { return status; }

void Appointment::setStatus(const char* st) {
    myStrCpy(status, st);
}

bool Appointment::operator==(const Appointment& other) const {
    if (this->doctorID == other.doctorID &&
        myStrCmp(this->date, other.date) == 0 &&
        myStrCmp(this->timeSlot, other.timeSlot) == 0)
    {
    
        if (myStrCmp(this->status, "cancelled") != 0 && myStrCmp(other.status, "cancelled") != 0) {
            return true;
        }
    }
    return false;
}

ostream& operator<<(ostream& out, const Appointment& a) {
    out << "Appt ID: " << a.appointmentID << " | Pat ID: " << a.patientID
        << " | Doc ID: " << a.doctorID << " | Date: " << a.date
        << " | Time: " << a.timeSlot << " | Status: " << a.status;
    return out;
}