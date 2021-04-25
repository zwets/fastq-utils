/* fasta-to-fastq.cpp
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

static const int DEFAULT_QUAL = 30;

static const std::string USAGE("\n"
"Usage: fasta-to-fastq [-q QUAL]\n"
"\n"
"  Read FASTA on stdin, write FASTQ to stdout.  Set phred score of all\n"
"  bases to QUAL (default 30, valid range [1,93]).\n"
"\n");

int main (int argc, char *argv[]) 
{
    int qual = DEFAULT_QUAL;

    while (*++argv) 
    {
        if (!std::strcmp("-q", *argv) && *++argv) {
            qual = std::atoi(*argv);
        }
        else {
            std::cerr << USAGE;
            return 1;
        }

        if (qual < 1 || qual > 93) {
            std::cerr << "fasta-to-fastq: invalid phred score\n";
            return 1;
        }
    }
    
    std::string line;
    long count;
    
    bool more = static_cast<bool>(getline(std::cin, line));

    while (more)
    {
        if (line.empty() || line[0] != '>') 
        {
            throw std::runtime_error("expected FASTA header");
        }

        line[0] = '@';
        std::cout << line << std::endl;

        count = 0;
        while ((more = static_cast<bool>(getline(std::cin, line))) && !line.empty() && line[0] != '>')
        {
            count += line.length();
            std::cout << line;
        }

        std::cout << std::endl << '+' << std::endl;
        while (count--)
        {
            std::cout << static_cast<char>((char)33 + qual);
        }
        std::cout << std::endl;
    }

    return 0;
}

// vim: sts=4:sw=4:et:si:ai
