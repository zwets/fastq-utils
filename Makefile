CPPFLAGS=-O3

all: fasta-to-fastq fastq-mask fastq-revcomp fastq-stats fastq-to-fasta fastq-unbreak

clean:
	rm -f fasta-to-fastq fastq-mask fastq-revcomp fastq-stats fastq-to-fasta fastq-unbreak

