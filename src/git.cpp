#include "git.hpp"
#include "pch.h"
#include <iostream>

void gitInit(WindowManager* wmp) {
    std::system("git config core.autocrlf input");
    int result = std::system("git init");
    if (result != 0) {
        wmp->Alert("Could not create git repository");
        return;
    }
    fflib::touch(".gitignore",
        "out/\n"
        "build/\n"
        ".vscode/\n"
        ".vs/\n"
        ".idea/\n"
        "*.o\n"
        "*.obj\n"
        "*.exe\n"
        "*_i.c\n"
        "*_p.c\n"
        "*_h.h\n"
        "*.ilk\n"
        "*.meta\n"
        "*.obj\n"
        "*.iobj\n"
        "*.pch\n"
        "*.pdb\n"
        "*.ipdb\n"
        "*.pgc\n"
        "*.pgd\n"
        "*.rsp\n"
        "*.sbr\n"
        "*.tlb\n"
        "*.tli\n"
        "*.tlh\n"
        "*.tmp\n"
        "*.tmp_proj\n"
        "*_wpftmp.csproj\n"
        "*.log\n"
        "*.vspscc\n"
        "*.vssscc\n"
        ".builds\n"
        "*.pidb\n"
        "*.svclog\n"
        "*.scc\n"
    );
    std::system("git add .");
    std::system("git commit -m \"Initial commit\"");
}

void gitClone(WindowManager* wmp, const std::string& repoUrl) {
    if (repoUrl.empty()) {
        wmp->Alert("Repository URL cannot be empty.");
        return;
    }

    std::string command = "git clone " + repoUrl;
    int result = std::system(command.c_str());

    if (result != 0) {
        wmp->Alert("Failed to clone repository.");
    }
}

void gitCommitAll(WindowManager* wmp, const std::string& message) {
    if (message.empty()) {
        wmp->Alert("Commit message cannot be empty.");
        return;
    }
    int addResult = std::system("git add .");
    if (addResult != 0) {
        wmp->Alert("Failed to stage files.");
        return;
    }
    std::string sanitizedMessage = message;
    size_t pos = 0;
    while ((pos = sanitizedMessage.find("\"", pos)) != std::string::npos) {
        sanitizedMessage.replace(pos, 1, "\\\"");
        pos += 2;
    }

    std::string command = "git commit -m \"" + sanitizedMessage + "\"";
    int commitResult = std::system(command.c_str());

    if (commitResult != 0) {
        wmp->Alert("Failed to commit changes (or nothing to commit).");
    }
}

void gitPushToOrigin(WindowManager* wmp, const std::string& branch) {
    std::string targetBranch = branch.empty() ? "main" : branch;
    std::string command = "git push origin " + targetBranch;
    int result = std::system(command.c_str());

    if (result != 0) {
        wmp->Alert("Failed to push to origin " + targetBranch + ".");
    }
}

void gitFetch(WindowManager* wmp, const std::string& remote) {
    std::string targetRemote = remote.empty() ? "origin" : remote;
    std::string command = "git fetch " + targetRemote;

    int result = std::system(command.c_str());
    if (result != 0) {
        wmp->Alert("Failed to fetch from " + targetRemote + ".");
    }
}

void gitPull(WindowManager* wmp, const std::string& remote, const std::string& branch) {
    std::string targetRemote = remote.empty() ? "origin" : remote;
    std::string targetBranch = branch.empty() ? "main" : branch;
    std::string command = "git pull " + targetRemote + " " + targetBranch;

    int result = std::system(command.c_str());
    if (result != 0) {
        wmp->Alert("Failed to pull from " + targetRemote + "/" + targetBranch + ".");
    }
}