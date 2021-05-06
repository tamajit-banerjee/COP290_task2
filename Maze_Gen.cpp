// *************************************
// ** M A Z E   C O O R D   C L A S S **
// *************************************

#include <iostream>

enum COORDTYPE { FLOOR, WALL, BORDER };

class MazeCoord
{
    // private data members
    // [x-coordinate, y-coordinate, maximum value of x, maximum value of y,
    //  "wall or floor" variable, total number of coordinates]
    int x, y, maxX, maxY, coordType, numCoord;


public:

    // Constructors
    MazeCoord() { }
    MazeCoord(int xarg, int yarg)  { x = xarg; y = yarg; }
    MazeCoord(const MazeCoord &coordCopy) { x = coordCopy.x; y = coordCopy.y; }

    // Destructor
    ~MazeCoord() { }

    // Get coordinate and coordinate type functions
    // These function return a specified private data member.
    const int getCoordX() { return x; }
    const int getCoordY() { return y; }
    const int getMaxX() { return maxX; }
    const int getMaxY() { return maxY; }
    const int getCoordType() { return coordType; }
    const int getNumCoord() { return numCoord; }

    // Set functions for maze generation
    // These functions assign a specified value to a data member.
    int setMaxX(int maxXArg) { maxX = maxXArg; return maxX; }
    int setMaxY(int maxYArg) { maxY = maxYArg; return maxY; }
    int setFloor() { coordType = FLOOR; return FLOOR; }
    int setWall() { coordType = WALL; return WALL; }
    int setBorder() { coordType = BORDER; return BORDER; }
    int setNumCoord() { numCoord = (maxX + 1) * (maxY + 1); return numCoord; }


    // Operator overloading functions

    // Relational operators
    friend bool operator==(const MazeCoord coordA, const MazeCoord coordB) {
        return (coordA.x == coordB.x && coordA.y == coordB.y) ? 1 : 0;
    }
    friend bool operator!=(const MazeCoord coordA, const MazeCoord coordB) {
        return (coordA.x != coordB.x || coordA.y != coordB.y) ? 1 : 0;
    }
    friend bool operator>=(const MazeCoord coordA, const MazeCoord coordB) {
        return (coordA.x >= coordB.x || coordA.y >= coordB.y) ? 1 : 0;
    }
    friend bool operator<=(const MazeCoord coordA, const MazeCoord coordB) {
        return (coordA.x <= coordB.x || coordA.y <= coordB.y) ? 1 : 0;
    }
    friend bool operator>(const MazeCoord coordA, const MazeCoord coordB) {
        return (coordA.x > coordB.x || coordA.y > coordB.y) ? 1 : 0;
    }
    friend bool operator<(const MazeCoord coordA, const MazeCoord coordB) {
        return (coordA.x < coordB.x || coordA.y < coordB.y) ? 1 : 0;
    }

    // Increment operator
    MazeCoord& operator++() {
        if (x == maxX && y == maxY) return *this;
        else if (x == maxX && y != maxY) { x = 0; y++; return *this; }
        else { x++; return *this; }
    }

    // Assignment operator
    MazeCoord& operator= (const MazeCoord& coord) {
        x = coord.x;
        y = coord.y;
        maxX = coord.maxX;
        maxY = coord.maxY;
        coordType = coord.coordType;
        numCoord = coord.numCoord;
        return *this;
    }

    // Output operator
    friend std::ostream& operator<<(std::ostream& output, MazeCoord& coord) {
        output << "(" << coord.x << "," << coord.y << ")";
        return output;
    }


    // Directional coordinate functions
    // These functions return a coordinate in the given direction by
    // incrementing or decrementing the x or y variables of the this argument.

    MazeCoord getNorth()
    {
        if (y != maxY) y--;     // decrement would cause underflow if false
        MazeCoord coord(x, y);
        return coord;
    }

    MazeCoord getSouth()
    {
        if (y != 0) y++;        // increment would cause overflow if false
        MazeCoord coord(x, y);
        return coord;
    }

    MazeCoord getEast()
    {
        if (x != maxX) x++;     // increment would cause overflow if false
        MazeCoord coord(x, y);
        return coord;
    }

