#include <iomanip>
#include <limits>
#include "userManager.h"

// Initialize UserManager with empty user list and sample data

UserManager::UserManager() {
    head = nullptr;
    userIdCounter = 101;

    // Add sample users for testing
    addUser("tanvir", "tanvir@gmail.com", "1234");
    addUser("sadik",  "sadik@gmail.com",  "abcd");
    addUser("Himel",  "Himel@gmail.com",  "abcd");
}

// Admin login authentication with hardcoded credentials
bool UserManager::adminLogin() {
    string uname, pass;
    cout << "\nAdmin Username: ";
    cin >> uname;
    cout << "Admin Password: ";
    cin >> pass;
    if (uname == "admin" && pass == "admin123") {
        system("cls");
        return true;
    }
    return false;
}

/**
 * Add new user to the end of linked list
 * Creates user with auto-incremented ID and active status
 */
void UserManager::addUser(string username, string email, string password) {
    User* newUser = new User{userIdCounter++, username, email, password, true, nullptr, nullptr};

    if (!head) {
        head = newUser;
    } else {
        // Find the last node and append
        User* temp = head;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = newUser;
    }
}

/**
 * Search for user by ID using linear search
 * Returns pointer to user if found, nullptr otherwise
 */
User* UserManager::findById(int id) {
    User* temp = head;
    while (temp) {
        if (temp->id == id) return temp;
        temp = temp->next;
    }
    return nullptr;
}

/**
 * Calculate and display system statistics
 * Traverses all users and their tasks to count totals
 */
void UserManager::showAdminDashboard() {
    int totalUsers = 0, active = 0, inactive = 0;
    int totalTasks = 0, ongoing = 0, completed = 0;

    User* temp = head;
    while (temp) {
        totalUsers++;
        if (temp->isActive) active++;
        else inactive++;

        // Count tasks for current user
        Task* t = temp->taskHead;
        while (t) {
            totalTasks++;
            if (t->status == "Completed") completed++;
            else ongoing++;
            t = t->next;
        }
        temp = temp->next;
    }

    cout << "\n------- Admin Dashboard Stats ---------\n";
    cout << "Total Users: " << totalUsers << endl;
    cout << "Active Users: " << active << endl;
    cout << "Inactive Users: " << inactive << endl;
    cout << "Total Tasks: " << totalTasks << endl;
    cout << "Ongoing Tasks: " << ongoing << endl;
    cout << "Completed Tasks: " << completed << endl;
}

// Main admin menu with input validation and error handling
void UserManager::adminDashboardMenu() {
    int choice;
    do {
        cout << "\n------- Admin Options -------\n";
        cout << "1. Search User\n";
        cout << "2. View All Users\n";
        cout << "3. Filter Users\n";
        cout << "4. View All Users Task\n";
        cout << "5. Deactivate/Reactivate Account\n";
        cout << "6. Back to Main Menu\n";
        cout << "Enter your choice: ";

        // Input validation for menu choice
        if (!(cin >> choice)) {
            cout << "Invalid input! Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: {
                system("cls");
                cout << "\n--- Search Users ---\n";
                string keyword;
                cout << "Enter keyword (id/email/username): ";
                getline(cin, keyword);

                if (!keyword.empty()) {
                    cout << "Searching for: " << keyword << "\n";
                    searchUsers(keyword);
                } else {
                    cout << "Search keyword cannot be empty!\n";
                }
                break;
            }

            case 2: {
                system("cls");
                cout << "\n--- View All Users ---\n";
                viewAllUsers();
                break;
            }

            case 3: {
                system("cls");
                cout << "\n--- Filter Users ---\n";
                filterUsers();
                break;
            }

            case 4: {
                system("cls");
                cout << "\n--- View All User Tasks ---\n";
                viewAllUserTasks();
                break;
            }

            case 5: {
                system("cls");
                cout << "\n--- Toggle User Activation ---\n";
                int id;
                cout << "Enter User ID to toggle activation: ";

                if (cin >> id) {
                    cout << "Processing User ID: " << id << "\n";
                    deactivateUser(id);
                } else {
                    cout << "Invalid User ID! Please enter a number.\n";
                    cin.clear();
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }

            case 6: {
                cout << "\n--- Returning to Main Menu ---\n";
                cout << "Goodbye!\n";
                break;
            }

            default: {
                cout << "\nInvalid choice!\n";
                cout << "Please enter a number between 1-6.\n";
                break;
            }
        }

        if (choice != 6) {
            cout << "\nPress Enter to continue...";
            cin.get();
        }

    } while (choice != 6);
}

