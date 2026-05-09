#include "MediCoreGUI.h"
#include "CustomStringHelper.h"
#include "Exceptions.h"
#include <iostream>

int MediCoreGUI::getStrLen(const char* str) { int len = 0; while (str[len] != '\0') len++; return len; }
void MediCoreGUI::copyStr(char* dest, const char* src) { int i = 0; while (src[i] != '\0') { dest[i] = src[i]; i++; } dest[i] = '\0'; }

void MediCoreGUI::myStrCat(char* dest, const char* src) {
    int i = 0; while (dest[i] != '\0') i++;
    int j = 0; while (src[j] != '\0') { dest[i] = src[j]; i++; j++; }
    dest[i] = '\0';
}

MediCoreGUI::MediCoreGUI(MediCoreSystem& backend) : systemApp(backend) {
    window.create(sf::VideoMode(800, 600), "MediCore Hospital System", sf::Style::Default);
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) std::cout << "[GUI ERROR] Missing Arial font!\n";

    currentState = LOGIN; loggedInUserID = 0; selectedRole = 1; statusMessage[0] = '\0'; promptAction = 0;
    listType = 0; listPage = 0; searchBuffer[0] = '\0'; searchDocID = 0; searchDate[0] = '\0'; searchPatID = 0;

    loginIDBox.setup(300, 200, 200, 40, 10, font); loginIDBox.setSelected(true);
    loginPassBox.setup(300, 280, 200, 40, 20, font);

    tbName.setup(300, 150, 200, 30, 40, font); tbAge.setup(300, 200, 200, 30, 5, font); tbGender.setup(300, 250, 200, 30, 2, font);
    tbContact.setup(300, 300, 200, 30, 15, font); tbPass.setup(300, 350, 200, 30, 20, font); tbBal.setup(300, 400, 200, 30, 10, font);

    tbDName.setup(300, 150, 200, 30, 40, font); tbDSpec.setup(300, 200, 200, 30, 30, font); tbDContact.setup(300, 250, 200, 30, 15, font);
    tbDPass.setup(300, 300, 200, 30, 20, font); tbDFee.setup(300, 350, 200, 30, 10, font);

    tbRxApptID.setup(300, 150, 200, 30, 10, font); tbRxMeds.setup(300, 200, 300, 30, 400, font); tbRxNotes.setup(300, 250, 300, 30, 400, font);
    tbBookDocID.setup(300, 150, 200, 30, 15, font); tbBookDate.setup(300, 200, 200, 30, 15, font); tbBookTime.setup(300, 250, 200, 30, 10, font);
    tbPromptID.setup(300, 250, 200, 40, 30, font);
}

void MediCoreGUI::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::Resized) {
                sf::FloatRect visibleArea(0, 0, (float)event.size.width, (float)event.size.height);
                window.setView(sf::View(visibleArea));
            }

            if (currentState == LOGIN) handleLoginEvents(event);
            else if (currentState == PATIENT_MENU) handlePatientEvents(event);
            else if (currentState == DOCTOR_MENU) handleDoctorEvents(event);
            else if (currentState == ADMIN_MENU) handleAdminEvents(event);
            else if (currentState == ADMIN_ADD_PATIENT) handleAddPatientEvents(event);
            else if (currentState == ADMIN_ADD_DOCTOR) handleAddDoctorEvents(event);
            else if (currentState == DOC_WRITE_RX) handleWriteRxEvents(event);
            else if (currentState == PAT_BOOK_APPT) handleBookApptEvents(event);
            else if (currentState == PROMPT_ID) handlePromptIDEvents(event);
            else if (currentState == PROMPT_SLOTS) handlePromptSlotsEvents(event);
            else if (currentState == VIEW_LIST) handleViewListEvents(event);
            else if (currentState == VIEW_REPORT) handleViewReportEvents(event);
          
        }
        window.clear(sf::Color(240, 248, 255));
        if (currentState == LOGIN) drawLoginScreen();
        else if (currentState == PATIENT_MENU) drawPatientMenu();
        else if (currentState == DOCTOR_MENU) drawDoctorMenu();
        else if (currentState == ADMIN_MENU) drawAdminMenu();
        else if (currentState == ADMIN_ADD_PATIENT) drawAddPatientScreen();
        else if (currentState == ADMIN_ADD_DOCTOR) drawAddDoctorScreen();
        else if (currentState == DOC_WRITE_RX) drawWriteRxScreen();
        else if (currentState == PAT_BOOK_APPT) drawBookApptScreen();
        else if (currentState == PROMPT_ID) drawPromptIDScreen();
        else if (currentState == PROMPT_SLOTS) drawPromptSlotsScreen();
        else if (currentState == VIEW_LIST) drawViewListScreen();
        else if (currentState == VIEW_REPORT) drawViewReportScreen();
        window.display();
    }
}

// LOGIN
void MediCoreGUI::handleLoginEvents(sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f m = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)); float mx = m.x; float my = m.y;
        loginIDBox.checkClick(mx, my); loginPassBox.checkClick(mx, my);
        if (mx >= 200 && mx <= 280 && my >= 140 && my <= 180) selectedRole = 1;
        else if (mx >= 300 && mx <= 380 && my >= 140 && my <= 180) selectedRole = 2;
        else if (mx >= 400 && mx <= 480 && my >= 140 && my <= 180) selectedRole = 3;
        else if (mx >= 300 && mx <= 500 && my >= 360 && my <= 400) {
            try {
                int parsedID = myAtoi(loginIDBox.getText());
                if (systemApp.login(selectedRole, parsedID, loginPassBox.getText())) {
                    loggedInUserID = parsedID; loginIDBox.clear(); loginPassBox.clear(); statusMessage[0] = '\0';
                    if (selectedRole == 1) currentState = PATIENT_MENU;
                    else if (selectedRole == 2) currentState = DOCTOR_MENU;
                    else if (selectedRole == 3) currentState = ADMIN_MENU;
                }
                else copyStr(statusMessage, "Error: Invalid Credentials.");
            }
            catch (const HospitalException& e) { copyStr(statusMessage, e.what()); }
        }
    }
    loginIDBox.typeText(event); loginPassBox.typeText(event);
}
void MediCoreGUI::drawLoginScreen() {
    sf::Text title("MediCore System Login", font, 36); title.setFillColor(sf::Color(0, 51, 102)); title.setPosition(220, 50); window.draw(title);
    sf::RectangleShape pBtn(sf::Vector2f(80, 40)); pBtn.setPosition(200, 140); pBtn.setFillColor(selectedRole == 1 ? sf::Color(0, 153, 204) : sf::Color(200, 200, 200));
    sf::RectangleShape dBtn(sf::Vector2f(80, 40)); dBtn.setPosition(300, 140); dBtn.setFillColor(selectedRole == 2 ? sf::Color(0, 153, 204) : sf::Color(200, 200, 200));
    sf::RectangleShape aBtn(sf::Vector2f(80, 40)); aBtn.setPosition(400, 140); aBtn.setFillColor(selectedRole == 3 ? sf::Color(0, 153, 204) : sf::Color(200, 200, 200));
    window.draw(pBtn); window.draw(dBtn); window.draw(aBtn);
    sf::Text t1("Patient", font, 18); t1.setFillColor(sf::Color::White); t1.setPosition(210, 150); window.draw(t1);
    sf::Text t2("Doctor", font, 18); t2.setFillColor(sf::Color::White); t2.setPosition(310, 150); window.draw(t2);
    sf::Text t3("Admin", font, 18); t3.setFillColor(sf::Color::White); t3.setPosition(410, 150); window.draw(t3);
    sf::Text idL("User ID:", font, 20); idL.setFillColor(sf::Color::Black); idL.setPosition(200, 205); window.draw(idL); loginIDBox.draw(window);
    sf::Text pL("Password:", font, 20); pL.setFillColor(sf::Color::Black); pL.setPosition(180, 285); window.draw(pL); loginPassBox.draw(window);
    sf::RectangleShape loginBtn(sf::Vector2f(200, 40)); loginBtn.setPosition(300, 360); loginBtn.setFillColor(sf::Color(0, 102, 51));
    sf::Text lText("LOGIN", font, 20); lText.setFillColor(sf::Color::White); lText.setPosition(365, 368); window.draw(loginBtn); window.draw(lText);
    sf::Text status(statusMessage, font, 18); status.setFillColor(sf::Color::Red); status.setPosition(250, 420); window.draw(status);
}

