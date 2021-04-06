﻿#pragma once

#include "MinDefines.h"
#include <utility>
#include <string>

namespace MinConsoleNative
{
    enum class CharWidth
    {
        Unknown = 0,
        Half = 1,
        Full = 2,
    };

    enum class ConsoleColor
    {
        BLACK = 0,
        DARKBLUE = 1,
        DARKGREEN = 2,
        DARKCYAN = 3,
        DARKRED = 4,
        DARKMAGENTA = 5,
        DARKYELLOW = 6,
        GRAY = 7,
        DARKGRAY = 8,
        BLUE = 9,
        GREEN = 10,
        CYAN = 11,
        RED = 12,
        MAGENTA = 13,
        YELLOW = 14,
        WHITE = 15,
    };

    inline ushort ConsoleColorToUshort(ConsoleColor foreColor, ConsoleColor backColor)
    {
        return (ushort)((ushort)foreColor | ((ushort)backColor << 4));
    }

    inline std::pair<ConsoleColor, ConsoleColor> UshortToConsoleColor(ushort u)
    {
        std::pair<ConsoleColor, ConsoleColor> result;
        result.first = (ConsoleColor)(u & 0x000F);
        result.second = (ConsoleColor)((u & 0x00F0) / 16);
        return result;
    }

    struct ConsoleColorPair
    {
    public:
        ConsoleColor foreColor;
        ConsoleColor backColor;

        ConsoleColorPair()
        {
            this->foreColor = ConsoleColor::GRAY;
            this->backColor = ConsoleColor::BLACK;
        }

        ConsoleColorPair(ConsoleColor foreColor)
        {
            this->foreColor = foreColor;
            this->backColor = ConsoleColor::BLACK;
        }

        ConsoleColorPair(ConsoleColor foreColor, ConsoleColor backColor)
        {
            this->foreColor = foreColor;
            this->backColor = backColor;
        }

        ConsoleColorPair(ushort u)
        {
            std::pair<ConsoleColor, ConsoleColor> cp = UshortToConsoleColor(u);
            this->foreColor = cp.first;
            this->backColor = cp.second;
        }

        ushort ToUshort()
        {
            return ConsoleColorToUshort(foreColor, backColor);
        }
    };

    struct Color24
    {
    public:
        uint r;
        uint g;
        uint b;

        Color24()
        {
            this->r = 0;
            this->g = 0;
            this->b = 0;
        }

        Color24(uint r, uint g, uint b)
        {
            this->r = r;
            this->g = g;
            this->b = b;
        }

        bool operator ==(const Color24& other)
        {
            if (r == other.r && g == other.g && b == other.b)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        bool operator !=(const Color24& other)
        {
            if (r == other.r && g == other.g && b == other.b)
            {
                return false;
            }
            else
            {
                return true;
            }
        }

        //Algorithm:https://stackoverflow.com/questions/1988833/converting-color-to-consolecolor
        ConsoleColor ToConsoleColor()
        {
            int index = (r > 128 || g > 128 || b > 128) ? 8 : 0; // Bright bit
            index |= (r > 64) ? 4 : 0; // Red bit
            index |= (g > 64) ? 2 : 0; // Green bit
            index |= (b > 64) ? 1 : 0; // Blue bit
            return (ConsoleColor)index;
        }
    };

    struct ConsoleInputMode
    {
    public:
        bool _ENABLE_PROCESSED_INPUT;               //default is TRUE(recommend to FALSE to avoid pressing CTRL+C to close the console program)
        bool _ENABLE_LINE_INPUT;                    //default is TRUE
        bool _ENABLE_ECHO_INPUT;                    //default is TRUE
        bool _ENABLE_WINDOW_INPUT;                  //default is FALSE(recommend to TRUE)
        bool _ENABLE_MOUSE_INPUT;                   //default is TRUE
        bool _ENABLE_INSERT_MODE;                   //default is TRUE
        bool _ENABLE_QUICK_EDIT_MODE;               //default is TRUE(recommend to FALSE)
        bool _ENABLE_EXTENDED_FLAGS;                //default is TRUE
        bool _ENABLE_AUTO_POSITION;                 //default is TRUE
        bool _ENABLE_VIRTUAL_TERMINAL_INPUT;        //default is FALSE

        ConsoleInputMode()
        {
            this->_ENABLE_PROCESSED_INPUT = true;
            this->_ENABLE_LINE_INPUT = true;
            this->_ENABLE_ECHO_INPUT = true;
            this->_ENABLE_WINDOW_INPUT = false;
            this->_ENABLE_MOUSE_INPUT = true;
            this->_ENABLE_INSERT_MODE = true;
            this->_ENABLE_QUICK_EDIT_MODE = true;
            this->_ENABLE_EXTENDED_FLAGS = true;
            this->_ENABLE_AUTO_POSITION = true;
            this->_ENABLE_VIRTUAL_TERMINAL_INPUT = false;
        }

