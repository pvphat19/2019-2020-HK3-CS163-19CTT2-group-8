#include <iostream>
#include <string>
#include <exception>
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

    void setColorAttr()
    {
        size_t colorAttr = ((unsigned)backColor << 4) | (unsigned)foreColor;
        if (!SetConsoleTextAttribute(hConsole, colorAttr))
            throw std::runtime_error("Cannot set text attribute of console.");
    }

    void setCoord()
    {
        COORD newCoord;
        newCoord.Y = line;
        newCoord.X = col;

        if (!SetConsoleCursorPosition(hConsole, newCoord))
            throw std::runtime_error("Cannot set cursor position of console.");
    }

    void getCursorPos()
    {
        if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
            throw std::runtime_error("Cannot get information of console handle.");

        line = csbi.dwCursorPosition.Y;
        col = csbi.dwCursorPosition.X;
    }

public:
    size_t line = 0, col = 0;
    ConsoleColor foreColor = GRAY, backColor = BLACK;

    Console()
    {
        if ((hConsole = GetStdHandle(STD_OUTPUT_HANDLE)) == INVALID_HANDLE_VALUE)
            throw std::runtime_error("Cannot get handle of console.");

        if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
            throw std::runtime_error("Cannot get information of console handle.");

        line = csbi.dwCursorPosition.Y;
        col = csbi.dwCursorPosition.X;
        foreColor = static_cast<ConsoleColor>((size_t)csbi.wAttributes % 16);
        backColor = static_cast<ConsoleColor>((size_t)csbi.wAttributes >> 4);
    }

    Console& clear()    // CLEAR SCREEN FUNCTION
    {
        system("cls");
        return *this;
    }

    Console& setCursorPos (size_t vert, size_t hori) // SET CURSOR POSITION
    {
        COORD newCoord;
        newCoord.Y = vert;
        newCoord.X = hori;

        if (!SetConsoleCursorPosition(hConsole, newCoord))
            throw std::runtime_error("Cannot set cursor position of console.");

        line = vert;
        col = hori;

        return *this;
    }

    Console& getCurrentConsoleRect (size_t& top, size_t& left, size_t& bottom, size_t& right) // GET CURRENT CURSOR POSITION
    {
        top = left = bottom = right = 0;

        if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
            throw std::runtime_error("Cannot get information of console handle.");

        top = csbi.srWindow.Top;
        left = csbi.srWindow.Left;
        bottom = csbi.srWindow.Bottom;
        right = csbi.srWindow.Right;

        return *this;
    }

    Console& getCurrentConsoleSize (size_t& height, size_t& width) // GET CURRENT CONSOLE SIZE
    {
        height = width = 0;

        size_t top, left, bottom, right;
        getCurrentConsoleRect(top, left, bottom, right);

        height = (int)bottom - (int)top + 1;
        width = (int)right - (int)left + 1;

        return *this;
    }

    Console& getMaximumConsoleSize (size_t& height, size_t& width)
    {
        height = width = 0;

        if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
            throw std::runtime_error("Cannot get information of console handle.");

        height = csbi.dwMaximumWindowSize.Y;
        width = csbi.dwMaximumWindowSize.X;

        return *this;
    }

    Console& toLine (size_t newLine) //MOVE TO LINE newLine (y axis)
    {
        getCursorPos();

        line = newLine;
        setCoord();

        return *this;
    }

    Console& toCol (size_t newCol) //MOVE TO COLUMN newCol (x axis)
    {
        getCursorPos();

        col = newCol;
        setCoord();

        return *this;
    }

    Console& setForeColor (ConsoleColor color) //SET WORD COLOR
    {
        foreColor = color;
        setColorAttr();

        return *this;
    }

    Console& setBackColor (ConsoleColor color) //SET BACKGROUND COLOR
    {
        backColor = color;
        setColorAttr();

        return *this;
    }

    Console& write (const std::string& text, bool wrap = true) //WRITE TEXT  (START FROM CURSOR POSITION)
    {
        getCursorPos();

        if (!wrap)
        {
            for (size_t i = 0; i < text.length(); ++i)
                putchar(text[i]);
        }
        else
        {
            size_t top, left, bottom, right;
            getCurrentConsoleRect(top, left, bottom, right);

            std::string newText = text.substr(0, right - col + 1);
            for (size_t i = 0; i < newText.length(); ++i)
                putchar(newText[i]);
        }

        getCursorPos();

        return *this;
    }

    Console& pause (bool cmd = true)    //PAUSE SCREEN
    {
        if (cmd)
            system("PAUSE");
        else
            _getch();
    }
};

