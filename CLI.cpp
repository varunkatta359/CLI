#include <iostream>
#include <vector>
#include <memory>   // for smart pointers
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>  // for std::find_if

// Task structure
class Task {
private:
    int id;
    std::string description;
    int priority;

public:
    Task(int id, const std::string &desc, int priority)
        : id(id), description(desc), priority(priority) {}

    // Getter methods
    int getId() const { return id; }
    const std::string& getDescription() const { return description; }
    int getPriority() const { return priority; }
};

// TaskManager class to handle tasks
class TaskManager {
private:
    std::vector<std::unique_ptr<Task>> tasks;
    int nextTaskID = 1;

public:
    // Add a task
    void addTask(const std::string &description, int priority) {
        tasks.push_back(std::make_unique<Task>(nextTaskID++, description, priority));
        std::cout << "Task added successfully!" << std::endl;
    }

    // Delete a task
    void deleteTask(int id) {
        auto it = std::find_if(tasks.begin(), tasks.end(), [id](const std::unique_ptr<Task>& task) {
            return task->getId() == id;
        });
        if (it != tasks.end()) {
            tasks.erase(it);
            std::cout << "Task deleted successfully!" << std::endl;
        } else {
            std::cerr << "Error: Task not found!" << std::endl;
        }
    }

    // View all tasks
    void viewTasks() const {
        if (tasks.empty()) {
            std::cout << "No tasks available." << std::endl;
            return;
        }
        for (const auto& task : tasks) {
            std::cout << "ID: " << task->getId() << " | Description: " << task->getDescription() << " | Priority: " << task->getPriority() << std::endl;
        }
    }

    // Search for tasks by a keyword
    void searchTask(const std::string &keyword) const {
        bool found = false;
        for (const auto& task : tasks) {
            if (task->getDescription().find(keyword) != std::string::npos) {
                std::cout << "ID: " << task->getId() << " | Description: " << task->getDescription() << " | Priority: " << task->getPriority() << std::endl;
                found = true;
            }
        }
        if (!found) {
            std::cout << "No tasks found with the keyword: " << keyword << std::endl;
        }
    }

    // Save tasks to a file
    void saveTasks(const std::string &filename) const {
        std::ofstream outFile(filename);
        if (!outFile) {
            std::cerr << "Error: Could not open file for writing." << std::endl;
            return;
        }
        for (const auto& task : tasks) {
            outFile << task->getId() << ',' << task->getDescription() << ',' << task->getPriority() << std::endl;
        }
        std::cout << "Tasks saved successfully!" << std::endl;
    }

    // Load tasks from a file
    void loadTasks(const std::string &filename) {
        std::ifstream inFile(filename);
        if (!inFile) {
            std::cerr << "Error: Could not open file for reading." << std::endl;
            return;
        }
        tasks.clear();
        std::string line;
        while (std::getline(inFile, line)) {
            std::istringstream iss(line);
            std::string description;
            int id, priority;
            char delim;
            
            // Parse the line in the format: id,description,priority
            if (iss >> id >> delim && std::getline(iss, description, ',') && iss >> priority) {
                tasks.push_back(std::make_unique<Task>(id, description, priority));
            } else {
                std::cerr << "Error: Invalid file format." << std::endl;
                return;
            }
        }
        std::cout << "Tasks loaded successfully!" << std::endl;
    }
};


// Custom argument parsing
void showUsage() {
    std::cout << "Usage: taskmanager --add <description> <priority> | --delete <id> | --view | --search <keyword> | --save <filename> | --load <filename>" << std::endl;
}

int main(int argc, char* argv[]) {
    TaskManager manager;

    if (argc < 2) {
        showUsage();
        return 1;
    }

    std::string command = argv[1];

    try {
        if (command == "--add" && argc == 4) {
            std::string desc = argv[2];
            int priority = std::stoi(argv[3]);
            manager.addTask(desc, priority);
        } else if (command == "--delete" && argc == 3) {
            int id = std::stoi(argv[2]);
            manager.deleteTask(id);
        } else if (command == "--view") {
            manager.viewTasks();
        } else if (command == "--search" && argc == 3) {
            std::string keyword = argv[2];
            manager.searchTask(keyword);
        } else if (command == "--save" && argc == 3) {
            std::string filename = argv[2];
            manager.saveTasks(filename);
        } else if (command == "--load" && argc == 3) {
            std::string filename = argv[2];
            manager.loadTasks(filename);
        } else {
            showUsage();
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
