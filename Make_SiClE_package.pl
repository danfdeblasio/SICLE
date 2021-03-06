#!/usr/bin/perl
use strict;

my $rc_name = shift;

system("mkdir -p Releases/SiClE_$rc_name/examples");
open MAKEFILE,">Releases/SiClE_$rc_name/Makefile";
system("cp en.lproj/Phylogeny.cpp en.lproj/Phylogeny.h SisterFinder/main.cpp SisterFinder/en.lproj/SiClE.cpp SisterFinder/en.lproj/SiClE.h Releases/SiClE_$rc_name");
system("cp testing_examples/groupfile.txt testing_examples/Phy001MQP3_SALRD_new.tre Releases/SiClE_$rc_name/examples");
print MAKEFILE "all: SiClE\n\n";
print MAKEFILE "clean:\n\trm Phylogeny.o SiClE.o SiClE\n\n";
print MAKEFILE "Phylogeny.o: Phylogeny.cpp Phylogeny.h\n\tg++ -std=c++0x -c Phylogeny.cpp\n\n";
print MAKEFILE "SiClE.o: SiClE.cpp SiClE.h\n\tg++ -std=c++0x -c SiClE.cpp\n\n";
print MAKEFILE "SiClE: main.cpp SiClE.o Phylogeny.o\n\tg++ -o SiClE main.cpp SiClE.o Phylogeny.o\n\n";

close MAKEFILE;

open README, ">Releases/SiClE_$rc_name/README";
print README "Compilation
------------
Move to the uncompressed destination and run the following command:

        make

or alternatly you can compile it manulally using the following commands:

	g++ -std=c++0x -o SiClE main.cpp Phylogeny.cpp SiClE.cpp

Simple Runtime
--------------

./SiClE [-h?] [--groupFile <groupFile>] [--E] <SearchPrefix> <File1> <File2> .....


Required Arguments:
<SearchPrefix> 		 	the prefix of the target family or group for which SiClE will find sisters.
<FileX> 		 		file with a single phylogenetic tree in Newik format.

Optional Arguments:
-h, -? 					 Prints this help message.
--groupFile <groupFile>  SiClE will use a hierarical clustering of leaf prefixes found in <GroupFile>
			 		     to group non-monophyletic sister clades. The format is shown in the included README
--E 			 		 SiClE will uses regular expression searching rather than strict exact prefix
			 			 matching to find the target group.
			 
Group File
----------
An example group file can be found on the SiClE website at http://eebweb.arizona.edu/sicle/ or directly at 
http://eebweb.arizona.edu/sicle/examples/groupFile.txt or included in this package.

Examples
--------
We have included an example tree in this file from the experements described in the SICLE publication. This tree is Phy001MQP3_SALRD_new.tre

Using SICLE to search for Cyanobacteria in this tree shows that Betaproteobacteria is the only monophyletic sister clade and the minimum boostrap suport along the branch that conncts them is 0.997.

./SiClE Cyanobacteria examples/Phy001MQP3_SALRD_new.tre
	examples/Phy001MQP3_SALRD_new.tre:
	Cyanobacteria	Betaproteobacteria	0.997

If you use the provided hierarchy you find that the other sister to Cyanobacteria consists only of different Eubacteria

./SiClE --groupFile examples/groupfile.txt examples/Cyanobacteria examples/Phy001MQP3_SALRD_new.tre
	examples/Phy001MQP3_SALRD_new.tre:
	Cyanobacteria	Eubacteria	0.997
	Cyanobacteria	Betaproteobacteria	0.997
	
The output of SICLE is tab-delimited and can easily be parsed a hand wirtten script or any available spreadsheet application like EXCEL. 

Complex Runtime
----------------
The main.cpp file is a simple driver for the SiClE class (which requires the included Phylogeny class).  Working from this example you can include the SiClE methods in any existing C++ program. 

Version History
---------------
v1.2:   updated the README
        fully supported regular expression now included 
        added '-h' and '-?' and no-input handling 
v1.1:	fixed a bug when parent-parent is root
v0.11b: bug fixes on rerooting (just run initial finding after rerooting)
	disable multi file input (not working for some reason
	include example group file in package 
v0.8b : changed the rerooting policy for inital CLADE finding
v0.4a : added checkes for file not found
	more use friendly for multiple files
v0.3a : fixed errors with rerooting 
	completed grouping procedures
	general bug fixes
v0.2a : fixed errors with rerooting
	fixed errors with sister group bring the same as the siters monophyloetic group (i.e. the parents parents other child is the same as the parents monophyoletic group)
v0.1a : initial setup, works for a single search and a small group of files.  The files must have single newick trees.";
close README;
