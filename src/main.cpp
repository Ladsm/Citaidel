#include <iostream>
#include "pch.h"
#include "projectManager.hpp"
#include "projectFileManager.hpp"
#include "headerLibrariesAdd.hpp"
#include "git.hpp"

#define CITAIDEL_VERSION_MAJOR 1
#define CITAIDEL_VERSION_MINOR 2
#define CITAIDEL_VERSION_PATCH 0
#define CITAIDEL_VERSION_TEXT "1.2.0"

WindowManagerPalette wmpal = WindowManagerPalette(
    "\033[38;2;255;255;255;48;2;0;0;0m",
    "\033[38;2;255;255;255;48;2;18;18;18m",
    "\033[38;2;255;255;255;48;2;36;36;36m"
);
WindowPalette winpal = WindowPalette(
    "\033[38;2;255;255;255;48;2;37;0;100m",
    "\033[38;2;255;255;255;48;2;102;49;0m",
    "\033[38;2;255;255;255;48;2;5;5;5m",
    "\033[38;2;255;255;255;48;2;84;84;84m",
    "\033[38;2;255;255;255;48;2;15;15;15m",
    "\033[48;2;45;45;45m"
);

WindowManager wm("Citaidel", wmpal, winpal);
std::string vcpkgPath = "";

enum class projectType {
    binary,
    library
};

class aboutWindow : public Window {
    std::vector<std::string> cilogo = {
    "  ▟██ █ █████ ▟██▙ █ ██▙ ███ █    ",
    "  █       █   █  █   █ █ █   █    ",
    "  █   █   █   ████ █ █ █ ███ █    ",
    "  █   █   █   █  █ █ █ █ █   █    ",
    "  ▜██ █   █   █  █ █ ██▛ ███ ███  "
    };
    std::vector<std::string> info = {
        "C/C++ IDE built in Lad-in-the-Window.",
        "Uses CMake to build and has Git",
        "integration."
    };
public:
    aboutWindow() : Window("About", 41, 21, winpal) {
        auto& vbox = Add<VerticalContainer>(2, 2, 1);
        vbox.Add<TextBox>(cilogo);
        vbox.Add<TextBox>(info);
        auto& hbox = vbox.Add<HorizontalContainer>();
        hbox.Add<Label>(CITAIDEL_VERSION_TEXT);
        hbox.Add<Label>("From Ladsm");
        hbox.Add<Label>("31/8/2026");
        vbox.Add<Label>("https://github.com/Ladsm/Citaidel");
        vbox.Add<Button>("Close", [this]() { wm.RemoveWindow(this); });
    }
};

class projectManager : public Window {
    std::string projectName = "";
    projectType pt = projectType::binary;
    bool projectTypeBool = false;
    bool Release = false;
    Label* projectLabel = nullptr;
    Label* projectTypeLabel = nullptr;
public:
    projectManager() : Window("Project Manager", 56, 19, winpal) {
        auto& vbox = Add<VerticalContainer>(2, 2, 1);
        projectLabel = &vbox.Add<Label>("Project: ");
        auto& hbox = vbox.Add<HorizontalContainer>();
        hbox.Add<Label>("Change project name ");
        hbox.Add<TextInput>(26, &projectName);
        projectTypeLabel = &vbox.Add<Label>("");
        auto& hbox2 = vbox.Add<HorizontalContainer>();
        hbox2.Add<Toggle>("Change project type", projectTypeBool);
        hbox2.Add<Toggle>("Release", Release);
        vbox.Add<Separator>();
        auto& hbox3 = vbox.Add<HorizontalContainer>();
        hbox3.Add<Button>("Init", [this]() { init(projectTypeBool, projectName, &wm); });
        hbox3.Add<Button>("clean build folder", [this]() { clean(&wm); });
        hbox3.Add<Button>("build", [this]() { build(&wm, Release, vcpkgPath); });
        hbox3.Add<Button>("run", [this]() { run(&wm); });
        auto& hbox4 = vbox.Add<HorizontalContainer>();
        hbox4.Add<Button>("Close", [this]() { wm.RemoveWindow(this); });
        hbox4.Add<Button>("Exit Citaidel", [this]() { wm.exit(0); });
    }

    void Draw(std::ostream& buffer) override {
        std::string ProjectTypeString = "Project Type : ";
        if (projectLabel) {
            projectLabel->text = "Project: " + projectName;
        }
        if (!projectTypeBool) {
            pt = projectType::binary;
        }
        else {
            pt = projectType::library;
        }
        if (projectTypeLabel) {
            if (pt == projectType::binary) {
                projectTypeLabel->text = ProjectTypeString + "Binary";
            }
            else {
                projectTypeLabel->text = ProjectTypeString + "Library";
            }
        }
        Window::Draw(buffer);
    }
};

