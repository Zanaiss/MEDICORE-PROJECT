#include "Person.h"

Person::Person() {
    id = 0;
    name[0] = '\0';
    password[0] = '\0';
}

Person::Person(int id, const char* name, const char* password) {
    this->id = id;
    myStrCpy(this->name, name);
    myStrCpy(this->password, password);
}

Person::~Person() {
    
}

int Person::getID() const {
    return id;
}

const char* Person::getName() const {
    return name;
}

const char* Person::getPassword() const {
    return password;
}

void Person::setID(int id) {
    this->id = id;
}

void Person::setName(const char* name) {
    myStrCpy(this->name, name);
}

void Person::setPassword(const char* password) {
    myStrCpy(this->password, password);
}