int readkey (bool &fn, bool &arrow)
{
    fn = arrow = 0;
    int tmp = _getch();
    fn = !tmp;
    arrow = tmp == 224;
    if (fn || arrow)
    {
        tmp = _getch();
        return tmp;
    }
    return tmp;
}

void write_SE(Console &c) {
    c.setForeColor(LIGHT_RED).toCol(1).toLine(2).write(" ___  ____    __    ____   ___  _   _    ____  _  _  ___  ____  _  _  ____ ")
                       .toCol(1).toLine(3).write("/ __)( ___)  /__\\  (  _ \\ / __)( )_( )  ( ___)( \\( )/ __)(_  _)( \\( )( ___)")
                       .toCol(1).toLine(4).write("\\__ \\ )__)  /(__)\\  )   /( (__  ) _ (    )__)  )  (( (_-. _)(_  )  (  )__) ")
                       .toCol(1).toLine(5).write("(___/(____)(__)(__)(_)\\_) \\___)(_) (_)  (____)(_)\\_)\\___/(____)(_)\\_)(____)");
}

//Thieu doc query
void SEARCH_GUI(Console &c) {
    c.clear();
    write_SE(c);

    c.toLine(8).toCol(32).setForeColor(LIGHT_AQUA).write("-SEARCH BOX-");
    c.toLine(10).toCol(5).setForeColor(WHITE).write("|=================================================================|");
    c.toLine(11).toCol(5).setForeColor(WHITE).write("|                                                                 |");
    c.toLine(12).toCol(5).setForeColor(WHITE).write("|                                                                 |");
    c.toLine(13).toCol(5).setForeColor(WHITE).write("|                                                                 |");
    c.toLine(14).toCol(5).setForeColor(WHITE).write("|=================================================================|");

    /*do {

    } while (); // Read String Here*/


    c.pause(false); //Delete this line after writing do while loop above

}

//Thieu INDEX DATA FUNCTION
void menu(Console &c) {
    while (1) {
        c.clear();
        write_SE(c);
        c.toLine(8).toCol(35).setForeColor(YELLOW).write("-MENU-");
        c.toLine(10).toCol(8).setForeColor(WHITE).write("> 1. Index data");
        c.toLine(11).toCol(10).setForeColor(WHITE).write("2. Search");
        c.toLine(12).toCol(10).setForeColor(WHITE).write("3. Exit");

        int ch, cur = 0;
        bool fn,arrow;
        do {
            ch=readkey(fn, arrow);
            if (arrow)
            {
                if (ch==UP_ARROW)
                {
                    c.toLine(10 + cur).toCol(8).write(" ");
                    cur = (cur + 2) % 3;
                    c.toLine(10 + cur).toCol(8).setForeColor(LIGHT_YELLOW).write(">").toCol(8);
                }
                else if (ch==DOWN_ARROW)
                {
                    c.toLine(10 + cur).toCol(8).write(" ");
                    cur = (cur + 1) % 3;
                    c.toLine(10 + cur).toCol(8).setForeColor(LIGHT_YELLOW).write(">").toCol(8);
                }
            } else {
                if (ch >= 10 && ch <= 13) {
                    if (cur==0) {
                        //INDEX FUNCTION
                        break;
                    } else if (cur==1) {
                        SEARCH_GUI(c);
                        break;
                    } else
                        return;
                }
            }
        } while (1);
    }
}
