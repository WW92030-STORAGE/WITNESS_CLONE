#ifndef OBJECT_H
#define OBJECT_H

#include <climits>
#include <iostream>
#include <memory>

namespace EntityColor {

enum Color { // Color is an enum to prevent exploiting loopholes (e.g. 2 similar colors considered different)
    NIL = INT_MAX, // Empty color
    
    RGB_RED = 0xFF0000,
    RGB_GREEN = 0x00FF00,
    RGB_BLUE = 0x0000FF,
    RGB_YELLOW = 0xFFFF00,
    RGB_CYAN = 0x00FFFF,
    RGB_MAGENTA = 0xFF00FF,
    RGB_BLACK = 0,
    RGB_WHITE = 0xFFFFFF,
    
    RGB_ORANGE = 0xFF8000,
    RGB_GREY = 0x808080,
    RGB_DARK = 0x404040,
    RGB_LIGHT = 0x0C0C0C,
    
    // Furries!
    
    SP2 = 0xCEB7FF,
    SP1 = 0x8AD8FF
};

};

class Entity {
    public:
    
    EntityColor::Color color;
    bool isPath; // Is Pathable (false if cut or is symbol cell)
    bool isPathOccupied; // Is there a path here
    
    Entity() {
        isPath = false;
        isPathOccupied = false;
        color = EntityColor::NIL;
    }
    
    std::shared_ptr<Entity> clear() {
        std::shared_ptr<Entity> res = std::shared_ptr<Entity>(new Entity());
        res->isPath = isPath;
        res->isPathOccupied = isPathOccupied;
        res->color = EntityColor::NIL;
        return res;
    }
    
    virtual ~Entity() {
        // std::cout << "!";
    }
};

#endif