// MENUS 
void MediCoreGUI::handlePatientEvents(sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f m = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)); float mx = m.x; float my = m.y;
        if (mx >= 50 && mx <= 300 && my >= 150 && my <= 190) { listType = 1; listPage = 0; currentState = VIEW_LIST; }
        else if (mx >= 50 && mx <= 300 && my >= 210 && my <= 250) { tbBookDocID.clear(); tbBookDate.clear(); tbBookTime.clear(); statusMessage[0] = '\0'; currentState = PAT_BOOK_APPT; }
        else if (mx >= 50 && mx <= 300 && my >= 270 && my <= 310) { tbPromptID.clear(); promptAction = 5; statusMessage[0] = '\0'; currentState = PROMPT_ID; }
        else if (mx >= 50 && mx <= 300 && my >= 330 && my <= 370) { tbPromptID.clear(); promptAction = 8; statusMessage[0] = '\0'; currentState = PROMPT_ID; }
        else if (mx >= 50 && mx <= 300 && my >= 390 && my <= 430) { tbBookDocID.clear(); tbBookDate.clear(); statusMessage[0] = '\0'; currentState = PROMPT_SLOTS; }

        else if (mx >= 450 && mx <= 700 && my >= 150 && my <= 190) { listType = 2; listPage = 0; currentState = VIEW_LIST; }
        else if (mx >= 450 && mx <= 700 && my >= 210 && my <= 250) { tbPromptID.clear(); promptAction = 6; statusMessage[0] = '\0'; currentState = PROMPT_ID; }
        else if (mx >= 450 && mx <= 700 && my >= 270 && my <= 310) { tbPromptID.clear(); promptAction = 7; statusMessage[0] = '\0'; currentState = PROMPT_ID; }
        else if (mx >= 450 && mx <= 700 && my >= 330 && my <= 370) { listType = 6; listPage = 0; currentState = VIEW_LIST; }
        else if (mx >= 300 && mx <= 500 && my >= 500 && my <= 540) { systemApp.logout(); loggedInUserID = 0; currentState = LOGIN; }
    }
}
void MediCoreGUI::drawPatientMenu() {
 
    sf::Text title("Patient Dashboard", font, 36); title.setFillColor(sf::Color(0, 51, 102)); title.setPosition(250, 30); window.draw(title);

    Patient* p = systemApp.patients.findByID(loggedInUserID);
    if (p) {
        char msg[200]; copyStr(msg, "Welcome, "); myStrCat(msg, p->getName());
        myStrCat(msg, "   |   Balance: PKR ");
        char balBuf[20]; myItoa((int)p->getBalance(), balBuf); myStrCat(msg, balBuf);
        sf::Text wTxt(msg, font, 18); wTxt.setFillColor(sf::Color(0, 102, 51)); wTxt.setPosition(50, 90); window.draw(wTxt);
    }

    sf::RectangleShape b1(sf::Vector2f(250, 40)); b1.setPosition(50, 150); b1.setFillColor(sf::Color(0, 102, 204)); sf::Text t1("View Appts (GUI)", font, 16); t1.setFillColor(sf::Color::White); t1.setPosition(60, 160); window.draw(b1); window.draw(t1);
    sf::RectangleShape b2(sf::Vector2f(250, 40)); b2.setPosition(50, 210); b2.setFillColor(sf::Color(0, 153, 51)); sf::Text t2("Book Appointment (GUI)", font, 16); t2.setFillColor(sf::Color::White); t2.setPosition(60, 220); window.draw(b2); window.draw(t2);
    sf::RectangleShape b3(sf::Vector2f(250, 40)); b3.setPosition(50, 270); b3.setFillColor(sf::Color(204, 51, 0)); sf::Text t3("Cancel Appointment (GUI)", font, 16); t3.setFillColor(sf::Color::White); t3.setPosition(60, 280); window.draw(b3); window.draw(t3);
    sf::RectangleShape b4(sf::Vector2f(250, 40)); b4.setPosition(50, 330); b4.setFillColor(sf::Color(102, 0, 153)); sf::Text t4("Search Doctor by Spec (GUI)", font, 16); t4.setFillColor(sf::Color::White); t4.setPosition(60, 340); window.draw(b4); window.draw(t4);
    sf::RectangleShape b5(sf::Vector2f(250, 40)); b5.setPosition(50, 390); b5.setFillColor(sf::Color(102, 0, 153)); sf::Text t5("View Time Slots (GUI)", font, 16); t5.setFillColor(sf::Color::White); t5.setPosition(60, 400); window.draw(b5); window.draw(t5);

    sf::RectangleShape b6(sf::Vector2f(250, 40)); b6.setPosition(450, 150); b6.setFillColor(sf::Color(0, 102, 204)); sf::Text t6("View Bills (GUI)", font, 16); t6.setFillColor(sf::Color::White); t6.setPosition(460, 160); window.draw(b6); window.draw(t6);
    sf::RectangleShape b7(sf::Vector2f(250, 40)); b7.setPosition(450, 210); b7.setFillColor(sf::Color(204, 153, 0)); sf::Text t7("Pay Bill (GUI)", font, 16); t7.setFillColor(sf::Color::White); t7.setPosition(460, 220); window.draw(b7); window.draw(t7);
    sf::RectangleShape b8(sf::Vector2f(250, 40)); b8.setPosition(450, 270); b8.setFillColor(sf::Color(204, 153, 0)); sf::Text t8("Top Up Balance (GUI)", font, 16); t8.setFillColor(sf::Color::White); t8.setPosition(460, 280); window.draw(b8); window.draw(t8);
    sf::RectangleShape b9(sf::Vector2f(250, 40)); b9.setPosition(450, 330); b9.setFillColor(sf::Color(0, 102, 204)); sf::Text t9("View Medical Records (GUI)", font, 16); t9.setFillColor(sf::Color::White); t9.setPosition(460, 340); window.draw(b9); window.draw(t9);

    sf::RectangleShape logoutBtn(sf::Vector2f(200, 40)); logoutBtn.setPosition(300, 500); logoutBtn.setFillColor(sf::Color(153, 0, 0));
    sf::Text logoutText("LOGOUT", font, 20); logoutText.setFillColor(sf::Color::White); logoutText.setPosition(355, 508); window.draw(logoutBtn); window.draw(logoutText);
}

