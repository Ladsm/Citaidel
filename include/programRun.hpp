#pragma once
#include <string>
#include <cstdlib>

inline void programRun(const std::string& command) {
#if defined(_WIN32) || defined(_WIN64)
    std::string full_command = "start cmd /K \"" + command + "\"";
    std::system(full_command.c_str());
#else
    std::string full_command = "gnome-terminal -- bash -c \"" + command + "; exec bash\"";
    int result = std::system(full_command.c_str());

    if (result != 0) {
        full_command = "konsole --hold -e \"" + command + "\"";
        result = std::system(full_command.c_str());
    }
    else if (result != 0) {
        full_command = "xterm -hold -e \"" + command + "\"";
        std::system(full_command.c_str());
    }
    else if (result != 0) {
        full_command = "kitty --hold -e \"" + command + "\"";
        std::system(full_command.c_str());
    }
#endif
}
