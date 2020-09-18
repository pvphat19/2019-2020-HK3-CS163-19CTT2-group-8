#include "Console.h"


void Console::setColorAttr()
{
    size_t colorAttr = ((unsigned)backColor << 4) | (unsigned)foreColor;
    if (!SetConsoleTextAttribute(hConsole, colorAttr))
        throw std::runtime_error("Cannot set text attribute of console.");
}

void Console::setCoord()
{
    COORD newCoord;
    newCoord.Y = line;
    newCoord.X = col;

    if (!SetConsoleCursorPosition(hConsole, newCoord))
        throw std::runtime_error("Cannot set cursor position of console.");
}

void Console::getCursorPos()
{
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
        throw std::runtime_error("Cannot get information of console handle.");

    line = csbi.dwCursorPosition.Y;
    col = csbi.dwCursorPosition.X;
}

Console::Console()
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

Console& Console::clear()    // CLEAR SCREEN FUNCTION
{
    system("cls");
    return *this;
}

Console& Console::setCursorPos (size_t vert, size_t hori) // SET CURSOR POSITION
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

Console& Console::getCurrentConsoleRect (size_t& top, size_t& left, size_t& bottom, size_t& right) // GET CURRENT CURSOR POSITION
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

Console& Console::getCurrentConsoleSize (size_t& height, size_t& width) // GET CURRENT CONSOLE SIZE
{
    height = width = 0;

    size_t top, left, bottom, right;
    getCurrentConsoleRect(top, left, bottom, right);

    height = (int)bottom - (int)top + 1;
    width = (int)right - (int)left + 1;

    return *this;
}

Console& Console::getMaximumConsoleSize (size_t& height, size_t& width)
{
    height = width = 0;

    if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
        throw std::runtime_error("Cannot get information of console handle.");

    height = csbi.dwMaximumWindowSize.Y;
    width = csbi.dwMaximumWindowSize.X;

    return *this;
}

Console& Console::toLine (size_t newLine) //MOVE TO LINE newLine (y axis)
{
    getCursorPos();

    line = newLine;
    setCoord();

    return *this;
}

Console& Console::toCol (size_t newCol) //MOVE TO COLUMN newCol (x axis)
{
    getCursorPos();

    col = newCol;
    setCoord();

    return *this;
}

Console& Console::setForeColor (ConsoleColor color) //SET WORD COLOR
{
    foreColor = color;
    setColorAttr();

    return *this;
}

Console& Console::setBackColor (ConsoleColor color) //SET BACKGROUND COLOR
{
    backColor = color;
    setColorAttr();

    return *this;
}

Console& Console::write (const std::string& text, bool wrap = true) //WRITE TEXT  (START FROM CURSOR POSITION)
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

Console& Console::pause (bool cmd = true)    //PAUSE SCREEN
{
    if (cmd)
        system("PAUSE");
    else
        _getch();

    return *this;
}

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

void write_SE(Console &c)
{
    c.setForeColor(LIGHT_RED).toCol(32).toLine(2).write(" ___  ____    __    ____   ___  _   _    ____  _  _  ___  ____  _  _  ____ ")
    .toCol(32).toLine(3).write("/ __)( ___)  /__\\  (  _ \\ / __)( )_( )  ( ___)( \\( )/ __)(_  _)( \\( )( ___)")
    .toCol(32).toLine(4).write("\\__ \\ )__)  /(__)\\  )   /( (__  ) _ (    )__)  )  (( (_-. _)(_  )  (  )__) ")
    .toCol(32).toLine(5).write("(___/(____)(__)(__)(_)\\_) \\___)(_) (_)  (____)(_)\\_)\\___/(____)(_)\\_)(____)");
}

//Thieu doc query
void SEARCH_GUI(Console &c, string query)
{
    c.clear();
    write_SE(c);

    c.toLine(8).toCol(62).setForeColor(LIGHT_AQUA).write("-SEARCH BOX-");
    c.toLine(10).toCol(37).setForeColor(WHITE).write("|=================================================================|");
    c.toLine(11).toCol(37).setForeColor(WHITE).write("|");
    c.toLine(11).toCol(39).setForeColor(WHITE).write(query);
    c.toLine(11).toCol(103).setForeColor(WHITE).write("|");
    c.toLine(12).toCol(37).setForeColor(WHITE).write("|                                                                 |");
    c.toLine(13).toCol(37).setForeColor(WHITE).write("|=================================================================|");
    c.setCursorPos(11, 39 + query.size());
    /*do {

    } while (); // Read String Here*/


   //Delete this line after writing do while loop above

}

//Thieu INDEX DATA FUNCTION
void menu(Console &c, trieNode* mainTrie, trieNode* titleTrie, trieNode2* history, vector<string> docPath)
{
    while (1)
    {
        c.clear();
        write_SE(c);
        c.toLine(8).toCol(66).setForeColor(YELLOW).write("-MENU-");
        c.toLine(10).toCol(39).setForeColor(WHITE).write("> 1. Index data");
        c.toLine(11).toCol(41).setForeColor(WHITE).write("2. Search");
        c.toLine(12).toCol(41).setForeColor(WHITE).write("3. Exit");

        int ch, cur = 0;
        bool fn,arrow;
        do
        {
            ch=readkey(fn, arrow);
            if (arrow)
            {
                if (ch==UP_ARROW)
                {
                    c.toLine(10 + cur).toCol(39).write(" ");
                    cur = (cur + 2) % 3;
                    c.toLine(10 + cur).toCol(39).setForeColor(LIGHT_YELLOW).write(">").toCol(39);
                }
                else if (ch==DOWN_ARROW)
                {
                    c.toLine(10 + cur).toCol(39).write(" ");
                    cur = (cur + 1) % 3;
                    c.toLine(10 + cur).toCol(39).setForeColor(LIGHT_YELLOW).write(">").toCol(39);
                }
            }
            else
            {
                if (ch >= 10 && ch <= 13)
                {
                    if (cur==0)
                    {
                       /* if (cin.rdbuf()) {
                            cin.clear();
                        }*/
                        c.clear();
                        userIndexNewDoc(mainTrie, titleTrie, docPath);
                        system("PAUSE");
                        break;
                    }
                    else if (cur==1)
                    {
                       /* if (cin.rdbuf()) {
                            cin.clear();
                        }*/
                        string query;
                        getInput(history, query, c);
                        removeStopWords(query);
                        c.clear();
                        presentResult(mainTrie, titleTrie, query, docPath, c);
                        system("PAUSE");
                        break;
                    }
                    else
                        return;
                }
            }
        }
        while (1);
    }
}

