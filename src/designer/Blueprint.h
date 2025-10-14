#ifndef BLUEPRINT_H
#define BLUEPRINT_H
#include <vector>

#include "VisualNode.h"


class Blueprint {
public:
    Blueprint() = default;
    ~Blueprint() = default;

    void Render();
private:
    std::vector<VisualNode*> _nodes;
};



#endif //BLUEPRINT_H
