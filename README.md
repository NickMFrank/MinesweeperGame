# Minesweeper by Nick Frank
## [Description]

I faithfully recreated minesweeper from scratch, finishing the project in late 2022. Left clicking on the GUI reveals a location, either showing the mine or a number representing the count of near bombs. Right clicking can flag unrevealed tiles to prevent the user from accidental left clicks. Flags can be removed by right clicking on the same tile.

## [How to Compile and Run]

After cloning the repository and cd'ing into the Assignment12_Project/part1, run the following command to compile the code:  
   
g++ -std=c++17 -g ./src/*.cpp -I./include -o prog -lsfml-graphics -lsfml-window -lsfml-system  

This should generate an executable called "./prog", which can be utilized to play games. This executable expects three parameters representing the width, height, and mine count of the generated board. For example, a game with a width of 6, height of 8, and 10 mines would be generated by using the command:  
  
./prog 6 8 10

## [Screenshots]

[]

## [Resources]

I primarily needed to use SFML and cppreference documentation. I did have to consult a [forum](https://en.sfml-dev.org/forums/index.php?topic=15275.0) when implementing flags, as they originally toggled on and off each frame (problematically).

## [Hindsight]

Could have used more separate classes though not fully necessary for such a small project. Randomized mine selection could be improved as it leaves the possibility for games that rely on luck.