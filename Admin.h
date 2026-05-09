#ifndef ADMIN_H
#define ADMIN_H

#include "Person.h"
#include <iostream>

class Admin : public Person {
public:
    Admin();
    Admin(int id, const char* name, const char* password);


    void displayProfile() const override;
    const char* getRole() const override;
};

#endif