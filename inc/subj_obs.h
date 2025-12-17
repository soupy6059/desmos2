#pragma once

class Observer;

class Subject {
    std::vector<std::shared_ptr<Observer>> observers;
public:
    Subject(): observers{} {}
    virtual ~Subject() = 0;
    void attach(decltype(observers)::value_type observer) {
        observers.push_back(observer);
    }
    void detach(decltype(observers)::value_type observer) {
        for(decltype(observers)::iterator it{observers.begin()}; it != observers.end(); ++it) 
            if(*it == observer) {
                observers.erase(it);
            }
    }
    void clear_observers() { observers.clear(); }
};
Subject::~Subject() {}

class Observer {
public:
    Observer() {}
    virtual ~Observer() = 0;
};
Observer::~Observer() {}

