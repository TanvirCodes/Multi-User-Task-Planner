#include <iostream>
#include <limits>
#include "userManager.h"
using namespace std;

void mainMenu(UserManager &um) {
    int choice;

    // Main Application
    do {
        cout << "--------- Task Planner Main Menu ---------" << endl;
        cout << "1. Admin Dashboard" << endl;
        cout << "2. User Portal" << endl;
        cout << "3. Help Desk" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input! Please enter a number." << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer

        // Users menu selection
        switch (choice) {
            case 1: {
                if (um.adminLogin()) {
                    um.showAdminDashboard(); // Display system statistics
                    um.adminDashboardMenu(); // Display admin operations menu
                } else {
                    cout << "Invalid Admin credentials. Access denied." << endl;
                }
                break;
            }

            case 2: {
                system("cls");
                cout << "Welcome to User Portal!" << endl;
                um.userPortalMenu(); // Display User Portal System
                break;
            }

            case 3: {
                cout << "Help Desk is under development." << endl; // No features here
                break;
            }

            case 4: {
                cout << "Exiting program.........." << endl;
                break;
            }

            default: {
                cout << "Invalid choice." << endl;
                break;
            }
        }

        if (choice != 4) {
            cout << "Press Enter to continue......";
            cin.get();
        }

    } while (choice != 4);
}

// Main Function Of Multi-User Task Planner application
int main() {
    UserManager userManager; // Initialize UserManager object
    mainMenu(userManager); // Start the main application interface

    return 0;
}