std::vector<std::string> loadFileLines(const std::filesystem::path& path) {
    std::string content = fflib::cat(path);
    std::vector<std::string> lines;
    std::string currentLine;

    for (char ch : content) {
        if (ch == '\n') {
            lines.push_back(currentLine);
            currentLine.clear();
        }
        else if (ch != '\r') {
            currentLine += ch;
        }
    }
    lines.push_back(currentLine);
    return lines;
}
class textEditor : public Window {
    std::vector<std::string> text = { "" };
    std::string filename = "file.cpp";
    std::filesystem::path filePath;
    LargeTextInput* textInput = nullptr;

public:
    textEditor(const std::filesystem::path& pathToOpen = "")
        : Window("Text Editor", 60, 30, winpal), filePath(pathToOpen) {

        if (!filePath.empty()) {
            filename = filePath.filename().string();
            text = loadFileLines(filePath);
        }

        auto& vbox = Add<VerticalContainer>(2, 2, 1);
        textInput = &vbox.Add<LargeTextInput>(20, 56, &text, true);
        vbox.Add<Separator>();

        auto& hbox = vbox.Add<HorizontalContainer>();
        hbox.Add<Button>("Save", [this]() {
            saveFile();
            });
        hbox.Add<TextInput>(34, &filename);
        hbox.Add<Button>("Close", [this]() { wm.RemoveWindow(this); });
    }

    void saveFile() {
        std::string fullContent;
        for (size_t i = 0; i < text.size(); ++i) {
            fullContent += text[i];
            if (i + 1 < text.size()) {
                fullContent += "\n";
            }
        }
        std::filesystem::path targetPath = filePath.empty() ? std::filesystem::path(filename) : filePath;
        fflib::create_file(targetPath, fullContent);
    }

    void Draw(std::ostream& buffer) override {
        title = "Text Editor - editing: " + filename;
        textInput->width = width - 10;
        textInput->height = height - 10;
        Window::Draw(buffer);
    }
};

class Files : public Window {
    std::filesystem::path currentPath = fflib::current_path();
    std::filesystem::path pendingPath = "";
    bool needsRefresh = false;
    Label* PathLabel = nullptr;
    VerticalContainer* fileListContainer;
public:
    void changeDirectory(const std::filesystem::path& newPath) {
        pendingPath = newPath;
        needsRefresh = true;
    }

    void fileList() {
        fileListContainer->children.clear();
        fileListContainer->x = 2;
        fileListContainer->y = 2;
        fileListContainer->spacing = 0;

        fileListContainer->Add<Button>("Force Refresh", [this]() {
            needsRefresh = true;
            });

        if (currentPath.has_parent_path()) {
            fileListContainer->Add<Button>("..", [this]() {
                changeDirectory(currentPath.parent_path());
                });
        }

        std::vector<std::string> entries = fflib::ls(currentPath);
        for (const auto& entryName : entries) {
            std::filesystem::path fullPath = currentPath / entryName;
            std::error_code ec;

            if (std::filesystem::is_directory(fullPath, ec)) {
                auto& row = fileListContainer->Add<HorizontalContainer>();
                row.Add<Button>("Delete", [this, fullPath]() {
                    fflib::rm_rf(fullPath);
                    needsRefresh = true;
                    });
                row.Add<Button>("  Open   ", [this, fullPath]() {
                    changeDirectory(fullPath);
                    });

                row.Add<Label>("Directory  " + entryName);
            }
            else {
                auto& row = fileListContainer->Add<HorizontalContainer>();
                row.Add<Button>("Delete", [this, fullPath]() {
                    fflib::rm(fullPath);
                    needsRefresh = true;
                    });
                row.Add<Button>("edit file", [fullPath]() {
                    auto editor = mksharedWindow<textEditor>(fullPath);
                    wm.AddWindow(editor);
                    });
                row.Add<Label>("file       " + entryName);
            }
        }
    }

    Files() : Window("Files", 75, 30, winpal) {
        auto& vbox = Add<VerticalContainer>(2, 2, 1);
        PathLabel = &vbox.Add<Label>("");
        vbox.Add<Separator>();
        fileListContainer = &vbox.Add<VerticalContainer>();
        fileList();
    }

