#include "Patient.h"
using namespace std;
Patient::Patient() : Person() {
    age = 0;
    gender = 'U';
    contact[0] = '\0';
    balance = 0.0f;
}

Patient::Patient(int id, const char* name, int age, char gender, const char* contact, const char* password, float balance)
    : Person(id, name, password) {
    this->age = age;
    this->gender = gender;
    myStrCpy(this->contact, contact);
    this->balance = balance;
}

int Patient::getAge() const { return age; }
char Patient::getGender() const { return gender; }
const char* Patient::getContact() const { return contact; }
float Patient::getBalance() const { return balance; }

void Patient::setAge(int age) { this->age = age; }
void Patient::setGender(char gender) { this->gender = gender; }
void Patient::setContact(const char* contact) { myStrCpy(this->contact, contact); }
void Patient::setBalance(float balance) { this->balance = balance; }

void Patient::displayProfile() const {
    cout << "Patient ID: " << id << " | Name: " << name << " | Balance: PKR " << balance << endl;
}

const char* Patient::getRole() const {
    return "Patient";
}


void Patient::operator+=(float amount) {
    this->balance += amount;
}


void Patient::operator-=(float amount) {
    this->balance -= amount;
}

bool Patient::operator==(const Patient& other) const {
    return this->id == other.id;
}

ostream& operator<<(ostream& out, const Patient& p) {
    out << p.id << " | " << p.name << " | Age: " << p.age << " | Gender: " << p.gender
        << " | Contact: " << p.contact << " | Balance: PKR " << p.balance;
    return out;
}