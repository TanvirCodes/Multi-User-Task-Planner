#include <iostream>
#include <limits>
#include "userManager.h"
using namespace std;

void mainMenu(UserManager &um) {
    int choice;

    // Main application loop
    do {
        cout << "\n--------- Task Planner Main Menu ---------\n";
        cout << "1. Admin Dashboard" << endl;
        cout << "2. User Portal" << endl;
        cout << "3. Help Desk (Coming Soon)" << endl;
        cout << "4. Exit\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input! Please enter a number.\n";
            cin.clear(); // Clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer

        // Process user's menu selection
        switch (choice) {
            case 1: {
                // Take Admin authentication before granting access
                if (um.adminLogin()) {
                    // Display system statistics and enter Admin operations menu
                    um.showAdminDashboard();
                    um.adminDashboardMenu();
                } else {
                    cout << "Invalid Admin credentials. Access denied.\n";
                }
                break;
            }

            case 2: {
                // User Portal System
                system("cls");
                cout << "Welcome to User Portal!\n";
                um.userPortalMenu();
                break;
            }

            case 3: {
                cout << "Help Desk is under development.\n";
                break;
            }

            case 4: {
                cout << "Exiting program...\n";
                break;
            }

            default: {
                cout << "Invalid choice! Please enter a number between 1-4.\n";
                break;
            }
        }

        if (choice != 4) {
            cout << "Press Enter to continue...";
            cin.get();
        }

    } while (choice != 4);
}

/**
 * Main Function - Entry point of the Multi-User Task Planner application
 * Initializes the user management system and starts the main menu interface
 */
int main() {
    // Initialize UserManager object
    UserManager userManager;

    // Start the main application interface
    mainMenu(userManager);

    return 0;
}
