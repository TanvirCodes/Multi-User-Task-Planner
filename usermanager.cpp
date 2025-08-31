#include <iomanip>
#include <limits>
#include <algorithm>
#include <cctype>
#include "userManager.h"

// Initialize UserManager with empty user list and sample data
UserManager::UserManager() {
    head = nullptr;
    userIdCounter = 101;

    // Add sample users for testing
    addUser("tanvir", "tanvir@gmail.com", "1234");
    addUser("sadik",  "sadik@gmail.com",  "abcd");
    addUser("Himel",  "Himel@gmail.com",  "abcd");
    addUser("Anam",  "Anamul@gmail.com",  "ab1234");
    addUser("Tayeen",  "tayeen@gmail.com",  "ab234");
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
 * Main User Portal Menu - Entry point for user operations
 * Provides registration, login, and exit options
 */
void UserManager::userPortalMenu() {
    int choice;

    do {
        cout << "\n--------- USER PORTAL ---------\n";
        cout << "1. Registration\n";
        cout << "2. Log In\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";

        // Input validation
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
                cout << "\n--------- USER REGISTRATION ---------\n";
                if (userRegistration()) {
                    cout << "Registration successful! You are now logged in.\n";
                    cout << "Press Enter to continue...";
                    cin.get();
                    // Auto-login after successful registration
                    User* newUser = findByUsername(""); // Will be set by registration
                    // In real implementation, we'd store the username from registration. For now, we'll handle this in the registration function
                }
                break;
            }

            case 2: {
                system("cls");
                cout << "\n--------- USER LOGIN ---------\n";
                User* loggedInUser = userLogin();
                if (loggedInUser) {
                    cout << "Login successful! Welcome " << loggedInUser->username << "!\n";
                    cout << "Press Enter to continue...";
                    cin.get();
                    userDashboard(loggedInUser);
                }
                break;
            }

            case 3: {
                cout << "\nExiting User Portal...\n";
                break;
            }

            default: {
                cout << "\nInvalid choice! Please enter 1-3.\n";
                break;
            }
        }

        if (choice != 3) {
            cout << "\nPress Enter to return to User Portal menu...";
            cin.get();
        }

    } while (choice != 3);
}

/**
 * User Registration Process
 * Collects user information with validation and creates new account
 */
bool UserManager::userRegistration() {
    string username, password, email;

    // Get username with uniqueness validation
    do {
        cout << "Enter username: ";
        getline(cin, username);

        if (username.empty()) {
            cout << "Username cannot be empty! Please try again.\n";
            continue;
        }

        if (!isUsernameUnique(username)) {
            cout << "Username '" << username << "' is already taken! Please choose another.\n";
            continue;
        }
        break;
    } while (true);

    // Get password
    do {
        cout << "Enter password: ";
        getline(cin, password);

        if (password.empty()) {
            cout << "Password cannot be empty! Please try again.\n";
            continue;
        }

        if (password.length() < 3) {
            cout << "Password must be at least 3 characters long! Please try again.\n";
            continue;
        }
        break;
    } while (true);

    // Get email with validation
    do {
        cout << "Enter email: ";
        getline(cin, email);

        if (email.empty()) {
            cout << "Email cannot be empty! Please try again.\n";
            continue;
        }

        if (!isValidEmail(email)) {
            cout << "Invalid email format! Please enter a valid email.\n";
            continue;
        }
        break;
    } while (true);

    // Create new user account
    addUser(username, email, password);
    cout << "\nAccount created successfully!\n";
    cout << "Username: " << username << "\n";
    cout << "Email: " << email << "\n";

    // Auto-login after registration
    User* newUser = findByUsername(username);
    if (newUser) {
        userDashboard(newUser);
        return true;
    }

    return false;
}

/**
 * User Login Process with 3-attempt limit
 * Authenticates user and handles failed login attempts
 */
