#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include <iostream>
using namespace std;

// Task structure for linked list of user tasks
struct Task {
    int taskId;
    string title, dueDate, status;
    Task* next;
};

// User structure for linked list of system users
struct User {
    int id;
    string username, email, password;
    bool isActive;          // True = active, False = deactivated
    Task* taskHead;         // Pointer to user's first task
    User* next;
};

class UserManager {
private:
    User* head;             // First user in linked list
    int userIdCounter;      // For generating unique user IDs
    User* findById(int id); // Helper method to find user by ID for activate or deactivate easily

public:
    UserManager();
    bool adminLogin();  // Authentication
    void showAdminDashboard(); // Dashboard operations
    void adminDashboardMenu();

    // User management
    void addUser(string username, string email, string password);
    void viewAllUsers();
    void viewAllUserTasks();
    void filterUsers();
    void searchUsers(string keyword);

    // User status control
    void deactivateUser(int id);        // toggles active/inactive status
    bool isUserActiveById(int id);      // check if user exists and is active
};

#endif
