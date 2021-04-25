/* fastq-mask.cpp
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
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

static const int DEFAULT_QUAL = 20;
static const int DEFAULT_REPL = 'N';

static const std::string USAGE("\n"
"Usage: fastq-mask [-q QUAL] [-r [CHAR]]\n"
"\n"
"  Turn bases below threshold QUAL (phred score, default 20) to lowercase,\n"
"  and those at QUAL or above to uppercase.  Alternatively, with option -r\n"
"  replace inferior bases with CHAR (default 'N').\n"
"\n"
"  Reads FASTQ on stdin, writes FASTQ to stdout.\n"
"\n");

static int repl = DEFAULT_REPL;
static bool flip = true;

void write_masking (const std::string& bs, const std::string& qs, char min_phred)
{
    std::string::const_iterator p = bs.begin();
    std::string::const_iterator p1 = bs.end();
    std::string::const_iterator q = qs.begin();
    while (p != p1) {
        if (flip) {
            char c = *p++;
            std::cout << static_cast<char>(*q++ < min_phred ? tolower(c) : toupper(c));
        }
        else {
            std::cout << static_cast<char>(*q++ < min_phred ? repl : toupper(*p));
            ++p;
        }
    }
    std::cout << std::endl;
}

int main (int argc, char *argv[]) 
{
    int qual = DEFAULT_QUAL;

    while (*++argv) 
    {
        if (!std::strcmp("-q", *argv) && *++argv) {
            qual = std::atoi(*argv);
        }
        else if (!strcmp("-r", *argv)) {
            flip = false;
            if (*(argv+1) && **(argv+1) != '-') {
                repl = **argv;
            }
        }
        else {
            std::cerr << USAGE;
            return 1;
        }

        if (qual < 1 || qual > 93 || repl == 0) {
            std::cerr << USAGE;
            return 1;
        }
    }
    
    std::string l1, l2, l3, l4;

    while (getline(std::cin, l1))
    {
        if (! (getline(std::cin, l2) 
            && getline(std::cin, l3)
            && getline(std::cin, l4)))
        {
            throw std::runtime_error("no four-line stanza (fastq-unbreak first?)");
        }

        if (l3[0] != '+') 
        {
            throw std::runtime_error("no plus-line (fastq-unbreak first?)");
        }

        std::cout << l1 << std::endl; 
        write_masking(l2, l4, (char)33 + qual);
        std::cout << l3 << std::endl;
        std::cout << l4 << std::endl;
    }

    return 0;
}

// vim: sts=4:sw=4:et:si:ai
