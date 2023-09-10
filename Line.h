#ifndef LINE_H
#define LINE_H

#include "params.h"
#include "Point.h"

enum RelationType
{
    COINCIDENT,
    IN_FRONT,
    BEHIND,
    SPANNING
};
std::ostream &operator<<(std::ostream &os, const RelationType &type);

class Line;
class Vector;
class LineSegment
{
private:
    Point p1;
    Point p2;

public:
    LineSegment() : p1(), p2() {}
    LineSegment(Point p1, Point p2) : p1(p1), p2(p2) {}
    ~LineSegment() = default;

    Point getP1() const { return p1; }
    Point getP2() const { return p2; }

    void setP1(Point p1) { this->p1 = p1; }
    void setP2(Point p2) { this->p2 = p2; }

    NType length() const
    {
        return p1.distance(p2);
    }
    bool operator==(const LineSegment &l) const
    {
        return p1 == l.p1 && p2 == l.p2;
    }
    bool operator!=(const LineSegment &l) const
    {
        return !(*this == l);
    }
    Vector getDirection() const;

    NType distance(Point Q, bool sign = false) const;

    // Relación con una línea
    RelationType relationToLine(const Line &l, LineSegment &front, LineSegment &back) const;

    // Intersección
    bool intersect(const LineSegment &l) const;

    // Imprimir
    friend std::ostream &operator<<(std::ostream &os, const LineSegment &l)
    {
        os << "[" << l.p1 << " to " << l.p2 << "]";
        return os;
    }
};

class Vector : public Point
{
public:
    Vector() : Point(0, 0) {}
    Vector(NType x, NType y) : Point(x, y) {}
    Vector(const Point &p) : Point(p) {}
    ~Vector() = default;

    // Producto Punto
    NType dotProduct(const Vector &v) const
    {
        return getX() * v.getX() + getY() * v.getY();
    }
    NType dotProduct(const Point &p) const
    {
        return getX() * p.getX() + getY() * p.getY();
    }

    // Operadores aritméticos
    Vector operator-() const
    {
        return Vector(-getX(), -getY());
    }
    Vector operator*(const NType k) const
    {
        return Vector(getX() * k, getY() * k);
    }
    Vector operator/(const NType k) const
    {
        return Vector(getX() / k, getY() / k);
    }
    Vector operator+(const Vector &v) const
    {
        return Vector(getX() + v.getX(), getY() + v.getY());
    }
    Vector operator-(const Vector &v) const
    {
        return Vector(getX() - v.getX(), getY() - v.getY());
    }
    Vector &operator+=(const Vector &v)
    {
        setX(getX() + v.getX());
        setY(getY() + v.getY());
        return *this;
    }
    Vector &operator-=(const Vector &v)
    {
        setX(getX() - v.getX());
        setY(getY() - v.getY());
        return *this;
    }
    Vector &operator*=(NType k)
    {
        setX(getX() * k);
        setY(getY() * k);
        return *this;
    }
    Vector &operator/=(NType k)
    {
        setX(getX() / k);
        setY(getY() / k);
        return *this;
    }
    Vector getPerpendicular() const
    {
        return Vector(-getY(), getX());
    }

    // Magnitud y dirección
    NType magnitude() const
    {
        return sqrt(pow(getX(), 2) + pow(getY(), 2));
    }
    Vector unitVector() const
    {
        return *this / magnitude();
    }
    Vector direction() const
    {
        return *this / magnitude();
    }

    // Normalizar el vector
    void normalize()
    {
        *this /= magnitude();
    }

    // Imprimir
    friend std::ostream &operator<<(std::ostream &os, const Vector &v)
    {
        os << "{" << v.getX().getValue() << "," << v.getY().getValue() << "}";
        return os;
    }
};

class Line
{
private:
    Point p;  // Punto de la línea
    Vector v; // Vector unitario (dirección) de la línea
public:
    Line() : p(), v() {}
    Line(Point p, Vector v) : p(p), v(v.unitVector()) {}
    Line(Point p1, Point p2) : p(p1), v(Vector(p2 - p1).unitVector()) {}
    Line(const LineSegment &l) : p(l.getP1()), v(Vector(l.getP2() - l.getP1()).unitVector()) {}
    ~Line() = default;

    Point getPoint() const { return p; }
    Vector getUnitVector() const { return v; }

    void setPoint(Point p) { this->p = p; }
    void setVector(Vector v) { this->v = v.unitVector(); }

    Vector getNormalVector() const
    {
        return Vector(-v.getY(), v.getX());
    }

