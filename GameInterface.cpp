#include "GameInterface.h"

GameInterface::GameInterface()
: window(sf::VideoMode(800, 600), "Game Interface")
, gameBoard(sf::Vector2f(500, 500))
, startButton(sf::Vector2f(70, 30)) 
, pauseButton(sf::Vector2f(70, 30)) 
, isPaused(true)
{
    // Inicializar elementos gráficos
    gameBoard.setFillColor(sf::Color::Black);
    gameBoard.setPosition(50, 50);

    // Cargar la fuente
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
    }

    // Inicializar texto de los botones
    startButton.setFillColor(sf::Color(128, 128, 128)); 
    startButton.setPosition(600, 500);
    startButtonText.setFont(font);
    startButtonText.setString("Start");
    startButtonText.setCharacterSize(12);
    startButtonText.setFillColor(sf::Color::Black); 
    startButtonText.setPosition(610, 505);  

    pauseButton.setFillColor(sf::Color(128, 128, 128));
    pauseButton.setPosition(720, 500);
    pauseButtonText.setFont(font);
    pauseButtonText.setString("Pause");
    pauseButtonText.setCharacterSize(12);
    pauseButtonText.setFillColor(sf::Color::Black); 
    pauseButtonText.setPosition(730, 505);  


    // Cargar la fuente
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf")) {
        std::cerr << "Failed to load font from 'content/Ubuntu-L.ttf'" << std::endl;
    }
    timeDisplay.setFont(font);
    timeDisplay.setCharacterSize(14);
    timeDisplay.setFillColor(sf::Color::White);
    timeDisplay.setPosition(700, 50);

    // Maze
    walls.setPrimitiveType(sf::Quads);
    walls.resize(8);

    const auto& segments = maze.getWalls();
    float lineThickness = 5.0f; // Grosor de la línea en píxeles

    // Asegúrate de que el sf::VertexArray tenga suficientes vértices para todos los cuadriláteros
    walls.resize(segments.size() * 4);

    for (const auto& wall : segments) {
        sf::Vector2f p1(wall.getP1().getX().getValue(), wall.getP1().getY().getValue());
        sf::Vector2f p2(wall.getP2().getX().getValue(), wall.getP2().getY().getValue());

        sf::Vector2f direction = p2 - p1;
        sf::Vector2f unitDirection = direction / std::sqrt(direction.x * direction.x + direction.y * direction.y);
        sf::Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x);

        sf::Vector2f offset = (lineThickness / 2.f) * unitPerpendicular;

        walls.append(sf::Vertex(p1 - offset, sf::Color::Green));
        walls.append(sf::Vertex(p2 - offset, sf::Color::Green));
        walls.append(sf::Vertex(p2 + offset, sf::Color::Green));
        walls.append(sf::Vertex(p1 + offset, sf::Color::Green));
    }

    // Insertar todos los segmentos al BSP-tree
    collisionTree.insert_many(segments);

    // Player
    Point centerOfGameBoard(300, 300); 
    Vector initialPlayerVelocity(0, 0);
    player = Player(centerOfGameBoard, initialPlayerVelocity);

    playerBall.setRadius(GameParams::CIRCLE_RADIUS);
    playerBall.setFillColor(sf::Color::Red);
    Point playerPos = player.getPosition();
    playerBall.setPosition(playerPos.getX().getValue(), playerPos.getY().getValue());

    // Enemies
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(50, 450); // Ajusta según el tamaño de tu gameBoard

    for (int i = 0; i < 10; ++i) {
        Point randomPoint(dis(gen), dis(gen));
        Vector randomVelocity(2*dis(gen) / 450.0 - 1, 2*dis(gen) / 450.0 - 1);
        randomVelocity.normalize();
        randomVelocity = randomVelocity * GameParams::ENEMY_SPEED;
        enemies.push_back(Enemy(randomPoint, randomVelocity));
    }

    // Inicializar enemyBalls
    enemyBalls.resize(enemies.size(), sf::CircleShape(GameParams::CIRCLE_RADIUS));
    for (auto& enemyBall : enemyBalls) {
        enemyBall.setFillColor(sf::Color::White);
    }
}

