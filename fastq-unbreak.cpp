#include <stdexcept>
#include <sstream>
#include <iostream>
#include <string>

void error_exit (const std::string& s, char c = ' ')
{
	std::stringstream ss;
	ss << s << c;
	throw std::runtime_error (ss.str());
}

int main (int argc, char *argv[]) 
{
	std::string line;

	if (getline(std::cin, line)) 
	{
		if (line[0] != '@') {
			error_exit("expected character: ", '@');
			//throw std::runtime_error("expected '@'");
		}

		while (line[0] == '@') {

			int n = 0;

			std::cout << line << std::endl;

			while (getline(std::cin, line) && line[0] != '+') 
			{
				std::cout << line;
				n += line.length();
			}

			if (line[0] != '+') {
				error_exit("expected character: ", '+');
			}

			std::cout << std::endl << line << std::endl;

			while (getline(std::cin, line) && line[0] != '@') 
			{
				std::cout << line;
				n -= line.length();
			}

			std::cout << std::endl;

			if (n != 0) {
				throw std::runtime_error("length mismatch bases vs phreds");
			}
		}
	}

	return 0;
}

