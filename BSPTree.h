#ifndef BSP_H
#define BSP_H

#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <set>
#include "Point.h"
#include "Line.h"

struct CollisionResult
{
    bool collisionDetected;
    bool collisionWithPlayer;
    Vector normal;
};

class BSPNode
{
private:
    std::vector<LineSegment> segments;
    Line line;       // Linea que divide el nodo
    bool leafStatus; // Indica si el nodo es una hoja

public:
    BSPNode *frontNode;
    BSPNode *backNode;
    BSPNode *parent;

    BSPNode() : segments(), line(), leafStatus(false), frontNode(nullptr), backNode(nullptr), parent(nullptr) {}
    ~BSPNode() = default;

    // Constructor para nodo hoja
    BSPNode(const std::vector<LineSegment> &segments) : segments(segments), line(), leafStatus(true), frontNode(nullptr), backNode(nullptr) {}

    // Constructor para nodo interno
    BSPNode(Line line) : segments(), line(line), leafStatus(false), frontNode(nullptr), backNode(nullptr) {}
    BSPNode(Line line, BSPNode *frontNode, BSPNode *backNode) : segments(), line(line), leafStatus(false), frontNode(frontNode), backNode(backNode) {}

    // Getters
    std::vector<LineSegment> getSegments() const { return segments; }
    Line getLine() const { return line; }
    BSPNode *getFrontNode() const { return frontNode; }
    BSPNode *getBackNode() const { return backNode; }
    bool isLeaf() const { return leafStatus; }

    // Setters
    void setSegments(const std::vector<LineSegment> &segments) { this->segments = segments; }
    void setLine(const Line &line) { this->line = line; }
    void setLeaf() { this->leafStatus = true; }
    void setInternal() { this->leafStatus = false; }
    void setFrontNode(BSPNode *node) { frontNode = node; }
    void setBackNode(BSPNode *node) { backNode = node; }

    // Inserción
    void insert(const LineSegment &segment);
    BSPNode *insert(const std::vector<LineSegment> &segments);

    static BSPNode *findLowestCommonAncestor(BSPNode *node1, BSPNode *node2);
    BSPNode *findLeafContainingPoint(const Point &point);

    // Auxiliares
    void print(BSPNode *node = nullptr, int depth = 0);
    bool verifySegment(const LineSegment &segment) const;
};

class BSPTree
{
private:
    BSPNode *root;

public:
    BSPTree() : root(nullptr) {}
    void insert(const LineSegment &segment, bool verbose = false);
    void insert(const std::vector<LineSegment> &segments);

    CollisionResult checkCollision(const Point &initialPoint, const Point &finalPoint, const Point &Player) const;

    void print();
    void test();
    // void testCollisionDetection();
};

#endif // BSP_H