void MediCoreGUI::handleDoctorEvents(sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f m = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)); float mx = m.x; float my = m.y;
        if (mx >= 100 && mx <= 350 && my >= 150 && my <= 200) { listType = 3; listPage = 0; currentState = VIEW_LIST; }
        else if (mx >= 100 && mx <= 350 && my >= 220 && my <= 270) { tbPromptID.clear(); promptAction = 2; statusMessage[0] = '\0'; currentState = PROMPT_ID; }
        else if (mx >= 100 && mx <= 350 && my >= 290 && my <= 340) { tbPromptID.clear(); promptAction = 3; statusMessage[0] = '\0'; currentState = PROMPT_ID; }
        else if (mx >= 400 && mx <= 650 && my >= 150 && my <= 200) { tbRxApptID.clear(); tbRxMeds.clear(); tbRxNotes.clear(); statusMessage[0] = '\0'; currentState = DOC_WRITE_RX; }
        else if (mx >= 400 && mx <= 650 && my >= 220 && my <= 270) { tbPromptID.clear(); promptAction = 4; statusMessage[0] = '\0'; currentState = PROMPT_ID; }
        else if (mx >= 300 && mx <= 500 && my >= 500 && my <= 540) { systemApp.logout(); loggedInUserID = 0; currentState = LOGIN; }
    }
}
void MediCoreGUI::drawDoctorMenu() {
    sf::Text title("Doctor Dashboard", font, 36); title.setFillColor(sf::Color(0, 51, 102)); title.setPosition(250, 30); window.draw(title);

    Doctor* d = systemApp.doctors.findByID(loggedInUserID);
    if (d) {
        char msg[200]; copyStr(msg, "Welcome, Dr. "); myStrCat(msg, d->getName());
        myStrCat(msg, "   |   Specialization: "); myStrCat(msg, d->getSpecialization());
        sf::Text wTxt(msg, font, 18); wTxt.setFillColor(sf::Color(0, 102, 51)); wTxt.setPosition(50, 90); window.draw(wTxt);
    }

    sf::RectangleShape vBtn(sf::Vector2f(250, 50)); vBtn.setPosition(100, 150); vBtn.setFillColor(sf::Color(0, 102, 204)); sf::Text vText("View All Appts (GUI)", font, 16); vText.setFillColor(sf::Color::White); vText.setPosition(110, 165); window.draw(vBtn); window.draw(vText);
    sf::RectangleShape cBtn(sf::Vector2f(250, 50)); cBtn.setPosition(100, 220); cBtn.setFillColor(sf::Color(0, 153, 51)); sf::Text cText("Mark Appt Complete (GUI)", font, 16); cText.setFillColor(sf::Color::White); cText.setPosition(110, 235); window.draw(cBtn); window.draw(cText);
    sf::RectangleShape nBtn(sf::Vector2f(250, 50)); nBtn.setPosition(100, 290); nBtn.setFillColor(sf::Color(204, 102, 0)); sf::Text nText("Mark Appt No-Show (GUI)", font, 16); nText.setFillColor(sf::Color::White); nText.setPosition(110, 305); window.draw(nBtn); window.draw(nText);
    sf::RectangleShape wBtn(sf::Vector2f(250, 50)); wBtn.setPosition(400, 150); wBtn.setFillColor(sf::Color(102, 0, 153)); sf::Text wText("+ Write Prescription (GUI)", font, 16); wText.setFillColor(sf::Color::White); wText.setPosition(410, 165); window.draw(wBtn); window.draw(wText);
    sf::RectangleShape hBtn(sf::Vector2f(250, 50)); hBtn.setPosition(400, 220); hBtn.setFillColor(sf::Color(0, 102, 204)); sf::Text hText("Patient History (GUI)", font, 16); hText.setFillColor(sf::Color::White); hText.setPosition(410, 235); window.draw(hBtn); window.draw(hText);
    sf::RectangleShape logoutBtn(sf::Vector2f(200, 40)); logoutBtn.setPosition(300, 500); logoutBtn.setFillColor(sf::Color(153, 0, 0)); sf::Text logoutText("LOGOUT", font, 20); logoutText.setFillColor(sf::Color::White); logoutText.setPosition(355, 508); window.draw(logoutBtn); window.draw(logoutText);
}

void MediCoreGUI::handleAdminEvents(sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f m = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)); float mx = m.x; float my = m.y;
        if (mx >= 50 && mx <= 300 && my >= 150 && my <= 190) { listType = 4; listPage = 0; currentState = VIEW_LIST; }
        else if (mx >= 50 && mx <= 300 && my >= 210 && my <= 250) { tbName.clear(); tbAge.clear(); tbGender.clear(); tbContact.clear(); tbPass.clear(); tbBal.clear(); statusMessage[0] = '\0'; currentState = ADMIN_ADD_PATIENT; }
        else if (mx >= 50 && mx <= 300 && my >= 270 && my <= 310) { tbPromptID.clear(); promptAction = 1; statusMessage[0] = '\0'; currentState = PROMPT_ID; }
        else if (mx >= 50 && mx <= 300 && my >= 330 && my <= 370) { listType = 8; listPage = 0; currentState = VIEW_LIST; }
        else if (mx >= 50 && mx <= 300 && my >= 390 && my <= 430) { listType = 7; listPage = 0; currentState = VIEW_LIST; }

        else if (mx >= 450 && mx <= 700 && my >= 150 && my <= 190) { listType = 5; listPage = 0; currentState = VIEW_LIST; }
        else if (mx >= 450 && mx <= 700 && my >= 210 && my <= 250) { tbDName.clear(); tbDSpec.clear(); tbDContact.clear(); tbDPass.clear(); tbDFee.clear(); statusMessage[0] = '\0'; currentState = ADMIN_ADD_DOCTOR; }
        else if (mx >= 450 && mx <= 700 && my >= 270 && my <= 310) { tbPromptID.clear(); promptAction = 9; statusMessage[0] = '\0'; currentState = PROMPT_ID; }
        else if (mx >= 450 && mx <= 700 && my >= 330 && my <= 370) { systemApp.viewSecurityLog(); copyStr(statusMessage, "Check Console for Security Log!"); }
        else if (mx >= 450 && mx <= 700 && my >= 390 && my <= 430) { statusMessage[0] = '\0'; currentState = VIEW_REPORT; }
        else if (mx >= 300 && mx <= 500 && my >= 500 && my <= 540) { systemApp.logout(); loggedInUserID = 0; currentState = LOGIN; }
    }
}
void MediCoreGUI::drawAdminMenu() {
    sf::Text title("Admin Dashboard", font, 36); title.setFillColor(sf::Color(0, 51, 102)); title.setPosition(250, 30); window.draw(title);

    Admin* a = systemApp.admins.findByID(loggedInUserID);
    if (a) {
        char msg[100]; copyStr(msg, "Welcome, "); myStrCat(msg, a->getName());
        sf::Text wTxt(msg, font, 18); wTxt.setFillColor(sf::Color(0, 102, 51)); wTxt.setPosition(50, 90); window.draw(wTxt);
    }

    sf::RectangleShape b1(sf::Vector2f(250, 40)); b1.setPosition(50, 150); b1.setFillColor(sf::Color(0, 102, 204)); sf::Text t1("View All Patients (GUI)", font, 16); t1.setFillColor(sf::Color::White); t1.setPosition(60, 160); window.draw(b1); window.draw(t1);
    sf::RectangleShape b2(sf::Vector2f(250, 40)); b2.setPosition(50, 210); b2.setFillColor(sf::Color(0, 153, 51)); sf::Text t2("+ Add Patient (GUI)", font, 16); t2.setFillColor(sf::Color::White); t2.setPosition(60, 220); window.draw(b2); window.draw(t2);
    sf::RectangleShape b3(sf::Vector2f(250, 40)); b3.setPosition(50, 270); b3.setFillColor(sf::Color(204, 51, 0)); sf::Text t3("- Remove Patient (GUI)", font, 16); t3.setFillColor(sf::Color::White); t3.setPosition(60, 280); window.draw(b3); window.draw(t3);
    sf::RectangleShape b4(sf::Vector2f(250, 40)); b4.setPosition(50, 330); b4.setFillColor(sf::Color(0, 102, 204)); sf::Text t4("View Unpaid Bills (GUI)", font, 16); t4.setFillColor(sf::Color::White); t4.setPosition(60, 340); window.draw(b4); window.draw(t4);
    sf::RectangleShape b5(sf::Vector2f(250, 40)); b5.setPosition(50, 390); b5.setFillColor(sf::Color(0, 102, 204)); sf::Text t5("View All Appointments (GUI)", font, 16); t5.setFillColor(sf::Color::White); t5.setPosition(60, 400); window.draw(b5); window.draw(t5);

    sf::RectangleShape b6(sf::Vector2f(250, 40)); b6.setPosition(450, 150); b6.setFillColor(sf::Color(0, 102, 204)); sf::Text t6("View All Doctors (GUI)", font, 16); t6.setFillColor(sf::Color::White); t6.setPosition(460, 160); window.draw(b6); window.draw(t6);
    sf::RectangleShape b7(sf::Vector2f(250, 40)); b7.setPosition(450, 210); b7.setFillColor(sf::Color(0, 153, 51)); sf::Text t7("+ Add Doctor (GUI)", font, 16); t7.setFillColor(sf::Color::White); t7.setPosition(460, 220); window.draw(b7); window.draw(t7);
    sf::RectangleShape b8(sf::Vector2f(250, 40)); b8.setPosition(450, 270); b8.setFillColor(sf::Color(204, 51, 0)); sf::Text t8("- Remove Doctor (GUI)", font, 16); t8.setFillColor(sf::Color::White); t8.setPosition(460, 280); window.draw(b8); window.draw(t8);
    sf::RectangleShape b9(sf::Vector2f(250, 40)); b9.setPosition(450, 330); b9.setFillColor(sf::Color(102, 102, 102)); sf::Text t9("View Security Log (Console)", font, 16); t9.setFillColor(sf::Color::White); t9.setPosition(460, 340); window.draw(b9); window.draw(t9);
    sf::RectangleShape b10(sf::Vector2f(250, 40)); b10.setPosition(450, 390); b10.setFillColor(sf::Color(102, 102, 102)); sf::Text t10("Daily Report (GUI)", font, 16); t10.setFillColor(sf::Color::White); t10.setPosition(460, 400); window.draw(b10); window.draw(t10);

    sf::RectangleShape logoutBtn(sf::Vector2f(200, 40)); logoutBtn.setPosition(300, 500); logoutBtn.setFillColor(sf::Color(153, 0, 0));
    sf::Text logoutText("LOGOUT", font, 20); logoutText.setFillColor(sf::Color::White); logoutText.setPosition(355, 508); window.draw(logoutBtn); window.draw(logoutText);
    sf::Text status(statusMessage, font, 16); status.setFillColor(sf::Color::Red); status.setPosition(250, 110); window.draw(status);
}

