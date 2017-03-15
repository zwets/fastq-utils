#include <stdexcept>
#include <sstream>
#include <iostream>
#include <string>

void error_exit (const std::string& s, char c = ' ')
{
    std::stringstream ss;
    ss << s << c;
    throw std::runtime_error (ss.str());
}

void write_reverse_complement (const std::string& s)
{
    std::string::const_iterator p0 = s.begin();
    std::string::const_iterator p1 = s.end();
    while (p1 != p0) {
        int c;
        switch ((c = *--p1)) {
            case 'A': std::cout << 'T'; break;
            case 'T': std::cout << 'A'; break;
            case 'C': std::cout << 'G'; break;
            case 'G': std::cout << 'C'; break;
            case 'N': std::cout << 'N'; break;
            default: error_exit ("non-base character: ", c);
        }
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