User* UserManager::userLogin() {
    string username, password;
    int attempts = 0;
    const int MAX_ATTEMPTS = 3;

    cout << "Enter username: ";
    getline(cin, username);

    if (username.empty()) {
        cout << "Username cannot be empty!\n";
        return nullptr;
    }

    // Check if user exists
    User* user = findByUsername(username);
    if (!user) {
        cout << "User '" << username << "' not found!\n";
        return nullptr;
    }

    // Check if user is active
    if (!user->isActive) {
        cout << "Your account has been deactivated. Please contact administrator.\n";
        return nullptr;
    }

    // Password verification with attempt limit
    while (attempts < MAX_ATTEMPTS) {
        cout << "Enter password: ";
        getline(cin, password);

        if (user->password == password) {
            return user; // Successful login
        }

        attempts++;
        cout << "Incorrect password! ";

        if (attempts < MAX_ATTEMPTS) {
            cout << "You have " << (MAX_ATTEMPTS - attempts) << " attempt(s) remaining.\n";
        }
    }

    // Deactivate user after 3 failed attempts
    cout << "\nToo many failed login attempts. Your account has been deactivated for security.\n";
    cout << "Please contact administrator to reactivate your account.\n";
    user->isActive = false;

    return nullptr;
}

/**
 * User Dashboard - Main menu after successful login
 * Provides access to profile editing and task management
 */
void UserManager::userDashboard(User* currentUser) {
    int choice;

    do {
        cout << "\n--------- USER DASHBOARD ---------\n";
        cout << "Welcome, " << currentUser->username << "!\n";
        cout << "Account Status: " << (currentUser->isActive ? "Active" : "Deactivated") << "\n";
        cout << "Email: " << currentUser->email << "\n";
        cout << "User ID: " << currentUser->id << "\n";
        cout << "\n1. Edit Profile\n";
        cout << "2. Task Management\n";
        cout << "3. Logout\n";
        cout << "Enter your choice: ";

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
                editProfile(currentUser);
                break;
            }

            case 2: {
                system("cls");
                taskManagement(currentUser);
                break;
            }

            case 3: {
                cout << "\nLogging out... Goodbye " << currentUser->username << "!\n";
                break;
            }

            default: {
                cout << "\nInvalid choice! Please enter 1-3.\n";
                break;
            }
        }

        if (choice != 3) {
            cout << "\nPress Enter to continue...";
            cin.get();
        }

    } while (choice != 3);
}

/**
 * Edit Profile Menu - Allows users to modify their account information
 * Provides username, password, email editing and account deletion
 */
void UserManager::editProfile(User* currentUser) {
    int choice;

    do {
        cout << "\n--------- EDIT PROFILE ---------\n";
        cout << "Current Information:\n";
        cout << "Username: " << currentUser->username << "\n";
        cout << "Email: " << currentUser->email << "\n";
        cout << "User ID: " << currentUser->id << "\n";
        cout << "\n1. Edit Username\n";
        cout << "2. Change Password\n";
        cout << "3. Change Email\n";
        cout << "4. Delete Account\n";
        cout << "5. Back to Dashboard\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input! Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: {
                // Edit Username
                string newUsername;
                cout << "\n--- Edit Username ---\n";
                cout << "Current username: " << currentUser->username << "\n";

                do {
                    cout << "Enter new username: ";
                    getline(cin, newUsername);

                    if (newUsername.empty()) {
                        cout << "Username cannot be empty!\n";
                        continue;
                    }

                    if (newUsername == currentUser->username) {
                        cout << "New username is same as current username!\n";
                        break;
                    }

                    if (!isUsernameUnique(newUsername)) {
                        cout << "Username '" << newUsername << "' is already taken!\n";
                        continue;
                    }

                    // Update username
                    currentUser->username = newUsername;
                    cout << "Username updated successfully to: " << newUsername << "\n";
                    break;

                } while (true);
                break;
            }

            case 2: {
                // Change Password
                string currentPass, newPass, confirmPass;
                cout << "\n--- Change Password ---\n";

                cout << "Enter current password: ";
                getline(cin, currentPass);

                if (currentUser->password != currentPass) {
                    cout << "Current password is incorrect!\n";
                    break;
                }

                do {
                    cout << "Enter new password: ";
                    getline(cin, newPass);

                    if (newPass.empty() || newPass.length() < 3) {
                        cout << "Password must be at least 3 characters long!\n";
                        continue;
                    }

                    cout << "Confirm new password: ";
                    getline(cin, confirmPass);

                    if (newPass != confirmPass) {
                        cout << "Passwords do not match! Please try again.\n";
                        continue;
                    }

                    // Update password
                    currentUser->password = newPass;
                    cout << "Password changed successfully!\n";
                    break;

                } while (true);
                break;
            }

            case 3: {
                // Change Email
                string newEmail;
                cout << "\n--- Change Email ---\n";
                cout << "Current email: " << currentUser->email << "\n";

                do {
                    cout << "Enter new email: ";
                    getline(cin, newEmail);

                    if (newEmail.empty()) {
                        cout << "Email cannot be empty!\n";
                        continue;
                    }

                    if (newEmail == currentUser->email) {
                        cout << "New email is same as current email!\n";
                        break;
                    }

                    if (!isValidEmail(newEmail)) {
                        cout << "Invalid email format!\n";
                        continue;
                    }

                    // Update email
                    currentUser->email = newEmail;
                    cout << "Email updated successfully to: " << newEmail << "\n";
                    break;

                } while (true);
                break;
            }

            case 4: {
                // Delete Account
                deleteUserAccount(currentUser);
                return; // Exit if account is deleted
            }

            case 5: {
                cout << "\nReturning to dashboard...\n";
                break;
            }

            default: {
                cout << "\nInvalid choice! Please enter 1-5.\n";
                break;
            }
        }

        if (choice != 5) {
            cout << "\nPress Enter to continue...";
            cin.get();
        }

    } while (choice != 5);
}

