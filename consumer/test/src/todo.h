#pragma once

#include <string>
#include <vector>

class Project {
  public:
    unsigned int id;
    std::string name;
    std::string due;
};

class TodoClient {
  public:
    std::string serverUrl;

    TodoClient();

    std::vector<Project> getProjects(std::string format = "json");
};
