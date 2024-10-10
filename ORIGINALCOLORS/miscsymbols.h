#ifndef MISC_H
#define MISC_H

#include "object.h"

class Endpoint : public Entity {
    public:
    bool starting;
    Endpoint(bool s) : Entity() {
        starting = s;
        isPath = true;
    }
};

class Dot : public Entity {
    public:
    Dot() : Entity() {
        isPath = true;
    }
};

class Blob : public Entity {
    public:
    Blob() : Entity() {
        color = EntityColor::RGB_BLACK;
    }
    
    Blob(EntityColor::Color c) : Entity() {
        color = c;
    }
};

class Star : public Entity {
    public:
    Star() : Entity() {
        color = EntityColor::RGB_WHITE;
    }
    
    Star(EntityColor::Color c) : Entity() {
        color = c;
    }
};

class Triangle : public Entity {
    public:
    int x;
    Triangle(int n) : Entity() {
        x = n;
        color = EntityColor::RGB_ORANGE;
    }
    
    Triangle(int n, EntityColor::Color c) : Entity() {
        x = n;
        color = c;
    }
};

class Cancel : public Entity {
    public:
    bool ignored;
    Cancel() : Entity() {
        color = EntityColor::NIL;
        ignored = false;
    }
};

#endif