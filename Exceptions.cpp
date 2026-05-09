#include "Exceptions.h"


HospitalException::HospitalException(const char* msg) {
    myStrCpy(this->message, msg);
}

const char* HospitalException::what() const {
    return message;
}


FileNotFoundException::FileNotFoundException(const char* file) : HospitalException("") {
    myStrCpy(this->message, "Error: Could not open file -> ");
    int len1 = myStrLen(this->message);
    int len2 = myStrLen(file);
    for (int i = 0; i < len2 && (len1 + i) < 199; i++) {
        this->message[len1 + i] = file[i];
    }
    this->message[len1 + len2] = '\0';
}

InsufficientFundsException::InsufficientFundsException()
    : HospitalException("Error: Insufficient funds in patient balance.") {
}

InvalidInputException::InvalidInputException(const char* msg)
    : HospitalException(msg) {
}

SlotUnavailableException::SlotUnavailableException()
    : HospitalException("Error: That time slot is already booked.") {
}