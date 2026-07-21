#pragma once

#include <fstream>
#include <iostream>
#include <string>

void log_message(
	std::string func_name,
	std::string type_err,
	std::string concrete = "global",
	std::string comment = "");
