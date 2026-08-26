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

class textEditor : public Window {
    std::vector<std::string> text = { "" };
    std::string filename = "file.cpp";
public:
    textEditor()
        : Window("Text Editor", 60, 30, winpal) {
        auto& vbox = Add<VerticalContainer>(2, 2, 1);
        vbox.Add<LargeTextInput>(20, 56, &text, true);
        vbox.Add<Separator>();
        auto& hbox = vbox.Add<HorizontalContainer>();
        hbox.Add<Button>("Save", [this]() {  });
        hbox.Add<TextInput>(34, &filename);
        hbox.Add<Button>("Close", [this]() { wm.RemoveWindow(this); });
    }
    void Draw(std::ostream& buffer) override {
        title = "Text Editor - editing: " + filename;
        Window::Draw(buffer);
    }
};

int main() {
    auto start = startmenu<StartMenuWindow>(&wm, winpal);
    start->AddItem<projectManager>("Project Manager");
    start->AddItem<textEditor>("Text Editor");
    wm.SetStartMenu(start);
    wm.AddWindow(start);
    wm.Run();
    wm.exit(1);
    return 0;
}
