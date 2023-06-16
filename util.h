#ifndef UTIL_H
#define UTIL_H

#include <string>
#include "object.h"
#include "blockgroup.h"
#include "miscsymbols.h"

using std::string;
using std::to_string;

template<typename Base, typename T>
inline bool instanceof(T *ptr) {
    T* x = dynamic_cast<Base*>(ptr);
    return x != nullptr;
}

inline string get_type(Object* o) {
    if (instanceof<BlockGroup>(o)) return "+BLOCK";
    if (instanceof<Endpoint>(o)) {
        if ((dynamic_cast<Endpoint*>(o))->starting) return "START!";
        return "ENDPT!";
    }
    if (instanceof<Dot>(o)) return "PATHDT";
    if (instanceof<Star>(o)) return "_STAR_";
    if (instanceof<Blob>(o)) return "_BLOB_";
    if (instanceof<Triangle>(o)) {
        return "TRIX_" + to_string((dynamic_cast<Triangle*>(o))->x);
    }
    if (instanceof<Cancel>(o)) {
        if (!(dynamic_cast<Cancel*>(o))->ignored) return "CANCEL";
        else return "OBJECT";
    }
    return "OBJECT";
}

inline bool isStartingPoint(Object* o) {
    if (instanceof<Endpoint>(o)) {
        if ((dynamic_cast<Endpoint*>(o))->starting) return true;
        return false;
    }
    return false;
}

inline bool isEndingPoint(Object* o) {
    if (instanceof<Endpoint>(o)) {
        if ((dynamic_cast<Endpoint*>(o))->starting) return false;
        return true;
    }
    return false;
}

inline bool isSymbol(Object* o) {
    if (instanceof<BlockGroup>(o)) return true;
    if (instanceof<Endpoint>(o)) return true;
    if (instanceof<Dot>(o)) return true;
    if (instanceof<Star>(o)) return true;
    if (instanceof<Blob>(o)) return true;
    if (instanceof<Triangle>(o)) return true;
    if (instanceof<Cancel>(o)) return true;
    return false;
}

#endif