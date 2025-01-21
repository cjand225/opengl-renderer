#ifndef COMPONENT_H
#define COMPONENT_H

class Component {
public:
    virtual ~Component() = default;

    virtual void initialize() = 0;
    virtual void update()     = 0;
    virtual void cleanup()    = 0;
};

#endif  // COMPONENT_H