void GameInterface::run() {
    sf::Clock frameClock;
    while (window.isOpen()) {
        sf::Time start = frameClock.getElapsedTime();
        
        processEvents();
        update();
        render();
        
        sf::Time elapsed = frameClock.getElapsedTime() - start;
        sf::Time frameTime = sf::milliseconds(GameParams::FRAME_TIME_MS);
        if (elapsed < frameTime) {
            sf::sleep(frameTime - elapsed);
        }
    }
}

void GameInterface::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        
        // Captura de clics en botones
        if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
            if (startButton.getGlobalBounds().contains(mousePosF)) {
                handleStartPauseButton();
            }
        }
    }

    // Cambio de color al pasar el mouse por encima de los botones
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

    if (startButton.getGlobalBounds().contains(mousePosF)) {
        startButton.setFillColor(sf::Color(192, 192, 192));  // Gris claro
    } else {
        startButton.setFillColor(sf::Color(128, 128, 128));  // Gris original
    }

    if (pauseButton.getGlobalBounds().contains(mousePosF)) {
        pauseButton.setFillColor(sf::Color(192, 192, 192));  // Gris claro
    } else {
        pauseButton.setFillColor(sf::Color(128, 128, 128));  // Gris original
    }

    // Aquí se añade el control del teclado para la pelotita del usuario
    if (!isPaused) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            player.setVelocity(Vector(0, -GameParams::PLAYER_SPEED));
            player.updatePosition();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            player.setVelocity(Vector(0, GameParams::PLAYER_SPEED));
            player.updatePosition();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            player.setVelocity(Vector(-GameParams::PLAYER_SPEED, 0));
            player.updatePosition();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            player.setVelocity(Vector(GameParams::PLAYER_SPEED, 0));
            player.updatePosition();
        }
    }
}

void GameInterface::update() {
    if (!isPaused) {
        // Actualizar la posición del jugador en la pantalla
        Point playerPos = player.getPosition();
        playerBall.setPosition(playerPos.getX().getValue(), playerPos.getY().getValue());

        // Actualizar la posición de los enemigos
        for (auto& enemy : enemies) {
            Point oldEnemyPos = enemy.getPosition();
            Point newEnemyPos = oldEnemyPos + enemy.getVelocity();

            CollisionResult result = collisionTree.checkCollision(oldEnemyPos, newEnemyPos,player.getPosition());

            if (result.collisionWithPlayer){
                std::cout << "Perdiste" << std::endl;
                isPaused = true;
                startButtonText.setString("Start");
                gameClock.restart();
            }

            if (result.collisionDetected) {
                enemy.handleWallCollision(); 
                enemy.updatePositionBasedOnWallCollision(result.normal);
                enemy.updatePosition();
            } else {
                enemy.setPosition(newEnemyPos);
            }
        }

        updateTimeDisplay();
    }
}

void GameInterface::render() {
    window.clear(sf::Color::White);  
    window.draw(gameBoard); 
    window.draw(startButton);  
    window.draw(startButtonText);  
    window.draw(pauseButton);  
    window.draw(pauseButtonText);  
    window.draw(timeDisplay);
    window.draw(walls);
    
    // Dibujo la pelotita del jugador
    Point playerPos = player.getPosition();
    playerBall.setPosition(playerPos.getX().getValue(), playerPos.getY().getValue());
    window.draw(playerBall);

    // Dibujo las pelotitas de los enemigos
    for (size_t i = 0; i < enemies.size(); ++i) {
        Point enemyPos = enemies[i].getPosition();
        enemyBalls[i].setPosition(enemyPos.getX().getValue(), enemyPos.getY().getValue());
        window.draw(enemyBalls[i]);
    }
    
    window.display();
}

void GameInterface::handleStartPauseButton() {
    // Cambiar estado del juego y actualizar etiqueta del botón
    isPaused = !isPaused;
    if (isPaused) {
        startButtonText.setString("Resume");
        gameClock.restart();
    } else {
        startButtonText.setString("Pause");
    }
}

void GameInterface::updateTimeDisplay() {
    if (!isPaused) {
        sf::Time elapsed = gameClock.getElapsedTime();
        std::stringstream ss;
        ss << "Tiempo: " << elapsed.asSeconds();
        timeDisplay.setString(ss.str());
    }
}