/**
 * Task Management Menu - Placeholder for future task operations
 * Will handle user's personal task management
 */
void UserManager::taskManagement(User* currentUser) {
    cout << "\n--------- TASK MANAGEMENT ---------\n";
    cout << "User: " << currentUser->username << "\n";
    cout << "\nTask Management features coming soon!\n";
    cout << "This will include:\n";
    cout << "- Add new tasks\n";
    cout << "- View your tasks\n";
    cout << "- Edit task details\n";
    cout << "- Mark tasks as completed\n";
    cout << "- Delete tasks\n";
    cout << "\nPress Enter to return to dashboard...";
    cin.get();
}

/**
 * Delete User Account with confirmation
 * Permanently removes user from the system
 */
void UserManager::deleteUserAccount(User* currentUser) {
    string confirmation;

    cout << "\n--------- DELETE ACCOUNT ---------\n";
    cout << "WARNING: This action cannot be undone!\n";
    cout << "All your data including tasks will be permanently deleted.\n";
    cout << "\nAccount to delete:\n";
    cout << "Username: " << currentUser->username << "\n";
    cout << "Email: " << currentUser->email << "\n";
    cout << "User ID: " << currentUser->id << "\n";

    cout << "\nType 'DELETE' to confirm account deletion: ";
    getline(cin, confirmation);

    if (confirmation != "DELETE") {
        cout << "Account deletion cancelled.\n";
        return;
    }

    cout << "\nEnter your password to confirm: ";
    string password;
    getline(cin, password);

    if (currentUser->password != password) {
        cout << "Incorrect password! Account deletion cancelled.\n";
        return;
    }

    // Delete user from linked list
    if (head == currentUser) {
        // Deleting first user
        head = currentUser->next;
    } else {
        // Find previous user
        User* prev = head;
        while (prev && prev->next != currentUser) {
            prev = prev->next;
        }
        if (prev) {
            prev->next = currentUser->next;
        }
    }

    // Delete all user's tasks
    Task* task = currentUser->taskHead;
    while (task) {
        Task* nextTask = task->next;
        delete task;
        task = nextTask;
    }

    cout << "\nAccount '" << currentUser->username << "' has been permanently deleted.\n";
    cout << "Thank you for using our system. Goodbye!\n";

    delete currentUser;

    cout << "Press Enter to return to User Portal...";
    cin.get();
}

/**
 * Find user by username
 */
User* UserManager::findByUsername(string username) {
    User* temp = head;
    while (temp) {
        if (temp->username == username) {
            return temp;
        }
        temp = temp->next;
    }
    return nullptr;
}

/**
 * Check if username is unique in the system
 */
bool UserManager::isUsernameUnique(string username) {
    return (findByUsername(username) == nullptr);
}

/**
 * Basic email validation
 * Checks for @ symbol and basic format
 */
bool UserManager::isValidEmail(string email) {
    if (email.length() < 5) return false;

    size_t atPos = email.find('@');
    if (atPos == string::npos || atPos == 0 || atPos == email.length() - 1) {
        return false;
    }

    size_t dotPos = email.find('.', atPos);
    if (dotPos == string::npos || dotPos == atPos + 1 || dotPos == email.length() - 1) {
        return false;
    }

    return true;
}

/**
 * Authenticate user credentials
 * Returns true if username and password match
 */
bool UserManager::authenticateUser(string username, string password) {
    User* user = findByUsername(username);
    return (user && user->password == password && user->isActive);
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
