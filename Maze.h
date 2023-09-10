#ifndef MAZE_H
#define MAZE_H


#include "Point.h"
#include "Line.h"
#include <vector>

class Maze
{
private:
    std::vector<LineSegment> walls;
public:
    Maze();
    ~Maze() = default;
    const std::vector<LineSegment>& getWalls() const;
    void createSimpleSquare();
};

#endif // MAZE_H