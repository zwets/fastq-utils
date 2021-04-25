/* fastq-unbreak.cpp
 *
 * Copyright (C) 2017  Marco van Zwetselaar <io@zwets.it>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdexcept>
#include <sstream>
#include <iostream>
#include <string>

static const std::string USAGE("\n"
"Usage: fastq-unbreak\n"
"  Reads FASTQ data from stdin.  Rectifies sequences that are\n"
"  broken across lines into proper FASTQ.  Writes to stdout.\n"
"\n");

void error_exit (const std::string& s, char c = ' ')
{
    std::stringstream ss;
    ss << s << c;
    throw std::runtime_error (ss.str());
}

int main (int argc, char *argv[])
{
    if (argc != 1) {
        std::cerr << USAGE;
        return 1;
    }

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
