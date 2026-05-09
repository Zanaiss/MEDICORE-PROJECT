#include "Doctor.h"
using namespace std;
Doctor::Doctor() : Person() {
    specialization[0] = '\0';
    contact[0] = '\0';
    fee = 0.0f;
}

Doctor::Doctor(int id, const char* name, const char* specialization, const char* contact, const char* password, float fee)
    : Person(id, name, password) {
    myStrCpy(this->specialization, specialization);
    myStrCpy(this->contact, contact);
    this->fee = fee;
}

const char* Doctor::getSpecialization() const { return specialization; }
const char* Doctor::getContact() const { return contact; }
float Doctor::getFee() const { return fee; }

void Doctor::displayProfile() const {
    cout << "Dr. " << name << " | Specialization: " << specialization << " | Fee: PKR " << fee << endl;
}

const char* Doctor::getRole() const {
    return "Doctor";
}


bool Doctor::operator==(const Doctor& other) const {
    return this->id == other.id;
}

ostream& operator<<(ostream& out, const Doctor& d) {
    out << d.id << " | Dr. " << d.name << " | " << d.specialization << " | Contact: " << d.contact << " | Fee: PKR " << d.fee;
    return out;
}