// FORMS & UNIVERSAL PROMPT
void MediCoreGUI::handleBookApptEvents(sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f m = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)); float mx = m.x; float my = m.y;
        tbBookDocID.checkClick(mx, my); tbBookDate.checkClick(mx, my); tbBookTime.checkClick(mx, my);
        if (mx >= 300 && mx <= 500 && my >= 460 && my <= 500) {
            try {
                systemApp.bookAppointment(loggedInUserID, myAtoi(tbBookDocID.getText()), tbBookDate.getText(), tbBookTime.getText());
                copyStr(statusMessage, "Appointment Booked!"); tbBookDocID.clear(); tbBookDate.clear(); tbBookTime.clear();
            }
            catch (const HospitalException& e) { copyStr(statusMessage, e.what()); }
        }
        else if (mx >= 300 && mx <= 500 && my >= 520 && my <= 560) { currentState = PATIENT_MENU; }
    }
    tbBookDocID.typeText(event); tbBookDate.typeText(event); tbBookTime.typeText(event);
}
void MediCoreGUI::drawBookApptScreen() {
    sf::Text title("Book Appointment", font, 36); title.setFillColor(sf::Color(0, 51, 102)); title.setPosition(250, 50); window.draw(title);
    sf::Text l1("Doctor ID:", font, 18); l1.setFillColor(sf::Color::Black); l1.setPosition(200, 155); window.draw(l1); tbBookDocID.draw(window);
    sf::Text l2("Date (DD-MM-YYYY):", font, 18); l2.setFillColor(sf::Color::Black); l2.setPosition(110, 205); window.draw(l2); tbBookDate.draw(window);
    sf::Text l3("Time (HH:MM):", font, 18); l3.setFillColor(sf::Color::Black); l3.setPosition(150, 255); window.draw(l3); tbBookTime.draw(window);
    sf::RectangleShape subBtn(sf::Vector2f(200, 40)); subBtn.setPosition(300, 460); subBtn.setFillColor(sf::Color(0, 153, 51));
    sf::Text subText("SUBMIT", font, 20); subText.setFillColor(sf::Color::White); subText.setPosition(360, 468); window.draw(subBtn); window.draw(subText);
    sf::RectangleShape bBtn(sf::Vector2f(200, 40)); bBtn.setPosition(300, 520); bBtn.setFillColor(sf::Color(153, 0, 0));
    sf::Text bText("BACK", font, 20); bText.setFillColor(sf::Color::White); bText.setPosition(370, 528); window.draw(bBtn); window.draw(bText);
    sf::Text status(statusMessage, font, 16); status.setFillColor(sf::Color::Red); status.setPosition(300, 120); window.draw(status);
}

void MediCoreGUI::handlePromptSlotsEvents(sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f m = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)); float mx = m.x; float my = m.y;
        tbBookDocID.checkClick(mx, my); tbBookDate.checkClick(mx, my);
        if (mx >= 300 && mx <= 500 && my >= 460 && my <= 500) {
            searchDocID = myAtoi(tbBookDocID.getText()); copyStr(searchDate, tbBookDate.getText());
            listType = 10; listPage = 0; currentState = VIEW_LIST; tbBookDocID.clear(); tbBookDate.clear(); 
        }
        else if (mx >= 300 && mx <= 500 && my >= 520 && my <= 560) { currentState = PATIENT_MENU; }
    }
    tbBookDocID.typeText(event); tbBookDate.typeText(event);
}
void MediCoreGUI::drawPromptSlotsScreen() {
    sf::Text title("View Available Time Slots", font, 36); title.setFillColor(sf::Color(0, 51, 102)); title.setPosition(200, 50); window.draw(title);
    sf::Text l1("Doctor ID:", font, 18); l1.setFillColor(sf::Color::Black); l1.setPosition(200, 155); window.draw(l1); tbBookDocID.draw(window);
    sf::Text l2("Date (DD-MM-YYYY):", font, 18); l2.setFillColor(sf::Color::Black); l2.setPosition(110, 205); window.draw(l2); tbBookDate.draw(window);
    sf::RectangleShape subBtn(sf::Vector2f(200, 40)); subBtn.setPosition(300, 460); subBtn.setFillColor(sf::Color(0, 153, 51)); sf::Text subText("SUBMIT", font, 20); subText.setFillColor(sf::Color::White); subText.setPosition(360, 468); window.draw(subBtn); window.draw(subText);
    sf::RectangleShape bBtn(sf::Vector2f(200, 40)); bBtn.setPosition(300, 520); bBtn.setFillColor(sf::Color(153, 0, 0)); sf::Text bText("BACK", font, 20); bText.setFillColor(sf::Color::White); bText.setPosition(370, 528); window.draw(bBtn); window.draw(bText);
}

void MediCoreGUI::handleAddPatientEvents(sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f m = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)); float mx = m.x; float my = m.y;
        tbName.checkClick(mx, my); tbAge.checkClick(mx, my); tbGender.checkClick(mx, my); tbContact.checkClick(mx, my); tbPass.checkClick(mx, my); tbBal.checkClick(mx, my);
        if (mx >= 300 && mx <= 500 && my >= 460 && my <= 500) {
            try {
                systemApp.addPatient(tbName.getText(), myAtoi(tbAge.getText()), tbGender.getText()[0], tbContact.getText(), tbPass.getText(), myAtof(tbBal.getText()));
                copyStr(statusMessage, "Patient Added Successfully!"); tbName.clear(); tbAge.clear(); tbGender.clear(); tbContact.clear(); tbPass.clear(); tbBal.clear();
            }
            catch (const HospitalException& e) { copyStr(statusMessage, e.what()); }
        }
        else if (mx >= 300 && mx <= 500 && my >= 520 && my <= 560) { currentState = ADMIN_MENU; }
    }
    tbName.typeText(event); tbAge.typeText(event); tbGender.typeText(event); tbContact.typeText(event); tbPass.typeText(event); tbBal.typeText(event);
}
void MediCoreGUI::drawAddPatientScreen() {
    sf::Text title("Add New Patient", font, 36); title.setFillColor(sf::Color(0, 51, 102)); title.setPosition(250, 50); window.draw(title);
    sf::Text labels[6] = { sf::Text("Name:", font, 18), sf::Text("Age:", font, 18), sf::Text("Gender:", font, 18), sf::Text("Contact:", font, 18), sf::Text("Password:", font, 18), sf::Text("Balance:", font, 18) };
    for (int i = 0; i < 6; i++) { labels[i].setFillColor(sf::Color::Black); labels[i].setPosition(200, 155 + (i * 50)); window.draw(labels[i]); }
    tbName.draw(window); tbAge.draw(window); tbGender.draw(window); tbContact.draw(window); tbPass.draw(window); tbBal.draw(window);
    sf::RectangleShape subBtn(sf::Vector2f(200, 40)); subBtn.setPosition(300, 460); subBtn.setFillColor(sf::Color(0, 153, 51)); sf::Text subText("SUBMIT", font, 20); subText.setFillColor(sf::Color::White); subText.setPosition(360, 468); window.draw(subBtn); window.draw(subText);
    sf::RectangleShape bBtn(sf::Vector2f(200, 40)); bBtn.setPosition(300, 520); bBtn.setFillColor(sf::Color(153, 0, 0)); sf::Text bText("BACK", font, 20); bText.setFillColor(sf::Color::White); bText.setPosition(370, 528); window.draw(bBtn); window.draw(bText);
    sf::Text status(statusMessage, font, 16); status.setFillColor(sf::Color::Red); status.setPosition(300, 120); window.draw(status);
}

