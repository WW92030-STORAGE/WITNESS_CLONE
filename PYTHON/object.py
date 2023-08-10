import sys
from enum import Enum

class Color(Enum):
    NIL = sys.maxsize
    
    RED = 0xFF0000
    GREEN = 0x00FF00
    BLUE = 0x0000FF
    YELLOW = 0xFFFF00
    CYAN = 0x00FFFF
    MAGENTA = 0xFF00FF
    BLACK = 0
    WHITE = 0xFFFFFF
    
    ORANGE = 0xFF8000
    GREY = 0x808080
    DARK = 0x404040
    LIGHT = 0x0C0C0C
    
    # Furries!
    
    SP2 = 0xCEB7FF
    SP1 = 0x8AD8FF

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
    
    
    