/**
 * Display all users in formatted table
 * Shows ID, username, email and status for each user
 */
void UserManager::viewAllUsers() {
    if (!head) {
        cout << "No users found in the system.\n";
        return;
    }

    // Print table header
    cout << left << setw(10) << "ID"
         << setw(15) << "Username"
         << setw(25) << "Email"
         << setw(15) << "Status" << "\n";
    cout << string(65, '-') << "\n";

    User* temp = head;
    while (temp) {
        cout << left << setw(10) << temp->id
             << setw(15) << temp->username
             << setw(25) << temp->email
             << setw(15) << (temp->isActive ? "Active" : "Deactivated") << "\n";
        temp = temp->next;
    }
}

/**
 * Search users with partial matching
 * It checks if keyword exists anywhere in the fields
 */
void UserManager::searchUsers(string keyword) {
    bool found = false;
    User* temp = head;

    cout << "\nSearch Results:\n";
    cout << string(50, '-') << "\n";

    while (temp) {
        bool matchFound = false;

        // Convert ID to string for searching
        string idAsString = to_string(temp->id);

        // Check ID
        for (int i = 0; i <= (int)idAsString.length() - (int)keyword.length(); i++) {
            if (idAsString.substr(i, keyword.length()) == keyword) {
                matchFound = true;
                break;
            }
        }

        // Check email
        if (!matchFound) {
            for (int i = 0; i <= (int)temp->email.length() - (int)keyword.length(); i++) {
                if (temp->email.substr(i, keyword.length()) == keyword) {
                    matchFound = true;
                    break;
                }
            }
        }

        // Check username
        if (!matchFound) {
            for (int i = 0; i <= (int)temp->username.length() - (int)keyword.length(); i++) {
                if (temp->username.substr(i, keyword.length()) == keyword) {
                    matchFound = true;
                    break;
                }
            }
        }

        // If any field matched, display the user
        if (matchFound) {
            cout << "Found User: "
                 << temp->id << " " << temp->username << " " << temp->email
                 << " [" << (temp->isActive ? "Active" : "Deactivated") << "]\n";
            found = true;
        }

        temp = temp->next;
    }

    if (!found) {
        cout << "No users found matching \"" << keyword << "\"\n";
    }
}

/**
 * Display all users
 * Counts total, completed and ongoing tasks for each user
 */
void UserManager::viewAllUserTasks() {
    if (!head) {
        cout << "No users found in the system.\n";
        return;
    }

    User* temp = head;
    while (temp) {
        int taskCount = 0, completed = 0, ongoing = 0;

        // Count tasks for current user
        Task* t = temp->taskHead;
        while (t) {
            taskCount++;
            if (t->status == "Completed") completed++;
            else ongoing++;
            t = t->next;
        }

        // Display user info with task summary
        cout << "\nUser ID: " << temp->id
             << ", Name: " << temp->username;

        if (!temp->isActive) {
            cout << " (DEACTIVATED)";
        }

        cout << ", Email: " << temp->email << "\n";
        cout << "Total Tasks: " << taskCount
             << ", Completed: " << completed
             << ", Ongoing: " << ongoing << "\n";

        temp = temp->next;
    }
}

/**
 * Will be implemented to filter users by various criteria
 * Until please wait
 */
void UserManager::filterUsers() {
    cout << "Filter logic coming soon (Sort by task, active/inactive etc.)\n";
}

/**
 * Toggle user activation status between active and inactive
 */
void UserManager::deactivateUser(int id) {
    User* u = findById(id);
    if (!u) {
        cout << "User not found.\n";
        return;
    }

    // Toggle status and provide feedback
    u->isActive = !u->isActive;
    if (u->isActive) {
        cout << "User " << u->username << " re-activated.\n";
    } else {
        cout << "User " << u->username << " deactivated.\n";
    }
}

/**
 * Check if user exists and is active
 * Used by other system components to validate user operations
 */
bool UserManager::isUserActiveById(int id) {
    User* u = findById(id);
    return (u && u->isActive);
}