    MazeCoord getWest()
    {
        if (x != 0) x--;        // decrement would cause underflow if false
        MazeCoord coord(x, y);
        return coord;
    }

    MazeCoord getTwoNorth()
    {
        if (y != maxY && y != maxY - 1) { y -= 2; }
        MazeCoord coord(x, y);
        return coord;
    }

    MazeCoord getTwoSouth()
    {
        if (y != 0 && y != 1 ) { y += 2; }
        MazeCoord coord(x, y);
        return coord;
    }

    MazeCoord getTwoEast()
    {
        if (x != maxX && x != maxX - 1) { x += 2; }
        MazeCoord coord(x, y);
        return coord;
    }

    MazeCoord getTwoWest()
    {
        if (x != 0 && x != 1) { x -= 2; }
        MazeCoord coord(x, y);
        return coord;
    }
};

// *********************************************************
// ** M A Z E   C O O R D   C L A S S   F U N C T I O N S **
// *********************************************************

#include "MazeCoord.h"

#include <stack>
#include <vector>

MazeCoord getNorth(MazeCoord coordArg);
MazeCoord getSouth(MazeCoord coordArg);
MazeCoord getEast(MazeCoord coordArg);
MazeCoord getWest(MazeCoord coordArg);
int findNeighbourWalls(std::vector<MazeCoord> &maze, MazeCoord &coordArg);

int findCoord(std::vector<MazeCoord> &maze, int xarg, int yarg);
int findCoord(std::vector<MazeCoord> &maze, MazeCoord &coord);


void setBorder(std::vector<MazeCoord> &maze, MazeCoord &coord);

struct Direction
{
    bool isNorth;
    bool isSouth;
    bool isEast;
    bool isWest;

    Direction(bool north, bool south, bool east, bool west) {
        isNorth = north; isSouth = south; isEast = east; isWest = west;


    }
};

    Direction findDirection(std::vector<MazeCoord> &maze, MazeCoord &coordArg,
                            Direction &direct);

void genMaze(std::vector<MazeCoord> &maze, MazeCoord &coord2)
{
    srand(time(NULL));

    std::stack<MazeCoord> coordinates;
    MazeCoord coord(1, maze[0].getMaxY());
    Direction direct(false, false, false, false);

    int index = findCoord(maze, coord);
    maze[index].setFloor();
    coordinates.push(maze[index]);

    coord = coord.getNorth();
    index = findCoord(maze, coord);
    maze[index].setFloor();

    int visitedCoord = 1;
    int initNumCoord = maze[0].getMaxY() * 2 + (maze[0].getMaxX() - 2) * 2;

    while (visitedCoord <= initNumCoord) {
        findDirection(maze, maze[index], direct);
        int chosenDirection = 0;
        chosenDirection = (rand() % 4) + 1;

        bool isSuccess = false;
        do {
            if (chosenDirection == 1 && direct.isNorth == true) {
                coord = coord.getNorth();
                index = findCoord(maze, coord);
                maze[index].setFloor();
                coord = coord.getNorth();
                index = findCoord(maze, coord);
                maze[index].setFloor();
                visitedCoord += 2;
                isSuccess = true;
            }
            else if (chosenDirection == 2 && direct.isSouth == true) {
                coord = coord.getSouth();
                index = findCoord(maze, coord);
                maze[index].setFloor();
                coord = coord.getSouth();
                index = findCoord(maze, coord);
                maze[index].setFloor();
                visitedCoord += 2;
                isSuccess = true;
            }
            else if (chosenDirection == 3 && direct.isEast == true) {
                coord = coord.getEast();
                index = findCoord(maze, coord);
                maze[index].setFloor();
                coord = coord.getEast();
                index = findCoord(maze, coord);
                maze[index].setFloor();
                visitedCoord += 2;
                isSuccess = true;
            }
            else if (chosenDirection == 4 && direct.isWest == true) {
                coord = coord.getWest();
                index = findCoord(maze, coord);
                maze[index].setFloor();
                coord = coord.getWest();
                index = findCoord(maze, coord);
                maze[index].setFloor();
                visitedCoord += 2;
                isSuccess = true;
            }
            else {
                coordinates.pop();
                maze[index] = coordinates.top();
            }
        } while (isSuccess == false);
    }

    return;
}

