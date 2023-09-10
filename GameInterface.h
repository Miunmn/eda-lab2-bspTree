#ifndef GAME_INTERFACE_H
#define GAME_INTERFACE_H

#include <SFML/Graphics.hpp>
#include <sstream>
#include <random>
#include "params.h"
#include "Maze.h"
#include "Ball.h"
#include "BSPTree.h"

class GameInterface {
public:
    GameInterface();

    // Funciones principales
    void run();
    void processEvents();
    void update();
    void render();

    // Funciones para manejar botones y tiempo de juego
    void handleStartPauseButton();
    void updateTimeDisplay();

private:
    // Ventana y elementos gráficos
    sf::RenderWindow window;
    sf::RectangleShape gameBoard;
    sf::RectangleShape startButton;
    sf::RectangleShape pauseButton;
    sf::Text startButtonText;
    sf::Text pauseButtonText;
    sf::Text timeDisplay;
    sf::Font font;

    sf::VertexArray walls;
    Maze maze;

    Player player;
    std::vector<Enemy> enemies;
    sf::CircleShape playerBall;
    std::vector<sf::CircleShape> enemyBalls;

    BSPTree collisionTree;

    // Variables de estado
    bool isPaused;
    sf::Clock gameClock;
};

#endif // GAME_INTERFACE_H