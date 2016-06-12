//
//  SiClE.h
//  Phylogeny Assesment
//
//  Created by Dan DeBlasio on 12/13/11.
//  Copyright 2011-2012 Univeristy of Arizona. All rights reserved.
//
//  SiClE main class definition. The code can be found in the corespoinding cpp file.
//

#ifndef Phylogeny_Assesment_SiClE_h
#include "Phylogeny.h"
#include <vector>

class SiClE{
    private:
        Phylogeny* phylo;
        Phylogeny_Node* target;
        Phylogeny_Node* all_match(Phylogeny_Node* n,string s);
        Phylogeny_Node* whole_tree_by_name(Phylogeny_Node* n,string s);
        Phylogeny_Node* find_match(Phylogeny_Node* n,string s);
        string print_name_if_same(Phylogeny_Node* n);
        void get_sisters();
        bool sameGroup(string, string);  
        vector<string*> mapping;
    public:
        SiClE(Phylogeny* p, string s);
        SiClE(Phylogeny*, string, string);
        string* sister_names;
        int sister_names_count;
        double bootstrap();
        bool bootstrapIsNull();
        void groupConfig(string fname);

};
#define Phylogeny_Assesment_SiClE_h



#endif
