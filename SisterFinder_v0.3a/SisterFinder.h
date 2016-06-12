//
//  SisterFinder.h
//  Phylogeny Assesment
//
//  Created by Dan DeBlasio on 12/13/11.
//  Copyright 2011-2012 Univeristy of Arizona. All rights reserved.
//
//  SisterFinder main class definition. The code can be found in the corespoinding cpp file.
//

#ifndef Phylogeny_Assesment_SisterFinder_h
#include "Phylogeny.h"
#include <vector>

class SisterFinder{
    private:
        Phylogeny* phylo;
        Phylogeny_Node* target;
        Phylogeny_Node* all_match(Phylogeny_Node* n,string s);
        string print_name_if_same(Phylogeny_Node* n);
        void get_sisters();
        bool sameGroup(string, string);  
        vector<string*> mapping;
public:
        SisterFinder(Phylogeny* p, string s);
        SisterFinder(Phylogeny*, string, string);
        string* sister_names;
        int sister_names_count;
        double bootstrap();
        void groupConfig(string fname);

};
#define Phylogeny_Assesment_SisterFinder_h



#endif
