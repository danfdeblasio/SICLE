#!/usr/bin/perl
use strict;

my $rc_name = shift;

system("mkdir SisterFinder_$rc_name");
open MAKEFILE,">SisterFinder_$rc_name/Makefile";
system("cp en.lproj/Phylogeny.cpp en.lproj/Phylogeny.h SisterFinder/main.cpp SisterFinder/en.lproj/SisterFinder.cpp SisterFinder/en.lproj/SisterFinder.h SisterFinder_$rc_name");
print MAKEFILE "all: SisterFinder\n\n";
print MAKEFILE "clean:\n\trm Phylogeny.o SisterFinder.o SisterFinder\n\n";
print MAKEFILE "Phylogeny.o: Phylogeny.cpp Phylogeny.h\n\tg++ -c Phylogeny.cpp\n\n";
print MAKEFILE "SisterFinder.o: SisterFinder.cpp SisterFinder.h\n\tg++ -c SisterFinder.cpp\n\n";
print MAKEFILE "SisterFinder: main.cpp SisterFinder.o Phylogeny.o\n\tg++ -o SisterFinder main.cpp SisterFinder.o Phylogeny.o\n\n";

close MAKEFILE;

open README, ">SisterFinder_$rc_name/README";
print README "Compilation
------------
Move to the uncompressed destination and run the following command:

        make

or alternatly you can compile it manulally using the following commands:

	g++ -o SisterFinder main.cpp Phylogeny.cpp SisterFinder.cpp

Simple Runtime
--------------
You can run the program in the following way:

    ./SisterFinder <SearchPrefix> <File1> <File2> .....

Where search term is the prefix of the family or group you want the sisters for

Complex Runtime
----------------
The main.cpp file is a simple driver for the sister finder class, wihch requires the Phylogeny class included.  From here you should be able to see the somple workings of the progrmand how you would be able to modify this for your own uses

Version History
---------------
v0.2b : fixed errors with rerooting
	fixed errors with sister group bring the same as the siters monophyloetic group (i.e. the parents parents other child is the same as the parents monophyoletic group)
v0.1b : initial setup, works for a single search and a small group of files.  The files must have single newick trees.";
close README;
