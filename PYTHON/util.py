from object import Object, Color
from miscsymbols import Endpoint, Dot, Blob, Star, Triangle, Cancel
from blockgroup import BlockGroup

def getType(thing):
    if (isinstance(thing, Blob)):
        return "_BLOB_"
    if (isinstance(thing, Dot)):
        return "PATHDT"
    if (isinstance(thing, Endpoint)):
        if (thing.starting):
            return "START!"
        return "ENDPT!"
    if (isinstance(thing, Star)):
        return "_STAR_"
    if (isinstance(thing, BlockGroup)):
        return "+BLOCK"
    if (isinstance(thing, Triangle)):
        return "TRIX_" + str(thing.x)
    if (isinstance(thing, Cancel)):
        return "CANCEL"
        
    return "OBJECT"

def isStartingPoint(obj):
    if (not isinstance(obj, Endpoint)):
        return False
    return obj.starting

def isEndingPoint(obj):
    if (not isinstance(obj, Endpoint)):
        return False
    return not obj.starting

def isSymbol(obj):
    if (isinstance(obj, Blob)):
        return True
    if (isinstance(obj, Star)):
        return True
    if (isinstance(obj, Dot)):
        return True
    if (isinstance(obj, BlockGroup)):
        return True
    if (isinstance(obj, Triangle)):
        return True
    if (isinstance(obj, Cancel)):
        return True
    if (isinstance(obj, Endpoint)):
        return True
    return False