void MediCoreGUI::handleAddDoctorEvents(sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f m = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)); float mx = m.x; float my = m.y;
        tbDName.checkClick(mx, my); tbDSpec.checkClick(mx, my); tbDContact.checkClick(mx, my); tbDPass.checkClick(mx, my); tbDFee.checkClick(mx, my);
        if (mx >= 300 && mx <= 500 && my >= 460 && my <= 500) {
            try {
                systemApp.addDoctor(tbDName.getText(), tbDSpec.getText(), tbDContact.getText(), tbDPass.getText(), myAtof(tbDFee.getText()));
                copyStr(statusMessage, "Doctor Added Successfully!"); tbDName.clear(); tbDSpec.clear(); tbDContact.clear(); tbDPass.clear(); tbDFee.clear();
            }
            catch (const HospitalException& e) { copyStr(statusMessage, e.what()); }
        }
        else if (mx >= 300 && mx <= 500 && my >= 520 && my <= 560) { currentState = ADMIN_MENU; }
    }
    tbDName.typeText(event); tbDSpec.typeText(event); tbDContact.typeText(event); tbDPass.typeText(event); tbDFee.typeText(event);
}
void MediCoreGUI::drawAddDoctorScreen() {
    sf::Text title("Add New Doctor", font, 36); title.setFillColor(sf::Color(0, 51, 102)); title.setPosition(250, 50); window.draw(title);
    sf::Text labels[5] = { sf::Text("Name:", font, 18), sf::Text("Specialty:", font, 18), sf::Text("Contact:", font, 18), sf::Text("Password:", font, 18), sf::Text("Fee:", font, 18) };
    for (int i = 0; i < 5; i++) { labels[i].setFillColor(sf::Color::Black); labels[i].setPosition(200, 155 + (i * 50)); window.draw(labels[i]); }
    tbDName.draw(window); tbDSpec.draw(window); tbDContact.draw(window); tbDPass.draw(window); tbDFee.draw(window);
    sf::RectangleShape subBtn(sf::Vector2f(200, 40)); subBtn.setPosition(300, 460); subBtn.setFillColor(sf::Color(0, 153, 51)); sf::Text subText("SUBMIT", font, 20); subText.setFillColor(sf::Color::White); subText.setPosition(360, 468); window.draw(subBtn); window.draw(subText);
    sf::RectangleShape bBtn(sf::Vector2f(200, 40)); bBtn.setPosition(300, 520); bBtn.setFillColor(sf::Color(153, 0, 0)); sf::Text bText("BACK", font, 20); bText.setFillColor(sf::Color::White); bText.setPosition(370, 528); window.draw(bBtn); window.draw(bText);
    sf::Text status(statusMessage, font, 16); status.setFillColor(sf::Color::Red); status.setPosition(300, 120); window.draw(status);
}

void MediCoreGUI::handleWriteRxEvents(sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f m = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)); float mx = m.x; float my = m.y;
        tbRxApptID.checkClick(mx, my); tbRxMeds.checkClick(mx, my); tbRxNotes.checkClick(mx, my);
        if (mx >= 300 && mx <= 500 && my >= 460 && my <= 500) {
            try {
                systemApp.writePrescription(loggedInUserID, myAtoi(tbRxApptID.getText()), tbRxMeds.getText(), tbRxNotes.getText());
                copyStr(statusMessage, "Prescription Saved!"); tbRxApptID.clear(); tbRxMeds.clear(); tbRxNotes.clear();
            }
            catch (const HospitalException& e) { copyStr(statusMessage, e.what()); }
        }
        else if (mx >= 300 && mx <= 500 && my >= 520 && my <= 560) { currentState = DOCTOR_MENU; }
    }
    tbRxApptID.typeText(event); tbRxMeds.typeText(event); tbRxNotes.typeText(event);
}
void MediCoreGUI::drawWriteRxScreen() {
    sf::Text title("Write Prescription", font, 36); title.setFillColor(sf::Color(0, 51, 102)); title.setPosition(250, 50); window.draw(title);
    sf::Text l1("Appt ID:", font, 18); l1.setFillColor(sf::Color::Black); l1.setPosition(200, 155); window.draw(l1); tbRxApptID.draw(window);
    sf::Text l2("Meds:", font, 18); l2.setFillColor(sf::Color::Black); l2.setPosition(200, 205); window.draw(l2); tbRxMeds.draw(window);
    sf::Text l3("Notes:", font, 18); l3.setFillColor(sf::Color::Black); l3.setPosition(200, 255); window.draw(l3); tbRxNotes.draw(window);
    sf::RectangleShape subBtn(sf::Vector2f(200, 40)); subBtn.setPosition(300, 460); subBtn.setFillColor(sf::Color(0, 153, 51)); sf::Text subText("SUBMIT", font, 20); subText.setFillColor(sf::Color::White); subText.setPosition(360, 468); window.draw(subBtn); window.draw(subText);
    sf::RectangleShape bBtn(sf::Vector2f(200, 40)); bBtn.setPosition(300, 520); bBtn.setFillColor(sf::Color(153, 0, 0)); sf::Text bText("BACK", font, 20); bText.setFillColor(sf::Color::White); bText.setPosition(370, 528); window.draw(bBtn); window.draw(bText);
    sf::Text status(statusMessage, font, 16); status.setFillColor(sf::Color::Red); status.setPosition(300, 120); window.draw(status);
}

