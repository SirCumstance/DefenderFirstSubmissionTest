#include "game.h"
#include <iostream>
Game::Game()
    : score(0), highScore(0), gameStarted(false),
      quitConfirmation(false), isSplashScreenVisible(true),
      isPauseScreenVisible(false), isWinScreenVisible(false), isGameOver(false), level(1), previousLevelScore(0) //, isGamePaused(false)
{
    // Load background texture
    backgroundTexture.loadFromFile("resources/background.jpg");
    // Set the background sprite
    backgroundSprite.setTexture(backgroundTexture);
    if (!font.loadFromFile("resources/sansation.ttf"))
    {
        std::cout << "Loading font error" << std::endl;
    }
    // Initialize landers
    landerTexture.loadFromFile("resources/landerShip.png");

    playerTexture.loadFromFile("resources/playerShip.png");

    player.setTexture(playerTexture);
    player.setScale(sf::Vector2f(0.1, 0.1));
    player.setPosition(WINDOW_WIDTH / 2 - 25, WINDOW_HEIGHT - 60);
    srand(static_cast<unsigned>(time(0))); // Seed random number generator
}

void Game::run(sf::RenderWindow &window)
{
    handleInput(window);
    update();
    render(window);
}

void Game::resetGame()
{
    // Reset player position
    player.setPosition(WINDOW_WIDTH / 2 - 25, WINDOW_HEIGHT - 60);

    // Clear the list of landers and missiles
    landers.clear();
    missiles.clear();
    lasers.clear();
    // Reset any other game-related variables if needed
}

void Game::handleInput(sf::RenderWindow &window)
{
    sf::Event event;
    sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    sf::Vector2f playerCenter = player.getPosition() + sf::Vector2f(player.getGlobalBounds().width / 2, player.getGlobalBounds().height / 2);
    sf::Vector2f rightScale(0.1f, 0.1f); // Set scale factors for X and Y uniformly
    sf::Vector2f leftScale(-0.1f, 0.1f); // Swap direction

    if (mousePosition.x > playerCenter.x)
    {
        player.setScale(rightScale); // Fire laser to the right
    }
    else
    {
        player.setScale(leftScale); // Flip horizontally by setting X scale to negative
    }

    if (!gameStarted && !isPauseScreenVisible && !isGameOver && !isWinScreenVisible)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
        {
            gameStarted = true;
            isSplashScreenVisible = false;
            score = 0; // Reset the score when the game starts
        }
    }

    else if (gameStarted && !isPauseScreenVisible && !isGameOver && !isWinScreenVisible)
    {
        int oldScore = score; // Check if the player has won, ending the game, unless another level is chosen.

        if (score >= previousLevelScore + 100)
        {
            isWinScreenVisible = true;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
        {
            isPauseScreenVisible = true;
        }
    }

 else if (isPauseScreenVisible)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Y))
        {
            isPauseScreenVisible = false;
            gameStarted = false;
            if (score > highScore)
            {
                highScore = score; // Save the highscore
            }
            score = 0;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::N))
        {
            isPauseScreenVisible = false;
        }
    }
    // Handle input for game over screen
    else if (isWinScreenVisible)
    {
        gameStarted = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Y)) // Continue the next level
        {
            isWinScreenVisible = false;
            gameStarted = true;
            level++;
            previousLevelScore = score;
            gameStarted = true;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::N)) // Restart a new game

        {
            isWinScreenVisible = false;
            isSplashScreenVisible = true;
            resetGame();
            score = 0;
            previousLevelScore = 0;
            level = 1;
        }
    }
    else if (isGameOver)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Y))
        {
            isGameOver = false;
            gameStarted = true;
            isSplashScreenVisible = false;
            if (score > highScore)
            {
                highScore = score;
            }
            score = 0;
            previousLevelScore = 0;
            level = 1;
            resetGame();
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::N))
        {
            gameStarted = false;
            isSplashScreenVisible = true;
            resetGame();
            window.close();
        }
    }

    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }

        if (event.type == sf::Event::MouseButtonPressed)
        {

            if (event.mouseButton.button == sf::Mouse::Left)
            {

                Laser laser;
                laser.shape.setSize(sf::Vector2f(5, 20));
                laser.shape.setFillColor(sf::Color::Cyan);
                laser.shape.setPosition(player.getPosition().x + 22, player.getPosition().y);
                laser.velocity.x = (mousePosition.x > playerCenter.x) ? LASER_SPEED : -LASER_SPEED;

                laser.setFired(true);
                lasers.push_back(laser);
            }
        }
    }
