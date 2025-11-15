#ifndef APP_H
#define APP_H
#include <vector>

#include "ProjectWindow.h"


class App {
public:
    App();
    void tick();
private:
    std::atomic_uint idCounter = 0;
    std::vector<std::unique_ptr<ProjectWindow>> projectWindows {};
    unsigned int selectedProject = 0;

    void addNewProject();
};



#endif //APP_H
