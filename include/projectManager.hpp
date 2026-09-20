#pragma once
#include <string>
void init(bool isLibrary, std::string name, WindowManager* wmp);
void clean(WindowManager* wmp);
void build(WindowManager* wmp, bool Rel, std::string vcpkgpath);
void run(WindowManager* wmp);
