// Compile with: g++ -std=c++17 -g ./src/*.cpp -I./include -o prog -lsfml-graphics -lsfml-window -lsfml-system
// Run with: ./prog

// C++ Standard Libraries
#include <iostream>
#include <math.h>

// Third Party Libraries
#include <SFML/Graphics.hpp>

// Our Libraries
#include "Board.hpp"

bool mouseInBounds(const sf::RenderWindow& window){
    sf::Vector2i mouse = sf::Mouse::getPosition(window);
    if(mouse.x > -1 && mouse.x < window.getSize().x-1){
        if(mouse.y > -1 && mouse.y < window.getSize().y-1){
            return true;
        }
    }
    return false;
}

// Entry point to the program
int main(int argc, char* argv[]){

    const char *invalidParamsMsg = "Error running program:\ne.g. \"./prog 8 8 10\" \nFirst argument is" 
        "desired width (1-30)\nSecond argument is desired height (1-30)\nThird argument is desired mine count";

    if(argc < 3) {
        std::cout << invalidParamsMsg << std::endl;
        exit(1);
    }

    int width = std::stoi(argv[1]);
    int height = std::stoi(argv[2]);
    int mines = std::stoi(argv[3]);

    if (width <= 0 || height <= 0 || mines < 0 || width > 30 || height > 30 || width * height < mines) {
        std::cout << invalidParamsMsg << std::endl;
        exit(1);
    }

    // Set dimensions of the window
    const int windowWidth = 25 * width;
    const int windowHeight = 25 * height;
    // Create a board
    Board b(width,height,mines);
    // Create the window
    sf::RenderWindow window(sf::VideoMode(windowWidth,windowHeight), "Minesweeper");
    // Limit to 30 FPS
    window.setFramerateLimit(30);
    window.setKeyRepeatEnabled(false);
    // Main loop of the program
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed){
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if(event.mouseButton.button == sf::Mouse::Left && !b.GetGameOver()){
                    if(mouseInBounds(window)){
                        sf::Vector2i mouse = sf::Mouse::getPosition(window);
                        int x = mouse.x/25;
                        int y = mouse.y/25;
                        // Left click on non-flagged, un-revealed tiles to reveal them; may end game
                        if (!b.GetRevealed(x, y) && !b.IsFlagged(x, y)) {
                            if (b.GetMinePresent(x, y) || b.GetCurrentScore() == b.GetMaxScore()) {
                                std::cout << "Game over! Score: " << b.GetCurrentScore() << "/" << b.GetMaxScore() << std::endl;
                            }
                        }
                    }
                } else if (event.mouseButton.button == sf::Mouse::Right && !b.GetGameOver()) {
                    if(mouseInBounds(window)){
                        sf::Vector2i mouse = sf::Mouse::getPosition(window);
                        int x = mouse.x/25;
                        int y = mouse.y/25;
                        // Right click on un-revealed tiles to toggle their flag
                        if (!b.GetRevealed(x, y)) {
                            if (b.IsFlagged(x, y)) {
                                b.Unflag(x, y);
                            } else {
                                b.Flag(x, y);
                            }
                        }
                    }
                }
            }
        }

	    // Clear the window at the start of every frame
        window.clear();

        // Do the board rendering
        b.Render(window);

	    // Display to the window
        window.display();
    }

    return 0;
}