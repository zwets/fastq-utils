CPPFLAGS=-O3 -Wall -Wpedantic

all: fasta-to-fastq fastq-mask fastq-rc fastq-stats fastq-to-fasta fastq-unbreak

clean:
	rm -f fasta-to-fastq fastq-mask fastq-rc fastq-stats fastq-to-fasta fastq-unbreak

