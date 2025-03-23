// Board.cpp
#include "Board.hpp"

Board::Board(unsigned int width, unsigned int height, unsigned int mines) {
    this->width = width;
    this->height = height;
    this->currentScore = 0;
    this->maxScore = width * height - mines;
    this->mineLocationGrid = new bool[width*height];
    this->minesNearGrid = new int[width*height];
    this->revealedMines = new bool[width*height];
    this->flaggedMines = new bool[width*height];
    this->gameOver = false;
    this->won = false;
    this->textureMine.loadFromFile("media/mine.png");
    this->textureFlag.loadFromFile("media/flag.png");
    this->textureBlank.loadFromFile("media/blank.png");
    this->texture0.loadFromFile("media/tile0.png");
    this->texture1.loadFromFile("media/tile1.png");
    this->texture2.loadFromFile("media/tile2.png");
    this->texture3.loadFromFile("media/tile3.png");
    this->texture4.loadFromFile("media/tile4.png");
    this->texture5.loadFromFile("media/tile5.png");
    this->texture6.loadFromFile("media/tile6.png");
    this->texture7.loadFromFile("media/tile7.png");
    this->texture8.loadFromFile("media/tile8.png");

    // Assign all tiles to have no mines
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            mineLocationGrid[j * width + i] = false;
        }
    }

    // Randomly assign all mines to unique positions
    std::srand(time(NULL));
    for (int i = 0; i < mines; i++) {
        int random = std::rand() % (width*height - 1) + 0;
        if (!mineLocationGrid[random]) {
            mineLocationGrid[random] = true;
        }
        else {
            i--;
        }
    }

    // Generate counts for near mines
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            minesNearGrid[j * width + i] = CountNearMines(j * width + i);
        }
    }

    // Assign all tiles to be invisible to the player
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            revealedMines[j * width + i] = false; // CHANGE THIS ONCE DONE TESTING
        }
    }

    // Assign all tiles to be unflagged for the player
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            flaggedMines[j * width + i] = false;
        }
    }
}

int Board::CountNearMines(int location) {
    int count = 0;
    bool hasLeft = location % width != 0;
    bool hasRight = location % width != width-1;

    // Middle row
    if (mineLocationGrid[location])                             {count += 1;}
    if (hasLeft && mineLocationGrid[location - 1])              {count += 1;}
    if (hasRight && mineLocationGrid[location + 1])             {count += 1;}

    // Top row
    if (location > width) {
        if (mineLocationGrid[location - width])                 {count += 1;}
        if (hasLeft && mineLocationGrid[location - width - 1])  {count += 1;}
        if (hasRight && mineLocationGrid[location - width + 1]) {count += 1;}
    }

    // Bottom row
    if (location < width * (height - 1)) {
        if (mineLocationGrid[location + width])                 {count += 1;}
        if (hasLeft && mineLocationGrid[location + width - 1])  {count += 1;}
        if (hasRight && mineLocationGrid[location + width + 1]) {count += 1;}
    }
    return count;
}

int Board::GetNear(unsigned int x, unsigned int y) {
    return minesNearGrid[x + y * width];
}

bool Board::GetRevealed(unsigned int x, unsigned int y) {
    return revealedMines[x + y * width];
}

void Board::Flag(unsigned int x, unsigned int y) {
    flaggedMines[x + y * width] = true;
}

void Board::Unflag(unsigned int x, unsigned int y) {
    flaggedMines[x + y * width] = false;
}

bool Board::IsFlagged(unsigned int x, unsigned int y) {
    return flaggedMines[x + y * width];
}

bool Board::GetMinePresent(unsigned int x, unsigned int y) {
    // Essentially registers a guess
    revealedMines[x + y * width] = true;
    if (mineLocationGrid[x + y * width]) {
        gameOver = true;
        won = false;
        return true;
    } else {
        std::vector<unsigned int> visited;
        currentScore++;
        RevealNear(x, y, visited);
        if (currentScore == maxScore) {
            won = true;
            gameOver = true;
        }
        return false;
    }
}

int Board::GetCurrentScore() {
    return currentScore;
}

int Board::GetMaxScore() {
    return maxScore;
}

