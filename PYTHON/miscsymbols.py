from object import Object, Color

class Endpoint(Object):
    starting = False
    def __init__(self, start):
        self.starting = start
        self.isPath = True

class Dot(Object):
    def __init__(self):
        self.isPath = True

class Blob(Object):
    def __init__(self, col = Color.BLACK):
        self.color = col
        
class Star(Object):
    def __init__(self, col = Color.WHITE):
        self.color = col

class Triangle(Object):
    x = 1
    def __init__(self, num, col = Color.ORANGE):
        self.color = col
        self.x = num

class Cancel(Object):
    ignored = False
    def __init__(self):
        self.color = Color.NIL
        self.ignored = False
