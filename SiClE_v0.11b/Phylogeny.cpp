/*
 *  Phylogeny.cpp
 *  Phylogeny Assesment
 *
 *  Created by Dan DeBlasio on 8/20/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
/*
 *  Phylogeny.cpp
 *  Phylogeny Assesment
 *
 *  Created by Dan DeBlasio on 8/20/11.
 *  Copyright 2011-2012 Univeristy of Arizona. All rights reserved.
 *
 */

#include "Phylogeny.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Generates a phylogeny object from a string.  Must be ";" terminated.  If () wrapped, may have extra nodes in the object, this is not recomended but is okay.  Assumes boostrap on all branching nodes, and branch lengths on ALL nodes.
Phylogeny::Phylogeny(string s){
	if (s[s.length()-1] != ';') {
		cerr << "Malformed Newik Format file, the last character should be a ';', it's '"<< s[s.length()-1] <<"'" << endl;
		root = NULL;
	}else{
        root = new Phylogeny_Node(s.substr(0, s.length()-1),NULL);
    }
}

// searches a subtree for the maximum of the label values.  The values are set by hand (using the label by name function) and are integeers stored on leaf nodes.
int Phylogeny_Node::max_label(){
    if(leaf) return label;
    int l = left_child->max_label();
    int r = right_child->max_label();
    if(l>r) return l;
    return r;
}

// Finds a node by name and returns a pointer to the name 
Phylogeny_Node* Phylogeny_Node::find_by_name(string s){
    if(leaf){
       if(name == s){
        return this;
       }
        return NULL;
    }
    return NULL;
}

int depth = 0;
//prints a node in standard newick format, strings can be addeed to the start of elements (sich as indention tabs etc.
string Phylogeny_Node::print(string append){
    if (leaf) {
        char temp[name.length() + 10];
        sprintf(temp,"%s:%f",name.c_str(),branch_length);
        return string(temp) ;
    }
    string s;
    if(left_child!=NULL && right_child!=NULL) s = "("+left_child->print(append)+","+right_child->print(append)+")";
    else if(left_child!=NULL) s = "("+left_child->print(append)+",NULL)";
    else if(right_child!=NULL) s = "(NULL,"+right_child->print(append)+")";
    else s = "(NULL,NULL)";
            
    char temp[s.length() + 20];
    sprintf(temp,"%s%f:%f",s.c_str(),bootstrap,branch_length);
    
    return string(temp);
}

void Phylogeny_Node::compute_changes(int max){
    changes = new int[max+1];
    if (leaf) {
        for (int i=0; i<=max; i++) {
            changes[i] = (i!=label);
        }
    }else{
        left_child->compute_changes(max);
        right_child->compute_changes(max);
        for (int i=0; i<=max; i++) {
            int l=left_child->changes[i],r=right_child->changes[i];
            for (int j=0; j<=max; j++) {
                l = (l<(left_child->changes[j]+1))?l:(left_child->changes[j]+1);
                r = (r<(right_child->changes[j]+1))?r:(right_child->changes[j]+1);
            }
            changes[i] = l + r;
        }
    }
}

int Phylogeny_Node::min_changes(){
    int max = max_label();
    compute_changes(max);
    int min = changes[0];
    for (int i=0; i<=max; i++) {
        if(min>changes[i]) min = changes[i];
    }
    return min;
}

//if the string given is the name, give it label l
void Phylogeny_Node::label_by_name(string s, int l){
    if (leaf) {
        if (name == s) {
            label = l;
        }
    }
    else{
        left_child->label_by_name(s, l);
        right_child->label_by_name(s, l);
    }
}

