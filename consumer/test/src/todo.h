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

class TodoClient {
  public:
    std::string serverUrl;

    TodoClient();

    std::vector<Project> getProjects(std::string format = "json");
};