        static ConsoleInputMode Standard()
        {
            ConsoleInputMode cim;
            cim._ENABLE_PROCESSED_INPUT = false;
            cim._ENABLE_WINDOW_INPUT = true;
            cim._ENABLE_QUICK_EDIT_MODE = false;
            return cim;
        }
    };

    struct ConsoleOutputMode
    {
    public:
        bool _ENABLE_PROCESSED_OUTPUT;              //default is TRUE
        bool _ENABLE_WRAP_AT_EOL_OUTPUT;            //default is TRUE
        bool _ENABLE_VIRTUAL_TERMINAL_PROCESSING;   //default is FALSE(recommend to TRUE, If want to use the VT100 sequence)
        bool _DISABLE_NEWLINE_AUTO_RETURN;          //default is FALSE
        bool _ENABLE_LVB_GRID_WORLDWIDE;            //default is FALSE

        ConsoleOutputMode()
        {
            this->_ENABLE_PROCESSED_OUTPUT = true;
            this->_ENABLE_WRAP_AT_EOL_OUTPUT = true;
            this->_ENABLE_VIRTUAL_TERMINAL_PROCESSING = false;
            this->_DISABLE_NEWLINE_AUTO_RETURN = false;
            this->_ENABLE_LVB_GRID_WORLDWIDE = false;
        }

        static ConsoleOutputMode Standard()
        {
            ConsoleOutputMode com;
            com._ENABLE_VIRTUAL_TERMINAL_PROCESSING = true;
            return com;
        }
    };

    struct ConsoleMode
    {
    public:
        ConsoleInputMode inputMode;
        ConsoleOutputMode outputMode;

        ConsoleMode()
        {
            this->inputMode = ConsoleInputMode();
            this->outputMode = ConsoleOutputMode();
        }

        ConsoleMode(ConsoleInputMode inputMode, ConsoleOutputMode outputMode)
        {
            this->inputMode = inputMode;
            this->outputMode = outputMode;
        }

        static ConsoleMode Standard()
        {
            auto inputMode = ConsoleInputMode::Standard();
            auto outputMode = ConsoleOutputMode::Standard();
            ConsoleMode consoleMode(inputMode, outputMode);
            return consoleMode;
        }
    };

    struct ConsoleSession
    {
    public:
        HWND consoleWindow;
        HANDLE consoleInput;
        HANDLE consoleOutput;

    public:
        ConsoleSession()
        {
            this->consoleWindow = nullptr;
            this->consoleInput = nullptr;
            this->consoleOutput = nullptr;
        }

        ConsoleSession(HWND consoleWindow, HANDLE consoleInput, HANDLE consoleOutput)
        {
            this->consoleWindow = consoleWindow;
            this->consoleInput = consoleInput;
            this->consoleOutput = consoleOutput;
        }
    };

    struct ConsoleFont
    {
    public:
        DWORD FontIndex{ 0 };
        COORD FontSize{ 0 };
        uint FontFamily{ 0 };
        uint FontWeight{ 0 }; //Over 400 is bold
        wchar FaceName[LF_FACESIZE]{ 0 };

        ConsoleFont()
        {
        }

        void SetFaceNameQuick(const std::wstring& faceName)
        {
            size_t fontNameLength = faceName.size();
            ::wcscpy_s(this->FaceName, fontNameLength + 1, faceName.c_str());
        }
    };

    extern const int MAX_INPUT_CHAR_COUNT;

    //Initialize the console inside, don't worry.
    EXPORT_FUNC MinInitConsoleSession(ConsoleSession* cons);

    //return true means successful enable VT!
    EXPORT_FUNC MinEnableConsoleVT(HANDLE consoleInput, HANDLE consoleOutput);

    EXPORT_FUNC MinGetConsolePalette(HANDLE consoleOutput, DWORD index, Color24* color);

    EXPORT_FUNC MinSetConsolePalette(HANDLE consoleOutput, DWORD index, Color24 color);

    EXPORT_FUNC MinGetConsoleMode(HANDLE consoleInput, HANDLE consoleOutput, ConsoleMode* consoleMode);

    EXPORT_FUNC MinSetConsoleMode(HANDLE consoleInput, HANDLE consoleOutput, ConsoleMode consoleMode);

    EXPORT_FUNC MinGetConsoleFont(HANDLE consoleOutput, ConsoleFont* consoleFont);

    EXPORT_FUNC MinSetConsoleFont(HANDLE consoleOutput, ConsoleFont consoleFont);

    EXPORT_FUNC MinGetConsoleWindowSize(HANDLE consoleOutput, POINT* size);

    EXPORT_FUNC MinSetConsoleWindowSize(HANDLE consoleOutput, POINT size);

    EXPORT_FUNC MinGetConsoleBufferSize(HANDLE consoleOutput, POINT* size);

    EXPORT_FUNC MinSetConsoleBufferSize(HANDLE consoleOutput, POINT size);

    EXPORT_FUNC MinGetConsoleForeColor(HANDLE consoleOutput, ConsoleColor* foreColor);