    void Draw(std::ostream& buffer) override {
        if (!pendingPath.empty()) {
            currentPath = pendingPath;
            pendingPath.clear();
            fflib::cd(currentPath);
            needsRefresh = true;
        }

        if (needsRefresh) {
            fileList();
            needsRefresh = false;
        }

        if (PathLabel) {
            PathLabel->text = "Files of: " + currentPath.string();
        }

        Window::Draw(buffer);
    }
};

class projectFileManager : public Window {
    std::string filename = "";

    bool isHeader = false;
    bool isBoth = false;
public:
    projectFileManager() : Window("Project File Manager", 50, 19, winpal) {
        auto& vbox = Add<VerticalContainer>(2, 2, 1);
        auto& hbox = vbox.Add<HorizontalContainer>();
        hbox.Add<Label>("Filename: ");
        hbox.Add<TextInput>(20, &filename);
        vbox.Add<Label>("(Don't add file extension)");
        auto& hbox2 = vbox.Add<HorizontalContainer>();
        hbox2.Add<Toggle>("is header", isHeader);
        hbox2.Add<Toggle>("is header and cpp", isBoth);
        vbox.Add<Separator>();
        vbox.Add<Button>("Add", [this]() {
            fileAdd(&wm, filename, isHeader, isBoth);
            });
        vbox.Add<Separator>();
        vbox.Add<Button>("Remove", [this]() {
            fileRemove(&wm, filename, isHeader, isBoth);
            });
    }
    void Draw(std::ostream& buffer) override {
        Window::Draw(buffer);
    }
};

class gitWindow : public Window {
    std::vector<std::string> text = { "" };
    std::string cloneUrl = "https://github.com/";
    LargeTextInput* textInput = nullptr;
    VerticalContainer* VCGit = nullptr;
    bool lastGitState = false;

    bool inGitDirectory() {
        return fflib::exists(".git");
    }

    void VCGitSet() {
        textInput = nullptr;
        VCGit->children.clear();

        if (!inGitDirectory()) {
            VCGit->Add<Button>("Initialize git repository", [this]() {
                gitInit(&wm);
                });

            auto& cloneRow = VCGit->Add<HorizontalContainer>();
            cloneRow.Add<Label>("URL: ");
            cloneRow.Add<TextInput>(60, &cloneUrl);
            VCGit->Add<Button>("Clone", [this]() {
                gitClone(&wm, cloneUrl);
                });
        }
        else {
            VCGit->Add<Label>("Commit Message");
            textInput = &VCGit->Add<LargeTextInput>(6, 40, &text, false);
            auto& actionRow = VCGit->Add<HorizontalContainer>();
            actionRow.Add<Button>("Commit All", [this]() {
                std::string fullMsg;
                for (size_t i = 0; i < text.size(); ++i) {
                    fullMsg += text[i] + (i + 1 < text.size() ? "\n" : "");
                }
                gitCommitAll(&wm, fullMsg);
                });

            actionRow.Add<Button>("Fetch", [this]() {
                gitFetch(&wm, "origin");
                });

            actionRow.Add<Button>("Pull", [this]() {
                gitPull(&wm, "origin", "main");
                });

            actionRow.Add<Button>("Push to Origin", [this]() {
                gitPushToOrigin(&wm, "main");
                });
        }
    }

public:
    gitWindow() : Window("Git Manager", 65, 10, winpal) {
        auto& vbox = Add<VerticalContainer>(2, 2, 1);
        VCGit = &vbox.Add<VerticalContainer>();
        VCGit->spacing = 1;

        lastGitState = inGitDirectory();
        VCGitSet();
    }

    void Draw(std::ostream& buffer) override {
        bool currentState = inGitDirectory();

        if (currentState != lastGitState) {
            if (lastGitState == true) {
                startHeight = 10;
                height = 10;
            }
            lastGitState = currentState;
            VCGitSet();
        }

        if (inGitDirectory()) {
            startHeight = 15;
            if (height < 20) {
                height = 15;
            }
        }

        if (textInput) {
            textInput->width = width - 10;
            textInput->height = height - 10;
        }

        Window::Draw(buffer);
    }
};

class ShellWindow : public Window {
public:
    ShellWindow() : Window("Terminal", 80, 24, winpal) {}
    static std::shared_ptr<ShellWindow> Create() {
        auto win = std::make_shared<ShellWindow>();
        win->Add<ShellWidget>(1, 1, win);
        return win;
    }
};

