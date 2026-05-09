#ifndef VALIDATOR_H
#define VALIDATOR_H

class Validator {
public:
    static bool isValidDate(const char* date); // strictly DD-MM-YYYY
    static bool isValidTimeSlot(const char* timeSlot); // Only the 8 accepted slots
    static bool isValidContact(const char* contact); // Exactly 11 digits
    static bool isValidPassword(const char* password); // Min 6 chars
    static bool isPositiveFloat(float val);
};

#endif