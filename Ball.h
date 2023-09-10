#ifndef BALL_H
#define BALL_H

#include "Point.h"
#include "Line.h"

class Ball {
protected:
    Point position;
    Vector velocity;
public:
    Ball(Point initial_position, Vector initial_velocity)
        : position(initial_position), velocity(initial_velocity) {}
    virtual ~Ball() = default;

    Point getPosition() const { return position; }
    Vector getVelocity() const { return velocity; }

    void setPosition(const Point& new_position) { position = new_position; }
    void setVelocity(const Vector& new_velocity) { velocity = new_velocity; }

    virtual void updatePosition() {
        position = position + velocity;
    }
};

class Player : public Ball {
public:
    Player() : Ball(Point(0, 0), Vector(0, 0)) {}
    Player(Point initial_position, Vector initial_velocity)
        : Ball(initial_position, initial_velocity) {}

    // Sobrescribimos para mantener la velocidad constante
    void updatePosition() override {
        position = position + velocity;
    }
};

class Enemy : public Ball
{
public:
    Enemy() : Ball(Point(0, 0), Vector(0, 0)) {}
    Enemy(Point initial_position, Vector initial_velocity)
        : Ball(initial_position, initial_velocity) {}

    // Sobrescribimos para actualizar la velocidad en caso de colisión con la pared
    void handleWallCollision() {
        velocity = velocity * 1.5;
    }

    // Sobrescribimos para considerar el rebote contra la pared
    void updatePositionBasedOnWallCollision(Vector wallNormal) {
        velocity = velocity - (wallNormal * 2.0 * velocity.dotProduct(wallNormal));
        // position = position + velocity;
    }

    // Sobrescribimos para mover el Enemy de acuerdo a su lógica
    void updatePosition() override {
        // Aquí implementas cómo se mueve el enemigo
        position = position + velocity;
    }
};

#endif // BALL_H