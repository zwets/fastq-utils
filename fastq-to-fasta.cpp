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
