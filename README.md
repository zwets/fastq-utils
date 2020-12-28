# fastq-utils

Simple utilities operating on FASTQ files

Homed at: <https://github.com/zwets/fastq-utils>


## Background

I wrote these tiny tools whenever searching for them seemed to take longer
than just write them.  You may find comparable functionality in for instance
[NGSTools](https://github.com/ngstools).


## Tools

| Tool | Function |
|------|----------|
|`fastq-mask`     | Convert bases not passing quality threshold to lowercase or N |
|`fastq-revcomp`  | Reverse complement a FASTQ file |
|`fastq-stats`    | Compute number of reads, bases, and bases passing a Phred threshold in a FASTQ file |
|`fastq-unbreak`  | Revert multi-line FASTQ (such as emitted by `vcfutils.pl vcf2fq`) to standard FASTQ |
|`fastq-to-fasta` | Convert FASTQ to FASTA |
|`fasta-to-fastq` | Convert FASTA to FASTQ |


## Usage

All tools read from standard input and write to standard output.  Just pipe
them together.  For instance,

    ... | vcfutils.pl vcf2fq | fastq-unbreak | fastq-mask -r | fastq-to-fasta

When the input is compressed, `zcat` is your friend:

    zcat myfile.fq.gz | fastq-stats


## Installation

All tools are plain vanilla C++.  To compile, run `make`.


---

#### Licence

fastq-utils - simple utilities operating on fastq files
Copyright (C) 2020  Marco van Zwetselaar <io@zwets.it>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

