//
//  SisterFinder.h
//  Phylogeny Assesment
//
//  Created by Dan DeBlasio on 12/13/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef Phylogeny_Assesment_SisterFinder_h
#include "Phylogeny.h"

class SisterFinder{
    private:
        Phylogeny* phylo;
        Phylogeny_Node* target;
        Phylogeny_Node* all_match(Phylogeny_Node* n,string s);
        string print_name_if_same(Phylogeny_Node* n);
        void get_sisters();
    public:
        SisterFinder(Phylogeny* p, string s);
        string* sister_names;
        int sister_names_count;
        double bootstrap();

};
#define Phylogeny_Assesment_SisterFinder_h



#endif
