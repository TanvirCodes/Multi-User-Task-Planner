#include <iomanip>
#include <limits>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "usermanager.h"

// Initialize UserManager with hash tables and sample data
UserManager::UserManager()
{
    userIdCounter = 101;
    taskIdCounter = 1001;
    activeUsersCount = 0;
    totalTasksCount = 0;
    completedTasksCount = 0;

    // Add sample data for testing
    addUser("tanvir", "tanvir@gmail.com", "1234");
    addUser("sadik", "sadik@gmail.com", "abcd");
    addUser("Himel", "Himel@gmail.com", "abcd");
    addUser("Anam", "Anamul@gmail.com", "ab1234");
    addUser("Tayeen", "tayeen@gmail.com", "ab234");

    // Update task counter to avoid conflicts with any existing tasks
    for (const auto& userPair : usersById)
    {
        for (const auto& taskPair : userPair.second.tasks)
        {
            if (taskPair.first >= taskIdCounter)
            {
                taskIdCounter = taskPair.first + 1;
            }
        }
    }
}

// Destructor - Hash tables handle cleanup automatically
UserManager::~UserManager()
{

}

// Update statistics counters - O(1) dashboard operations
void UserManager::updateStatistics()
{
    activeUsersCount = 0;
    totalTasksCount = 0;
    completedTasksCount = 0;

    for (auto& pair : usersById)
    {
        if (pair.second.isActive)
        {
            activeUsersCount++;
        }

        for (auto& taskPair : pair.second.tasks)
        {
            totalTasksCount++;
            if (taskPair.second.status == "Completed")
            {
                completedTasksCount++;
            }
        }
    }
}

// O(1) user lookup by ID
User* UserManager::findUserById(int id)
{
    auto it = usersById.find(id);
    return (it != usersById.end()) ? &it->second : nullptr;
}

// O(1) user lookup by username
User* UserManager::findUserByUsername(const string& username)
{
    auto it = usersByUsername.find(username);
    if (it != usersByUsername.end())
    {
        return findUserById(it->second);
    }
    return nullptr;
}

// O(1) username uniqueness check
bool UserManager::isUsernameUnique(const string& username)
{
    return usedUsernames.find(username) == usedUsernames.end();
}

// O(1) email uniqueness check
bool UserManager::isEmailUnique(const string& email)
{
    return usedEmails.find(email) == usedEmails.end();
}

// O(1) user authentication
bool UserManager::authenticateUser(const string& username, const string& password)
{
    User* user = findUserByUsername(username);
    return (user && user->password == password && user->isActive);
}

// Get next unique task ID
int UserManager::getNextTaskId()
{
    return taskIdCounter++;
}

// Email validation with basic format checking
bool UserManager::isValidEmail(const string& email)
{
    if (email.length() < 5) return false;

    size_t atPos = email.find('@');
    if (atPos == string::npos || atPos == 0 || atPos == email.length() - 1)
    {
        return false;
    }

    size_t dotPos = email.find('.', atPos);
    if (dotPos == string::npos || dotPos == atPos + 1 || dotPos == email.length() - 1)
    {
        return false;
    }

    return true;
}

// Date validation for DD/MM/YYYY format
bool UserManager::isValidDate(const string& date)
{
    if (date.length() != 10) return false;
    if (date[2] != '/' || date[5] != '/') return false;

    for (int i = 0; i < 10; i++)
    {
        if (i == 2 || i == 5) continue;
        if (date[i] < '0' || date[i] > '9') return false;
    }

    string dayStr = date.substr(0, 2);
    string monthStr = date.substr(3, 2);
    string yearStr = date.substr(6, 4);

    int day = stoi(dayStr);
    int month = stoi(monthStr);
    int year = stoi(yearStr);

    if (day < 1 || day > 31) return false;
    if (month < 1 || month > 12) return false;
    if (year < 2024 || year > 2030) return false;

    return true;
}

// Admin login with hardcoded credentials
bool UserManager::adminLogin()
{
    string uname, pass;
    cout << "\nAdmin Username: ";
    cin >> uname;
    cout << "Admin Password: ";
    cin >> pass;

    if (uname == "admin" && pass == "123")
    {
        system("cls");
        return true;
    }
    return false;
}

// O(1) Dashboard statistics display using counters
void UserManager::showAdminDashboard()
{
    updateStatistics(); // Update counters

    cout << "\n------- Admin Dashboard Stats ---------\n" << endl;
    cout << "Total Users: " << getTotalUsersCount() << endl;
    cout << "Active Users: " << getActiveUsersCount() << endl;
    cout << "Inactive Users: " << (getTotalUsersCount() - getActiveUsersCount()) << endl;
    cout << "Total Tasks: " << getTotalTasksCount() << endl;
    cout << "Ongoing Tasks: " << (getTotalTasksCount() - getCompletedTasksCount()) << endl;
    cout << "Completed Tasks: " << getCompletedTasksCount() << endl;
    cout << "\n---------------------------------------\n";
}

