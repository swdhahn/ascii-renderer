#include "Files.h"

std::string readFile(const char* path)
{
	std::string content;
	std::ifstream fs(path, std::ios::in);

	if (!fs.is_open()) {
		std::cerr << "Could not read file " << path << ". File does not exist." << std::endl;
		return "";
	}

	std::string line = "";
	while (!fs.eof()) {
		std::getline(fs, line);
		content.append(line + "\n");
	}

	fs.close();
	return content;
	
}

std::vector<std::string> readFileIntoArray(const char* path) {
	std::vector<std::string> content;
	std::ifstream fs(path, std::ios::in);

	if (!fs.is_open()) {
		std::cerr << "Could not read file " << path << ". File does not exist." << std::endl;
		return content;
	}

	std::string line = "";
	while (!fs.eof()) {
		std::getline(fs, line);
		content.push_back(line);
	}

	fs.close();
	return content;
}

void writeFile(const char* path, const char* text) 
{
	std::ofstream file(path);
	if (file.is_open()) 
	{
		file << text << "\n";
		file.close();
	}
	else {
		std::cout << "Unable to write to file: " << path << std::endl;
	}
}

std::vector<std::string> split(const std::string& str, const std::string& delim)
{
	std::vector<std::string> tokens;
	size_t prev = 0, pos = 0;
	do
	{
		pos = str.find(delim, prev);

		if (pos == std::string::npos) 
			pos = str.length();

		std::string token = str.substr(prev, pos - prev);

		if (!token.empty()) 
			tokens.push_back(token);

		prev = pos + delim.length();
	} while (pos < str.length() && prev < str.length());
	return tokens;
}