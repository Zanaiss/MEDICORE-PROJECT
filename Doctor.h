#ifndef DOCTOR_H
#define DOCTOR_H

#include "Person.h"
#include <iostream>

class Doctor : public Person {
private:
    char specialization[50];
    char contact[15];
    float fee;

public:
    Doctor();
    Doctor(int id, const char* name, const char* specialization, const char* contact, const char* password, float fee);

    const char* getSpecialization() const;
    const char* getContact() const;
    float getFee() const;

    void displayProfile() const override;
    const char* getRole() const override;

    bool operator==(const Doctor& other) const;

    friend std::ostream& operator<<(std::ostream& out, const Doctor& d);
};

#endif