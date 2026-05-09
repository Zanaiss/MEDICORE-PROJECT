# MediCore Hospital Management System

**Author:** Zoraiz Khalid  
**Section:** BCS-2G  
**Roll Number:** 25L-0531
**GitHub Repository:** https://github.com/Zanaiss/MEDICORE-PROJECT

---

Project Overview
MediCore is a comprehensive, object-oriented Hospital Management System built in C++ with a fully graphical user interface powered by SFML. 

**Technical Highlights:**
* **Strict Memory Management:** Developed entirely without the standard `<string>` or `<vector>` libraries. All string manipulations and dynamic array resizing are handled via custom-built helper classes (`CustomStringHelper` and `Storage<T>`).
* **Persistent Data:** All system states (Patients, Doctors, Appointments, Bills, Prescriptions, Security Logs, and Discharged records) are dynamically read from and written to `.txt` files in real-time.
* **Graphical Interface:** Utilizes SFML to render interactive forms, clickable dashboards, and paginated dynamic database tables directly on the screen, complete with window resizing support.
* **Robust Validation:** Comprehensive exception handling ensures invalid inputs, scheduling conflicts, and insufficient funds are caught gracefully without crashing the system.

---
 Prerequisites & Dependencies
To compile and run this project, you will need:
* **IDE:** Visual Studio 2022 (Recommended)
* **Compiler:** MSVC (C++14 or higher)
* **Graphics Library:** SFML 2.6.x (64-bit)

---
 How to Compile and Run (Visual Studio)

If you are opening the provided `.sln` file, the project is already configured. However, if you are setting it up on a new machine, follow these steps to ensure SFML links correctly:

1. **Extract the ZIP:** Extract the submitted `[RollNumber].zip` to a local directory.
2. **Open the Solution:** Double-click the `.sln` file to open the project in Visual Studio.
3. **Set Build Configuration:** At the top of Visual Studio, ensure the configuration is set to **Debug** and the platform is set to **x64**.
4. **Verify DLLs:** Ensure the SFML `.dll` files (e.g., `sfml-graphics-2.dll`, `sfml-window-2.dll`, `sfml-system-2.dll`) are located in the same directory as the executable or the main project folder. *(These should already be included in the submission folder).*
5. **Compile & Run:** Press **F5** (Local Windows Debugger) to build and launch the application.

---
 Default System Credentials

Upon launching the application, use the following credentials to access the Administrator Dashboard and begin populating the system:

* **Role:** Admin
* **User ID:** `1`
* **Password:** `admin123`

*(Note: Additional Doctors and Patients can be added dynamically through the Admin GUI. Their IDs are auto-generated and displayed upon creation).*

---
 File Structure Reference
* `main.cpp`: Entry point and boot sequence.
* `MediCoreGUI.h/cpp`: SFML Frontend, event routing, and graphical table generation.
* `MediCoreSystem.h/cpp`: Core business logic, data routing, and report generation.
* `Storage.h`: Generic template class for dynamic array management.
* `FileHandler.h/cpp`: Handles all CSV-style `.txt` database reading and writing operations.
* `CustomStringHelper.h/cpp`: Custom C-string manipulation library (replacing `<string>`).
* `Validator.h/cpp`: Input validation and sanitization.
* `Exceptions.h/cpp`: Custom exception classes (e.g., `InsufficientFundsException`, `SlotUnavailableException`).
* `Person, Patient, Doctor, Admin, Appointment, Bill, Prescription`: Core entity classes demonstrating inheritance and operator overloading.