void MediCoreGUI::handlePromptIDEvents(sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f m = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)); float mx = m.x; float my = m.y;
        tbPromptID.checkClick(mx, my);
        if (mx >= 300 && mx <= 500 && my >= 360 && my <= 400) {
            try {
                if (promptAction == 8) { 
                    copyStr(searchBuffer, tbPromptID.getText()); listType = 9; listPage = 0; currentState = VIEW_LIST; tbPromptID.clear();
                }
                else if (promptAction == 4) { 
                    searchPatID = myAtoi(tbPromptID.getText()); listType = 11; listPage = 0; currentState = VIEW_LIST; tbPromptID.clear();
                }
                else if (promptAction == 7) {
                    systemApp.topUpBalance(loggedInUserID, myAtof(tbPromptID.getText())); copyStr(statusMessage, "Balance Updated!"); tbPromptID.clear();
                }
                else {
                    int id = myAtoi(tbPromptID.getText());
                    if (promptAction == 1) { systemApp.removePatient(id); copyStr(statusMessage, "Patient Removed!"); }
                    else if (promptAction == 2) { systemApp.markAppointmentComplete(loggedInUserID, id); copyStr(statusMessage, "Marked Complete!"); }
                    else if (promptAction == 3) { systemApp.markAppointmentNoShow(loggedInUserID, id); copyStr(statusMessage, "Marked No-Show!"); }
                    else if (promptAction == 5) { systemApp.cancelAppointment(loggedInUserID, id); copyStr(statusMessage, "Cancelled & Refunded!"); }
                    else if (promptAction == 6) { systemApp.payBill(loggedInUserID, id); copyStr(statusMessage, "Bill Paid!"); }
                    else if (promptAction == 9) { systemApp.removeDoctor(id); copyStr(statusMessage, "Doctor Removed!"); }
                    tbPromptID.clear();
                }
            }
            catch (const HospitalException& e) { copyStr(statusMessage, e.what()); }
        }
        else if (mx >= 300 && mx <= 500 && my >= 420 && my <= 460) {
            if (promptAction == 1 || promptAction == 9) currentState = ADMIN_MENU;
            else if (promptAction >= 5 && promptAction <= 8) currentState = PATIENT_MENU;
            else currentState = DOCTOR_MENU;
        }
    }
    tbPromptID.typeText(event);
}
void MediCoreGUI::drawPromptIDScreen() {
    const char* tStr = "Enter ID";
    if (promptAction == 1) tStr = "Remove Patient ID"; else if (promptAction == 2) tStr = "Mark Appt Complete"; else if (promptAction == 3) tStr = "Mark Appt No-Show";
    else if (promptAction == 4) tStr = "View History for Pat ID"; else if (promptAction == 5) tStr = "Cancel Appointment ID"; else if (promptAction == 6) tStr = "Pay Bill ID";
    else if (promptAction == 7) tStr = "Enter Top Up Amount"; else if (promptAction == 8) tStr = "Enter Spec (e.g. Cardiology)"; else if (promptAction == 9) tStr = "Remove Doctor ID";

    sf::Text title(tStr, font, 24); title.setFillColor(sf::Color(0, 51, 102)); title.setPosition(150, 100); window.draw(title);
    sf::Text l1("Input:", font, 18); l1.setFillColor(sf::Color::Black); l1.setPosition(220, 255); window.draw(l1); tbPromptID.draw(window);

    sf::RectangleShape subBtn(sf::Vector2f(200, 40)); subBtn.setPosition(300, 360); subBtn.setFillColor(sf::Color(0, 153, 51)); sf::Text subText("CONFIRM", font, 20); subText.setFillColor(sf::Color::White); subText.setPosition(350, 368); window.draw(subBtn); window.draw(subText);
    sf::RectangleShape bBtn(sf::Vector2f(200, 40)); bBtn.setPosition(300, 420); bBtn.setFillColor(sf::Color(153, 0, 0)); sf::Text bText("BACK", font, 20); bText.setFillColor(sf::Color::White); bText.setPosition(370, 428); window.draw(bBtn); window.draw(bText);
    sf::Text status(statusMessage, font, 16); status.setFillColor(sf::Color::Red); status.setPosition(250, 180); window.draw(status);
}

// LIST VIEWER 

