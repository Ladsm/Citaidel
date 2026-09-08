#include "headerLibrariesAdd.hpp"
#include "textBlobs.hpp"
#include "pch.h"

void createMktui(WindowManager* wmp) {
	if (!fflib::exists("CMakeLists.txt")) {
		wmp->Alert("No CMakeLists.txt in directory");
		return;
	}
	if(!fflib::exists("include")) {
		wmp->Alert("No include directory");
		return;
	}
	fflib::cd("include");
	fflib::touch("mktui.h", mktui());
	fflib::cddotdot();
}


void createFflib(WindowManager* wmp) {
	if (!fflib::exists("CMakeLists.txt")) {
		wmp->Alert("No CMakeLists.txt in directory");
		return;
	}
	if(!fflib::exists("include")) {
		wmp->Alert("No include directory");
		return;
	}
	fflib::cd("include");
	fflib::touch("fflib.hpp", fflibstr());
	fflib::cddotdot();
}