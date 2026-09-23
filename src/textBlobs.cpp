#include "textBlobs.hpp"
std::string make_binary(const std::string& project_name) {
    return "#\n"
        "#Auto generated Citaidel CmakeLists.txt\n"
        "#\n\n"
        "cmake_minimum_required(VERSION 3.20)\n"
        "project(" + project_name + " VERSION 1.0 LANGUAGES CXX)\n"
        R"(
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BIN_SOURCES
    src/main.cpp
    #CITAIDEL_ANCHOR
)
add_executable(${PROJECT_NAME} ${BIN_SOURCES})
target_precompile_headers(${PROJECT_NAME} PRIVATE include/pch.h)
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
#CITAIDEL_LIBRARY_ANCHOR
if(WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE /utf-8)
    target_link_libraries(${PROJECT_NAME} PRIVATE user32 gdi32 shell32 advapi32)
else()
    find_package(Threads REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
endif()
)";
}

std::string make_library(const std::string& library_name) {
    return "#\n"
        "#Auto generated Citaidel CmakeLists.txt\n"
        "#\n\n"
        "cmake_minimum_required(VERSION 3.20)\n"
        "project(" + library_name + " VERSION 1.0 LANGUAGES CXX)\n"
        R"(
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(LIB_SOURCES
    src/lib.cpp
    #CITAIDEL_ANCHOR
)
add_library(${PROJECT_NAME} STATIC ${LIB_SOURCES})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_precompile_headers(${PROJECT_NAME} PRIVATE include/pch.h)
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
#CITAIDEL_LIBRARY_ANCHOR
if(WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE /utf-8)
    target_link_libraries(${PROJECT_NAME} PUBLIC user32 gdi32 shell32 advapi32)
else()
    find_package(Threads REQUIRED)
    target_link_libraries(${PROJECT_NAME} PUBLIC Threads::Threads)
endif()
add_executable(${PROJECT_NAME}_demo demo/demo.cpp)
target_link_libraries(${PROJECT_NAME}_demo PRIVATE ${PROJECT_NAME})
target_precompile_headers(${PROJECT_NAME}_demo PRIVATE include/pch.h)
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
install(FILES cmake/${PROJECT_NAME}Config.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
)";
}

std::string make_config(const std::string& project_name) {
    return "include(\"${CMAKE_CURRENT_LIST_DIR}/" + project_name + "Targets.cmake\")\n"
        R"(
if(NOT WIN32)
    find_package(Threads REQUIRED)
endif()
)";
}

std::string mktui() {
    return R"(/*
 *  ██████   ██████  █████   ████  ███████████  █████  █████  █████      █████   █████
 * ░░██████ ██████  ░░███   ███░  ░█░░░███░░░█ ░░███  ░░███  ░░███      ░░███   ░░███
 *  ░███░█████░███   ░███  ███    ░   ░███  ░   ░███   ░███   ░███       ░███    ░███
 *  ░███░░███ ░███   ░███████         ░███      ░███   ░███   ░███       ░███████████
 *  ░███ ░░░  ░███   ░███░░███        ░███      ░███   ░███   ░███       ░███░░░░░███
 *  ░███      ░███   ░███ ░░███       ░███      ░███   ░███   ░███       ░███    ░███
 *  █████     █████  █████ ░░████     █████     ░░████████    █████  ██  █████   █████
 * ░░░░░     ░░░░░  ░░░░░   ░░░░     ░░░░░       ░░░░░░░░    ░░░░░  ░░  ░░░░░   ░░░░░
 *
 *  mktui — Minimal Cross-Platform TUI Utilities
 *  ------------------------------------------------------------
 *  A lightweight, single-header C++ utility for building
 *  terminal user interfaces (TUIs).
 *
 *  It fetures:
 *   - Cross-platform (Windows / Linux / Unix)
 *   - Keyboard + mouse input handling
 *   - Console control (cursor, screen, title, etc.)
 *   - ANSI colors (8 / 16 / 24-bit)
 *   - No dependencies
 *
 *  Philosophy
 *  ----------
 *  mktui does the low-level, annoying console work so you don’t have to.
 *  It avoids opinionated abstractions and stays out of your way.
 *
 *  - No frameworks
 *  - No forced rendering model
 *  - No std::cout abuse (only ANSI/control sequences when needed)
 *
 *  You build the UI. mktui gives you the tools.
 *
 *  Usage Notes
 *  -----------
 *  - Use mktui::console_guard for proper setup/cleanup
 *  - Do not mix with other terminal control libraries
 *  - Designed for real-time / interactive console apps
 *
 *  License
 *  -------
 *
 * MIT License
 *
 * Copyright (c) 2026 Ladsm(https://github.com/ladsm/)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#if defined(_WIN32)
#include <windows.h>
#include <conio.h>
#else
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <poll.h>
#endif
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <iomanip>
#include <cstring>
#include <cstdio>
#include <memory>
#include <cctype>

namespace mktui {
#ifndef _WIN32
    inline static struct termios originalTermios;
#endif
    inline int get_Console_Width() {
#ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
            return csbi.srWindow.Right - csbi.srWindow.Left + 1;
        }
#else
        struct winsize w;
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) { return w.ws_col; }
#endif
        return 80;
    }
    inline int get_Console_Height() {
#ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
            return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        }
#else
        struct winsize w;
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) { return w.ws_row; }
#endif
        return 25;
    }
    enum class Input_Type {
        None,
        Arrow_Up,
        Arrow_Down,
        Arrow_Left,
        Arrow_Right,
        Num_0,
        Num_1,
        Num_2,
        Num_3,
        Num_4,
        Num_5,
        Num_6,
        Num_7,
        Num_8,
        Num_9,
        Enter,
        Space,
        Escape,
        Tab,
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
        Q, W, E, R, T, Y, U, I, O, P,
        A, S, D, F, G, H, J, K, L,
        Z, X, C, V, B, N, M,
        Mouse_Move,
        Mouse_Left_Down,
        Mouse_Left_Up,
        Mouse_Right_Down,
        Mouse_Right_Up
    };
