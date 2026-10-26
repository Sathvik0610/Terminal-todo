#include <cstddef>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

using namespace std;

class Tasks {
  public:
    int priority;
    string taskname;
    string tdescipt;

    Tasks(int p, string n, string d) : priority(p), taskname(n), tdescipt(d) {}
};

// Get task details from the user.
Tasks task_input() {
    string name, description;
    int priority;

    cout << "\nEnter a task name: ";
    getline(cin, name);

    cout << "Enter a description: ";
    getline(cin, description);

    cout << "Enter priority (0-2): ";

    while (!(cin >> priority) || priority < 0 || priority > 2) {
        cout << "Invalid priority. Enter a number from 0 to 2: ";

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    return Tasks(priority, name, description);
}

// Display one task.
void show_task(const Tasks &task) {
    cout << "Task name: " << task.taskname << '\n';
    cout << "Priority: " << task.priority << '\n';
    cout << "Description: " << task.tdescipt << '\n';
}

// Display all tasks with numbers.
void show_all_tasks(const vector<Tasks> &tasks) {
    if (tasks.empty()) {
        cout << "No tasks yet.\n";
        return;
    }

    for (size_t i = 0; i < tasks.size(); ++i) {
        cout << "\n" << i + 1 << ". ";
        show_task(tasks[i]);
        cout << "--------------------\n";
    }
}

// Return a zero-based index, or -1 if no tasks exist.
int select_task(const vector<Tasks> &tasks) {
    if (tasks.empty()) {
        cout << "No tasks available.\n";
        return -1;
    }

    show_all_tasks(tasks);

    int idx = 0;

    cout << "Enter task number (1-" << tasks.size() << "): ";

    while (!(cin >> idx) || idx <= 0 ||
           static_cast<size_t>(idx) > tasks.size()) {
        cout << "Invalid task number. Enter a number from 1 to " << tasks.size()
             << ": ";

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    return idx - 1;
}

// Save every task to tasks.txt.
void save_tasks(const vector<Tasks> &tasks) {
    ofstream file("tasks.txt");

    if (!file) {
        cerr << "Error: Could not save tasks.txt.\n";
        return;
    }

    for (const Tasks &task : tasks) {
        file << task.priority << ' ' << quoted(task.taskname) << ' '
             << quoted(task.tdescipt) << '\n';
    }

    if (!file) {
        cerr << "Error: Writing tasks.txt failed.\n";
    }
}

// Load tasks from tasks.txt when the program starts.
void load_tasks(vector<Tasks> &tasks) {
    ifstream file("tasks.txt");

    if (!file) {
        // No file yet is normal on the first run.
        return;
    }

    int priority;
    string name, description;

    while (file >> priority >> quoted(name) >> quoted(description)) {
        if (priority < 0 || priority > 2) {
            cerr << "Skipping task with invalid priority.\n";
            continue;
        }

        tasks.emplace_back(priority, name, description);
    }

    if (!file.eof()) {
        cerr << "Warning: Could not read all data in tasks.txt.\n";
    }
}

void update_task(vector<Tasks> &tasks) {
    int idx = select_task(tasks);

    if (idx == -1) {
        return;
    }

    cout << "\nEnter the replacement task details:\n";
    tasks[idx] = task_input();

    save_tasks(tasks);
    cout << "Task updated successfully.\n";
}

void delete_task(vector<Tasks> &tasks) {
    int idx = select_task(tasks);

    if (idx == -1) {
        return;
    }

    tasks.erase(tasks.begin() + idx);

    save_tasks(tasks);
    cout << "Task deleted successfully.\n";
}

int main() {
    vector<Tasks> tasks;

    // Restore previously saved tasks.
    load_tasks(tasks);

    int choice = 0;

    while (choice != 5) {
        cout << "\n=== Todo Manager ===\n"
             << "1. Create a task\n"
             << "2. Update a task\n"
             << "3. Delete a task\n"
             << "4. Show all tasks\n"
             << "5. Exit\n"
             << "Enter a choice (1-5): ";

        if (!(cin >> choice)) {
            cout << "Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
        case 1:
            tasks.push_back(task_input());
            save_tasks(tasks);
            cout << "Task added successfully.\n";
            break;

        case 2:
            update_task(tasks);
            break;

        case 3:
            delete_task(tasks);
            break;

        case 4:
            show_all_tasks(tasks);
            break;

        case 5:
            save_tasks(tasks);
            cout << "Tasks saved. Goodbye!\n";
            break;

        default:
            cout << "Invalid choice. Select 1-5.\n";
            break;
        }
    }

    return 0;
}
