#include <stdexcept>
#include <sstream>
#include <iostream>
#include <string>

static const std::string USAGE("\n"
"Usage: fastq-revcomp\n"
"\n"
"  Reads FASTQ on stdin, writes its reverse complement on stdout.\n"
"\n");

void error_exit (const std::string& s, char c = ' ')
{
    std::stringstream ss;
    ss << s << c;
    throw std::runtime_error (ss.str());
}

// static const char NUCL_ALPHABET[] = "AaCcGgTtNnKkMmSsWwYyRrBbVvDdHh";
// static const char COMP_ALPHABET[] = "TtGgCcAaNnMmKkWwSsRrYyVvBbHhDd";
static char REVERSE_TABLE[256] = {
  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,
  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,
  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,
  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,
  0 , 'T', 'V', 'G', 'H',  0 ,  0 , 'C', 'D',  0 ,  0 , 'M',  0 , 'K', 'N',  0 ,
  0 ,  0 , 'Y', 'W', 'A',  0 , 'B', 'S',  0 , 'R',  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,
  0 , 't', 'v', 'g', 'h',  0 ,  0 , 'c', 'd',  0 ,  0 , 'm',  0 , 'k', 'n',  0 ,
  0 ,  0 , 'y', 'w', 'a',  0 , 'b', 's',  0 , 'r',  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,
  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,
  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,
  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,
  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,
  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,
  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,
  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,
  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 
};

inline char complement (char c)
{
    char r = REVERSE_TABLE[c];
    if (!r)
        error_exit ("non-base character: ", c);
    return r;
}

void write_reverse_complement (const std::string& s)
{
    std::string::const_iterator p0 = s.begin();
    std::string::const_iterator p1 = s.end();
    while (p1 != p0) {
        std::cout << complement(*--p1);
    }
    std::cout << std::endl;
}

void write_reverse_line (const std::string& s)
{
    std::string::const_iterator p0 = s.begin();
    std::string::const_iterator p1 = s.end();
    while (p1 != p0) {
        std::cout << *--p1;
    }
    std::cout << std::endl;
}

int main (int argc, char *argv[]) 
{
    if (argc != 1) {
        std::cerr << USAGE;
        return 1;
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
        write_reverse_complement(l2);
        std::cout << l3 << std::endl;
        write_reverse_line(l4);
    }

    return 0;
}

// vim: sts=4:sw=4:et:si:ai
