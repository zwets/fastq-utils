#include <stdexcept>
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

static const int DEFAULT_QUAL = 30;

static const std::string USAGE("\n"
"Usage: fastq-stats [-q QUAL]\n"
"\n"
"  Count number of reads, number of bases, bases >= QUAL (default 30),\n"
"  percent GC, and the number of N bases per 100,000.\n"
"\n"
"  Reads FASTQ on stdin, writes tab-separated stats to stdout.\n"
"\n");

static char BINS[256] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

static void fill_bins() {
    BINS['g'] = BINS['G'] = BINS['c'] = BINS['C'] = 1;
    BINS['a'] = BINS['A'] = BINS['t'] = BINS['T'] = 2;
    BINS['N'] = BINS['n'] = 3;
}

int main (int argc, char *argv[]) 
{
    int qual = DEFAULT_QUAL;

    fill_bins();

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
            switch (BINS[*b++]) {
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

    double n_100k = static_cast<int>(0.5 + 100000.0 * static_cast<double>(n_n) / static_cast<double>(n_bases));

    std::cout << 
        "n_reads\t" << n_reads << std::endl <<
        "n_bases\t" << n_bases << std::endl <<
        "pct_q" << (qual-33) << '\t' << pct_qhigh << std::endl <<
        "pct_gc\t" << pct_gc << std::endl <<
        "n_100k\t" << n_100k << std::endl;
    //std::cout << n_reads << '\t' << n_bases << '\t' << pct_qhigh << '\t' << pct_gc << '\t' << n_100k << std::endl;

    return 0;
}

// vim: sts=4:sw=4:et:si:ai
