#include <stdexcept>
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

static const int DEFAULT_QUAL = 30;

static const std::string USAGE("\n"
"Usage: fastq-stats [-q QUAL]\n"
"\n"
"  Count number of reads, number of bases, bases >= QUAL (default 30).\n"
"\n"
"  Reads FASTQ on stdin, writes tab-separated stats to stdout.\n"
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
            std::cerr << USAGE;
            return 1;
        }
    }

    qual = 33 + qual; // phred+33

    std::string l1, l2, l3, l4;

    long n_reads = 0;
    long n_bases = 0;
    long n_qhigh = 0;

    while (getline(std::cin, l1))
    {
        n_reads++;

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

        std::string::const_iterator p = l4.begin();
        std::string::const_iterator p1 = l4.end();

        n_bases += (p1 - p);

        while (p != p1) if (*p++ >= qual) n_qhigh++;
    }

    std::cout << n_reads << '\t' << n_bases << '\t' << 
        static_cast<int>(0.5 + 1000.0 * static_cast<double>(n_qhigh) / static_cast<double>(n_bases)) / 10.0 << '\n';

    return 0;
}

// vim: sts=4:sw=4:et:si:ai
