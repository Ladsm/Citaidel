#pragma once
#include <string>

class WindowManager;

void gitInit(WindowManager* wmp);
void gitClone(WindowManager* wmp, const std::string& repoUrl);
void gitCommitAll(WindowManager* wmp, const std::string& message);
void gitPushToOrigin(WindowManager* wmp, const std::string& branch = "main");
void gitFetch(WindowManager* wmp, const std::string& remote = "origin");
void gitPull(WindowManager* wmp, const std::string& remote = "origin", const std::string& branch = "main");