void Board::RevealNear(unsigned int x, unsigned int y, std::vector<unsigned int>& visited) {
    visited.push_back(x + y * width); // Add to visited list so we don't infinitely recurse
    if (minesNearGrid[x + y * width] == 0) {
        bool hasLeft = x > 0;
        bool hasRight = x < (width-1);
        bool hasUp = y > 0;
        bool hasDown = y < (height-1);
        // Middle row
        if (hasLeft) {
            if (!std::count(visited.begin(), visited.end(), (x - 1) + y * width)) {
                RevealNear(x-1, y, visited);
            }
            if (!revealedMines[x - 1 + y * width]) {
                revealedMines[x - 1 + y * width] = true;
                currentScore++;
            }
        }
        if (hasRight) {
            if (!std::count(visited.begin(), visited.end(), (x + 1) + y * width)) {
                RevealNear(x+1, y, visited);
            }
            if (!revealedMines[x + 1 + y * width]) {
                revealedMines[x + 1 + y * width] = true;
                currentScore++;
            }
        }
        // Top row
        if (hasUp) {
            if (!std::count(visited.begin(), visited.end(), x + (y - 1) * width)) {
                RevealNear(x, y-1, visited);
            }
            if (!revealedMines[x + (y - 1) * width]) {
                revealedMines[x + (y - 1) * width] = true;
                currentScore++;
            }
            if (hasLeft) {
                if (!std::count(visited.begin(), visited.end(), (x - 1) + (y - 1) * width)) {
                    RevealNear(x-1, y-1, visited);
                }
                if (!revealedMines[(x - 1) + (y - 1) * width]) {
                    revealedMines[(x - 1) + (y - 1) * width] = true;
                    currentScore++;
                }
            }
            if (hasRight) {
                if (!std::count(visited.begin(), visited.end(), (x + 1) + (y - 1) * width)) {
                    RevealNear(x+1, y-1, visited);
                }
                if (!revealedMines[(x + 1) + (y - 1) * width]) {
                    revealedMines[(x + 1) + (y - 1) * width] = true;
                    currentScore++;
                }
            }
        }
        // Bottom row
        if (hasDown) {
            if (!std::count(visited.begin(), visited.end(), x + (y + 1) * width)) {
                RevealNear(x, y+1, visited);
            }
            if (!revealedMines[x + (y + 1) * width]) {
                    revealedMines[x + (y + 1) * width] = true;
                    currentScore++;
                }
            if (hasLeft) {
                if (!std::count(visited.begin(), visited.end(), (x - 1) + (y + 1) * width)) {
                    RevealNear(x-1, y+1, visited);
                }
                if (!revealedMines[(x - 1) + (y + 1) * width]) {
                    revealedMines[(x - 1) + (y + 1) * width] = true;
                    currentScore++;
                }
            }
            if (hasRight) {
                if (!std::count(visited.begin(), visited.end(), (x + 1) + (y + 1) * width)) {
                    RevealNear(x+1, y+1, visited);
                }
                if (!revealedMines[(x + 1) + (y + 1) * width]) {
                    revealedMines[(x + 1) + (y + 1) * width] = true;
                    currentScore++;
                }
            }
        }
    }
}

void Board::Render(sf::RenderWindow& window) {
    sf::Sprite sprite;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            if (revealedMines[i + j * width]) {
                if (mineLocationGrid[i + j * width]) {
                    // Show mine
                    sprite.setTexture(textureMine);
                } else {
                    // Show based on near mines count (originally done cleaner by delegating to a separate method,
                    // but abandoned due to performance limitations of calling loadFromFile repeatedly)
                    switch (minesNearGrid[i + j * width]) {
                        case(0):
                            sprite.setTexture(texture0); break;
                        case(1):
                            sprite.setTexture(texture1); break;
                        case(2):
                            sprite.setTexture(texture2); break;
                        case(3):
                            sprite.setTexture(texture3); break;
                        case(4):
                            sprite.setTexture(texture4); break;
                        case(5):
                            sprite.setTexture(texture5); break;
                        case(6):
                            sprite.setTexture(texture6); break;
                        case(7):
                            sprite.setTexture(texture7); break;
                        case(8):
                            sprite.setTexture(texture8); break;
                    }
                }
            } else {
                if (flaggedMines[i + j * width]) {
                    // Show flag
                    sprite.setTexture(textureFlag);
                }
                else {
                    // Show blank
                    sprite.setTexture(textureBlank);
                }
            }
            sprite.setPosition(25 * i, 25 * j);
            window.draw(sprite);
        }
    }
}

bool Board::GetGameOver() {
    return gameOver;
}

bool Board::GetWon() {
    return won;
}

Board::~Board(){
    delete[] mineLocationGrid;
    delete[] minesNearGrid;
    delete[] revealedMines;
    delete[] flaggedMines;
}