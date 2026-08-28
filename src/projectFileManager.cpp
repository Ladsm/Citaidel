#include "projectFileManager.hpp"
#include "pch.h"

void fileAdd(WindowManager* wmp, std::string filename, bool header, bool both) {
	bool createdCpp = false;

	if (filename == "") {
		wmp->Alert("Filename cant be blank");
		return;
	}

	if (!fflib::exists("CMakeLists.txt")) {
		wmp->Alert("No CMakeLists.txt in directory");
		return;
	}
	if (header) {
		if (fflib::cd("include")) {
			fflib::touch(filename + ".hpp", "#pragma once\n\n");
			fflib::cddotdot();
			return;
		}
		else {
			wmp->Alert("No include folder in directory");
			return;
		}
	}

	else if (both) {
		if (fflib::cd("include")) {
			fflib::touch(filename + ".hpp", "#pragma once\n\n");
			fflib::cddotdot();
		}
		else {
			wmp->Alert("No include folder in directory");
		}

		if (fflib::cd("src")) {
			if (fflib::touch(filename + ".cpp", "#include \"" + filename + ".hpp\"\n\n")) {
				createdCpp = true;
			};
			fflib::cddotdot();
		}
		else {
			wmp->Alert("No src folder in directory");
		}

	}

	else {
		if (fflib::cd("src")) {
			if (fflib::touch(filename + ".cpp", "#include \"" + filename + ".hpp\"\n\n")) {
				createdCpp = true;
			};
			fflib::cddotdot();
		}
		else {
			wmp->Alert("No src folder in directory");
		}
	}
	if (createdCpp) {
		if (fflib::cat("CMakeLists.txt").find("#CITAIDEL_ANCHOR") != std::string::npos) {
			fflib::append_at_point_file(
				"CMakeLists.txt",
				"#CITAIDEL_ANCHOR",
				"\n    src/" + filename + ".cpp"
			);
		}
		else {
			wmp->Alert("Could not find #CITAIDEL_ANCHOR in CMakeLists.txt. Did not add as target.");
		}
	}

}

void fileRemove(WindowManager* wmp, std::string filename, bool header, bool both) {
	bool removedCpp = false;

	if (filename == "") {
		wmp->Alert("Filename cant be blank");
		return;
	}

	if (!fflib::exists("CMakeLists.txt")) {
		wmp->Alert("No CMakeLists.txt in directory");
		return;
	}

	if (header || both) {
		if (fflib::cd("include")) {
			if (!fflib::exists(filename + ".hpp")) {
				wmp->Alert("No file with that name");
				fflib::cddotdot();
				goto cpp;
			}
			fflib::rm(filename + ".hpp");
			fflib::cddotdot();
		}
		else {
			wmp->Alert("No include folder in directory");
		}
	}

	cpp:
	if (!header) {
		if (fflib::cd("src")) {
			if (!fflib::exists(filename + ".cpp")) {
				wmp->Alert("No file with that name");
				fflib::cddotdot();
				goto cmakelists;
			}
			fflib::rm(filename + ".cpp");
			fflib::cddotdot();
		}
		else {
			wmp->Alert("No src folder in directory");
		}
	}

	cmakelists:
	if (!header) {
		fflib::find_and_replace_file(
			"CMakeLists.txt",
			filename + ".cpp",
			""
		);
	}
}