void MediCoreGUI::handleViewListEvents(sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f m = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)); float mx = m.x; float my = m.y;
        if (mx >= 100 && mx <= 200 && my >= 520 && my <= 560) { if (listPage > 0) listPage--; }
        else if (mx >= 500 && mx <= 600 && my >= 520 && my <= 560) { listPage++; }
        else if (mx >= 300 && mx <= 400 && my >= 520 && my <= 560) {
            if (selectedRole == 1) currentState = PATIENT_MENU; else if (selectedRole == 2) currentState = DOCTOR_MENU; else if (selectedRole == 3) currentState = ADMIN_MENU;
        }
    }
}
void MediCoreGUI::drawViewListScreen() {
    sf::Text title("Database Viewer", font, 30); title.setFillColor(sf::Color(0, 51, 102)); title.setPosition(50, 30); window.draw(title);
    sf::RectangleShape hBg(sf::Vector2f(700, 30)); hBg.setPosition(50, 80); hBg.setFillColor(sf::Color(200, 200, 200)); window.draw(hBg);

    char h1[50] = "ID", h2[50] = "Col2", h3[50] = "Col3", h4[50] = "Col4";
    int startIdx = listPage * 8; int itemsDrawn = 0; char buffer[50], buf2[50];

    if (listType == 1 || listType == 3) {
        copyStr(h2, "Date"); copyStr(h3, "Time"); copyStr(h4, "Status");
        Appointment* arr = systemApp.appointments.getAll();
        int size = systemApp.appointments.getSize();
        int matchCount = 0;

  
        char today[15];
        time_t t = time(NULL);
        struct tm* tm_info = localtime(&t);
        strftime(today, 15, "%d-%m-%Y", tm_info);

        for (int i = 0; i < size; i++) {
          
            bool isPatMatch = (listType == 1 && arr[i].getPatientID() == loggedInUserID);
            bool isDocMatch = (listType == 3 && arr[i].getDoctorID() == loggedInUserID && myStrCmp(arr[i].getDate(), today) == 0);

            if (isPatMatch || isDocMatch) {
                if (matchCount >= startIdx && itemsDrawn < 8) {
                    float y = 120 + (itemsDrawn * 45);
                    myItoa(arr[i].getID(), buffer); sf::Text t1(buffer, font, 18); t1.setFillColor(sf::Color::Black); t1.setPosition(60, y); window.draw(t1);
                    sf::Text t2(arr[i].getDate(), font, 18); t2.setFillColor(sf::Color::Black); t2.setPosition(150, y); window.draw(t2);
                    sf::Text t3(arr[i].getTimeSlot(), font, 18); t3.setFillColor(sf::Color::Black); t3.setPosition(350, y); window.draw(t3);
                    sf::Text t4(arr[i].getStatus(), font, 18); t4.setFillColor(sf::Color::Black); t4.setPosition(550, y); window.draw(t4);
                    sf::RectangleShape line(sf::Vector2f(700, 1)); line.setPosition(50, y + 30); line.setFillColor(sf::Color(150, 150, 150)); window.draw(line);
                    itemsDrawn++;
                }
                matchCount++;
            }
        }
    }
    else if (listType == 2) {
        copyStr(h2, "Amount"); copyStr(h3, "Status"); copyStr(h4, "Date");
        Bill* arr = systemApp.bills.getAll(); int size = systemApp.bills.getSize(); int matchCount = 0;
        for (int i = 0; i < size; i++) {
            if (arr[i].getPatientID() == loggedInUserID) {
                if (matchCount >= startIdx && itemsDrawn < 8) {
                    float y = 120 + (itemsDrawn * 45);
                    myItoa(arr[i].getID(), buffer); sf::Text t1(buffer, font, 18); t1.setFillColor(sf::Color::Black); t1.setPosition(60, y); window.draw(t1);
                    myItoa((int)arr[i].getAmount(), buffer); sf::Text t2(buffer, font, 18); t2.setFillColor(sf::Color::Black); t2.setPosition(150, y); window.draw(t2);
                    sf::Text t3(arr[i].getStatus(), font, 18); t3.setFillColor(sf::Color::Black); t3.setPosition(350, y); window.draw(t3);
                    sf::Text t4(arr[i].getDate(), font, 18); t4.setFillColor(sf::Color::Black); t4.setPosition(550, y); window.draw(t4);
                    sf::RectangleShape line(sf::Vector2f(700, 1)); line.setPosition(50, y + 30); line.setFillColor(sf::Color(150, 150, 150)); window.draw(line); itemsDrawn++;
                } matchCount++;
            }
        }
    }
    else if (listType == 4) {
        copyStr(h2, "Name"); copyStr(h3, "Contact"); copyStr(h4, "Balance");
        Patient* arr = systemApp.patients.getAll(); int size = systemApp.patients.getSize();
        for (int i = startIdx; i < size && itemsDrawn < 8; i++) {
            float y = 120 + (itemsDrawn * 45);
            myItoa(arr[i].getID(), buffer); sf::Text t1(buffer, font, 18); t1.setFillColor(sf::Color::Black); t1.setPosition(60, y); window.draw(t1);
            sf::Text t2(arr[i].getName(), font, 18); t2.setFillColor(sf::Color::Black); t2.setPosition(150, y); window.draw(t2);
            sf::Text t3(arr[i].getContact(), font, 18); t3.setFillColor(sf::Color::Black); t3.setPosition(350, y); window.draw(t3);
            myItoa((int)arr[i].getBalance(), buffer); sf::Text t4(buffer, font, 18); t4.setFillColor(sf::Color::Black); t4.setPosition(550, y); window.draw(t4);
            sf::RectangleShape line(sf::Vector2f(700, 1)); line.setPosition(50, y + 30); line.setFillColor(sf::Color(150, 150, 150)); window.draw(line); itemsDrawn++;
        }
    }
    else if (listType == 5 || listType == 9) {
        copyStr(h2, "Name"); copyStr(h3, "Specialty"); copyStr(h4, "Fee");
        Doctor* arr = systemApp.doctors.getAll(); int size = systemApp.doctors.getSize(); int matchCount = 0;
        for (int i = 0; i < size; i++) {
            
            if (listType == 5 || myStrEqualsIgnoreCase(arr[i].getSpecialization(), searchBuffer)) {
                if (matchCount >= startIdx && itemsDrawn < 8) {
                    float y = 120 + (itemsDrawn * 45);
                    myItoa(arr[i].getID(), buffer); sf::Text t1(buffer, font, 18); t1.setFillColor(sf::Color::Black); t1.setPosition(60, y); window.draw(t1);
                    sf::Text t2(arr[i].getName(), font, 18); t2.setFillColor(sf::Color::Black); t2.setPosition(150, y); window.draw(t2);
                    sf::Text t3(arr[i].getSpecialization(), font, 18); t3.setFillColor(sf::Color::Black); t3.setPosition(350, y); window.draw(t3);
                    myItoa((int)arr[i].getFee(), buffer); sf::Text t4(buffer, font, 18); t4.setFillColor(sf::Color::Black); t4.setPosition(550, y); window.draw(t4);
                    sf::RectangleShape line(sf::Vector2f(700, 1)); line.setPosition(50, y + 30); line.setFillColor(sf::Color(150, 150, 150)); window.draw(line); itemsDrawn++;
                } matchCount++;
            }
        }
    }
    else if (listType == 6 || listType == 11) { 
        copyStr(h1, "Rx ID"); copyStr(h2, "Date"); copyStr(h3, "Medicines"); copyStr(h4, "Notes");
        Prescription* arr = systemApp.prescriptions.getAll(); int size = systemApp.prescriptions.getSize(); int matchCount = 0;
        for (int i = 0; i < size; i++) {
            if ((listType == 6 && arr[i].getPatientID() == loggedInUserID) || (listType == 11 && arr[i].getPatientID() == searchPatID && arr[i].getDoctorID() == loggedInUserID)) {
                if (matchCount >= startIdx && itemsDrawn < 8) {
                    float y = 120 + (itemsDrawn * 45);
                    myItoa(arr[i].getID(), buffer); sf::Text t1(buffer, font, 18); t1.setFillColor(sf::Color::Black); t1.setPosition(60, y); window.draw(t1);
                    sf::Text t2(arr[i].getDate(), font, 18); t2.setFillColor(sf::Color::Black); t2.setPosition(150, y); window.draw(t2);
                    char m[25]; copyStr(m, arr[i].getMedicines()); if (getStrLen(m) > 20) { m[20] = '.'; m[21] = '.'; m[22] = '.'; m[23] = '\0'; }
                    sf::Text t3(m, font, 18); t3.setFillColor(sf::Color::Black); t3.setPosition(350, y); window.draw(t3);
                    char n[25]; copyStr(n, arr[i].getNotes()); if (getStrLen(n) > 20) { n[20] = '.'; n[21] = '.'; n[22] = '.'; n[23] = '\0'; }
                    sf::Text t4(n, font, 18); t4.setFillColor(sf::Color::Black); t4.setPosition(550, y); window.draw(t4);
                    sf::RectangleShape line(sf::Vector2f(700, 1)); line.setPosition(50, y + 30); line.setFillColor(sf::Color(150, 150, 150)); window.draw(line); itemsDrawn++;
                } matchCount++;
            }
        }
    }
    else if (listType == 7) {
        copyStr(h2, "Pat ID"); copyStr(h3, "Doc ID"); copyStr(h4, "Date / Time");
        Appointment* arr = systemApp.appointments.getAll(); int size = systemApp.appointments.getSize();
        for (int i = startIdx; i < size && itemsDrawn < 8; i++) {
            float y = 120 + (itemsDrawn * 45);
            myItoa(arr[i].getID(), buffer); sf::Text t1(buffer, font, 18); t1.setFillColor(sf::Color::Black); t1.setPosition(60, y); window.draw(t1);
            myItoa(arr[i].getPatientID(), buf2); sf::Text t2(buf2, font, 18); t2.setFillColor(sf::Color::Black); t2.setPosition(150, y); window.draw(t2);
            myItoa(arr[i].getDoctorID(), buffer); sf::Text t3(buffer, font, 18); t3.setFillColor(sf::Color::Black); t3.setPosition(350, y); window.draw(t3);
            char dt[40]; copyStr(dt, arr[i].getDate()); int len = getStrLen(dt); dt[len] = ' '; dt[len + 1] = '\0'; char* ptr = dt + len + 1; copyStr(ptr, arr[i].getTimeSlot());
            sf::Text t4(dt, font, 18); t4.setFillColor(sf::Color::Black); t4.setPosition(550, y); window.draw(t4);
            sf::RectangleShape line(sf::Vector2f(700, 1)); line.setPosition(50, y + 30); line.setFillColor(sf::Color(150, 150, 150)); window.draw(line); itemsDrawn++;
        }
    }
    else if (listType == 8) {
        copyStr(h2, "Pat ID"); copyStr(h3, "Amount"); copyStr(h4, "Date");
        Bill* arr = systemApp.bills.getAll(); int size = systemApp.bills.getSize(); int matchCount = 0;
        for (int i = 0; i < size; i++) {
            if (myStrCmp(arr[i].getStatus(), "unpaid") == 0) {
                if (matchCount >= startIdx && itemsDrawn < 8) {
                    float y = 120 + (itemsDrawn * 45);
                    myItoa(arr[i].getID(), buffer); sf::Text t1(buffer, font, 18); t1.setFillColor(sf::Color::Black); t1.setPosition(60, y); window.draw(t1);
                    myItoa(arr[i].getPatientID(), buf2); sf::Text t2(buf2, font, 18); t2.setFillColor(sf::Color::Black); t2.setPosition(150, y); window.draw(t2);
                    myItoa((int)arr[i].getAmount(), buffer); sf::Text t3(buffer, font, 18); t3.setFillColor(sf::Color::Black); t3.setPosition(350, y); window.draw(t3);
                    sf::Text t4(arr[i].getDate(), font, 18); t4.setFillColor(sf::Color::Black); t4.setPosition(550, y); window.draw(t4);
                    sf::RectangleShape line(sf::Vector2f(700, 1)); line.setPosition(50, y + 30); line.setFillColor(sf::Color(150, 150, 150)); window.draw(line); itemsDrawn++;
                } matchCount++;
            }
        }
    }
    else if (listType == 10) { 
        copyStr(h1, "Slot #"); copyStr(h2, "Time"); copyStr(h3, "Status"); copyStr(h4, "");
        const char* allSlots[8] = { "09:00", "10:00", "11:00", "12:00", "13:00", "14:00", "15:00", "16:00" };
        Appointment* appts = systemApp.appointments.getAll(); int size = systemApp.appointments.getSize();

        for (int i = startIdx; i < 8 && itemsDrawn < 8; i++) {
            bool isBooked = false;
            for (int j = 0; j < size; j++) {
                if (appts[j].getDoctorID() == searchDocID && myStrCmp(appts[j].getDate(), searchDate) == 0 &&
                    myStrCmp(appts[j].getTimeSlot(), allSlots[i]) == 0 && myStrCmp(appts[j].getStatus(), "pending") == 0) {
                    isBooked = true; break;
                }
            }
            float y = 120 + (itemsDrawn * 45);
            myItoa(i + 1, buffer); sf::Text t1(buffer, font, 18); t1.setFillColor(sf::Color::Black); t1.setPosition(60, y); window.draw(t1);
            sf::Text t2(allSlots[i], font, 18); t2.setFillColor(sf::Color::Black); t2.setPosition(150, y); window.draw(t2);
            sf::Text t3(isBooked ? "Booked" : "Available", font, 18); t3.setFillColor(isBooked ? sf::Color::Red : sf::Color(0, 153, 51)); t3.setPosition(350, y); window.draw(t3);
            sf::RectangleShape line(sf::Vector2f(700, 1)); line.setPosition(50, y + 30); line.setFillColor(sf::Color(150, 150, 150)); window.draw(line); itemsDrawn++;
        }
    }

    sf::Text th1(h1, font, 18); th1.setFillColor(sf::Color::Black); th1.setPosition(60, 85); window.draw(th1);
    sf::Text th2(h2, font, 18); th2.setFillColor(sf::Color::Black); th2.setPosition(150, 85); window.draw(th2);
    sf::Text th3(h3, font, 18); th3.setFillColor(sf::Color::Black); th3.setPosition(350, 85); window.draw(th3);
    sf::Text th4(h4, font, 18); th4.setFillColor(sf::Color::Black); th4.setPosition(550, 85); window.draw(th4);

    sf::RectangleShape pBtn(sf::Vector2f(100, 40)); pBtn.setPosition(100, 520); pBtn.setFillColor(sf::Color(100, 100, 100));
    sf::Text pTxt("< PREV", font, 18); pTxt.setFillColor(sf::Color::White); pTxt.setPosition(115, 528); window.draw(pBtn); window.draw(pTxt);
    sf::RectangleShape bBtn(sf::Vector2f(100, 40)); bBtn.setPosition(300, 520); bBtn.setFillColor(sf::Color(153, 0, 0));
    sf::Text bTxt("BACK", font, 18); bTxt.setFillColor(sf::Color::White); bTxt.setPosition(320, 528); window.draw(bBtn); window.draw(bTxt);
    sf::RectangleShape nBtn(sf::Vector2f(100, 40)); nBtn.setPosition(500, 520); nBtn.setFillColor(sf::Color(100, 100, 100));
    sf::Text nTxt("NEXT >", font, 18); nTxt.setFillColor(sf::Color::White); nTxt.setPosition(515, 528); window.draw(nBtn); window.draw(nTxt);

    myItoa(listPage + 1, buffer); sf::Text pgTxt(buffer, font, 20); pgTxt.setFillColor(sf::Color::Black); pgTxt.setPosition(700, 528); window.draw(pgTxt);
}

