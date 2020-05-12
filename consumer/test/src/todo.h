#pragma once

#include <string>
#include <vector>

class Task {
  public:
    unsigned int id;
    std::string name;
    bool done;
};

class Project {
  public:
    unsigned int id;
    std::string name;
    std::string due;
    std::vector<Task> tasks;
};

/**
 * Example client class to demonstration the DSL 
 */
class TodoClient {
  public:
    std::string serverUrl;

    TodoClient();

    // Returns all the projects ans tasks
    std::vector<Project> getProjects(std::string format = "json");
    // creates a new task for a project by id using an image upload
    bool postImage(unsigned int id, std::string file_path);
};
