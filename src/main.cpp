#include <iostream>
#include "pch.h"
#include "projectManager.hpp"

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

enum class projectType {
    binary,
    library
};

class projectManager : public Window {
    std::string projectName = "";
    projectType pt = projectType::binary;
    bool projectTypeBool = false;
    Label* projectLabel = nullptr;
    Label* projectTypeLabel = nullptr;
public:
    projectManager() : Window("Project Manager", 40, 20, winpal) {
        auto& vbox = Add<VerticalContainer>(2, 2, 1);
        projectLabel = &vbox.Add<Label>("Project: ");
        auto& hbox = vbox.Add<HorizontalContainer>();
        hbox.Add<Label>("Change project name ");
        hbox.Add<TextInput>(10, &projectName);
        projectTypeLabel = &vbox.Add<Label>("");
        vbox.Add<Toggle>("Change project type", projectTypeBool);
        vbox.Add<Separator>();
        auto& hbox2 = vbox.Add<HorizontalContainer>();
        hbox2.Add<Button>("Init", [this]() { init(projectTypeBool, projectName, &wm); });
        hbox2.Add<Button>("clean build folder", [this]() { clean(&wm); });
        auto& hbox3 = vbox.Add<HorizontalContainer>();
        hbox3.Add<Button>("build", [this]() { build(&wm); });
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

public:
    textEditor(const std::filesystem::path& pathToOpen = "")
        : Window("Text Editor", 60, 30, winpal), filePath(pathToOpen) {

        if (!filePath.empty()) {
            filename = filePath.filename().string();
            text = loadFileLines(filePath);
        }

        auto& vbox = Add<VerticalContainer>(2, 2, 1);
        vbox.Add<LargeTextInput>(20, 56, &text, true);
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
        std::filesystem::path targetPath = filePath.empty() ? filename : filePath;
        fflib::create_file(targetPath, fullContent);
    }

    void Draw(std::ostream& buffer) override {
        title = "Text Editor - editing: " + filename;
        Window::Draw(buffer);
    }
};

class Files : public Window {
    std::filesystem::path currentPath = fflib::current_path();
    std::filesystem::path pendingPath = "";
    Label* PathLabel = nullptr;
    VerticalContainer* fileListContainer;
public:
    void changeDirectory(const std::filesystem::path& newPath) {
        pendingPath = newPath;
    }

    void fileList() {
        fileListContainer->children.clear();
        fileListContainer->x = 2;
        fileListContainer->y = 2;
        fileListContainer->spacing = 0;
        fileListContainer->Add<Button>("Fource Refresh", [this]() {
            fileList();
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
                    changeDirectory(currentPath);
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
                    changeDirectory(currentPath);
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
            fileList();
        }
        if (PathLabel) {
            PathLabel->text = "Files of: " + currentPath.string();
        }

        Window::Draw(buffer);
    }
};

int main() {
    auto start = startmenu<StartMenuWindow>(&wm, winpal);
    start->AddItem<projectManager>("Project Manager");
    start->AddItem<Files>("Files");
    start->AddItem<textEditor>("Text Editor");
    wm.SetStartMenu(start);
    wm.AddWindow(start);
    wm.Run();
    wm.exit(1);
    return 0;
}
