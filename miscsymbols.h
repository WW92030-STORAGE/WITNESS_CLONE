#ifndef MISC_H
#define MISC_H

class Endpoint : public Object {
    public:
    bool starting;
    Endpoint(bool s) : Object() {
        starting = s;
        isPath = true;
    }
};

class Dot : public Object {
    public:
    Dot() : Object() {
        isPath = true;
    }
};

class Blob : public Object {
    public:
    Blob() : Object() {
        color = BLACK;
    }
    
    Blob(Color c) : Object() {
        color = c;
    }
};

class Star : public Object {
    public:
    Star() : Object() {
        color = WHITE;
    }
    
    Star(Color c) : Object() {
        color = c;
    }
};

class Triangle : public Object {
    public:
    int x;
    Triangle(int n) : Object() {
        x = n;
        color = ORANGE;
    }
    
    Triangle(int n, Color c) : Object() {
        x = n;
        color = c;
    }
};

class Cancel : public Object {
    public:
    bool ignored;
    Cancel() : Object() {
        color = NIL;
        ignored = false;
    }
};

#endif