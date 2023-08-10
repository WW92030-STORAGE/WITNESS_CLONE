import sys
from enum import Enum

class Color(Enum):
    NIL = (sys.maxsize, sys.maxsize, sys.maxsize)
    
    RED = (255, 0, 0)
    GREEN = (0, 255, 0)
    BLUE = (0, 0, 255)
    YELLOW = (255, 255, 0)
    CYAN = (0, 255, 255)
    MAGENTA = (255, 0, 255)
    BLACK = (0, 0, 0)
    WHITE = (255, 255, 255)
    
    ORANGE = (255, 127, 0)
    GREY = (127, 127, 127)
    DARK = (63, 63, 63)
    LIGHT = (191, 191, 191)
    
    # Furries!
    
    SP2 = (206, 183, 255)
    SP1 = (138, 216, 255)

class Object:
    color = Color.NIL
    isPath = False
    isPathOccupied = False
    
    def __init__(self):
        isPath = False
        isPathOccupied = False
        color = Color.NIL
    
    def clear(self):
        res = Object()
        res.isPath = self.isPath
        res.isPathOccupied = self.isPathOccupied
        res.color = Color.NIL
        return res
    
    
    