// GRAPHICAL DAILY REPORT
void MediCoreGUI::handleViewReportEvents(sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f m = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

        if (m.x >= 300 && m.x <= 500 && m.y >= 520 && m.y <= 560) {
            currentState = ADMIN_MENU;
        }
    }
}

void MediCoreGUI::drawViewReportScreen() {
    char today[15];
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    strftime(today, 15, "%d-%m-%Y", tm_info);
    int totAppt = 0, pend = 0, comp = 0, noShow = 0, canc = 0;
    float rev = 0.0f;
    char buffer[50];

    Appointment* appts = systemApp.appointments.getAll();
    int apptSize = systemApp.appointments.getSize();
    for (int i = 0; i < apptSize; i++) {
        if (myStrCmp(appts[i].getDate(), today) == 0) {
            totAppt++;
            if (myStrCmp(appts[i].getStatus(), "pending") == 0) pend++;
            else if (myStrCmp(appts[i].getStatus(), "completed") == 0) comp++;
            else if (myStrCmp(appts[i].getStatus(), "no-show") == 0) noShow++;
            else if (myStrCmp(appts[i].getStatus(), "cancelled") == 0) canc++;
        }
    }

    Bill* bArr = systemApp.bills.getAll();
    int billSize = systemApp.bills.getSize();
    for (int i = 0; i < billSize; i++) {
        if (myStrCmp(bArr[i].getDate(), today) == 0 && myStrCmp(bArr[i].getStatus(), "paid") == 0) {
            rev += bArr[i].getAmount();
        }
    }

    sf::Text title("Daily Hospital Report", font, 36); title.setFillColor(sf::Color(0, 51, 102)); title.setPosition(220, 30); window.draw(title);

    sf::Text dLabel("Date: ", font, 20); dLabel.setFillColor(sf::Color::Black); dLabel.setPosition(300, 80); window.draw(dLabel);
    sf::Text dText(today, font, 20); dText.setFillColor(sf::Color(0, 102, 204)); dText.setPosition(360, 80); window.draw(dText);


    sf::RectangleShape bg(sf::Vector2f(700, 150)); bg.setPosition(50, 120); bg.setFillColor(sf::Color(230, 230, 230)); window.draw(bg);

    sf::Text tL("Total Appointments:", font, 20); tL.setFillColor(sf::Color::Black); tL.setPosition(70, 140); window.draw(tL);
    myItoa(totAppt, buffer); sf::Text tA(buffer, font, 20); tA.setFillColor(sf::Color::Red); tA.setPosition(250, 140); window.draw(tA);

  
    sf::Text bL("Pending: ", font, 18); bL.setFillColor(sf::Color(100, 100, 100)); bL.setPosition(70, 180); window.draw(bL);
    myItoa(pend, buffer); sf::Text bP(buffer, font, 18); bP.setFillColor(sf::Color::Black); bP.setPosition(150, 180); window.draw(bP);

    sf::Text cL("Completed: ", font, 18); cL.setFillColor(sf::Color(100, 100, 100)); cL.setPosition(200, 180); window.draw(cL);
    myItoa(comp, buffer); sf::Text bC(buffer, font, 18); bC.setFillColor(sf::Color(0, 153, 51)); bC.setPosition(300, 180); window.draw(bC);

    sf::Text nL("No-Show: ", font, 18); nL.setFillColor(sf::Color(100, 100, 100)); nL.setPosition(350, 180); window.draw(nL);
    myItoa(noShow, buffer); sf::Text bN(buffer, font, 18); bN.setFillColor(sf::Color(204, 102, 0)); bN.setPosition(440, 180); window.draw(bN);

    sf::Text caL("Cancelled: ", font, 18); caL.setFillColor(sf::Color(100, 100, 100)); caL.setPosition(500, 180); window.draw(caL);
    myItoa(canc, buffer); sf::Text bCa(buffer, font, 18); bCa.setFillColor(sf::Color(204, 51, 0)); bCa.setPosition(600, 180); window.draw(bCa);

    sf::Text rL("Total Revenue Today (PKR):", font, 20); rL.setFillColor(sf::Color::Black); rL.setPosition(70, 220); window.draw(rL);
    myItoa((int)rev, buffer); sf::Text rV(buffer, font, 20); rV.setFillColor(sf::Color(0, 153, 51)); rV.setPosition(320, 220); window.draw(rV);

  
    sf::Text uL("Unpaid Patients (Top 4):", font, 20); uL.setFillColor(sf::Color(0, 51, 102)); uL.setPosition(50, 300); window.draw(uL);
    Patient* pArr = systemApp.patients.getAll();
    int pSize = systemApp.patients.getSize();
    int drawn = 0;

    for (int i = 0; i < pSize && drawn < 4; i++) {
        float owed = 0;
        for (int j = 0; j < billSize; j++) {
            if (bArr[j].getPatientID() == pArr[i].getID() && myStrCmp(bArr[j].getStatus(), "unpaid") == 0) owed += bArr[j].getAmount();
        }
        if (owed > 0) {
            float y = 340 + (drawn * 30);
            sf::Text nT(pArr[i].getName(), font, 18); nT.setFillColor(sf::Color::Black); nT.setPosition(70, y); window.draw(nT);
            myItoa((int)owed, buffer); sf::Text oT(buffer, font, 18); oT.setFillColor(sf::Color::Red); oT.setPosition(300, y); window.draw(oT);
            drawn++;
        }
    }
    if (drawn == 0) { sf::Text none("No patients owe money.", font, 18); none.setFillColor(sf::Color(100, 100, 100)); none.setPosition(70, 340); window.draw(none); }

 
    sf::RectangleShape bBtn(sf::Vector2f(200, 40)); bBtn.setPosition(300, 520); bBtn.setFillColor(sf::Color(153, 0, 0));
    sf::Text bText("BACK TO MENU", font, 20); bText.setFillColor(sf::Color::White); bText.setPosition(325, 528); window.draw(bBtn); window.draw(bText);
}