#include "projectManager.hpp"
#include "programRun.hpp"
#include "textBlobs.hpp"
#include "pch.h"

static bool ensure_project_root(WindowManager* wmp) {
	if (fflib::exists("CMakeLists.txt")) {
		return true;
	}
	if (fflib::exists("../CMakeLists.txt")) {
		fflib::cddotdot();
		return true;
	}
	wmp->Alert("No CMakeLists.txt found in current or parent directory.");
	return false;
}

void init(bool isLibrary, std::string name, WindowManager* wmp) {
	if (name == "") {
		wmp->Alert("Project name is blank!");
		return;
	}
	if (fflib::exists("CMakeLists.txt") || fflib::exists("../CMakeLists.txt")) {
		wmp->Alert("Cant create project, if inside one already.");
		return;
	}
	fflib::mkdir(name);
	fflib::cd(name);
	std::string mainfile = R"(#include <iostream>

int main() {
    std::cout << "Hello, World!\n";
    return 0;
})";
	if (!isLibrary) {
		fflib::touch("CMakeLists.txt", make_binary(name));
		fflib::mkdir("src");
		fflib::mkdir("include");
		fflib::cd("src");
		fflib::touch("main.cpp", mainfile);
		fflib::cddotdot();
		fflib::cd("include");
		fflib::touch("pch.h", "#pragma once\n//add headers here...");
		fflib::cddotdot();
	}
	else {
		fflib::touch("CMakeLists.txt", make_library(name));
		fflib::mkdir("src");
		fflib::mkdir("include");
		fflib::mkdir("demo");
		fflib::mkdir("cmake");
		fflib::cd("cmake");
		fflib::touch(
			std::format("{}Config.cmake", name),
			make_config(name)
		);
		fflib::cddotdot();
		fflib::cd("demo");
		fflib::touch("demo.cpp", R"(#include "lib.hpp"

int main() {
	printtest();
	return 0;
})");
		fflib::cddotdot();
		fflib::cd("include");
		fflib::touch("lib.hpp", "#pragma once\nvoid printtest();");
		fflib::touch("pch.h", "#pragma once\n//add headers here...");
		fflib::cddotdot();
		fflib::cd("src");
		fflib::touch("lib.cpp", R"(#include <iostream>

void printtest() {
	std::cout << "Library working!\n";
	return;
})");
		fflib::cddotdot();
	}
	return;
}

void clean(WindowManager* wmp) {
	if (!ensure_project_root(wmp)) {
		return;
	}

	if (fflib::exists("build")) {
		fflib::rm_rf("build");
	}
	else {
		wmp->Alert("Nothing to clean.");
	}
}

void build(WindowManager* wmp) {
	if (!ensure_project_root(wmp)) {
		return;
	}

	if (!fflib::exists("build")) {
		fflib::mkdir("build");
	}
	fflib::cd("build");
#ifdef _WIN32
	std::system("cmake.exe ..");
	std::system("cmake.exe --build .");
#else
	std::system("cmake ..");
	std::system("cmake --build .");
#endif
	fflib::cddotdot();
}

void run(WindowManager* wmp) {
	if (!ensure_project_root(wmp)) {
		return;
	}

	if (!fflib::exists("build")) {
		wmp->Alert("No build directory found, try building first");
		return;
	}
	std::string full_path = fflib::current_path();
	size_t last_slash = full_path.find_last_of("/\\");
	std::string project_name = (last_slash != std::string::npos) ? full_path.substr(last_slash + 1) : "app";
	fflib::cd("build");
#ifdef _WIN32
	std::string exe_path = project_name + ".exe";
	if (fflib::exists(std::format("Debug\\{}.exe", project_name))) {
		exe_path = std::format("Debug\\{}.exe", project_name);
	}
	else if (fflib::exists(std::format("Release\\{}.exe", project_name))) {
		exe_path = std::format("Release\\{}.exe", project_name);
	}
	if (!fflib::exists(exe_path)) {
		wmp->Alert("Error: Executable not found.");
		fflib::cddotdot();
		return;
	}
	programRun(exe_path.c_str());
#else
	std::string elf_path = std::format("./{}", project_name);
	if (!fflib::exists(project_name)) {
		wmp->Alert("Error: Executable not found.");
		fflib::cddotdot();
		return;
	}
	programRun(elf_path.c_str());
#endif
	fflib::cddotdot();
}