#ifndef _WIN32
    inline int getch() {
        return getchar();
    }
#endif
    inline int g_mouseX = 0;
    inline int g_mouseY = 0;
    inline int get_MouseX() { return g_mouseX; }
    inline int get_MouseY() { return g_mouseY; }
    inline Input_Type Get_Input() {
#if defined(_WIN32)
        static bool consoleInitialized = false;
        static DWORD originalMode = 0;
        HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
        if (!consoleInitialized) {
            if (GetConsoleMode(hIn, &originalMode)) {
                DWORD mode = originalMode;
                mode &= ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
                mode |= ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
                SetConsoleMode(hIn, mode);
            }
            consoleInitialized = true;
        }
        INPUT_RECORD ir;
        DWORD read = 0;
        static DWORD prevMouseButtons = 0;
        while (true) {
            if (!ReadConsoleInput(hIn, &ir, 1, &read)) {
                int ch = _getch();
                if (ch == 9) return Input_Type::Tab;
                if (ch == 13) return Input_Type::Enter;
                if (ch == ' ') return Input_Type::Space;
                if (ch == 27) return Input_Type::Escape;
                if (ch == 0 || ch == 224) {
                    int ex = _getch();
                    if (ex >= 59 && ex <= 68) return static_cast<Input_Type>((int)Input_Type::F1 + (ex - 59));
                    if (ex == 133) return Input_Type::F11;
                    if (ex == 134) return Input_Type::F12;
                }
                if (ch >= '0' && ch <= '9') return static_cast<Input_Type>(static_cast<int>(Input_Type::Num_0) + (ch - '0'));
                switch (ch) {
                case 'q': case 'Q': return Input_Type::Q;
                case 'w': case 'W': return Input_Type::W;
                case 'e': case 'E': return Input_Type::E;
                case 'r': case 'R': return Input_Type::R;
                case 't': case 'T': return Input_Type::T;
                case 'y': case 'Y': return Input_Type::Y;
                case 'u': case 'U': return Input_Type::U;
                case 'i': case 'I': return Input_Type::I;
                case 'o': case 'O': return Input_Type::O;
                case 'p': case 'P': return Input_Type::P;
                case 'a': case 'A': return Input_Type::A;
                case 's': case 'S': return Input_Type::S;
                case 'd': case 'D': return Input_Type::D;
                case 'f': case 'F': return Input_Type::F;
                case 'g': case 'G': return Input_Type::G;
                case 'h': case 'H': return Input_Type::H;
                case 'j': case 'J': return Input_Type::J;
                case 'k': case 'K': return Input_Type::K;
                case 'l': case 'L': return Input_Type::L;
                case 'z': case 'Z': return Input_Type::Z;
                case 'x': case 'X': return Input_Type::X;
                case 'c': case 'C': return Input_Type::C;
                case 'v': case 'V': return Input_Type::V;
                case 'b': case 'B': return Input_Type::B;
                case 'n': case 'N': return Input_Type::N;
                case 'm': case 'M': return Input_Type::M;
                default: return Input_Type::None;
                }
            }
            if (ir.EventType == KEY_EVENT) {
                auto& ke = ir.Event.KeyEvent;
                if (!ke.bKeyDown) continue;
                if (ke.wVirtualKeyCode == VK_TAB) return Input_Type::Tab;
                if (ke.wVirtualKeyCode >= VK_F1 && ke.wVirtualKeyCode <= VK_F12) {
                    return static_cast<Input_Type>((int)Input_Type::F1 + (ke.wVirtualKeyCode - VK_F1));
                }
                char ch = (char)ke.uChar.AsciiChar;
                if (ch == 13) return Input_Type::Enter;
                if (ch == ' ') return Input_Type::Space;
                if (ch == 27) return Input_Type::Escape;
                switch (ke.wVirtualKeyCode) {
                case VK_UP:    return Input_Type::Arrow_Up;
                case VK_DOWN:  return Input_Type::Arrow_Down;
                case VK_LEFT:  return Input_Type::Arrow_Left;
                case VK_RIGHT: return Input_Type::Arrow_Right;
                }
                if (ch >= '0' && ch <= '9') return static_cast<Input_Type>(static_cast<int>(Input_Type::Num_0) + (ch - '0'));
                switch (ch) {
                case 'q': case 'Q': return Input_Type::Q;
                case 'w': case 'W': return Input_Type::W;
                case 'e': case 'E': return Input_Type::E;
                case 'r': case 'R': return Input_Type::R;
                case 't': case 'T': return Input_Type::T;
                case 'y': case 'Y': return Input_Type::Y;
                case 'u': case 'U': return Input_Type::U;
                case 'i': case 'I': return Input_Type::I;
                case 'o': case 'O': return Input_Type::O;
                case 'p': case 'P': return Input_Type::P;
                case 'a': case 'A': return Input_Type::A;
                case 's': case 'S': return Input_Type::S;
                case 'd': case 'D': return Input_Type::D;
                case 'f': case 'F': return Input_Type::F;
                case 'g': case 'G': return Input_Type::G;
                case 'h': case 'H': return Input_Type::H;
                case 'j': case 'J': return Input_Type::J;
                case 'k': case 'K': return Input_Type::K;
                case 'l': case 'L': return Input_Type::L;
                case 'z': case 'Z': return Input_Type::Z;
                case 'x': case 'X': return Input_Type::X;
                case 'c': case 'C': return Input_Type::C;
                case 'v': case 'V': return Input_Type::V;
                case 'b': case 'B': return Input_Type::B;
                case 'n': case 'N': return Input_Type::N;
                case 'm': case 'M': return Input_Type::M;
                default: return Input_Type::None;
                }
            }
            else if (ir.EventType == MOUSE_EVENT) {
                auto& me = ir.Event.MouseEvent;
                g_mouseX = me.dwMousePosition.X;
                g_mouseY = me.dwMousePosition.Y;
                DWORD btns = me.dwButtonState;
                if (me.dwEventFlags == MOUSE_MOVED) {
                    prevMouseButtons = btns;
                    return Input_Type::Mouse_Move;
                }
                DWORD leftMask = FROM_LEFT_1ST_BUTTON_PRESSED;
                bool prevLeft = (prevMouseButtons & leftMask) != 0;
                bool curLeft = (btns & leftMask) != 0;
                prevMouseButtons = btns;
                if (!prevLeft && curLeft) return Input_Type::Mouse_Left_Down;
                if (prevLeft && !curLeft) return Input_Type::Mouse_Left_Up;
                return Input_Type::Mouse_Move;
            }
        }
#else
        static bool consoleInitialized = false;
        if (!consoleInitialized) {
            tcgetattr(STDIN_FILENO, &originalTermios);
            struct termios raw = originalTermios;
            raw.c_lflag &= ~(ICANON | ECHO);
            raw.c_iflag &= ~(IXON | ICRNL);
            tcsetattr(STDIN_FILENO, TCSANOW, &raw);
            std::cout << "\033[?1002h\033[?1006h" << std::flush;
            consoleInitialized = true;
        }
        int ch = getch();
        if (ch == 9) return Input_Type::Tab;
        if (ch == 27) {
            int n1 = getch();
            if (n1 == '[') {
                int n2 = getch();
                if (n2 == '<') {
                    int cb = 0, cx = 0, cy = 0;
                    int c;
                    while ((c = getch()) >= '0' && c <= '9') { cb = cb * 10 + (c - '0'); }
                    if (c == ';') {
                        while ((c = getch()) >= '0' && c <= '9') { cx = cx * 10 + (c - '0'); }
                        if (c == ';') {
                            while ((c = getch()) >= '0' && c <= '9') { cy = cy * 10 + (c - '0'); }
                            if (c == 'M' || c == 'm') {
                                g_mouseX = cx > 0 ? cx - 1 : 0;
                                g_mouseY = cy > 0 ? cy - 1 : 0;
                                bool isMotion = (cb & 32) != 0;
                                int button = cb & 0b11;
                                if (isMotion) return Input_Type::Mouse_Move;
                                if (c == 'M') {
                                    if (button == 0) return Input_Type::Mouse_Left_Down;
                                    return Input_Type::Mouse_Move;
                                }
                                else {
                                    if (button == 0 || button == 3) return Input_Type::Mouse_Left_Up;
                                    return Input_Type::Mouse_Move;
                                }
                            }
                        }
                    }
                }
                else {
                    if (n2 == 'A') return Input_Type::Arrow_Up;
                    if (n2 == 'B') return Input_Type::Arrow_Down;
                    if (n2 == 'C') return Input_Type::Arrow_Right;
                    if (n2 == 'D') return Input_Type::Arrow_Left;
                    if (n2 >= '0' && n2 <= '9') {
                        int num = n2 - '0';
                        int next;
                        while ((next = getch()) >= '0' && next <= '9') num = num * 10 + (next - '0');
                        if (num == 15) return Input_Type::F5;
                        if (num == 17) return Input_Type::F6;
                        if (num == 18) return Input_Type::F7;
                        if (num == 19) return Input_Type::F8;
                        if (num == 20) return Input_Type::F9;
                        if (num == 21) return Input_Type::F10;
                        if (num == 23) return Input_Type::F11;
                        if (num == 24) return Input_Type::F12;
                    }
                }
            }
            else if (n1 == 'O') {
                int n2 = getch();
                if (n2 == 'P') return Input_Type::F1;
                if (n2 == 'Q') return Input_Type::F2;
                if (n2 == 'R') return Input_Type::F3;
                if (n2 == 'S') return Input_Type::F4;
            }
            return Input_Type::Escape;
        }
        if (ch == 10 || ch == 13) return Input_Type::Enter;
        if (ch == ' ') return Input_Type::Space;
        if (ch >= '0' && ch <= '9') return static_cast<Input_Type>(static_cast<int>(Input_Type::Num_0) + (ch - '0'));
        switch (ch) {
        case 'q': case 'Q': return Input_Type::Q;
        case 'w': case 'W': return Input_Type::W;
        case 'e': case 'E': return Input_Type::E;
        case 'r': case 'R': return Input_Type::R;
        case 't': case 'T': return Input_Type::T;
        case 'y': case 'Y': return Input_Type::Y;
        case 'u': case 'U': return Input_Type::U;
        case 'i': case 'I': return Input_Type::I;
        case 'o': case 'O': return Input_Type::O;
        case 'p': case 'P': return Input_Type::P;
        case 'a': case 'A': return Input_Type::A;
        case 's': case 'S': return Input_Type::S;
        case 'd': case 'D': return Input_Type::D;
        case 'f': case 'F': return Input_Type::F;
        case 'g': case 'G': return Input_Type::G;
        case 'h': case 'H': return Input_Type::H;
        case 'j': case 'J': return Input_Type::J;
        case 'k': case 'K': return Input_Type::K;
        case 'l': case 'L': return Input_Type::L;
        case 'z': case 'Z': return Input_Type::Z;
        case 'x': case 'X': return Input_Type::X;
        case 'c': case 'C': return Input_Type::C;
        case 'v': case 'V': return Input_Type::V;
        case 'b': case 'B': return Input_Type::B;
        case 'n': case 'N': return Input_Type::N;
        case 'm': case 'M': return Input_Type::M;
        }
        return Input_Type::None;
#endif
    }
    inline Input_Type Get_Input_Nonblocking() {
#if defined(_WIN32)
        static bool consoleInitialized = false;
        static DWORD originalMode = 0;
        HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
        if (!consoleInitialized) {
            if (GetConsoleMode(hIn, &originalMode)) {
                DWORD mode = originalMode;
                mode &= ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
                mode |= ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS;
                SetConsoleMode(hIn, mode);
            }
            consoleInitialized = true;
        }
        DWORD eventsWaiting = 0;
        if (!GetNumberOfConsoleInputEvents(hIn, &eventsWaiting) || eventsWaiting == 0) {
            return Input_Type::None;
        }
        INPUT_RECORD ir;
        DWORD read = 0;
        static DWORD prevMouseButtons = 0;
        if (!ReadConsoleInput(hIn, &ir, 1, &read)) return Input_Type::None;

        if (ir.EventType == KEY_EVENT) {
            auto& ke = ir.Event.KeyEvent;
            if (!ke.bKeyDown) return Input_Type::None;

            if (ke.wVirtualKeyCode == VK_TAB) return Input_Type::Tab;
            if (ke.wVirtualKeyCode >= VK_F1 && ke.wVirtualKeyCode <= VK_F12) {
                return static_cast<Input_Type>((int)Input_Type::F1 + (ke.wVirtualKeyCode - VK_F1));
            }
            char ch = (char)ke.uChar.AsciiChar;
            if (ch == 13) return Input_Type::Enter;
            if (ch == ' ') return Input_Type::Space;
            if (ch == 27) return Input_Type::Escape;
            switch (ke.wVirtualKeyCode) {
            case VK_UP:    return Input_Type::Arrow_Up;
            case VK_DOWN:  return Input_Type::Arrow_Down;
            case VK_LEFT:  return Input_Type::Arrow_Left;
            case VK_RIGHT: return Input_Type::Arrow_Right;
            }
            if (ch >= '0' && ch <= '9') return static_cast<Input_Type>(static_cast<int>(Input_Type::Num_0) + (ch - '0'));
            switch (tolower(ch)) {
            case 'q': case 'Q': return Input_Type::Q;
            case 'w': case 'W': return Input_Type::W;
            case 'e': case 'E': return Input_Type::E;
            case 'r': case 'R': return Input_Type::R;
            case 't': case 'T': return Input_Type::T;
            case 'y': case 'Y': return Input_Type::Y;
            case 'u': case 'U': return Input_Type::U;
            case 'i': case 'I': return Input_Type::I;
            case 'o': case 'O': return Input_Type::O;
            case 'p': case 'P': return Input_Type::P;
            case 'a': case 'A': return Input_Type::A;
            case 's': case 'S': return Input_Type::S;
            case 'd': case 'D': return Input_Type::D;
            case 'f': case 'F': return Input_Type::F;
            case 'g': case 'G': return Input_Type::G;
            case 'h': case 'H': return Input_Type::H;
            case 'j': case 'J': return Input_Type::J;
            case 'k': case 'K': return Input_Type::K;
            case 'l': case 'L': return Input_Type::L;
            case 'z': case 'Z': return Input_Type::Z;
            case 'x': case 'X': return Input_Type::X;
            case 'c': case 'C': return Input_Type::C;
            case 'v': case 'V': return Input_Type::V;
            case 'b': case 'B': return Input_Type::B;
            case 'n': case 'N': return Input_Type::N;
            case 'm': case 'M': return Input_Type::M;
            }
        }
        else if (ir.EventType == MOUSE_EVENT) {
            auto& me = ir.Event.MouseEvent;
            g_mouseX = me.dwMousePosition.X;
            g_mouseY = me.dwMousePosition.Y;
            DWORD btns = me.dwButtonState;
            if (me.dwEventFlags == MOUSE_MOVED) {
                prevMouseButtons = btns;
                return Input_Type::Mouse_Move;
            }
            DWORD leftMask = FROM_LEFT_1ST_BUTTON_PRESSED;
            bool prevLeft = (prevMouseButtons & leftMask) != 0;
            bool curLeft = (btns & leftMask) != 0;
            prevMouseButtons = btns;
            if (!prevLeft && curLeft) return Input_Type::Mouse_Left_Down;
            if (prevLeft && !curLeft) return Input_Type::Mouse_Left_Up;

            return Input_Type::Mouse_Move;
        }
        return Input_Type::None;

#else
        static bool consoleInitialized = false;
        if (!consoleInitialized) {
            tcgetattr(STDIN_FILENO, &originalTermios);
            struct termios raw = originalTermios;
            raw.c_lflag &= ~(ICANON | ECHO);
            raw.c_iflag &= ~(IXON | ICRNL);
            tcsetattr(STDIN_FILENO, TCSANOW, &raw);
            std::cout << "\033[?1002h\033[?1006h" << std::flush;
            consoleInitialized = true;
        }
        struct pollfd pfd = { STDIN_FILENO, POLLIN, 0 };
        if (poll(&pfd, 1, 0) <= 0) {
            return Input_Type::None;
        }
        int ch = getch();
        if (ch == 9) return Input_Type::Tab;
        if (ch == 27) {
            struct pollfd seq_pfd = { STDIN_FILENO, POLLIN, 0 };
            if (poll(&seq_pfd, 1, 50) <= 0) return Input_Type::Escape;
            int n1 = getch();
            if (n1 == '[') {
                int n2 = getch();
                if (n2 == '<') {
                    int cb = 0, cx = 0, cy = 0;
                    int c;
                    while ((c = getch()) >= '0' && c <= '9') { cb = cb * 10 + (c - '0'); }
                    if (c == ';') {
                        while ((c = getch()) >= '0' && c <= '9') { cx = cx * 10 + (c - '0'); }
                        if (c == ';') {
                            while ((c = getch()) >= '0' && c <= '9') { cy = cy * 10 + (c - '0'); }
                            if (c == 'M' || c == 'm') {
                                g_mouseX = cx > 0 ? cx - 1 : 0;
                                g_mouseY = cy > 0 ? cy - 1 : 0;
                                if ((cb & 32) != 0) return Input_Type::Mouse_Move;
                                int button = cb & 0b11;
                                if (c == 'M') return (button == 0) ? Input_Type::Mouse_Left_Down : Input_Type::Mouse_Move;
                                return (button == 0 || button == 3) ? Input_Type::Mouse_Left_Up : Input_Type::Mouse_Move;
                            }
                        }
                    }
                }
                else {
                    if (n2 == 'A') return Input_Type::Arrow_Up;
                    if (n2 == 'B') return Input_Type::Arrow_Down;
                    if (n2 == 'C') return Input_Type::Arrow_Right;
                    if (n2 == 'D') return Input_Type::Arrow_Left;
                    if (n2 >= '0' && n2 <= '9') {
                        int num = n2 - '0';
                        int next;
                        while ((next = getch()) >= '0' && next <= '9') num = num * 10 + (next - '0');
                        switch (num) {
                        case 15: return Input_Type::F5; case 17: return Input_Type::F6;
                        case 18: return Input_Type::F7; case 19: return Input_Type::F8;
                        case 20: return Input_Type::F9; case 21: return Input_Type::F10;
                        case 23: return Input_Type::F11; case 24: return Input_Type::F12;
                        }
                    }
                }
            }
            else if (n1 == 'O') {
                int n2 = getch();
                if (n2 == 'P') return Input_Type::F1;
                if (n2 == 'Q') return Input_Type::F2;
                if (n2 == 'R') return Input_Type::F3;
                if (n2 == 'S') return Input_Type::F4;
            }
            return Input_Type::Escape;
        }
        if (ch == 10 || ch == 13) return Input_Type::Enter;
        if (ch == ' ') return Input_Type::Space;
        if (ch >= '0' && ch <= '9') return static_cast<Input_Type>(static_cast<int>(Input_Type::Num_0) + (ch - '0'));
        switch (tolower(ch)) {
        case 'q': case 'Q': return Input_Type::Q;
        case 'w': case 'W': return Input_Type::W;
        case 'e': case 'E': return Input_Type::E;
        case 'r': case 'R': return Input_Type::R;
        case 't': case 'T': return Input_Type::T;
        case 'y': case 'Y': return Input_Type::Y;
        case 'u': case 'U': return Input_Type::U;
        case 'i': case 'I': return Input_Type::I;
        case 'o': case 'O': return Input_Type::O;
        case 'p': case 'P': return Input_Type::P;
        case 'a': case 'A': return Input_Type::A;
        case 's': case 'S': return Input_Type::S;
        case 'd': case 'D': return Input_Type::D;
        case 'f': case 'F': return Input_Type::F;
        case 'g': case 'G': return Input_Type::G;
        case 'h': case 'H': return Input_Type::H;
        case 'j': case 'J': return Input_Type::J;
        case 'k': case 'K': return Input_Type::K;
        case 'l': case 'L': return Input_Type::L;
        case 'z': case 'Z': return Input_Type::Z;
        case 'x': case 'X': return Input_Type::X;
        case 'c': case 'C': return Input_Type::C;
        case 'v': case 'V': return Input_Type::V;
        case 'b': case 'B': return Input_Type::B;
        case 'n': case 'N': return Input_Type::N;
        case 'm': case 'M': return Input_Type::M;
        }
        return Input_Type::None;
#endif
    }
