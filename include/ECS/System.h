#ifndef SYSTEM_H
#define SYSTEM_H

class System {
public:
    System()          = default;
    virtual ~System() = default;

    virtual void initialize() = 0;
    virtual void update()     = 0;
    virtual void cleanup()    = 0;
};

#endif  // SYSTEM_H
