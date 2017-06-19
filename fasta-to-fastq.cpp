#include <stdexcept>
#include <sstream>
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
    
    bool more = getline(std::cin, line);

    while (more)
    {
        if (line.empty() || line[0] != '>') 
        {
            throw std::runtime_error("expected FASTA header");
        }

        line[0] = '@';
        std::cout << line << std::endl;

        count = 0;
        while ((more = getline(std::cin, line)) && !line.empty() && line[0] != '>')
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
