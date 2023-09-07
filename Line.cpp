#include "Line.h"

RelationType LineSegment::relationToLine(const Line& l, LineSegment& front, LineSegment& back) const {
    // Aqui su codigo
}

bool LineSegment::intersect(const LineSegment& l) const{
    // Coordenadas de los puntos del primer segmento
    NType x1 = p1.getX(), y1 = p1.getY();
    NType x2 = p2.getX(), y2 = p2.getY();

    // Coordenadas de los puntos del segundo segmento
    NType x3 = l.getP1().getX(), y3 = l.getP1().getY();
    NType x4 = l.getP2().getX(), y4 = l.getP2().getY();

    // Calcular el determinante
    NType det = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

    if (abs(det) == 0) { // Las líneas son paralelas o coincidentes
        // Comprobar si se superponen
        if ((min(x1, x2) <= max(x3, x4) && max(x1, x2) >= min(x3, x4)) &&
            (min(y1, y2) <= max(y3, y4) && max(y1, y2) >= min(y3, y4))) {
            return true;
        }else{
            return false;
        }
    }

    // Encontrar los parámetros t y s
    NType t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / det;
    NType s = ((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / det;

    return (t >= 0 && t <= 1 && s >= 0 && s <= 1);
}


Vector LineSegment::getDirection() const {
    Vector v(p2 - p1);
    return v.unitVector();
}

std::ostream& operator<<(std::ostream& os, const RelationType& type){
    switch (type) {
        case COINCIDENT: os << "COINCIDENT"; break;
        case IN_FRONT:   os << "IN_FRONT";   break;
        case BEHIND:     os << "BEHIND";     break;
        case SPANNING:   os << "SPANNING";   break;
    }
    return os;
}