class headerLibrariesWindow : public Window {
    std::vector<std::string> text = {
        "This window is for large header only files.",
        "The only files right now are:",
        "mktui.h - TUI builder",
        "fflib - simple file library",
    };
public:
    headerLibrariesWindow() : Window("Header Libraries Add", 50, 14, winpal) {
        auto& vbox = Add<VerticalContainer>(2, 2, 1);
        vbox.Add<TextBox>(text);
        vbox.Add<Separator>();
        auto& hbox = vbox.Add<HorizontalContainer>();
        hbox.Add<Button>("mktui.h", [this](){
        		createMktui(&wm);
        });
        hbox.Add<Button>("fflib.hpp", [this](){
        		createFflib(&wm);
        });
    }
};

class CMakeLibraryAdd : public Window {
    std::vector<std::string> text = {
        "Very simple library adder, just adds",
        "find_package(name REQUIRED) and",
        "target_link_libraries(${PROJECT_NAME} PRIVATE name::name)"
    };
    std::string libraryName = "";
public:
    CMakeLibraryAdd() : Window("CMake Library Add", 61, 13, winpal) {
        auto& vbox = Add<VerticalContainer>(2, 2, 1);
        vbox.Add<TextBox>(text);
        vbox.Add<TextInput>(30, &libraryName);
        vbox.Add<Button>("Add", [this]() {
            if (!fflib::exists("CMakeLists.txt")) {
                wm.Alert("No CMakeLists.txt in directory");
                return;
            }
            if (fflib::cat("CMakeLists.txt").find("#CITAIDEL_LIBRARY_ANCHOR") != std::string::npos) {
                fflib::append_at_point_file(
                    "CMakeLists.txt",
                    "#CITAIDEL_LIBRARY_ANCHOR",
                    "\nfind_package(" + libraryName + " REQUIRED)\n"
                    "target_link_libraries(${PROJECT_NAME} PRIVATE " + libraryName + "::" + libraryName + ")\n"
                );
            }
            else {
                wm.Alert("Could not find #CITAIDEL_LIBRARY_ANCHOR in CMakeLists.txt. Did not add as target.");
            }
            });
    }
};

class VcpkgManager : public Window {
    std::string tempPath = vcpkgPath;
public:
    VcpkgManager() : Window("Vcpkg Manager", 60, 11, winpal) {
        auto& vbox = Add<VerticalContainer>(2, 2, 1);
        vbox.Add<TextInput>(55, &tempPath);
        vbox.Add<Button>("Update path", [this]() {
            vcpkgPath = tempPath;
            fflib::create_appdata_file("CITAIDEL_VCPKG_PATH.txt", vcpkgPath);
            });
        vbox.Add<Button>("Close", [this]() { 
            wm.RemoveWindow(this); 
            });
    }
};

class gitignoreManager : public Window {
    std::string gitignoreAdd = "";
public:
    gitignoreManager() : Window("gitignore Manager", 46, 9, winpal) {
        auto& vbox = Add<VerticalContainer>(2, 2, 1);
        vbox.Add<Label>("Appends text at end of gitignore");
        auto& hbox = vbox.Add<HorizontalContainer>();
        hbox.Add<TextInput>(30, &gitignoreAdd);
        hbox.Add<Button>("Append", [this]() {
            if (!fflib::exists(".gitignore")) {
                wm.Alert("Cannot find .gitignore");
                return;
            }
            else {
                fflib::append_file(".gitignore", gitignoreAdd);
            }
            });
    }
};

int main() {
    vcpkgPath = fflib::read_appdata_file("CITAIDEL_VCPKG_PATH.txt");
    if (vcpkgPath == "") {
        fflib::create_appdata_file("CITAIDEL_VCPKG_PATH.txt", "");
    }
    auto start = startmenu<StartMenuWindow>(&wm, winpal);
    start->AddItem<projectManager>("Project Manager");
    start->AddItem<projectFileManager>("Project File Manager");
    start->AddItem<gitWindow>("Git Manager");
    start->AddItem<gitignoreManager>("gitignore Manager");
    start->AddItem<Files>("Files");
    start->AddItem<CMakeLibraryAdd>("CMake Library Add");
    start->AddItem<headerLibrariesWindow>("Header Libraries Add");
    start->AddItem<VcpkgManager>("Vcpkg Manager");
    start->AddItem("Terminal", &ShellWindow::Create);
    start->AddItem<textEditor>("Text Editor");
    start->AddItem<aboutWindow>("About");
    wm.SetStartMenu(start);
    wm.AddWindow(start);
    wm.Run();
    wm.exit(1);
    return 0;
}
