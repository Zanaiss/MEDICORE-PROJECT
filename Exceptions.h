#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include "CustomStringHelper.h"

class HospitalException {
protected:
    char message[200];
public:
    HospitalException(const char* msg);
    virtual const char* what() const;
    virtual ~HospitalException() {}
};

class FileNotFoundException : public HospitalException {
public:
    FileNotFoundException(const char* file);
};

class InsufficientFundsException : public HospitalException {
public:
    InsufficientFundsException();
};

class InvalidInputException : public HospitalException {
public:
    InvalidInputException(const char* msg);
};

class SlotUnavailableException : public HospitalException {
public:
    SlotUnavailableException();
};

#endif