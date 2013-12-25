/**
 * Minesweeper
 *
 * Copyright 2013, Robert Berry
 */

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

// Here is a small helper for you ! Have a look.
#include "ResourcePath.hpp"
#include <string.h>
#include <GameBoard.h>
#include <memory.h>
#include "CellTextures.h"

const int CELL_HEIGHT = 16;
const int CELL_WIDTH = 16;

const int ROWS = 20;
const int COLUMNS = 30;
const int MINES = 20;

const int BOARD_X_OFFSET = 8;
const int BOARD_Y_OFFSET = 40;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

int main(int, char const**)
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Minesweeper");

    // Set the Icon
    sf::Image icon;
    if (!icon.loadFromFile(resourcePath() + "icon.png")) {
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    
    GameBoard gameBoard(COLUMNS, ROWS);
    gameBoard.populateMines(MINES);
    
    CellTextures cellTextures;
    
    sf::RectangleShape backFill;
    backFill.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    backFill.setFillColor(sf::Color(164, 164, 164));

    sf::RectangleShape gridBorder;
    gridBorder.setPosition(sf::Vector2f(BOARD_X_OFFSET, BOARD_Y_OFFSET));
    gridBorder.setSize(sf::Vector2f(COLUMNS * CELL_WIDTH + 1, ROWS * CELL_HEIGHT + 1));
    gridBorder.setFillColor(sf::Color(86, 86, 86));
    
    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::MouseButtonPressed) {
                int gridX = (event.mouseButton.x - BOARD_X_OFFSET) / CELL_WIDTH;
                int gridY = (event.mouseButton.y - BOARD_Y_OFFSET) / CELL_HEIGHT;
                
                if (gameBoard.validCoordinate(gridX, gridY)) {
                    if (event.mouseButton.button == sf::Mouse::Button::Left) {
                        gameBoard.onClick(gridX, gridY);
                    }
                    if (event.mouseButton.button == sf::Mouse::Button::Right) {
                        gameBoard.onRightClick(gridX, gridY);
                    }
                }
            }
            
            // Close window : exit
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Escape pressed : exit
            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
        }

        // Clear screen
        window.clear();
        
        window.draw(backFill);
        window.draw(gridBorder);

        // Draw the grid
        for (int y = 0; y < gameBoard.getRows(); y++) {
            for (int x = 0; x < gameBoard.getColumns(); x++) {
                Cell cell = gameBoard.getCell(x, y);
                sf::Sprite sprite(*cellTextures.getCellTexture(cell, gameBoard.getNeighbouringMines(x, y)));
                sprite.setPosition(BOARD_X_OFFSET + x * CELL_WIDTH, BOARD_Y_OFFSET + y * CELL_HEIGHT);
                window.draw(sprite);
            }
        }
 
        // Update the window
        window.display();
    }

    return EXIT_SUCCESS;
}
