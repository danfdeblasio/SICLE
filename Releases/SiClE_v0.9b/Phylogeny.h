/*
 *  Phylogeny.h
 *  Phylogeny Assesment
 *
 *  Created by Dan DeBlasio on 8/20/11.
 *  Copyright 2011-2012 Univeristy of Arizona. All rights reserved.
 *
 */
#include <iostream>
#include <string>

#ifndef Phylogeny_Assesment_Phylogeny_h
using namespace std;

class Phylogeny_Node{
public:
	double branch_length;
	double bootstrap;
	bool leaf;
	string name;
    int label;
	Phylogeny_Node * left_child;
	Phylogeny_Node * right_child;
    Phylogeny_Node * parent;
    void label_by_name(string s, int l);
    int min_changes();
	
    Phylogeny_Node* find_by_name(string s);
    string print(string);
    string print() {return print("");} 
	Phylogeny_Node(string s, Phylogeny_Node*);
    Phylogeny_Node(){}
   
protected:
    int* changes;
    int max_label();
    void compute_changes(int);
    
};

class Phylogeny{
public:
	Phylogeny_Node * root;
    void label(string s, int l){ root->label_by_name(s, l); }
    static Phylogeny_Node * reroot(Phylogeny_Node* parent, Phylogeny_Node* child, Phylogeny_Node* root);
    Phylogeny(string s);
};

#define Phylogeny_Assesment_Phylogeny_h
#endif