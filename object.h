enum Color { // Color is an enum to prevent exploiting loopholes (e.g. 2 similar colors considered different)
    NIL = INT_MAX, // Empty color
    
    RED = 0xFF0000,
    GREEN = 0x00FF00,
    BLUE = 0x0000FF,
    YELLOW = 0xFFFF00,
    CYAN = 0x00FFFF,
    MAGENTA = 0xFF00FF,
    BLACK = 0,
    WHITE = 0xFFFFFF,
    
    ORANGE = 0xFF8000,
    GREY = 0x808080,
    
    SP2 = 0xCEB7FF,
    SP1 = 0x8AD8FF
};

class Object {
    public:
    
    Color color;
    bool isPath; // Is Pathable (false if cut or is symbol cell)
    bool isPathOccupied; // Is there a path here
    
    Object() {
        isPath = false;
        color = NIL;
    }
    
    virtual ~Object() {
        
    }
};