// Admin dashboard menu with hash table optimized operations
void UserManager::adminDashboardMenu()
{
    int choice;
    do
    {
        cout << "\n------- Admin Options -------\n";
        cout << "1. Search User\n";
        cout << "2. View All Users\n";
        cout << "3. Filter Users\n";
        cout << "4. View All Users Task\n";
        cout << "5. Activate/Deactivate Account\n";
        cout << "6. Back to Main Menu\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice))
        {
            cout << "Invalid input! Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice)
        {
        case 1:
        {
            system("cls");
            cout << "\n--- Search Users ---\n";
            string keyword;
            cout << "Enter keyword (id/email/username): ";
            getline(cin, keyword);

            if (!keyword.empty())
            {
                searchUsers(keyword);
            }
            else
            {
                cout << "Search keyword cannot be empty!\n";
            }
            break;
        }

        case 2:
        {
            system("cls");
            cout << "\n--- All User List ---\n" << endl;
            viewAllUsers();
            break;
        }

        case 3:
        {
            system("cls");
            filterUsers();
            break;
        }

        case 4:
        {
            system("cls");
            cout << "\n--- View All User Tasks ---\n";
            viewAllUserTasks();
            break;
        }

        case 5:
        {
            system("cls");
            cout << "\n--- Toggle User Activation ---\n";
            int id;
            cout << "Enter User ID to toggle activation: ";

            if (cin >> id)
            {
                toggleUserActivation(id);
            }
            else
            {
                cout << "Invalid User ID! Please enter a number.\n";
                cin.clear();
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }

        case 6:
        {
            cout << "\n--- Returning to Main Menu ---\n" << endl;
            cout << "Goodbye favorite user!\n" << endl;
            break;
        }

        default:
        {
            cout << "\nInvalid choice! Please enter a number between 1-6.\n";
            break;
        }
        }

        if (choice != 6)
        {
            cout << "\nPress Enter to continue...";
            cin.get();
        }

    }
    while (choice != 6);
}

// O(1) user addition with multiple hash table indexing
bool UserManager::addUser(const string& username, const string& email, const string& password)
{
    // Check uniqueness using O(1) hash set lookups
    if (!isUsernameUnique(username))
    {
        return false; // Username already exists
    }

    if (!isEmailUnique(email))
    {
        return false; // Email already exists
    }

    int newUserId = userIdCounter++;

    // Create new user
    User newUser(newUserId, username, email, password, true);

    // Insert into all hash tables for O(1) access
    usersById[newUserId] = newUser;
    usersByUsername[username] = newUserId;
    usersByEmail[email] = newUserId;

    // Add to uniqueness sets
    usedUsernames.insert(username);
    usedEmails.insert(email);

    return true;
}

// Display all users using hash table iteration
void UserManager::viewAllUsers()
{
    cout << "\n--- All Users List ---\n";
    cout << setw(5) << "ID" << setw(15) << "Username" << setw(25) << "Email" << setw(10) << "Status" << endl;
    cout << string(55, '-') << endl;

    vector<User> sortedUsers;
    for (const auto& pair : usersById)
    {
        sortedUsers.push_back(pair.second);
    }
    sort(sortedUsers.begin(), sortedUsers.end(), [](const User& a, const User& b)
    {
        return a.id < b.id;
    });

    for (const auto& user : sortedUsers)
    {
        string status = user.isActive ? "Active" : "Inactive";
        cout << setw(5) << user.id
             << setw(15) << user.username
             << setw(25) << user.email
             << setw(10) << status << endl;
    }
}

// Optimized search with hash table lookups
void UserManager::searchUsers(const string& keyword)
{
    bool found = false;

    cout << "\nSearch Results:\n";

    // Try exact matches first (O(1) operations)

    // 1. Try as user ID
    try
    {
        int searchId = stoi(keyword);
        User* user = findUserById(searchId);
        if (user)
        {
            cout << "Found by ID: " << user->id << " " << user->username
                 << " " << user->email << " [" << (user->isActive ? "Active" : "Deactivated") << "]\n";
            found = true;
        }
    }
    catch (...)
    {
        // Not a valid integer, skip ID search
    }

    // 2. Try as username (O(1))
    User* userByName = findUserByUsername(keyword);
    if (userByName)
    {
        cout << "Found by Username: " << userByName->id << " " << userByName->username
             << " " << userByName->email << " [" << (userByName->isActive ? "Active" : "Deactivated") << "]\n";
        found = true;
    }

    // 3. Try as email (O(1))
    auto emailIt = usersByEmail.find(keyword);
    if (emailIt != usersByEmail.end())
    {
        User* userByEmailPtr = findUserById(emailIt->second);
        if (userByEmailPtr)
        {
            cout << "Found by Email: " << userByEmailPtr->id << " " << userByEmailPtr->username
                 << " " << userByEmailPtr->email << " [" << (userByEmailPtr->isActive ? "Active" : "Deactivated") << "]\n";
            found = true;
        }
    }

    // 4. Partial matching (still O(n) but faster iteration)
    if (!found)
    {
        for (const auto& pair : usersById)
        {
            const User& user = pair.second;

            // Convert ID to string for partial matching
            string idStr = to_string(user.id);

            if (idStr.find(keyword) != string::npos ||
                    user.username.find(keyword) != string::npos ||
                    user.email.find(keyword) != string::npos)
            {

                cout << "Found: " << user.id << " " << user.username
                     << " " << user.email << " [" << (user.isActive ? "Active" : "Deactivated") << "]\n";
                found = true;
            }
        }
    }

    if (!found)
    {
        cout << "No users found matching \"" << keyword << "\"\n";
    }
}

// Hash table based user filtering
void UserManager::filterUsers()
{
    int choice;
    do
    {
        cout << "\n--------- FILTER USERS ---------\n" << endl;
        cout << "1. Active Users\n";
        cout << "2. Inactive Users\n";
        cout << "3. Users with Tasks\n";
        cout << "4. Users without Tasks\n";
        cout << "5. Back to Admin Menu\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice))
        {
            cout << "Invalid input! Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        bool found = false;

        switch (choice)
        {
        case 1:
        {
            cout << "\n--- Active Users ---\n";
            for (const auto& pair : usersById)
            {
                const User& user = pair.second;
                if (user.isActive)
                {
                    cout << "ID: " << user.id << ", Username: " << user.username
                         << ", Email: " << user.email << "\n";
                    found = true;
                }
            }
            if (!found) cout << "No active users found.\n";
            break;
        }

        case 2:
        {
            cout << "\n--- Inactive Users ---\n";
            for (const auto& pair : usersById)
            {
                const User& user = pair.second;
                if (!user.isActive)
                {
                    cout << "ID: " << user.id << ", Username: " << user.username
                         << ", Email: " << user.email << "\n";
                    found = true;
                }
            }
            if (!found) cout << "No inactive users found.\n";
            break;
        }

        case 3:
        {
            cout << "\n--- Users with Tasks ---\n";
            for (const auto& pair : usersById)
            {
                const User& user = pair.second;
                if (!user.tasks.empty())
                {
                    cout << "ID: " << user.id << ", Username: " << user.username
                         << ", Email: " << user.email << " (Tasks: " << user.tasks.size() << ")\n";
                    found = true;
                }
            }
            if (!found) cout << "No users with tasks found.\n";
            break;
        }

        case 4:
        {
            cout << "\n--- Users without Tasks ---\n";
            for (const auto& pair : usersById)
            {
                const User& user = pair.second;
                if (user.tasks.empty())
                {
                    cout << "ID: " << user.id << ", Username: " << user.username
                         << ", Email: " << user.email << "\n";
                    found = true;
                }
            }
            if (!found) cout << "No users without tasks found.\n";
            break;
        }

        case 5:
        {
            cout << "Returning to Admin Menu...\n";
            break;
        }

        default:
            cout << "Invalid choice! Please enter 1-5.\n";
        }

        if (choice != 5)
        {
            cout << "\nPress Enter to continue...";
            cin.get();
        }

    }
    while (choice != 5);
}

// Display all user tasks using hash table iteration
void UserManager::viewAllUserTasks()
{
    if (usersById.empty())
    {
        cout << "No users found in the system.\n";
        return;
    }

    for (const auto& pair : usersById)
    {
        const User& user = pair.second;

        int taskCount = user.tasks.size();
        int completed = 0, ongoing = 0;

        // Count completed and ongoing tasks
        for (const auto& taskPair : user.tasks)
        {
            if (taskPair.second.status == "Completed")
            {
                completed++;
            }
            else
            {
                ongoing++;
            }
        }

        cout << "\nUser ID: " << user.id << ", Name: " << user.username;

        if (!user.isActive)
        {
            cout << " (DEACTIVATED)";
        }

        cout << ", Email: " << user.email << "\n";
        cout << "Total Tasks: " << taskCount << ", Completed: " << completed
             << ", Ongoing: " << ongoing << "\n";
    }
}

// O(1) user activation toggle
bool UserManager::toggleUserActivation(int id)
{
    User* user = findUserById(id);
    if (!user)
    {
        cout << "User not found.\n";
        return false;
    }

    user->isActive = !user->isActive;
    if (user->isActive)
    {
        cout << "User " << user->username << " re-activated.\n";
    }
    else
    {
        cout << "User " << user->username << " deactivated.\n";
    }
    return true;
}

// O(1) user activity check
bool UserManager::isUserActiveById(int id)
{
    User* user = findUserById(id);
    return (user && user->isActive);
}

// User Portal Menu
void UserManager::userPortalMenu()
{
    int choice;

    do
    {
        cout << "\n--------- USER PORTAL ---------" << endl;
        cout << "1. Registration" << endl;
        cout << "2. Log In" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";

        if (!(cin >> choice))
        {
            cout << "Invalid input! Please enter a number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice)
        {
        case 1:
        {
            system("cls");
            cout << "\n--------- USER REGISTRATION ---------\n";
            if (userRegistration())
            {
                cout << "Registration successful!\n";
            }
            break;
        }

        case 2:
        {
            system("cls");
            cout << "\n--------- USER LOGIN ---------" << endl;
            User* loggedInUser = userLogin();
            if (loggedInUser)
            {
                cout << "Login successful! Welcome " << loggedInUser->username << "!" << endl;
                cout << "Press Enter to continue.......";
                cin.get();
                userDashboard(loggedInUser);
            }
            break;
        }

        case 3:
        {
            cout << "\nExiting User Portal....." << endl;
            break;
        }

        default:
        {
            cout << "\nInvalid choice! Please enter 1-3." << endl;
            break;
        }
        }

        if (choice != 3)
        {
            cout << "\nPress Enter to return to User Portal menu.......";
            cin.get();
        }

    }
    while (choice != 3);
}

// User registration with hash table optimizations
bool UserManager::userRegistration()
{
    string username, password, email;

    // Get username with O(1) uniqueness validation
    do
    {
        cout << "Enter username: ";
        getline(cin, username);

        if (username.empty())
        {
            cout << "Username cannot be empty! Please try again." << endl;
            continue;
        }

        if (!isUsernameUnique(username))
        {
            cout << "Username '" << username << "' is already taken! Please choose another." << endl;
            continue;
        }
        break;
    }
    while (true);

    // Get password
    do
    {
        cout << "Enter password: ";
        getline(cin, password);

        if (password.empty())
        {
            cout << "Password cannot be empty! Please try again." << endl;
            continue;
        }

        if (password.length() < 3)
        {
            cout << "Password must be at least 3 characters long! Please try again." << endl;
            continue;
        }
        break;
    }
    while (true);

    // Get email with validation
    do
    {
        cout << "Enter email: ";
        getline(cin, email);

        if (email.empty())
        {
            cout << "Email cannot be empty! Please try again." << endl;
            continue;
        }

        if (!isValidEmail(email))
        {
            cout << "Invalid email format! Please enter a valid email." << endl;
            continue;
        }

        if (!isEmailUnique(email))
        {
            cout << "Email '" << email << "' is already registered! Please use another email." << endl;
            continue;
        }
        break;
    }
    while (true);

    // Create new user account using O(1) hash table insertion
    if (addUser(username, email, password))
    {
        cout << "\nAccount created successfully!" << endl;
        cout << "Username: " << username << endl;
        cout << "Email: " << email << endl;

        // Auto-login after registration
        User* newUser = findUserByUsername(username);
        if (newUser)
        {
            userDashboard(newUser);
            return true;
        }
    }
    else
    {
        cout << "Failed to create account. Please try again." << endl;
    }

    return false;
}

// O(1) user login with hash table lookup
User* UserManager::userLogin()
{
    string username, password;
    int attempts = 0;
    const int MAX_ATTEMPTS = 3;

    cout << "Enter username: ";
    getline(cin, username);

    if (username.empty())
    {
        cout << "Username cannot be empty!" << endl;
        return nullptr;
    }

    // O(1) user lookup
    User* user = findUserByUsername(username);
    if (!user)
    {
        cout << "User '" << username << "' not found!" << endl;
        return nullptr;
    }

    // Check if user is active
    if (!user->isActive)
    {
        cout << "Your account has been deactivated. Please contact administrator." << endl;
        return nullptr;
    }

    // Password verification with attempt limit
    while (attempts < MAX_ATTEMPTS)
    {
        cout << "Enter password: ";
        getline(cin, password);

        if (user->password == password)
        {
            return user; // Successful login
        }

        attempts++;
        cout << "Incorrect password! ";

        if (attempts < MAX_ATTEMPTS)
        {
            cout << "You have " << (MAX_ATTEMPTS - attempts) << " attempts remaining." << endl;
        }
    }

    // Deactivate user after 3 failed attempts
    cout << "\nToo many failed login attempts. Your account has been deactivated for security." << endl;
    cout << "Please contact administrator to reactivate your account." << endl;
    user->isActive = false;

    return nullptr;
}

// User Dashboard
void UserManager::userDashboard(User* currentUser)
{
    int choice;

    do
    {
        cout << "\n--------- USER DASHBOARD ---------" << endl;
        cout << "Welcome, " << currentUser->username << "!\n";
        cout << "Account Status: " << (currentUser->isActive ? "Active" : "Deactivated") << endl;
        cout << "Email: " << currentUser->email << endl;
        cout << "User ID: " << currentUser->id << endl;
        cout << "\n1. Edit Profile" << endl;
        cout << "2. Task Management" << endl;
        cout << "3. Logout" << endl;
        cout << "Enter your choice: ";

        if (!(cin >> choice))
        {
            cout << "Invalid input! Please enter a number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice)
        {
        case 1:
        {
            system("cls");
            editProfile(currentUser);
            break;
        }

        case 2:
        {
            system("cls");
            taskManagement(currentUser);
            break;
        }

        case 3:
        {
            cout << "\nLogging out... Goodbye " << currentUser->username << "!" << endl;
            break;
        }

        default:
        {
            cout << "\nInvalid choice!" << endl;
            break;
        }
        }

        if (choice != 3)
        {
            cout << "\nPress Enter to continue...";
            cin.get();
        }

    }
    while (choice != 3);
}

// Edit Profile Menu with hash table updates
void UserManager::editProfile(User* currentUser)
{
    int choice;

    do
    {
        cout << "\n--------- EDIT PROFILE ---------" << endl;
        cout << "Current Information:" << endl;
        cout << "Username: " << currentUser->username << endl;
        cout << "Email: " << currentUser->email << endl;
        cout << "User ID: " << currentUser->id << endl;
        cout << "\n1. Edit Username" << endl;
        cout << "2. Change Password" << endl;
        cout << "3. Change Email" << endl;
        cout << "4. Delete Account" << endl;
        cout << "5. Back to Dashboard" << endl;
        cout << "Enter your choice: ";

        if (!(cin >> choice))
        {
            cout << "Invalid input! Please enter a number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice)
        {
        case 1:
        {
            // Edit Username
            string newUsername;
            cout << "\n------- Edit Username -------" << endl;
            cout << "Current username: " << currentUser->username << endl;

            do
            {
                cout << "Enter new username: ";
                getline(cin, newUsername);

                if (newUsername.empty())
                {
                    cout << "Username cannot be empty!\n";
                    continue;
                }

                if (newUsername == currentUser->username)
                {
                    cout << "New username is same as current username!" << endl;
                    break;
                }

                if (!isUsernameUnique(newUsername))
                {
                    cout << "Username '" << newUsername << "' is already taken!" << endl;
                    continue;
                }

                // Update hash tables
                string oldUsername = currentUser->username;

                // Remove old username from hash tables
                usersByUsername.erase(oldUsername);
                usedUsernames.erase(oldUsername);

                // Update user and add new username to hash tables
                currentUser->username = newUsername;
                usersByUsername[newUsername] = currentUser->id;
                usedUsernames.insert(newUsername);

                cout << "Username updated successfully to: " << newUsername << endl;
                break;

            }
            while (true);
            break;
        }

        case 2:
        {
            // Change Password - O(1) update
            string currentPass, newPass, confirmPass;
            cout << "\n------- Change Password -------" << endl;

            cout << "Enter current password: ";
            getline(cin, currentPass);

            if (currentUser->password != currentPass)
            {
                cout << "Current password is incorrect!" << endl;
                break;
            }

            do
            {
                cout << "Enter new password: ";
                getline(cin, newPass);

                if (newPass.empty() || newPass.length() < 3)
                {
                    cout << "Password must be at least 3 characters long!" << endl;
                    continue;
                }

                cout << "Confirm new password: ";
                getline(cin, confirmPass);

                if (newPass != confirmPass)
                {
                    cout << "Passwords do not match! Please try again." << endl;
                    continue;
                }

                // Update password
                currentUser->password = newPass;
                cout << "Password changed successfully!" << endl;
                break;

            }
            while (true);
            break;
        }

        case 3:
        {
            // Change Email
            string newEmail;
            cout << "\n------ Change Email ------" << endl;
            cout << "Current email: " << currentUser->email << endl;

            do
            {
                cout << "Enter new email: ";
                getline(cin, newEmail);

                if (newEmail.empty())
                {
                    cout << "Email cannot be empty!" << endl;
                    continue;
                }

                if (newEmail == currentUser->email)
                {
                    cout << "New email is same as current email!" << endl;
                    break;
                }

                if (!isValidEmail(newEmail))
                {
                    cout << "Invalid email format!" << endl;
                    continue;
                }

                if (!isEmailUnique(newEmail))
                {
                    cout << "Email '" << newEmail << "' is already registered!" << endl;
                    continue;
                }

                // Update hash tables
                string oldEmail = currentUser->email;

                // Remove old email from hash tables
                usersByEmail.erase(oldEmail);
                usedEmails.erase(oldEmail);

                // Update user and add new email to hash tables
                currentUser->email = newEmail;
                usersByEmail[newEmail] = currentUser->id;
                usedEmails.insert(newEmail);

                cout << "Email updated successfully to: " << newEmail << endl;
                break;

            }
            while (true);
            break;
        }

        case 4:
        {
            // Delete Account
            deleteUserAccount(currentUser);
            return; // Exit if account is deleted
        }

        case 5:
        {
            cout << "\nReturning to dashboard......" << endl;
            break;
        }

        default:
        {
            cout << "\nInvalid choice!" << endl;
            break;
        }
        }

        if (choice != 5)
        {
            cout << "\nPress Enter to continue......";
            cin.get();
        }

    }
    while (choice != 5);
}

// Task Management Menu with hash table operations
void UserManager::taskManagement(User* currentUser)
{
    int choice;

    do
    {
        cout << "\n--------- TASK MANAGEMENT ---------\n";
        cout << "User: " << currentUser->username << "\n";

        // O(1) task counting using hash table size
        int totalTasks = currentUser->tasks.size();
        int completedTasks = 0, ongoingTasks = 0;

        for (const auto& pair : currentUser->tasks)
        {
            if (pair.second.status == "Completed")
            {
                completedTasks++;
            }
            else
            {
                ongoingTasks++;
            }
        }

        cout << "Total Tasks: " << totalTasks;
        cout << " | Completed: " << completedTasks;
        cout << " | Ongoing: " << ongoingTasks << "\n";

        cout << "\n1. Add New Task\n";
        cout << "2. View All Tasks\n";
        cout << "3. Edit Task\n";
        cout << "4. Mark Task as Completed\n";
        cout << "5. Delete Task\n";
        cout << "6. View Task Details\n";
        cout << "7. Back to Dashboard\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice))
        {
            cout << "Invalid input! Please enter a number.\n";
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        cin.ignore();

        switch (choice)
        {
        case 1:
        {
            addTask(currentUser);
            break;
        }

        case 2:
        {
            viewUserTasks(currentUser);
            break;
        }

        case 3:
        {
            editTask(currentUser);
            break;
        }

        case 4:
        {
            markTaskCompleted(currentUser);
            break;
        }

        case 5:
        {
            deleteTask(currentUser);
            break;
        }

        case 6:
        {
            viewTaskDetails(currentUser);
            break;
        }

        case 7:
        {
            cout << "\nReturning to dashboard...\n";
            break;
        }

        default:
        {
            cout << "\nInvalid choice!\n";
            break;
        }
        }

        if (choice != 7)
        {
            cout << "\nPress Enter to continue...";
            cin.get();
        }

    }
    while (choice != 7);
}

// O(1) task addition using hash table
void UserManager::addTask(User* currentUser)
{
    cout << "\n--- Add New Task ---\n";

    string title, dueDate;

    // Get task title
    do
    {
        cout << "Enter task title: ";
        getline(cin, title);

        if (title.empty())
        {
            cout << "Task title cannot be empty! Please enter title.\n";
            continue;
        }

        if (title.length() > 100)
        {
            cout << "Task title too long! Please keep it under 100 characters.\n";
            continue;
        }
        break;
    }
    while (true);

    // Get due date
    do
    {
        cout << "Enter due date (DD/MM/YYYY): ";
        getline(cin, dueDate);

        if (dueDate.empty())
        {
            cout << "Due date cannot be empty! Please try again.\n";
            continue;
        }

        if (!isValidDate(dueDate))
        {
            cout << "Invalid date format! Please use DD/MM/YYYY format.\n";
            continue;
        }
        break;
    }
    while (true);

    // Create new task with unique ID
    int taskId = getNextTaskId();
    Task newTask(taskId, title, dueDate, "Ongoing");

    // O(1) insertion into hash table
    currentUser->tasks[taskId] = newTask;

    cout << "\nTask added successfully!\n";
    cout << "Task ID: " << taskId << "\n";
    cout << "Title: " << title << "\n";
    cout << "Due Date: " << dueDate << "\n";
    cout << "Status: Ongoing\n";
}

// View all tasks using hash table iteration
void UserManager::viewUserTasks(User* currentUser)
{
    cout << "\n--- Your Tasks ---\n";

    if (currentUser->tasks.empty())
    {
        cout << "You have no tasks yet. Add some tasks to get started!\n";
        return;
    }

    cout << left << setw(8) << "ID";
    cout << setw(25) << "Title";
    cout << setw(12) << "Due Date";
    cout << setw(12) << "Status" << "\n";
    cout << string(57, '-') << "\n";

    // Iterate through hash table
    for (const auto& pair : currentUser->tasks)
    {
        const Task& task = pair.second;

        cout << left << setw(8) << task.taskId;

        // Truncate long titles
        string displayTitle = task.title;
        if (displayTitle.length() > 24)
        {
            displayTitle = displayTitle.substr(0, 21) + "...";
        }
        cout << setw(25) << displayTitle;
        cout << setw(12) << task.dueDate;
        cout << setw(12) << task.status << "\n";
    }
}

// O(1) task lookup and editing
void UserManager::editTask(User* currentUser)
{
    cout << "\n--- Edit Task ---\n";

    if (currentUser->tasks.empty())
    {
        cout << "You have no tasks to edit.\n";
        return;
    }

    // First show all tasks
    viewUserTasks(currentUser);

    int taskId;
    cout << "\nEnter Task ID to edit: ";
    if (!(cin >> taskId))
    {
        cout << "Invalid Task ID!\n";
        cin.clear();
        cin.ignore(1000, '\n');
        return;
    }
    cin.ignore();

    // O(1) task lookup
    Task* task = findTaskById(currentUser, taskId);
    if (task == nullptr)
    {
        cout << "Task with ID " << taskId << " not found!\n";
        return;
    }

    cout << "\nCurrent Task Details:\n";
    cout << "ID: " << task->taskId << "\n";
    cout << "Title: " << task->title << "\n";
    cout << "Due Date: " << task->dueDate << "\n";
    cout << "Status: " << task->status << "\n";

    int choice;
    cout << "\nWhat do you want to edit?\n";
    cout << "1. Title\n";
    cout << "2. Due Date\n";
    cout << "3. Status\n";
    cout << "4. Cancel\n";
    cout << "Enter your choice: ";

    if (!(cin >> choice))
    {
        cout << "Invalid choice!\n";
        cin.clear();
        cin.ignore(1000, '\n');
        return;
    }
    cin.ignore();

    switch (choice)
    {
    case 1:
    {
        string newTitle;
        cout << "Enter new title: ";
        getline(cin, newTitle);

        if (!newTitle.empty() && newTitle.length() <= 100)
        {
            task->title = newTitle;
            cout << "Title updated successfully!\n";
        }
        else
        {
            cout << "Invalid title! Title cannot be empty or too long.\n";
        }
        break;
    }

    case 2:
    {
        string newDueDate;
        cout << "Enter new due date (DD/MM/YYYY): ";
        getline(cin, newDueDate);

        if (isValidDate(newDueDate))
        {
            task->dueDate = newDueDate;
            cout << "Due date updated successfully!\n";
        }
        else
        {
            cout << "Invalid date format!\n";
        }
        break;
    }

    case 3:
    {
        int statusChoice;
        cout << "Select new status:\n";
        cout << "1. Ongoing\n";
        cout << "2. Completed\n";
        cout << "Enter choice: ";

        if (cin >> statusChoice)
        {
            if (statusChoice == 1)
            {
                task->status = "Ongoing";
                cout << "Status updated to Ongoing!\n";
            }
            else if (statusChoice == 2)
            {
                task->status = "Completed";
                cout << "Status updated to Completed!\n";
            }
            else
            {
                cout << "Invalid status choice!\n";
            }
        }
        else
        {
            cout << "Invalid input!\n";
        }
        cin.ignore();
        break;
    }

    case 4:
    {
        cout << "Edit cancelled.\n";
        break;
    }

    default:
    {
        cout << "Invalid choice!\n";
        break;
    }
    }
}

// O(1) task completion marking
void UserManager::markTaskCompleted(User* currentUser)
{
    cout << "\n--- Mark Task as Completed ---\n";

    if (currentUser->tasks.empty())
    {
        cout << "You have no tasks to mark as completed.\n";
        return;
    }

    // Show only ongoing tasks
    cout << "Ongoing Tasks:\n";
    cout << left << setw(8) << "ID";
    cout << setw(25) << "Title";
    cout << setw(12) << "Due Date" << "\n";
    cout << string(45, '-') << "\n";

    bool hasOngoingTasks = false;
    for (const auto& pair : currentUser->tasks)
    {
        const Task& task = pair.second;
        if (task.status == "Ongoing")
        {
            hasOngoingTasks = true;
            cout << left << setw(8) << task.taskId;

            string displayTitle = task.title;
            if (displayTitle.length() > 24)
            {
                displayTitle = displayTitle.substr(0, 21) + "...";
            }
            cout << setw(25) << displayTitle;
            cout << setw(12) << task.dueDate << "\n";
        }
    }

    if (!hasOngoingTasks)
    {
        cout << "No ongoing tasks found! All tasks are already completed.\n";
        return;
    }

    int taskId;
    cout << "\nEnter Task ID to mark as completed: ";
    if (!(cin >> taskId))
    {
        cout << "Invalid Task ID!\n";
        cin.clear();
        cin.ignore(1000, '\n');
        return;
    }
    cin.ignore();

    // O(1) task lookup and update
    Task* task = findTaskById(currentUser, taskId);
    if (task == nullptr)
    {
        cout << "Task with ID " << taskId << " not found!\n";
        return;
    }

    if (task->status == "Completed")
    {
        cout << "Task is already completed!\n";
        return;
    }

    task->status = "Completed";
    cout << "\nTask marked as completed successfully!\n";
    cout << "Task: " << task->title << "\n";
    cout << "Congratulations on completing your task!\n";
}

// O(1) task deletion
void UserManager::deleteTask(User* currentUser)
{
    cout << "\n--- Delete Task ---\n";

    if (currentUser->tasks.empty())
    {
        cout << "You have no tasks to delete.\n";
        return;
    }

    viewUserTasks(currentUser);

    int taskId;
    cout << "\nEnter Task ID to delete: ";
    if (!(cin >> taskId))
    {
        cout << "Invalid Task ID!\n";
        cin.clear();
        cin.ignore(1000, '\n');
        return;
    }
    cin.ignore();

    // O(1) task lookup
    Task* task = findTaskById(currentUser, taskId);
    if (task == nullptr)
    {
        cout << "Task with ID " << taskId << " not found!\n";
        return;
    }

    cout << "\nTask to delete:\n";
    cout << "ID: " << task->taskId << "\n";
    cout << "Title: " << task->title << "\n";
    cout << "Due Date: " << task->dueDate << "\n";
    cout << "Status: " << task->status << "\n";

    string confirmation;
    cout << "\nType 'DELETE' to confirm deletion: ";
    getline(cin, confirmation);

    if (confirmation != "DELETE")
    {
        cout << "Task deletion cancelled.\n";
        return;
    }

    // O(1) deletion from hash table
    currentUser->tasks.erase(taskId);
    cout << "\nTask deleted successfully!\n";
}

// O(1) task details viewing
void UserManager::viewTaskDetails(User* currentUser)
{
    cout << "\n--- Task Details ---\n";

    if (currentUser->tasks.empty())
    {
        cout << "You have no tasks.\n";
        return;
    }

    viewUserTasks(currentUser);

    int taskId;
    cout << "\nEnter Task ID to view details: ";
    if (!(cin >> taskId))
    {
        cout << "Invalid Task ID!\n";
        cin.clear();
        cin.ignore(1000, '\n');
        return;
    }
    cin.ignore();

    // O(1) task lookup
    Task* task = findTaskById(currentUser, taskId);
    if (task == nullptr)
    {
        cout << "Task with ID " << taskId << " not found!\n";
        return;
    }

    cout << "\n---------- TASK DETAILS ----------\n";
    cout << "Task ID: " << task->taskId << "\n";
    cout << "Title: " << task->title << "\n";
    cout << "Due Date: " << task->dueDate << "\n";
    cout << "Status: " << task->status << "\n";
    cout << "--------------------------------\n";

    if (task->status == "Completed")
    {
        cout << "This task has been completed!\n";
    }
    else
    {
        cout << "This task is still ongoing.\n";
    }
}

// O(1) task lookup by ID using hash table
Task* UserManager::findTaskById(User* currentUser, int taskId)
{
    auto it = currentUser->tasks.find(taskId);
    return (it != currentUser->tasks.end()) ? &it->second : nullptr;
}

// Delete User Account with hash table cleanup
void UserManager::deleteUserAccount(User* currentUser)
{
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

    if (confirmation != "DELETE")
    {
        cout << "Account deletion cancelled.\n";
        return;
    }

    cout << "\nEnter your password to confirm: ";
    string password;
    getline(cin, password);

    if (currentUser->password != password)
    {
        cout << "Incorrect password! Account deletion cancelled.\n";
        return;
    }

    // Store user info for goodbye message
    string username = currentUser->username;
    int userId = currentUser->id;

    // Remove from all hash tables - O(1) operations
    usersById.erase(userId);
    usersByUsername.erase(currentUser->username);
    usersByEmail.erase(currentUser->email);
    usedUsernames.erase(currentUser->username);
    usedEmails.erase(currentUser->email);

    cout << "\nAccount '" << username << "' has been permanently deleted.\n";
    cout << "Thank you for using our system. Goodbye!\n";

    cout << "Press Enter to return to User Portal...";
    cin.get();
}

// Save all user data and tasks to a file
void UserManager::saveToFile(const string& filename)
{
    ofstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error: Could not save data to file " << filename << endl;
        return;
    }

    // Header
    file << "\n\t\t*** Users Data ***\n" << endl;

    file << "     " << "User Id" << "     "
         << "User Name" << "       "
         << "Email" << "           "
         << "Password" << "      "
         << "Status" << endl;

    // To get users in ascending order, we must create a vector and sort it
    vector<User> sortedUsers;
    for (const auto& pair : usersById)
    {
        sortedUsers.push_back(pair.second);
    }
    sort(sortedUsers.begin(), sortedUsers.end(), [](const User& a, const User& b)
    {
        return a.id < b.id;
    });

    // Iterate through sorted users and their tasks
    for (const auto& user : sortedUsers)
    {
        // Only save valid users with proper data
        if (user.id > 0 && !user.username.empty() && !user.email.empty())
        {
            file << "\nUser --> " << user.id << "        "
                 << user.username << "        "
                 << user.email << "        "
                 << user.password << "        "
                 << (user.isActive ? "Active" : "Inactive") << endl;

            for (const auto& taskPair : user.tasks)
            {
                const Task& task = taskPair.second;
                string title_with_underscores = task.title;
                replace(title_with_underscores.begin(), title_with_underscores.end(), ' ', '_');
                file << "TASK " << task.taskId << " "
                     << title_with_underscores << " "
                     << task.dueDate << " "
                     << task.status << "\n";
            }
        }
    }

    // Footer
    file << "\n=====>>> Total Users: " << usersById.size() << "!\n";

    file.close();
    cout << "Data saved successfully to " << filename << endl;
}


// Load data from file
void UserManager::loadFromFile(const string& filename)
{
    ifstream file(filename);

    if (!file.is_open())
    {
        cerr << "Error: Could not open data file " << filename << endl;
        return;
    }

    // Skip the decorated header lines
    string line;
    while (getline(file, line) && line.find("*** Users Data ***") == string::npos)
    {
        // Skip until we find the header or end of file
        if (file.eof()) break;
    }

    // Skip the column headers
    getline(file, line);

    User* currentUser = nullptr;

    while (getline(file, line))
    {
        // Skip empty lines
        if (line.empty())
        {
            continue;
        }

        stringstream ss(line);
        string type;
        ss >> type;

        if (type == "User")
        {
            // The user's format is "User --> <id> ..."
            string arrow;
            int id;
            string username, email, password, statusStr;

            ss >> arrow >> id >> username >> email >> password >> statusStr;

            // Skip invalid entries - check for valid ID and non-empty username
            if (id <= 0 || username.empty() || email.empty())
            {
                currentUser = nullptr; // Reset currentUser to avoid adding tasks to invalid user
                continue;
            }

            bool isActive = (statusStr == "Active");

            // Only add user if all data is valid
            User newUser(id, username, email, password, isActive);
            usersById[id] = newUser;
            usersByUsername[username] = id;
            usersByEmail[email] = id;
            usedUsernames.insert(username);
            usedEmails.insert(email);
            currentUser = &usersById[id];

            // Update user counter to ensure they're correct
            if (id >= userIdCounter)
            {
                userIdCounter = id + 1;
            }
        }
        else if (type == "TASK" && currentUser)
        {
            int taskId;
            string title, dueDate, status;
            ss >> taskId >> title >> dueDate >> status;

            // Replace underscores back with spaces
            replace(title.begin(), title.end(), '_', ' ');

            Task newTask(taskId, title, dueDate, status);
            currentUser->tasks[taskId] = newTask;
        }
        else if (line.find("Total Users:") != string::npos)
        {
            // End of data block
            break;
        }
    }
    file.close();

    // Update task counter to ensure no conflicts with existing tasks
    for (const auto& userPair : usersById)
    {
        for (const auto& taskPair : userPair.second.tasks)
        {
            if (taskPair.first >= taskIdCounter)
            {
                taskIdCounter = taskPair.first + 1;
            }
        }
    }

    // Update task counter after all tasks are loaded
    for (const auto& userPair : usersById)
    {
        for (const auto& taskPair : userPair.second.tasks)
        {
            if (taskPair.second.taskId >= taskIdCounter)
            {
                taskIdCounter = taskPair.second.taskId + 1;
            }
        }
    }
    updateStatistics();
    cout << "Data loaded successfully." << endl;
}

