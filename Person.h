#ifndef PERSON_H
#define PERSON_H

#include "CustomStringHelper.h"


class Person {
protected:
    int id;
    char name[50];
    char password[20];

public:
    Person();
    Person(int id, const char* name, const char* password);
    virtual ~Person(); 

    int getID() const;
    const char* getName() const;
    const char* getPassword() const;

    void setID(int id);
    void setName(const char* name);
    void setPassword(const char* password);

    virtual void displayProfile() const = 0;
    virtual const char* getRole() const = 0;
};

#endif