#if defined(_WIN32)
    inline int read_Key() {
        int ch = _getch();
        if (ch == 0 || ch == 224) {
            int ch2 = _getch();
            return 1000 + ch2;
        }
        return ch;
    }
#else
    inline int read_Key() {
        termios oldt, newt;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        int ch = getchar();
        if (ch == 27) {
            if (getchar() == '[') {
                ch = 1000 + getchar();
            }
        }
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
    }
#endif
    /*
     * Warning:
     *   - Console state is modified when using mktui::console_guard.
     *   - Avoid mixing with other terminal manipulation libraries.
     *   - Always keep console_guard in scope while running your TUI.
     */
    struct console_guard {
        console_guard() {
#if defined(_WIN32)
            HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
            if (hOut != INVALID_HANDLE_VALUE) {
                DWORD mode = 0;
                if (GetConsoleMode(hOut, &mode)) {
                    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                    SetConsoleMode(hOut, mode);
                }
            }
            SetConsoleOutputCP(CP_UTF8);
            SetConsoleCP(CP_UTF8);
#else
            tcgetattr(STDIN_FILENO, &originalTermios);
            struct termios raw = originalTermios;
            raw.c_lflag &= ~(ICANON | ECHO);
            raw.c_iflag &= ~(IXON | ICRNL);
            tcsetattr(STDIN_FILENO, TCSANOW, &raw);
#endif
            std::cout << "\033[?25l\033[?1000h\033[?1002h\033[?1006h\033[2J\033[H\033[?1049h\033[?25l";
            std::cout.flush();
            std::atexit(reset_console);
        }
        ~console_guard() {
            reset_console();
        }
    private:
        static void reset_console() {
            static bool cleaned = false;
            if (cleaned) return;
#ifndef _WIN32
            tcsetattr(STDIN_FILENO, TCSANOW, &originalTermios);
#endif
            std::cout << "\033[?25h\033[?1000l\033[?1002l\033[?1006l\033[?1049l\033[2J\033[H\033[!p" << std::flush;
            cleaned = true;
        }
    };
    namespace colors {
        namespace bit8 {
            inline const std::string reset = "\033[0m";
            inline const std::string black = "\033[30m";
            inline const std::string red = "\033[31m";
            inline const std::string green = "\033[32m";
            inline const std::string yellow = "\033[33m";
            inline const std::string blue = "\033[34m";
            inline const std::string magenta = "\033[35m";
            inline const std::string cyan = "\033[36m";
            inline const std::string white = "\033[37m";
            inline const std::string bg_black = "\033[40m";
            inline const std::string bg_red = "\033[41m";
            inline const std::string bg_green = "\033[42m";
            inline const std::string bg_yellow = "\033[43m";
            inline const std::string bg_blue = "\033[44m";
            inline const std::string bg_magenta = "\033[45m";
            inline const std::string bg_cyan = "\033[46m";
            inline const std::string bg_white = "\033[47m";
        }
        namespace bit16 {
            inline const std::string black = "\033[90m";
            inline const std::string red = "\033[91m";
            inline const std::string green = "\033[92m";
            inline const std::string yellow = "\033[93m";
            inline const std::string blue = "\033[94m";
            inline const std::string magenta = "\033[95m";
            inline const std::string cyan = "\033[96m";
            inline const std::string white = "\033[97m";
            inline const std::string bg_black = "\033[100m";
            inline const std::string bg_red = "\033[101m";
            inline const std::string bg_green = "\033[102m";
            inline const std::string bg_yellow = "\033[103m";
            inline const std::string bg_blue = "\033[104m";
            inline const std::string bg_magenta = "\033[105m";
            inline const std::string bg_cyan = "\033[106m";
            inline const std::string bg_white = "\033[107m";
        }
        namespace bit24 {
            inline std::string fg(int r, int g, int b) {
                return "\033[38;2;" + std::to_string(r) + ";" +
                    std::to_string(g) + ";" + std::to_string(b) + "m";
            }
            inline std::string bg(int r, int g, int b) {
                return "\033[48;2;" + std::to_string(r) + ";" +
                    std::to_string(g) + ";" + std::to_string(b) + "m";
            }
        }
    }
    namespace attrs {
        inline const std::string reset = "\033[0m";
        inline const std::string bold = "\033[1m";
        inline const std::string italic = "\033[3m";
        inline const std::string underline = "\033[4m";
        inline const std::string inverse = "\033[7m";
        inline const std::string hidden = "\033[8m";
        inline const std::string blink = "\033[5m";
    }
    inline void set_cursor(int x, int y) {
        std::cout << "\033[" << (y + 1) << ";" << (x + 1) << "H";
    }
    inline void set_cursor_start() {
        std::cout << "\033[H";
    }
    inline void clear_screen() {
        std::cout << "\033[2J\033[H";
    }
    inline void clear_line() {
        std::cout << "\033[2K\r";
    }
    inline void set_title(const std::string& title) {
        std::cout << "\033]0;" << title << "\007";
    }
    inline void sleep_ms(int ms) {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }
    inline void sleep_sec(int s) {
        std::this_thread::sleep_for(std::chrono::seconds(s));
    }
    inline void cursor_move_up(int n = 1) { std::cout << "\033[" << n << "A"; }
    inline void cursor_move_down(int n = 1) { std::cout << "\033[" << n << "B"; }
    inline void cursor_move_right(int n = 1) { std::cout << "\033[" << n << "C"; }
    inline void cursor_move_left(int n = 1) { std::cout << "\033[" << n << "D"; }
    inline void debug_log(const std::string& msg) {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);

        std::tm time_info{};

