// to run the this code on the terminal    g++ -std=c++11 ~/Desktop/tetris.cpp -framework OpenGL -framework GLUT -o tetris
// and than   ./tetris

#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <algorithm>
#include <random>

using namespace std;

// Grid size and window configuration
const int noRows = 24, noCols = 10;
const int blockSize = 25;
const int windowWidth = blockSize * noCols + 200;
const int windowHeight = blockSize * noRows;

bool justReset = false;

// Each grid cell has a color and a flag indicating whether it's filled
struct Color
{
    float r = 1.0f, g = 1.0f, b = 1.0f;
    bool isFilled = false;
};

Color grid[noRows][noCols];

// Point structure to represent blocks of shapes
struct Point
{
    int x, y;
};

// Definitions of all 7 Tetriminos with their rotations
vector<vector<vector<Point>>> tetriminos = {
    {{{0, 0}, {1, 0}, {2, 0}, {3, 0}}, {{1, -1}, {1, 0}, {1, 1}, {1, 2}}},
    {{{0, 0}, {1, 0}, {0, 1}, {1, 1}}},
    {{{0, 0}, {1, 0}, {1, 1}, {2, 1}}, {{1, 0}, {0, 1}, {1, 1}, {0, 2}}},
    {{{1, 0}, {0, 1}, {1, 1}, {2, 1}}, {{1, 0}, {1, 1}, {2, 1}, {1, 2}}, {{0, 1}, {1, 1}, {2, 1}, {1, 2}}, {{1, 0}, {0, 1}, {1, 1}, {1, 2}}},
    {{{0, 0}, {0, 1}, {0, 2}, {1, 2}}, {{0, 1}, {1, 1}, {2, 1}, {2, 0}}, {{1, 0}, {1, 1}, {1, 2}, {0, 0}}, {{0, 1}, {1, 1}, {2, 1}, {0, 2}}},
    {{{1, 0}, {1, 1}, {1, 2}, {0, 2}}, {{0, 0}, {0, 1}, {1, 1}, {2, 1}}, {{0, 0}, {1, 0}, {0, 1}, {0, 2}}, {{0, 0}, {1, 0}, {2, 0}, {2, 1}}},
    {{{1, 0}, {2, 0}, {0, 1}, {1, 1}}, {{0, 0}, {0, 1}, {1, 1}, {1, 2}}}};

int currShape, currRot;
Point currPos;
float colorR, colorG, colorB;
bool isFalling = true;
int score = 0;
deque<int> pieceQueue; // Piece bag for 7-bag system

// Clears the entire grid
void resetGrid()
{
    for (int i = 0; i < noRows; ++i)
        for (int j = 0; j < noCols; ++j)
            grid[i][j] = Color();
}

// Refills the bag with a shuffled set of all 7 Tetriminos
void refillBag()
{
    vector<int> bag = {0, 1, 2, 3, 4, 5, 6};
    random_device rd;
    mt19937 g(rd());
    shuffle(bag.begin(), bag.end(), g);
    for (int i : bag)
        pieceQueue.push_back(i);
}

// Spawns a new shape from the bag
void spawnNewShape()
{
    if (pieceQueue.empty())
        refillBag();
    currShape = pieceQueue.front();
    pieceQueue.pop_front();

    currRot = 0;
    currPos = {noCols / 2 - 2, noRows - 3};
    do
    {
        colorR = rand() % 2;
        colorG = rand() % 2;
        colorB = rand() % 2;
    } while (colorR + colorG + colorB != 1);  // Ensure only one color channel is active
}

// Checks if a shape can be placed at an offset with given rotation
bool isValidPosition(Point offset, int rot)
{
    for (auto block : tetriminos[currShape][rot])
    {
        int x = currPos.x + block.x + offset.x;
        int y = currPos.y + block.y + offset.y;
        if (x < 0 || x >= noCols || y < 0 || y >= noRows)
            return false;
        if (grid[y][x].isFilled)
            return false;
    }
    return true;
}

// Places the current piece on the grid and checks for game over
void settlePiece()
{
    bool gameOver = false;
    for (auto block : tetriminos[currShape][currRot])
    {
        int x = currPos.x + block.x;
        int y = currPos.y + block.y;
        if (y < 0)
        {
            gameOver = true;
            continue;
        }
        grid[y][x].r = colorR;
        grid[y][x].g = colorG;
        grid[y][x].b = colorB;
        grid[y][x].isFilled = true;
    }

    if (gameOver)
    {
        cout << "Game Over! Score reset.\n";
        resetGrid();
        score = 0;
        justReset = true;
        return;
    }

    isFalling = false;
}

