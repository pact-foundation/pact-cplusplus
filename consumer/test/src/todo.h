#pragma once

#include <string>
#include <vector>

class Project {

};

class TodoClient {
  public:
    std::string serverUrl;

    TodoClient();

    std::vector<Project> getProjects(std::string format = "json");
};