#ifdef _WIN32
        localtime_s(&time_info, &in_time_t);
#else
        localtime_r(&in_time_t, &time_info);
#endif
        std::cerr << "[" << std::put_time(&time_info, "%Y-%m-%d %H:%M:%S") << "] [DEBUG] " << msg << "\n" << std::flush;
    }
#ifndef _WIN32
    inline bool command_exists(const std::string& cmd) {
        std::string check = "command -v " + cmd + " > /dev/null 2>&1";
        return (system(check.c_str()) == 0);
    }
#endif
    inline void copy_to_clipboard(const std::string& text) {
#ifdef _WIN32
        if (!OpenClipboard(nullptr)) return;
        EmptyClipboard();
        size_t size = text.size() + 1;
        HGLOBAL hGlob = GlobalAlloc(GMEM_MOVEABLE, size);
        if (hGlob) {
            void* lock = GlobalLock(hGlob);
            if (lock) {
                memcpy(lock, text.c_str(), size);
                GlobalUnlock(hGlob);
                if (!SetClipboardData(CF_TEXT, hGlob)) {
                    GlobalFree(hGlob);
                }
            }
            else {
                GlobalFree(hGlob);
            }
        }
        CloseClipboard();
#else
        std::string command = "";
        if (command_exists("pbcopy")) {
            command = "pbcopy";
        }
        else if (command_exists("wl-copy")) {
            command = "wl-copy";
        }
        else if (command_exists("xclip")) {
            command = "xclip -selection clipboard";
        }
        else if (command_exists("xsel")) {
            command = "xsel --clipboard --input";
        }

        if (!command.empty()) {
            FILE* pipe = popen(command.c_str(), "w");
            if (pipe) {
                fwrite(text.c_str(), 1, text.size(), pipe);
                pclose(pipe);
            }
        }
        else {
            debug_log("Error: No supported clipboard manager found (xclip, xsel, wl-copy, pbcopy).");
        }
#endif
    }
    inline void beep(int freq, int lan) {
#ifdef _WIN32
        std::thread([=]() {
            Beep(freq, lan);
            }).detach();
#endif
    }
    /*
    this isn't usless, it allows you to do:
    ```cpp
        mktui::beep(100, 100);
    ```

    insted of:
    ```cpp
#ifdef _WIN32
    Beep(100, 100);
#endif
    ```
    */
    struct event {
        Input_Type input;
        int mouse_x;
        int mouse_y;
    };
    inline event get_event() {
        event returner;
        returner.input = Get_Input();
        returner.mouse_x = get_MouseX();
        returner.mouse_y = get_MouseY();
        return returner;
    }
    inline event get_event_nonblocking() {
        event returner;
        returner.input = Get_Input_Nonblocking();
        returner.mouse_x = get_MouseX();
        returner.mouse_y = get_MouseY();
        return returner;
    }
    inline int utf8_length(const std::string& str) {
        int length = 0;
        for (size_t i = 0; i < str.length(); ) {
            unsigned char c = str[i];
            if (c < 0x80) i += 1;
            else if ((c & 0xE0) == 0xC0) i += 2;
            else if ((c & 0xF0) == 0xE0) i += 3;
            else if ((c & 0xF8) == 0xF0) i += 4;
            else i += 1;
            length++;
        }
        return length;
    }
    inline std::string utf8_substr(const std::string& str, int max_chars) {
        if (utf8_length(str) <= max_chars) return str;
        size_t byte_count = 0;
        int char_count = 0;
        while (char_count < max_chars && byte_count < str.length()) {
            unsigned char c = str[byte_count];
            if (c < 0x80) byte_count += 1;
            else if ((c & 0xE0) == 0xC0) byte_count += 2;
            else if ((c & 0xF0) == 0xE0) byte_count += 3;
            else if ((c & 0xF8) == 0xF0) byte_count += 4;
            else byte_count += 1;
            char_count++;
        }
        return str.substr(0, byte_count);
    }
})";
}