// Checks and clears filled lines, increases score
void clearLines()
{
    int linesCleared = 0;
    for (int i = 0; i < noRows; ++i)
    {
        bool full = true;
        for (int j = 0; j < noCols; ++j)
            if (!grid[i][j].isFilled)
                full = false;
        if (full)
        {
            ++linesCleared;
            for (int k = i; k < noRows - 1; ++k)
                for (int j = 0; j < noCols; ++j)
                    grid[k][j] = grid[k + 1][j];
            for (int j = 0; j < noCols; ++j)
                grid[noRows - 1][j] = Color();
            --i; // Recheck same row
        }
    }
    score += linesCleared * 100;
}

// Draws text at a given position
void drawText(float x, float y, const char *str, void *font = GLUT_BITMAP_9_BY_15)
{
    glRasterPos2f(x, y);
    while (*str)
    {
        glutBitmapCharacter(font, *str);
        str++;
    }
}

// Renders the settled blocks on the grid
void drawGrid()
{
    for (int i = 0; i < noRows; ++i)
    {
        for (int j = 0; j < noCols; ++j)
        {
            Color &c = grid[i][j];
            glColor3f(c.r, c.g, c.b);
            glBegin(GL_POLYGON);
            glVertex2i(j * blockSize, i * blockSize);
            glVertex2i((j + 1) * blockSize, i * blockSize);
            glVertex2i((j + 1) * blockSize, (i + 1) * blockSize);
            glVertex2i(j * blockSize, (i + 1) * blockSize);
            glEnd();
        }
    }
}

// Renders the currently falling Tetrimino
void drawCurrent()
{
    glColor3f(colorR, colorG, colorB);
    for (auto block : tetriminos[currShape][currRot])
    {
        int x = currPos.x + block.x;
        int y = currPos.y + block.y;
        glBegin(GL_POLYGON);
        glVertex2i(x * blockSize, y * blockSize);
        glVertex2i((x + 1) * blockSize, y * blockSize);
        glVertex2i((x + 1) * blockSize, (y + 1) * blockSize);
        glVertex2i(x * blockSize, (y + 1) * blockSize);
        glEnd();
    }
}

// Called each frame to draw everything
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawGrid();
    if (isFalling)
        drawCurrent();

    glColor3f(0, 0, 0);
    string s = "Score: " + to_string(score);
    drawText(noCols * blockSize + 20, windowHeight - 40, s.c_str());

    glutSwapBuffers();
}

// Timer function to update game logic periodically
void timer(int = 0)
{
    if (isFalling && isValidPosition({0, -1}, currRot))
    {
        currPos.y--;
    }
    else
    {
        if (isFalling)
            settlePiece();

        // Check if the board is entirely empty (after a game reset)
        bool emptyGrid = true;
        for (int i = 0; i < noRows; ++i)
        {
            for (int j = 0; j < noCols; ++j)
            {
                if (grid[i][j].isFilled)
                {
                    emptyGrid = false;
                    break;
                }
            }
            if (!emptyGrid)
                break;
        }

        if (!emptyGrid)
        {
            clearLines();
        }

        if (!justReset)
        {
            spawnNewShape();
            isFalling = true;
        }
        else
        {
            justReset = false; // Don't spawn now, wait until next tick
        }
    }

    glutPostRedisplay();
    glutTimerFunc(300, timer, 0);
}

// Handles arrow key presses for movement and rotation
void specialKeys(int key, int, int)
{
    if (!isFalling)
        return;
    if (key == GLUT_KEY_LEFT && isValidPosition({-1, 0}, currRot))
        currPos.x--;
    else if (key == GLUT_KEY_RIGHT && isValidPosition({1, 0}, currRot))
        currPos.x++;
    else if (key == GLUT_KEY_DOWN && isValidPosition({0, -1}, currRot))
        currPos.y--;
    else if (key == GLUT_KEY_UP)
    {
        int nextRot = (currRot + 1) % tetriminos[currShape].size();
        if (isValidPosition({0, 0}, nextRot))
            currRot = nextRot;
    }
    glutPostRedisplay();
}

// Initializes OpenGL and game state
void init()
{
    glClearColor(1, 1, 1, 1);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, windowWidth, 0, windowHeight);
    resetGrid();
    spawnNewShape();
}

// Entry point
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Refactored Tetris — Random & Scored");
    init();
    glutDisplayFunc(display);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(300, timer, 0);
    glutMainLoop();
    return 0;
}
