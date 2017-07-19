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

    bool more = static_cast<bool>(getline(std::cin, line));

    while (more)
    {
        if (line.empty() || line[0] != '@') {
            error_exit("expected character: ", '@');
        }

        std::cout << line << std::endl;

        long n = 0;

        while ((more = static_cast<bool>(getline(std::cin, line))) && (line.empty() || line[0] != '+'))
        {
            std::cout << line;
            n += line.length();
        }

        if (!more || line.empty() || line[0] != '+') {
            error_exit("expected character: ", '+');
        }

        std::cout << std::endl << line << std::endl;

        // take care that the phred score line may start with @
        while ((more = static_cast<bool>(getline(std::cin, line))) && n != 0)
        {
            std::cout << line;
            n -= line.length();
        }

        std::cout << std::endl;

        if (n != 0) {
            throw std::runtime_error("length mismatch bases vs phreds");
        }
    }

    return 0;
}

// vim: sts=4:sw=4:et:si:ai