std::string fflibstr() {
    return R"(/*
             ahh.txt 1kb
              ,_____.
              |     |
              | >:( |
              | ahh |
._________.-----------.
| FFlib.hpp           |
| Friendly(or fuckin) |
| File                |
| Library             |
|                     |
|                     |
`---------------------'
Why are files so bad my fucking god.
What the hell is a stream, and why are we using it for files?
Why is it not for just console.
Streams are so much worse for files.
ahhhhhhhhhhhhhhhhkjldasfhjksdafhuieoh lfngbm,.

F***, what? why did you censor me?
The advertizers will kill me if you continue acting like this.
Fine...
Heres my way of abstracting the horid C++ streams.

Copyright (c) 2026 https://github.com/ladsm

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <string_view>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <format>
#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#include <shlobj.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#endif

namespace fflib {
    namespace fs = std::filesystem;

    inline void debug_log(std::string_view msg) {
        const auto now = std::chrono::system_clock::now();
        const auto in_time_t = std::chrono::system_clock::to_time_t(now);
        std::tm time_info{};
#ifdef _WIN32
        localtime_s(&time_info, &in_time_t);
#else
        localtime_r(&in_time_t, &time_info);
#endif
        std::cerr << "[" << std::put_time(&time_info, "%Y-%m-%d %H:%M:%S") << "] [FFLIB] " << msg << "\n" << std::flush;
    }

    inline bool exists(const fs::path& p) {
        return fs::exists(p);
    }

    inline bool create_file(const fs::path& filepath, std::string_view content) {
        std::ofstream outFile(filepath, std::ios::out | std::ios::binary);
        if (!outFile) {
            debug_log(std::format("File could not be created. File name: {}", filepath.string()));
            return false;
        }
        outFile.write(content.data(), content.size());
        return outFile.good();
    }

    inline bool touch(const fs::path& filepath, std::string_view content) {
        return create_file(filepath, content);
    }

    inline bool delete_file(const fs::path& filepath) {
        std::error_code ec;
        bool result = fs::remove(filepath, ec);
        if (ec) {
            debug_log(std::format("Failed to delete file. File name: {} Error: {}", filepath.string(), ec.message()));
            return false;
        }
        return result;
    }

    inline bool rm(const fs::path& filepath) {
        return delete_file(filepath);
    }

    inline bool delete_dir(const fs::path& dirpath) {
        std::error_code ec;
        bool result = fs::remove(dirpath, ec);
        if (ec) {
            debug_log(std::format("Failed to delete directory. Directory: {} Error: {}", dirpath.string(), ec.message()));
            return false;
        }
        return result;
    }

    inline bool rmdir(const fs::path& dirpath) {
        return delete_dir(dirpath);
    }

    inline bool delete_dir_recursive(const fs::path& dirpath) {
        std::error_code ec;
        std::uintmax_t count = fs::remove_all(dirpath, ec);
        if (ec) {
            debug_log(std::format("Failed to delete directory. Directory: {} Error: {}", dirpath.string(), ec.message()));
            return false;
        }
        return count > 0;
    }

    inline bool rm_rf(const fs::path& dirpath) {
        return delete_dir_recursive(dirpath);
    }

    inline bool create_folder(const fs::path& folderPath) {
        if (fs::exists(folderPath)) {
            return true;
        }
        std::error_code ec;
        bool created = fs::create_directories(folderPath, ec);
        if (ec) {
            debug_log(std::format("Directory could not be made. Folder name: {} Error: {}", folderPath.string(), ec.message()));
            return false;
        }
        return created;
    }

    inline bool mkdir(const fs::path& folderPath) {
        return create_folder(folderPath);
    }

    inline bool change_directory(const fs::path& directory_path) {
        std::error_code ec;
        fs::current_path(directory_path, ec);
        if (ec) {
            debug_log(std::format("Failed to change directory. Directory name: {}. Error: {}", directory_path.string(), ec.message()));
            return false;
        }
        return true;
    }

    inline bool cd(const fs::path& directory_path) {
        return change_directory(directory_path);
    }

    inline bool cddotdot() {
        return change_directory("..");
    }

    inline std::string get_file_contents(const fs::path& filepath) {
        std::ifstream inFile(filepath, std::ios::in | std::ios::binary);
        if (!inFile) {
            debug_log(std::format("File could not be opened. File name: {}", filepath.string()));
            return "";
        }
        return std::string((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    }

    inline std::string cat(const fs::path& filepath) {
        return get_file_contents(filepath);
    }

    inline std::vector<std::string> list_dir(const fs::path& dirpath = ".") {
        std::vector<std::string> entries;
        std::error_code ec;

        if (!fs::exists(dirpath, ec) || !fs::is_directory(dirpath, ec)) {
            debug_log(std::format("Failed to list directory. Directory: {} Error: {}", dirpath.string(), ec ? ec.message() : "Not a directory"));
            return entries;
        }

        for (const auto& entry : fs::directory_iterator(dirpath, ec)) {
            if (ec) {
                debug_log(std::format("Error reading directory entry in: {} Error: {}", dirpath.string(), ec.message()));
                break;
            }
            entries.push_back(entry.path().filename().string());
        }

        return entries;
    }

    inline std::vector<std::string> ls(const fs::path& dirpath = ".") {
        return list_dir(dirpath);
    }

    inline bool append_at_point_file(const fs::path& filepath, std::string_view find, std::string_view appended_text) {
        std::string contents = get_file_contents(filepath);
        if (contents.empty() && !fs::exists(filepath)) {
            return false;
        }
        size_t pos = contents.find(find);
        if (pos == std::string::npos) {
            debug_log(std::format("Target text not found in file. File name: {}", filepath.string()));
            return false;
        }
        contents.insert(pos + find.length(), appended_text);
        std::ofstream outFile(filepath, std::ios::out | std::ios::binary);
        if (!outFile) {
            debug_log(std::format("Failed to write updates to file. File name: {}", filepath.string()));
            return false;
        }
        outFile.write(contents.data(), contents.size());
        return outFile.good();
    }

    inline bool prepend_at_point_file(const fs::path& filepath, std::string_view find, std::string_view prepended_text) {
        std::string contents = get_file_contents(filepath);
        if (contents.empty() && !fs::exists(filepath)) {
            return false;
        }
        size_t pos = contents.find(find);
        if (pos == std::string::npos) {
            debug_log(std::format("Target text not found in file. File name: {}", filepath.string()));
            return false;
        }
        contents.insert(pos, prepended_text);
        std::ofstream outFile(filepath, std::ios::out | std::ios::binary);
        if (!outFile) {
            debug_log(std::format("Failed to write updates to file. File name: {}", filepath.string()));
            return false;
        }
        outFile.write(contents.data(), contents.size());
        return outFile.good();
    }

    inline bool find_and_replace_file(const fs::path& filepath, std::string_view find, std::string_view replace_with) {
        std::string contents = get_file_contents(filepath);
        if (contents.empty() && !fs::exists(filepath)) {
            return false;
        }
        size_t pos = contents.find(find);
        if (pos == std::string::npos) {
            debug_log(std::format("Target text not found in file. File name: {}", filepath.string()));
            return false;
        }
        while (pos != std::string::npos) {
            contents.replace(pos, find.length(), replace_with);
            pos = contents.find(find, pos + replace_with.length());
        }

        std::ofstream outFile(filepath, std::ios::out | std::ios::binary);
        if (!outFile) {
            debug_log(std::format("Failed to write updates to file. File name: {}", filepath.string()));
            return false;
        }
        outFile.write(contents.data(), contents.size());
        return outFile.good();
    }

    inline bool sed(const fs::path& filepath, std::string_view find, std::string_view replace_with) {
        return find_and_replace_file(filepath, find, replace_with);
    }

    inline bool append_file(const fs::path& filepath, std::string_view content) {
        std::ofstream outFile(filepath, std::ios::out | std::ios::binary | std::ios::app);
        if (!outFile) {
            debug_log(std::format("File could not be opened for appending. File name: {}", filepath.string()));
            return false;
        }
        outFile.write(content.data(), content.size());
        return outFile.good();
    }

    inline bool prepend_file(const fs::path& filepath, std::string_view content) {
        std::string contents = get_file_contents(filepath);
        if (contents.empty() && !fs::exists(filepath)) {
            return create_file(filepath, content);
        }
        contents.insert(0, content);
        std::ofstream outFile(filepath, std::ios::out | std::ios::binary);
        if (!outFile) {
            debug_log(std::format("Failed to write prepend updates to file. File name: {}", filepath.string()));
            return false;
        }
        outFile.write(contents.data(), contents.size());
        return outFile.good();
    }

    inline bool clear_file(const std::filesystem::path& filepath) {
        std::ofstream file(filepath, std::ios::out | std::ios::trunc);
        if (!file) {
            debug_log(std::format("Could not open/clear file. File name: {}", filepath.string()));
            return false;
        }
        return file.good();
    }

    inline std::string current_path() {
        std::error_code ec;
        auto p = fs::current_path(ec);
        if (ec) {
            debug_log(std::format("Failed to get current path. Error: {}", ec.message()));
            return "";
        }
        return p.string();
    }
    inline fs::path get_appdata_dir(std::string_view app_name = "") {
        fs::path base_path;
#ifdef _WIN32
        PWSTR pszPath = nullptr;
        if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &pszPath))) {
            base_path = fs::path(pszPath);
            CoTaskMemFree(pszPath);
        }
#elif defined(__APPLE__)
        const char* home = std::getenv("HOME");
        if (!home) {
            if (auto* pw = getpwuid(getuid())) home = pw->pw_dir;
        }
        if (home) {
            base_path = fs::path(home) / "Library" / "Application Support";
        }
#else
        const char* xdg_data = std::getenv("XDG_DATA_HOME");
        if (xdg_data && *xdg_data) {
            base_path = fs::path(xdg_data);
        }
        else {
            const char* home = std::getenv("HOME");
            if (!home) {
                if (auto* pw = getpwuid(getuid())) home = pw->pw_dir;
            }
            if (home) {
                base_path = fs::path(home) / ".local" / "share";
            }
        }
#endif
        if (!app_name.empty()) {
            base_path /= app_name;
        }
        return base_path;
    }
    inline bool create_appdata_file(const fs::path& relative_path, std::string_view content) {
        fs::path full_path = get_appdata_dir() / relative_path;
        if (full_path.has_parent_path()) {
            if (!create_folder(full_path.parent_path())) {
                debug_log(std::format("Failed to ensure appdata directory structure: {}", full_path.parent_path().string()));
                return false;
            }
        }
        return create_file(full_path, content);
    }
    inline std::string read_appdata_file(const fs::path& relative_path) {
        fs::path full_path = get_appdata_dir() / relative_path;
        return get_file_contents(full_path);
    }
}

/*
hey








hey





me





over here











put me in your precompiled header so your whatever compiles faster.
ie CMake: target_precompile_headers(${PROJECT_NAME} PRIVATE include/pch.h)

put me as an include, yeah, yeah you're following along.

then compile and then make every file that uses me to include pch.h insted of fflib.hpp
*/)";
}