    EXPORT_FUNC MinGetConsoleBackColor(HANDLE consoleOutput, ConsoleColor* backColor);

    EXPORT_FUNC MinSetConsoleForeColor(HANDLE consoleOutput, ConsoleColor foreColor);

    EXPORT_FUNC MinSetConsoleBackColor(HANDLE consoleOutput, ConsoleColor backColor);

    EXPORT_FUNC MinGetConsoleCursorPos(HANDLE consoleOutput, COORD* pos);

    EXPORT_FUNC MinSetConsoleCursorPos(HANDLE consoleOutput, COORD pos);

    //size of buffer should be : charCount * sizeof(wchar)
    EXPORT_FUNC MinReadConsole(HANDLE consoleInput, wchar* buffer, int charCount);

    EXPORT_FUNC MinWriteConsole(HANDLE consoleOutput, const wchar* buffer);

    //You can use something like this:arr[i].Attributes |= COMMON_LVB_UNDERSCORE
    EXPORT_FUNC MinWriteConsoleOutput(HANDLE consoleOutput, const CHAR_INFO* charInfos, short x, short y, short width, short height);

    //This function is useful for mixed renderer.
    EXPORT_FUNC MinWriteConsoleOutputCharacter(HANDLE consoleOutput, const wchar* str, int charCount, COORD pos);

    //The created HANDLE can be recycled using the MinCloseConsoleScreenBuffer function
    EXPORT_FUNC MinCreateConsoleScreenBuffer(HANDLE* consoleOutput);

    EXPORT_FUNC MinSetConsoleActiveScreenBuffer(HANDLE consoleOutput);

    EXPORT_FUNC MinCloseConsoleScreenBuffer(HANDLE consoleOutput);

    //====================todo_start====================
    EXPORT_FUNC MinCreateFile();

    EXPORT_FUNC MinWriteFile();

    EXPORT_FUNC MinReadFile();
    //====================todo_end======================

    EXPORT_FUNC MinGetCharWidth(HWND consoleWindow, HANDLE consoleOutput, wchar c, CharWidth* cw);

    //sizeof(title) should be MAX_PATH
    EXPORT_FUNC MinGetTitle(wchar* titleBuffer, int sizeOfBuffer);

    EXPORT_FUNC MinSetTitle(const wchar* titleBuffer);

    EXPORT_FUNC MinGetConsoleCursorVisible(HANDLE consoleOutput, bool* visible);

    EXPORT_FUNC MinSetConsoleCursorVisible(HANDLE consoleOutput, bool visible);

    //See:https://docs.microsoft.com/en-us/windows/console/clearing-the-screen
    EXPORT_FUNC MinClear(HANDLE consoleOutput);

    //nullptr, true to disable ctrl + c signal
    EXPORT_FUNC MinSetConsoleCtrlHandler(PHANDLER_ROUTINE handler, bool add);

    class Console
    {
    public:
        ConsoleSession cons;

        Console();

        Console(ConsoleSession cons);

        Console(HWND consoleWindow, HANDLE consoleInput, HANDLE consoleOutput);

        Color24 GetConsolePalette(DWORD index);

        //Notice:After calling this API, the screen buffer will change!
        bool SetConsolePalette(DWORD index, const Color24& color);

        ConsoleMode GetConsoleMode();

        bool SetConsoleMode(const ConsoleMode& consoleMode);

        ConsoleFont GetConsoleFont();

        bool SetConsoleFont(const ConsoleFont& consoleFont);

        POINT GetConsoleWindowSize();

        bool SetConsoleWindowSize(POINT size);

        POINT GetConsoleBufferSize();

        bool SetConsoleBufferSize(POINT size);

        ConsoleColor GetConsoleForeColor();

        ConsoleColor GetConsoleBackColor();

        bool SetConsoleForeColor(ConsoleColor foreColor);

        bool SetConsoleBackColor(ConsoleColor backColor);

        COORD GetConsoleCursorPos();

        bool SetConsoleCursorPos(COORD pos);

        std::wstring ReadConsoleW();

        bool WriteConsoleW(const std::wstring& msg);

        bool WriteConsoleOutputW(const CHAR_INFO* charInfos, short x, short y, short width, short height);

        bool WriteConsoleOutputCharacterW(const std::wstring& str, COORD pos);

        HANDLE CreateConsoleScreenBuffer();

        bool SetConsoleActiveScreenBuffer(HANDLE consoleOutput);

        bool CloseConsoleScreenBuffer(HANDLE consoleOutput);

        bool CreateFileW();

        bool WriteFile();

        bool ReadFile();

        std::wstring GetTitle();

        bool SetTitle(const std::wstring& title);

        bool GetConsoleCursorVisible();

        bool SetConsoleCursorVisible(bool visible);

        //See:https://docs.microsoft.com/en-us/windows/console/clearing-the-screen
        bool Clear();

        CharWidth GetWcharWidth(wchar c);
    };
}