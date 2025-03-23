#ifndef BOARD_HPP
#define BOARD_HPP

// Third Party Libraries
#include <SFML/Graphics.hpp>
#include <vector>

// This is my board class 
// This is responsible for storing where mines are, which tiles are visible, and how many mines neighbor visible tiles
class Board {
private:
    
    bool* mineLocationGrid; // Holds all of the booleans for mine locations
    int* minesNearGrid; // Holds how many mines are near a revealed mine; this is utilized for optimization
    bool* revealedMines; // Holds which coordinates have been revealed by the player
    bool* flaggedMines; // Holds which coordinates have been flagged by the player
    unsigned int width, height, currentScore, maxScore; // Dimensions and scoring values
    bool gameOver, won; // Has game completed and whether it was a win or loss
    sf::Texture textureMine, textureFlag, textureBlank, texture0, texture1, texture2, texture3, 
        texture4, texture5, texture6, texture7, texture8; // Stored here to remove need for repeatedly loading from files

    // Returns count of mines surrounding a location (3x3 area)
    int CountNearMines(int location);

    // Reveals near coordinates recursively to speed up gameplay
    // Essentially spreads to unvisited tiles without nearby mines, reflecting the traditional game
    // Should likely be isolated into more helpers, but that felt unnecessary for how close to completion the project was
    void RevealNear(unsigned int x, unsigned int y, std::vector<unsigned int>& visited);

public:

    // Input parameters are the desired board dimensions followed by the number of mines to generate
    Board(unsigned int width, unsigned int height, unsigned int mines);

    // Default destructor
    ~Board();

    // No copy constructor
    Board(const Board& Board) = delete;

    // Gets count of mines near the provided coordinate
    int GetNear(unsigned int x, unsigned int y);

    // Gets if mine present at the provided coordinate and sets that tile to be visible
    bool GetMinePresent(unsigned int x, unsigned int y);

    // Gets if mine should be visible to player
    bool GetRevealed(unsigned int x, unsigned int y);

    // Flags coordinate
    void Flag(unsigned int x, unsigned int y);

    // Unflags coordinate
    void Unflag(unsigned int x, unsigned int y);

    // Gets if coordinate has a flag
    bool IsFlagged(unsigned int x, unsigned int y);

    // Render the board
    void Render(sf::RenderWindow& window);

    // Gets if game is over
    bool GetGameOver();

    // Gets if game was won or lost
    bool GetWon();

    // Gets current score
    int GetCurrentScore();

    // Gets maximum score
    int GetMaxScore();
};

#endif