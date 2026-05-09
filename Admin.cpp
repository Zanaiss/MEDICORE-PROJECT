#include "Admin.h"

Admin::Admin() : Person() {
}

Admin::Admin(int id, const char* name, const char* password)
    : Person(id, name, password) {
}

void Admin::displayProfile() const {
    std::cout << "Admin ID: " << id << " | Name: " << name << std::endl;
}

const char* Admin::getRole() const {
    return "Admin";
}