#ifndef PATIENT_H
#define PATIENT_H

#include "Person.h"
#include <iostream>

class Patient : public Person {
private:
    int age;
    char gender; // 'M' or 'F'
    char contact[15];
    float balance;

public:
    Patient();
    Patient(int id, const char* name, int age, char gender, const char* contact, const char* password, float balance);


    int getAge() const;
    char getGender() const;
    const char* getContact() const;
    float getBalance() const;

    void setAge(int age);
    void setGender(char gender);
    void setContact(const char* contact);
    void setBalance(float balance);

    void displayProfile() const override;
    const char* getRole() const override;

    void operator+=(float amount);
    void operator-=(float amount);
    bool operator==(const Patient& other) const;


    friend std::ostream& operator<<(std::ostream& out, const Patient& p);
};

#endif