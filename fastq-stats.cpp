/* fastq-stats.cpp
 *
 * Copyright (C) 2020  Marco van Zwetselaar <io@zwets.it>
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
"Usage: fastq-stats [-c] [-i ID] [-q QUAL]\n"
"\n"
"  Count number of reads, number of bases, bases >= QUAL (default 30),\n"
"  percent GC, per-million N.\n"
"\n"
"  Reads FASTQ on stdin, writes tab-separated stats to stdout.\n"
"  Option -c writes stats across columns rather than across rows.\n"
"  Option -i prepends a column with ID to the output.\n"
"\n");

static char BINS[256];

static void fill_bins() {
    bzero(BINS, sizeof(BINS));
    BINS['g'] = BINS['G'] = BINS['c'] = BINS['C'] = 1;
    BINS['a'] = BINS['A'] = BINS['t'] = BINS['T'] = 2;
    BINS['N'] = BINS['n'] = 3;
}

int main (int argc, char *argv[])
{
    int qual = DEFAULT_QUAL;
    bool cols = false;
    std::string id;

    fill_bins();

    while (*++argv)
    {
        if (!std::strcmp("-c", *argv)) {
            cols = true;
        }
        else if (!std::strcmp("-q", *argv)) {
            if (*++argv) { qual = std::atoi(*argv); }
            else { std::cerr << USAGE; return 1; }
        }
        else if (!std::strcmp("-i", *argv)) {
            if (*++argv) { id = *argv; }
            else { std::cerr << USAGE; return 1; }
        }
        else {
            std::cerr << USAGE;
            return 1;
        }

        if (qual < 1 || qual > 93) {
            std::cerr << USAGE;
            return 1;
        }
    }

    if (std::cin.peek() != '@')
    {
        std::cerr << (std::cin.peek() == '\x1f' ?
            "fastq-stats: error: not a fastq file (could it be gzipped?)" :
            "fastq-stats: error: not a fastq file, first character not @")
            << std::endl;
        std::exit(1);
    }

    qual = 33 + qual; // phred+33

    long n_reads = 0;
    long n_bases = 0;
    long n_qhigh = 0;

    long n_gc = 0, n_at = 0, n_n = 0;

    std::string l1, l2, l3, l4;

    while (getline(std::cin, l1))
    {
        n_reads++;

        if (! (getline(std::cin, l2)
            && getline(std::cin, l3)
            && getline(std::cin, l4)))
        {
            std::cerr << "fastq-stats: error: no four-line stanza (fastq-unbreak first?)" << std::endl;
            std::exit(1);
        }

        std::string::const_iterator b = l2.begin();
        std::string::const_iterator b1 = l2.end();

        while (b != b1) {
            switch (BINS[(*b++) & 0xFF]) {
                case 1: n_gc++; break;
                case 2: n_at++; break;
                case 3: n_n++; break;
            }
        }

        if (l3[0] != '+')
        {
            std::cerr << "fastq-stats: error: no plus-line (fastq-unbreak first?)" << std::endl;
            std::exit(1);
        }

        std::string::const_iterator p = l4.begin();
        std::string::const_iterator p1 = l4.end();

        n_bases += (p1 - p);

        while (p != p1) if (*p++ >= qual) n_qhigh++;
    }

    double frac_qhigh = static_cast<double>(n_qhigh) / static_cast<double>(n_bases);
    double pct_qhigh = static_cast<int>(0.5 + 1000.0 * frac_qhigh) / 10.0;

    double gc_at = static_cast<double>(n_gc) / static_cast<double>(n_at);
    double pct_gc = static_cast<int>(0.5 + 1000.0 * (gc_at / (1+gc_at))) / 10.0;

    int pmn_n = static_cast<int>(0.5 + 1000000.0 * static_cast<double>(n_n) / static_cast<double>(n_bases));

    if (cols) {
        if (!id.empty()) { std::cout << "id\t"; }
        std::cout << "reads\tbases\tpct_q" << (qual-33) << "\tpct_gc\tpmn_n" << std::endl;
        if (!id.empty()) { std::cout << id << '\t'; }
        std::cout << n_reads << '\t' << n_bases << '\t' << pct_qhigh << '\t' << pct_gc << '\t' << pmn_n << std::endl;
    }
    else {
        if (!id.empty()) {
            std::cout << "id\t" << id << std::endl;
        }
        std::cout <<
            "reads\t" << n_reads << std::endl <<
            "bases\t" << n_bases << std::endl <<
            "pct_q" << (qual-33) << '\t' << pct_qhigh << std::endl <<
            "pct_gc\t" << pct_gc << std::endl <<
            "pmn_n\t" << pmn_n << std::endl;
    }

    return 0;
}

// vim: sts=4:sw=4:et:si:ai
