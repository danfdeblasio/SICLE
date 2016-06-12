//
//  main.cpp
//  SiClE
//
//  Created by Dan DeBlasio on 12/13/11.
//  Copyright 2011-2012 Univeristy of Arizona. All rights reserved.
//
//  This is a standard driver for using the sister finder application.  Requires the inclusion of the SiClE header file and the Phylogeny and Sister finder source files in compilation.
//

#include <iostream>
#include <fstream>
#include "SiClE.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int main (int argc, const char * argv[])
{
    
    
    if(argc < 3 || string(argv[1]) == "-h" || string(argv[1]) == "-?"){
        cerr << "You can run the program in the following way:" << endl;
        cerr << "" << endl;
        cerr << "./SiClE [-h?] [--groupFile <groupFile>] [--E] <SearchPrefix> <File1> <File2> ....." << endl;
        cerr << "" << endl;
        cerr << "" << endl;
        cerr << "Required Arguments:" << endl;
        cerr << "<SearchPrefix> \t\t the prefix of the target family or group for which SiClE will find sisters." << endl;
        cerr << "<FileX> \t\t file with a single phylogenetic tree in Newik format." << endl;
        cerr << "" << endl;
        cerr << "Optional Arguments:" << endl;
        cerr << "-h, -? \t\t\t Prints this help message." << endl;
        cerr << "--groupFile <groupFile>  SiClE will use a hierarical clustering of leaf prefixes found in <GroupFile>  " << endl;
        cerr << "\t\t\t to group non-monophyletic sister clades. The format is shown in the included README" << endl;
        cerr << "--E \t\t\t SiClE will uses regular expression searching rather than strict exact prefix" << endl;
        cerr << "\t\t\t matching to find the target group." << endl;
        exit(0);
    }
    
    
    
    //get family name from command line
    string family_name = argv[1];
    int start = 2;
    string group_file = "";
    bool useRegex = false;
    
   
    
    if(family_name == "--groupFile"){
        group_file = argv[2];
        family_name = argv[3];
        start = 4;
    }
    
    if(family_name == "--E"){
        useRegex = true;
        if(group_file == ""){
            family_name = argv[2];
            start = 3;
        }else{
            family_name = argv[4];
            start = 5;
        }
    }
    
    //if(argc-start>1){
    //    cerr << "SiClE can handle only one tree at a time; multitree is coming soon" << endl;
    //    exit(15);
    //}
    for(int i=start;i<argc;i++){
        cout << argv[i] << ":" << endl;
        //open file and read data into a string
        ifstream file((char*)argv[i]);
        if(!file.is_open()){
            cerr << "The file " << argv[i] << " is not opened" << endl;
            exit(15);
        }
        string phylogeny;
        while(file.is_open() && !file.eof()){
            string line;
            getline(file, line);
            phylogeny += line;
        }
        file.close();
        
        //Generate a phylogeny object from a newick formatted string
        Phylogeny* p = new Phylogeny(string(phylogeny));
        
        SiClE* sfinder;
        //generate sister finding environment
        if(p->root != NULL){
            if(group_file != ""){
                sfinder = new SiClE(p,family_name,group_file, useRegex);
            }else{
                sfinder = new SiClE(p,family_name, useRegex);
            }
        }else{
            continue;
        }
        
        //the sister names will be in the sister names array on the sister finder object.  You will need to get the bootstap by calling the method.
        for (int i = 0; i<sfinder->sister_names_count; i++) {
            cout << family_name << "\t" << sfinder->sister_names[i] << "\t" ;
            double boot = sfinder->bootstrap();
            if(sfinder->bootstrapIsNull()){
                cout << "n/a";
            }else{
                cout << boot;
            }
            cout << endl;
        }
    }
    return 0;
}

