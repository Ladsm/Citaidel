#pragma once
#include <string>
void init(bool isLibrary, std::string name, WindowManager* wmp);
void clean(WindowManager* wmp);
void build(WindowManager* wmp, bool Rel);
void run(WindowManager* wmp);
