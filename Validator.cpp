#include "Validator.h"
#include "CustomStringHelper.h"

bool Validator::isValidDate(const char* date) {
    if (myStrLen(date) != 10) return false;
    if (date[2] != '-' || date[5] != '-') return false;

    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) continue;
        if (date[i] < '0' || date[i] > '9') return false;
    }
    return true;
}

bool Validator::isValidTimeSlot(const char* timeSlot) {
    const char* validSlots[8] = { "09:00", "10:00", "11:00", "12:00",
                                  "13:00", "14:00", "15:00", "16:00" };
    for (int i = 0; i < 8; i++) {
        if (myStrCmp(timeSlot, validSlots[i]) == 0) return true;
    }
    return false;
}

bool Validator::isValidContact(const char* contact) {
    if (myStrLen(contact) != 11) return false;
    for (int i = 0; i < 11; i++) {
        if (contact[i] < '0' || contact[i] > '9') return false;
    }
    return true;
}

bool Validator::isValidPassword(const char* password) {
    return myStrLen(password) >= 6;
}

bool Validator::isPositiveFloat(float val) {
    return val >= 0.0f;
}