Phylogeny_Node* Phylogeny::reroot(Phylogeny_Node* parent, Phylogeny_Node* child, Phylogeny_Node* rt){
    //cout << parent->print() << endl;
    
    //cout << child->print() << endl;
    
    //make a new node which will encapsulate the branch that is now the root
    Phylogeny_Node* n = new Phylogeny_Node();
    n->leaf = false;
    
    //set its children
    n->left_child = child;
    n->bootstrap = parent->bootstrap;
    n->branch_length = parent->branch_length;
    n->right_child = parent;
    
    
   
    
    //replace the pointer that was to the child with the pointer to parents parent
    if(parent->left_child == child){
        parent->left_child = parent->parent;
    }else{
        parent->right_child = parent->parent;
    }
    
    
    //set N's pointer and the pointer to it
    n->parent = rt->parent;
    if(rt->parent!=NULL){
        if(rt->parent->left_child == rt){
            rt->parent->left_child = n;
            //rt->parent->left_child = NULL;
        }else{
            rt->parent->right_child = n;
            //rt->parent->right_child = NULL;
        }
    }
    
    
    Phylogeny_Node* sub_child = parent;
    Phylogeny_Node* sub_target = parent->parent;
    
    
    parent->parent = n;
    
    //reverse the connections on the target subject to the child
    //initially these are surrounding the parent node
    do{
        
        
        //cout << ((sub_child!=rt)?"1":"0") << endl;
        
        if (sub_target->left_child == sub_child) {
            if(sub_target == rt) sub_target->left_child = NULL;
            else sub_target->left_child = sub_target->parent;
        } else {
            if(sub_target == rt) sub_target->right_child = NULL;
            else sub_target->right_child = sub_target->parent;
        }
        
        sub_target->bootstrap = sub_child->bootstrap;
        sub_target->branch_length = sub_child->branch_length;
        
        Phylogeny_Node* swap = sub_child;
        sub_child = sub_target;
        sub_target = sub_target->parent;
        sub_child->parent = swap;
        
        //cout << ((sub_child!=rt)?"1":"0") << endl;
    }while(sub_child!=rt);
    //cout << ((rt->parent==NULL)?"N":"__") << endl;
    if(rt->parent->left_child == rt){
        if (rt->left_child==NULL) {
            rt->parent->left_child = rt->right_child;
        } else {
            rt->parent->left_child = rt->left_child;
        }
    }else{
        if (rt->left_child==NULL) {
            rt->parent->right_child = rt->right_child;
        } else {
            rt->parent->right_child = rt->left_child;
        }
    }
    
    return n;
    //cout << n->print("\n") << endl;
    
}

//constructor for substring S, given the parent node p.  The rootnode would have a null parent pointer.  allows for bottom up traversal.
Phylogeny_Node::Phylogeny_Node(string s, Phylogeny_Node* p){
    parent = p;
    int comma = (int)s.find_last_of(',');
	if (comma<0) {
        int colon = (int)s.find_first_of(":");
        name = s.substr(0,colon);
        int cparen = (int)s.substr(colon+1).find_first_of(')');
        branch_length = atof(s.substr(colon+1,s.substr(colon+1).length()-colon-cparen).c_str());
        leaf = true;
        return;
    }
    leaf = false;

    int low_comma = (int)INFINITY;
    int num_open = 0;
    for (int i=0; i<s.length(); i++) {
        if(s[i] == '(') num_open++;
        if(s[i] == ')') num_open--;
        if(s[i] == ',') low_comma = (low_comma<num_open)?low_comma:num_open;
    }
	int seperating = 0;
    num_open = 0;
    for (int i=0; i<s.length(); i++) {
        if(s[i] == '(') num_open++;
        if(s[i] == ')') num_open--;
        if(s[i] == ',' && num_open==low_comma) seperating = i;
    }
    int start = 0;
    int end = (int)s.length();
    bootstrap = 0;
    branch_length = 0;
    if(low_comma>0){
        start = (int)s.find_first_of("(")+1;
        end = (int)s.find_last_of(")")-1;
        string info = s.substr(end+2);
        int colon = (int)info.find_last_of(":");
        bootstrap = atof(info.substr(0,colon).c_str());
        branch_length = atof(info.substr(colon+1).c_str());
    }
    //cout << seperating << "," << low_comma << endl;
	left_child = new Phylogeny_Node(s.substr(start, (seperating-1-start)),this);
	right_child = new Phylogeny_Node(s.substr(seperating+1,end-seperating),this);
}