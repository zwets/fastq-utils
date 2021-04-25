/* fastq-to-fasta.cpp
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

static const std::string USAGE("\n"
"Usage: fastq-to-fasta\n"
"\n"
"  Reads FASTQ on stdin, extracts the sequence data, writes to stdout.\n"
"\n");

int main (int argc, char *argv[]) 
{
    if (argc != 1) {
        std::cerr << USAGE;
        return 1;
    }

    std::string l1, l2, l3, l4;

    while (getline(std::cin, l1))
    {
        if (l1[0] != '@')
        {
            throw std::runtime_error("no @header line");
        }

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

        l1[0] = '>';
        std::cout << l1 << std::endl; 
        std::cout << l2 << std::endl; 
    }

    return 0;
}

// vim: sts=4:sw=4:et:si:ai