    // Paralinealidad
    bool isParallel(const Line &l) const
    {
        Vector v1 = this->getUnitVector();
        Vector v2 = l.getUnitVector();

        NType crossProduct = v1.dotProduct(v2);
        return abs(crossProduct) == 1;
    }
    bool isParallel(const Vector &v) const
    {
        NType crossProduct = this->getUnitVector().dotProduct(v);
        return abs(crossProduct) == 1;
    }
    bool isParallel(const LineSegment &l) const
    {
        Vector v1 = this->getUnitVector();
        Vector v2 = l.getDirection().unitVector();

        NType dotProduct = v1.dotProduct(v2);
        return abs(dotProduct) == 1;
    }

    // Intersección
    bool intersect(const Line &l) const
    {
        return !isParallel(l);
    }
    bool intersect(const LineSegment &l) const
    {
        Point P1 = l.getP1();
        Point P2 = l.getP2();
        Point Q = p;

        NType det = -v.getX() * (P2.getY() - P1.getY()) + v.getY() * (P2.getX() - P1.getX());
        if (abs(det) == 0)
        { // Las líneas son paralelas o coincidentes
            return false;
        }

        NType t = ((P1.getX() - Q.getX()) * (P2.getY() - P1.getY()) - (P1.getY() - Q.getY()) * (P2.getX() - P1.getX())) / det;
        NType u = (v.getX() * (P1.getY() - Q.getY()) - v.getY() * (P1.getX() - Q.getX())) / det;

        return (u >= 0.0 && u <= 1.0);
    }

    // Punto de intersección
    Point intersectionPoint(const Line &l) const
    {
        Vector v2 = l.getUnitVector();
        Point p2 = l.getPoint();
        NType det = v.getX() * v2.getY() - v.getY() * v2.getX();
        if (abs(det) == 0)
        {
            throw std::runtime_error("Las líneas no se intersectan");
        }

        NType t = ((p2.getX() - p.getX()) * v2.getY() - (p2.getY() - p.getY()) * v2.getX()) / det;

        return Point(p.getX() + t * v.getX(), p.getY() + t * v.getY());
    }
    Point intersectionPoint(const LineSegment &l) const
    {
        Point P1 = l.getP1();
        Point P2 = l.getP2();
        Point Q = p;

        NType det = -v.getX() * (P2.getY() - P1.getY()) + v.getY() * (P2.getX() - P1.getX());
        if (abs(det) == 0)
        {
            throw std::runtime_error("Las líneas no se intersectan");
        }

        NType t = ((P1.getY() - Q.getY()) * (P2.getX() - P1.getX()) - (P1.getX() - Q.getX()) * (P2.getY() - P1.getY())) / det;
        NType u = (v.getX() * (P1.getY() - Q.getY()) - v.getY() * (P1.getX() - Q.getX())) / det;

        if (u >= 0.0 && u <= 1.0)
        {
            return Point(Q.getX() + t * v.getX(), Q.getY() + t * v.getY());
        }
        else
        {
            throw std::runtime_error("Las líneas no se intersectan");
        }
    }

    // Perpendicularidad
    bool isPerpendicular(const Line &l) const
    {
        Vector v1 = this->getUnitVector();
        Vector v2 = l.getUnitVector();

        NType dotProduct = v1.dotProduct(v2);
        return dotProduct == 0;
    }
    bool isPerpendicular(const Vector &v) const
    {
        Vector v1 = this->getUnitVector();
        Vector v2 = v.unitVector();

        NType dotProduct = v1.dotProduct(v2);
        return dotProduct == 0;
    }

    bool operator==(const Line &l) const
    {
        return isParallel(l.p - p) && isParallel(l);
    }
    bool operator!=(const Line &l) const
    {
        return !(*this == l);
    }

    // Contenencia de puntos y segmentos
    bool contains(const Point &p) const
    {
        return isParallel(p) && isParallel(Vector(p - this->p));
    }
    bool contains(const LineSegment &l) const
    {
        return contains(l.getP1()) && contains(l.getP2());
    }

    // Distance
    NType distance(const Point &p, bool sign = false) const
    {
        Vector n = getNormalVector();
        Vector w = Vector(p - getPoint());

        NType d = n.dotProduct(w) / n.magnitude();
        return sign ? d : abs(d);
    }
    NType distance(const Line &l, bool sign = false) const
    {
        if (isParallel(l))
        {
            return distance(l.getPoint(), sign);
        }
        else
        {
            return NType(0);
        }
    }
    NType distance(const LineSegment &l, bool sign = false) const
    {
        if (isParallel(l))
        {
            return distance(l.getP1(), sign);
        }
        else
        {
            NType d1 = distance(l.getP1(), true);
            NType d2 = distance(l.getP2(), true);

            if (d1 * d2 <= 0)
            {
                return NType(0);
            }
            else
            {
                NType minDistance = min(abs(d1), abs(d2));
                return sign ? minDistance * (d1 < NType(0) ? NType(-1) : NType(1)) : minDistance;
            }
        }
    }

    // Imprimir
    friend std::ostream &operator<<(std::ostream &os, const Line &l)
    {
        os << "P:" << l.p << " V:" << l.v;
        return os;
    }
};

#endif