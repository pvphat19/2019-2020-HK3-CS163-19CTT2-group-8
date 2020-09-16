#include <string>
#include <exception>
#include <stdexcept>
#include <cstdio>
#include <cstdlib>
#include <windows.h>
#include <conio.h>

#define UP_ARROW     72
#define DOWN_ARROW   80
#define LEFT_ARROW   75
#define RIGHT_ARROW  77

enum ConsoleColor
{
    BLACK  = 0,  GRAY         = 8,
    BLUE   = 1,  LIGHT_BLUE   = 9,
    GREEN  = 2,  LIGHT_GREEN  = 10,
    AQUA   = 3,  LIGHT_AQUA   = 11,
    RED    = 4,  LIGHT_RED    = 12,
    PURPLE = 5,  LIGHT_PURPLE = 13,
    YELLOW = 6,  LIGHT_YELLOW = 14,
    WHITE  = 7,  BRIGHT_WHITE = 15
};

class Console
{
private:
    HANDLE hConsole;
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    void setColorAttr();
    void setCoord();
    void getCursorPos();

public:
    size_t line = 0, col = 0;
    ConsoleColor foreColor = GRAY, backColor = BLACK;

    Console();

    Console& clear();    // CLEAR SCREEN FUNCTION

    Console& setCursorPos (size_t vert, size_t hori); // SET CURSOR POSITION

    Console& getCurrentConsoleRect (size_t& top, size_t& left, size_t& bottom, size_t& right); // GET CURRENT CURSOR POSITION

    Console& getCurrentConsoleSize (size_t& height, size_t& width); // GET CURRENT CONSOLE SIZE

    Console& getMaximumConsoleSize (size_t& height, size_t& width);

    Console& toLine (size_t newLine); //MOVE TO LINE newLine (y axis)

    Console& toCol (size_t newCol); //MOVE TO COLUMN newCol (x axis)

    Console& setForeColor (ConsoleColor color); //SET WORD COLOR

    Console& setBackColor (ConsoleColor color); //SET BACKGROUND COLOR

    Console& write (const std::string& text, bool wrap); //WRITE TEXT  (START FROM CURSOR POSITION)

    Console& pause (bool cmd);   //PAUSE SCREEN
};

int readkey (bool &fn, bool &arrow);

void write_SE(Console &c);

void SEARCH_GUI(Console &c);

void menu(Console &c);
