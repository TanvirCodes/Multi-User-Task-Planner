#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
using namespace std;

// Task structure - now using unordered_map instead of linked list
struct Task {
    int taskId;
    string title;
    string dueDate;
    string status;

    // Default constructor
    Task() : taskId(0), status("Ongoing") {}

    // Parameterized constructor
    Task(int id, const string& t, const string& date, const string& s = "Ongoing")
        : taskId(id), title(t), dueDate(date), status(s) {}
};

// User structure - using hash maps for tasks
struct User {
    int id;
    string username;
    string email;
    string password;
    bool isActive;
    unordered_map<int, Task> tasks;  // Hash map: taskId -> Task (O(1) access)

    // Default constructor
    User() : id(0), isActive(true) {}

    // Parameterized constructor
    User(int userId, const string& user, const string& mail, const string& pass, bool active = true)
        : id(userId), username(user), email(mail), password(pass), isActive(active) {}
};

class UserManager {
private:
    unordered_map<int, User> usersById;                    // Primary storage: userId -> User
    unordered_map<string, int> usersByUsername;            // Index: username -> userId
    unordered_map<string, int> usersByEmail;               // Index: email -> userId
    unordered_set<string> usedUsernames;                   // Fast uniqueness check
    unordered_set<string> usedEmails;                      // Fast email uniqueness check

    int userIdCounter;      // For generating unique user IDs
    int taskIdCounter;      // For generating unique task IDs

    // Statistics counters
    int activeUsersCount;
    int totalTasksCount;
    int completedTasksCount;

    // Helper methods
    void updateStatistics();                               // Update counters
    User* findUserById(int id);                           // O(1) user lookup
    User* findUserByUsername(const string& username);     // O(1) user lookup
    bool isValidEmail(const string& email);               // Email validation
    bool isValidDate(const string& date);                 // Date validation
    int getNextTaskId();                                   // Generate unique task ID

    // User Portal helper methods
    void userDashboard(User* currentUser);
    void editProfile(User* currentUser);
    void taskManagement(User* currentUser);

public:
    UserManager();
    ~UserManager();  // Destructor for cleanup

    // Admin authentication and dashboard
    bool adminLogin();
    void showAdminDashboard();
    void adminDashboardMenu();

    // User management - All O(1) operations now!
    bool addUser(const string& username, const string& email, const string& password);
    void viewAllUsers();
    void viewAllUserTasks();
    void filterUsers();
    void searchUsers(const string& keyword);

    // User status control - O(1) operations
    bool toggleUserActivation(int id);
    bool isUserActiveById(int id);

    // User Portal functions
    void userPortalMenu();
    bool userRegistration();
    User* userLogin();
    void deleteUserAccount(User* currentUser);

    // Username and email validation - O(1) operations
    bool isUsernameUnique(const string& username);
    bool isEmailUnique(const string& email);
    bool authenticateUser(const string& username, const string& password);

    // Task management functions - O(1) task operations within user
    void addTask(User* currentUser);
    void viewUserTasks(User* currentUser);
    void editTask(User* currentUser);
    void markTaskCompleted(User* currentUser);
    void deleteTask(User* currentUser);
    void viewTaskDetails(User* currentUser);
    Task* findTaskById(User* currentUser, int taskId);

    // File persistence
    void saveToFile(const string& filename = "data.txt");
    void loadFromFile(const string& filename = "data.txt");

    // Utility functions for statistics
    int getTotalUsersCount() const { return usersById.size(); }
    int getActiveUsersCount() const { return activeUsersCount; }
    int getTotalTasksCount() const { return totalTasksCount; }
    int getCompletedTasksCount() const { return completedTasksCount; }
};

#endif
