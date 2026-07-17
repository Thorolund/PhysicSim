#include "logger.h"

void log(
		std::string func_name,
		std::string type_err,
		std::string concrete,
		std::string comment) {
	std::ofstream out("log.txt");
	out << func_name << " (" << concrete << "): " << type_err << " - " << comment << std::endl;
	out.close();
}
