# fastq-utils

Simple utilities operating on FASTQ files

Homed at: https://github.com/zwets/fastq-utils

## Background

I wrote these tiny tools whenever searching for them seemed to take longer
than just write them.  You may find comparable functionality in for instance
[NGSTools](https://github.com/ngstools).

## Tools

| Tool | Function |
|------|----------|
|`fastq-revcomp`  | Reverse complement a FASTQ file |
|`fastq-unbreak`  | Revert multi-line FASTQ (such as emitted by `vcfutils.pl vcf2fq`) to standard FASTQ |
|`fastq-mask`     | Convert bases not passing quality threshold to lowercase or N |
|`fastq-to-fasta` | Convert FASTQ to FASTA |
|`fasta-to-fastq` | Convert FASTA to FASTQ |

## Usage

All tools read from standard input and write to standard output.  Just pipe
them together.  For instance,

```bash
... | vcfutils.pl vcf2fq | fastq-unbreak | fastq-mask -r | fastq-to-fasta
```

## Installation

All tools are plain vanilla C++.  To compile, run `make`.

