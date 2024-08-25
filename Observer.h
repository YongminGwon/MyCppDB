#pragma once

class Observer {
public:
    virtual ~Observer() = default;
    virtual void update() = 0;
};
class Subject {
public:
    virtual ~Subject() = default;
    virtual void attach(Observer* observer) = 0;
    virtual void detach(Observer* observer) = 0;
    virtual void notify() = 0;
};

