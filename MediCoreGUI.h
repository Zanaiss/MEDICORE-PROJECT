#ifndef MEDICORE_GUI_H
#define MEDICORE_GUI_H

#include <SFML/Graphics.hpp>
#include "MediCoreSystem.h"
#include "TextBox.h"

class MediCoreGUI {
private:
    sf::RenderWindow window;
    sf::Font font;
    MediCoreSystem& systemApp;

    enum AppState {
        LOGIN, PATIENT_MENU, DOCTOR_MENU, ADMIN_MENU,
        ADMIN_ADD_PATIENT, ADMIN_ADD_DOCTOR, DOC_WRITE_RX,
        PAT_BOOK_APPT, PROMPT_ID, PROMPT_SLOTS, VIEW_LIST, VIEW_REPORT 
    };
    AppState currentState;
    int loggedInUserID;
    int selectedRole;
    char statusMessage[100];

    int listType;
    int listPage;

    char searchBuffer[50];
    int searchDocID;
    char searchDate[20];
    int searchPatID;

    TextBox loginIDBox, loginPassBox;
    TextBox tbName, tbAge, tbGender, tbContact, tbPass, tbBal;
    TextBox tbDName, tbDSpec, tbDContact, tbDPass, tbDFee;
    TextBox tbRxApptID, tbRxMeds, tbRxNotes;
    TextBox tbBookDocID, tbBookDate, tbBookTime;
    TextBox tbPromptID;
    int promptAction;

    int getStrLen(const char* str);
    void copyStr(char* dest, const char* src);
    void myStrCat(char* dest, const char* src);
    void handleLoginEvents(sf::Event& event);
    void handlePatientEvents(sf::Event& event);
    void handleDoctorEvents(sf::Event& event);
    void handleAdminEvents(sf::Event& event);
    void handleAddPatientEvents(sf::Event& event);
    void handleAddDoctorEvents(sf::Event& event);
    void handleWriteRxEvents(sf::Event& event);
    void handleBookApptEvents(sf::Event& event);
    void handlePromptIDEvents(sf::Event& event);
    void handlePromptSlotsEvents(sf::Event& event);
    void handleViewListEvents(sf::Event& event);
    void handleViewReportEvents(sf::Event& event);
    void drawViewReportScreen();
    void drawLoginScreen();
    void drawPatientMenu();
    void drawDoctorMenu();
    void drawAdminMenu();
    void drawAddPatientScreen();
    void drawAddDoctorScreen();
    void drawWriteRxScreen();
    void drawBookApptScreen();
    void drawPromptIDScreen();
    void drawPromptSlotsScreen();
    void drawViewListScreen();

public:
    MediCoreGUI(MediCoreSystem& backend);
    void run();
};

#endif