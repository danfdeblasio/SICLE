//
//  main.cpp
//  SisterFinder
//
//  Created by Dan DeBlasio on 12/13/11.
//  Copyright 2011 Univeristy of Arizona. All rights reserved.
//
//  This is a standard driver for using the sister finder application.  Requires the inclusion of the SisterFinder header file and the Phylogeny and Sister finder source files in compilation.
//

#include <iostream>
#include <fstream>
#include "SisterFinder.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int main (int argc, const char * argv[])
{
    //get family name from command line
    string family_name = argv[1];
    
    for(int i=2;i<argc;i++){
        //open file and read data into a string
        ifstream file((char*)argv[i]);
        string phylogeny;
        while(file.is_open() && !file.eof()){
            string line;
            getline(file, line);
            phylogeny += line;
        }
        file.close();
        
        //Generate a phylogeny object from a newick formatted string
        Phylogeny* p = new Phylogeny(string(phylogeny));
        
        
        //generate sister finding environment
        SisterFinder* sfinder = new SisterFinder(p,family_name);
        
        //the sister names will be in the sister names array on the sister finder object.  You will need to get the bootstap by calling the method.
        for (int i = 0; i<sfinder->sister_names_count; i++) {
            cout << family_name << "\t" << sfinder->sister_names[i] << "\t" << sfinder->bootstrap() << endl;
        }
    }
    return 0;
}

