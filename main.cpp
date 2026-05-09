#include <iostream>
#include "MediCoreSystem.h"
#include "MediCoreGUI.h"
using namespace std;
int main() {
    cout << "      MediCore Hospital System         \n";
    cout << "      Boot Sequence Initiated...       \n";

    try {
        MediCoreSystem app;
        app.loadData();
        MediCoreGUI gui(app);
        gui.run();

        cout << "\ngUI closed. Shutting down and saving data\n";
    }
    catch (...) {
        cout << "\nAn unknown exception occured\n";
    }
}