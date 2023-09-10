#include "Maze.h"

Maze::Maze() {
    createSimpleSquare();
}

const std::vector<LineSegment>& Maze::getWalls() const {
    return walls;
}

void Maze::createSimpleSquare() {
    walls.push_back(LineSegment(Point(50, 50), Point(550, 50)));
    walls.push_back(LineSegment(Point(550, 50), Point(550, 550)));
    walls.push_back(LineSegment(Point(550, 550), Point(50, 550)));
    walls.push_back(LineSegment(Point(50, 550), Point(50, 50)));
    //walls.push_back(LineSegment(Point(50, 0), Point(550, 0)));
}