int findNeighbourWalls(std::vector<MazeCoord> &maze, MazeCoord &coordArg)
{
    int numWalls = 0;

    int indexA = findCoord(maze, coordArg);
    int indexB = 0;

    MazeCoord coord;

    coord = maze[indexA].getNorth();
    indexB = findCoord(maze, coord);
    if (maze[indexB].getCoordType() == WALL) numWalls++;
    coord = maze[indexA].getSouth();
    indexB = findCoord(maze, coord);
    if (maze[indexB].getCoordType() == WALL) numWalls++;
    coord = maze[indexA].getEast();
    indexB = findCoord(maze, coord);
    if (maze[indexB].getCoordType() == WALL) numWalls++;
    coord = maze[indexA].getWest();
    indexB = findCoord(maze, coord);
    if (maze[indexB].getCoordType() == WALL) numWalls++;

    return numWalls;
}

Direction findDirection(std::vector<MazeCoord> &maze,
                         MazeCoord &coordArg, Direction &direct)
{
    if ((coordArg.getTwoNorth()).getCoordType() == WALL) direct.isNorth = true;
    if ((coordArg.getTwoSouth()).getCoordType() == WALL) direct.isSouth = true;
    if ((coordArg.getTwoEast()).getCoordType() == WALL) direct.isEast = true;
    if ((coordArg.getTwoWest()).getCoordType() == WALL) direct.isWest = true;

    return direct;
}

void display(std::vector<MazeCoord> &maze, MazeCoord &coord)
{
    for (unsigned int i = 0; i < maze.size(); i++) {
        if (maze[i].getCoordType() == FLOOR) std::cout << " ";
        else if (maze[i].getCoordType() == WALL) std::cout << "@";
        else std::cout << "#";
        if (maze[i].getCoordX() == coord.getMaxX()) std::cout << std::endl;
    }
}

void setAllWall(std::vector<MazeCoord> &maze, MazeCoord &coord)
{
    for (unsigned int i = 0; i < maze.size(); i++) {
        maze[i] = coord;
        maze[i].setWall();
        ++coord;
    }
}

void setBorder(std::vector<MazeCoord> &maze, MazeCoord &coord)
{
    for (unsigned int i = 0; i < maze.size(); i++) {
        if (maze[i].getCoordX() == 0 ||
            maze[i].getCoordX() == maze[i].getMaxX() ||
            maze[i].getCoordY() == 0 ||
            maze[i].getCoordY() == maze[i].getMaxY())
            maze[i].setBorder();
    }
}

int findCoord(std::vector<MazeCoord> &maze, int xarg, int yarg)
{
    int index;

    for (unsigned int i = 0; i < maze.size(); i++) {
        if (maze[i].getCoordX() == xarg && maze[i].getCoordY() == yarg) {
            index = i;
            break;
        }
    }

    return index;
}

int findCoord(std::vector<MazeCoord> &maze, MazeCoord &coord)
{
    int index;

    for (unsigned int i = 0; i < maze.size(); i++) {
        if (maze[i].getCoordX() == coord.getCoordX() &&
            maze[i].getCoordY() == coord.getCoordY()) {
            index = i;
            break;
        }
    }

    return index;
}

// *******************************
// ** M A I N   F U N C T I O N **
// *******************************

#include <iostream>
#include <vector>
#include "MazeCoord.h"

using namespace std;

#define MAXVALX 70
#define MAXVALY 20

int findNeighbourWalls(vector<MazeCoord> &maze, MazeCoord &coordArg);
int findCoord(vector<MazeCoord> &maze, int xarg, int yarg);
void genMaze(std::vector<MazeCoord> &maze, MazeCoord &coord);
void display(vector<MazeCoord> &maze, MazeCoord &coord);
void setAllWall(vector<MazeCoord> &maze, MazeCoord &coord);
void setBorder(std::vector<MazeCoord> &maze, MazeCoord &coord);

int main()
{
    MazeCoord coord(0, 0);
    coord.setMaxX(MAXVALX);
    coord.setMaxY(MAXVALY);
    coord.setNumCoord();

    vector<MazeCoord> maze(coord.getNumCoord());

    setAllWall(maze, coord);
    setBorder(maze, coord);

    genMaze(maze, coord);

    display(maze, coord);

    return 0;
}
