//
//  SiClE.cpp
//  Phylogeny Assesment
//
//  Created by Dan DeBlasio on 12/13/11.
//  Copyright 2011-2012 Univeristy of Arizona. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "Phylogeny.h"
#include "SiClE.h"

using namespace std;

bool is_prefix(string a, string b){
    if(b.length() > a.length()) return 0;
    for (int i=0; i<b.length(); i++) {
        if (b[i] != a[i]) {
            return 0;
        }
    }
    return 1;
}

/* Takes the file name of the grouping configuration file.
 * See the website for details on formatting, but grouping outputs a 
 * polyphletic group if they are all families in one group
 */
void SiClE::groupConfig(string fname){
    ifstream file(fname.c_str());
    if(!file.is_open()){
        cerr << "Couldnt open file: " << fname << endl;
        exit(15);
    }
    while(file.is_open() && !file.eof()){
        string* connection = new string[2];
        file >> connection[0] >> connection[1];
        mapping.push_back(connection);
        int count = mapping.size();
        for(int i=0;i<count;i++){
            /* groups of groups will be expanded, 
             * but the smaller group (i.e. the subset must be defined 
             *  higher in the file 
             */
            if(mapping[i][0] == connection[1]){
                string* new_connection = new string[2];
                new_connection[0] = connection[0];
                new_connection[1] = mapping[i][1];
                mapping.push_back(new_connection);
            }
        }
    }
    
    
}


/*
 * Boolean check on a family being in a group (can be reversed)
 */
bool SiClE::sameGroup(string group1, string group2){
    if(group1 == group2) return true;
    
    for(int i=0;i<mapping.size();i++){
        /*if(mapping[i][0] == group1 && mapping[i][1] == group2){
            return true;
        }*/
        if(mapping[i][1] == group1 && mapping[i][0] == group2){
            return true;
        }
    }
    return false;
}

string SiClE::lowest_common_group(string a, string b){
    for(int i=0;i<mapping.size();i++){
        for(int j=0;j<mapping.size();j++){
            if(mapping[i][1]==a && mapping[j][1]==b && mapping[i][0]==mapping[j][0]){
                return mapping[j][0];
            }
        }
    }
    return "";
}

//Returns the node at the root of the subtree (if one exists which is all of the children of the 
//  node are in the same family, assuming the the family name is is the first characters in the name of the leaf nodes 
Phylogeny_Node* SiClE::all_match(Phylogeny_Node* n,string s){
    //cerr << "All Match: " << n->name.substr(0,n->name.find_first_of("-")) << "\t" << s << endl;
    if (n->leaf) {
        //
        if (is_prefix(n->name,s) || sameGroup(n->name.substr(0,n->name.find_first_of("-")),s)) {
            //cerr << "YES" << endl;
            return n;
        } else {
            return NULL;
        }
    }else{
        //cerr << "All Match (IN): " << n->name.substr(0,n->name.find_first_of("-")) << "\t" << s << endl;

        Phylogeny_Node* l = all_match(n->left_child, s);
        Phylogeny_Node* r = all_match(n->right_child, s);
        if (l == NULL) {
            return r;
        }else if(r == NULL){
            return l;
        }else{
            if(l==n->left_child && r==n->right_child) return n;
            else{ 
                //cerr << "In Here? " << endl;
                //reroot at the parent of the one thats not the child
                Phylogeny_Node* newRoot;
                if (l != n->left_child) {
                    newRoot = n->left_child;
                    n->left_child = newRoot->left_child;
                    newRoot->parent = n->parent;
                    n->parent = newRoot;
                    newRoot->left_child = n;
                }else{
                    newRoot = n->right_child;
                    n->right_child = newRoot->right_child;
                    newRoot->parent = n->parent;
                    n->parent = newRoot;
                    newRoot->right_child = n;
                }
                double swap_b = newRoot->bootstrap;
                double swap_l = newRoot->branch_length;
                newRoot->bootstrap = n->bootstrap;
                newRoot->branch_length = n->branch_length;
                n->bootstrap = swap_b ;
                n->branch_length = swap_l;
                
                Phylogeny_Node* swap = newRoot->left_child;
                newRoot->left_child = newRoot->right_child;
                newRoot->right_child = swap;
                if (newRoot->parent == NULL) {
                    phylo->root = newRoot;
                }else{
                    if(n==newRoot->parent->left_child){
                        newRoot->parent->left_child = newRoot;
                    }else{
                        newRoot->parent->right_child = newRoot;
                    }
                }
                
                Phylogeny_Node* p= all_match(newRoot, s);
                return p;
            }
        }
    }
    return NULL;
}

Phylogeny_Node* SiClE::whole_tree_by_name(Phylogeny_Node* n,string s){
    //cerr << "All Match: " << n->name.substr(0,n->name.find_first_of("-")) << "\t" << s << endl;
    if (n->leaf) {
        if (n->name.substr(0,n->name.find_first_of("-")) == s || sameGroup(n->name.substr(0,n->name.find_first_of("-")),s)) {
            return n;
        } else {
            if(s == "Eukaryota"){
                //cerr << n->name << endl;
            }
            return NULL;
        }
    }else{
        Phylogeny_Node* l = whole_tree_by_name(n->left_child, s);
        Phylogeny_Node* r = whole_tree_by_name(n->right_child, s);
        if (l == NULL || r == NULL){
            return NULL;
        }else{
            if(l==n->left_child && r==n->right_child) return n;
        }
    }
    return NULL;
}

//If the tree has a consistant family name (defined as substing before "-" character) return this family name, 
//  otherwise return an empty string.
string SiClE::print_name_if_same(Phylogeny_Node* n){
    if (n->leaf) {
        string p = n->print();
        return p.substr(0,p.find_first_of("-"));
    }
    Phylogeny_Node* l = n->left_child;
    while (!l->leaf) {
        l = l->left_child;
    }
    string p = l->print();
    string name = p.substr(0,p.find_first_of("-"));
    Phylogeny_Node* a = whole_tree_by_name(n, name);
    if(a!=n){
        for(int i=0;i<mapping.size();i++){
            if(mapping[i][1] == name){
                //cerr << name << " " << mapping[i][0] << endl;
                a = whole_tree_by_name(n, mapping[i][0]);
                //cerr << ((a==NULL)?1:0) << endl;
                if(a==n){
                    return mapping[i][0];
                }
            }
        }
    }
    if (a==n) {
        return name;
    }
    return "";
}

//searches from the given find node, and finds sister nodes that are also consistant.  
/*SCHEME:
 * If the parent is the root of the tree, both (of multiple) children of the other branch of the root are sisters
 * if the parent of the parent is the root, and the other child is consistant OR is a leaf (leaves are always consistant) 
 * that is a sister as well as the direct sister in the tree (if consistant)
 * otherwise the direct sister is tested for consistancy.
 */
void SiClE::get_sisters(){
    string sister = "";
    string extra_sister = "";
    if (target != NULL) {
        if (target->parent == phylo->root) {
            Phylogeny_Node* to_print = NULL;
            if (target->parent->left_child == target) {
                to_print = target->parent->right_child;
            }else if (target->parent->right_child == target) {
                to_print = target->parent->left_child;
            }
            
            if(to_print->leaf){
                sister = print_name_if_same(to_print);
            }else{
                sister = print_name_if_same(to_print->left_child);
                extra_sister = print_name_if_same(to_print->right_child);
            }
        }else{
            if (target->parent->left_child == target) {
                sister = print_name_if_same(target->parent->right_child);
            }else if (target->parent->right_child == target) {
                sister = print_name_if_same(target->parent->left_child);
            }
            
            //cout << sister << endl;
            
            if (1==1 || target->parent->parent == phylo->root) {
                Phylogeny_Node* to_print = NULL;
                if (target->parent->parent->left_child == target->parent) {
                    to_print = target->parent->parent->right_child;
                }else if (target->parent->parent->right_child == target->parent) {
                    to_print = target->parent->parent->left_child;
                }
                
                if(!to_print->parent->parent == NULL){
                                                
                    extra_sister = print_name_if_same(to_print);
                    cerr << "New Sister Out: " << extra_sister << endl;
                    while(to_print->parent->parent != phylo->root && extra_sister != ""){
                        if(to_print->parent->parent == NULL){
                            break;
                        }
                        
                        if(to_print->parent->parent->left_child == to_print->parent){
                            to_print = to_print->parent->parent->right_child;
                        }else{
                            to_print = to_print->parent->parent->left_child;
                        }
                        string new_sister = print_name_if_same(to_print);
                        cerr << "New Sister: " << new_sister << "  " << extra_sister << endl;
                        if(!(sameGroup(new_sister,extra_sister) || sameGroup(extra_sister,new_sister))){
                            extra_sister = lowest_common_group(new_sister,extra_sister);
                        }else if(sameGroup(extra_sister,new_sister)){
                            extra_sister = new_sister;
                        }
                    }
                    if(to_print->parent->parent == phylo->root){
                        if(to_print->parent->parent->left_child == to_print->parent){
                            to_print = to_print->parent->parent->right_child;
                        }else{
                            to_print = to_print->parent->parent->left_child;
                        }
                        string new_sister = print_name_if_same(to_print);
                        //cerr << "New Sister: " << new_sister << "  " << extra_sister << endl;
                        if(!(sameGroup(new_sister,extra_sister) || sameGroup(extra_sister,new_sister))){
                            extra_sister = "";
                        }else if(sameGroup(extra_sister,new_sister)){
                            extra_sister = new_sister;
                        }
                    }
                }
                    
            }
            
            //cerr << "Sister: " << sister << endl;
        }
    }
    
    if (sister == "" && extra_sister != "") {
        sister = extra_sister;
        extra_sister = "";
    }
    if(sister==extra_sister){
        if(target->parent->parent!= NULL && target->parent->parent->bootstrap < target->parent->bootstrap){
            target->parent->parent->bootstrap = target->parent->bootstrap;
        }
        if(target->parent->parent!= NULL && target->parent==target->parent->parent->left_child){
            target->parent->parent->left_child = target;
            target->parent = target->parent->parent;
        }
        else if(target->parent->parent!= NULL && target->parent==target->parent->parent->right_child){
            target->parent->parent->right_child = target;
            target->parent = target->parent->parent;
        }
        extra_sister = "";
    }
    
    if(extra_sister != ""){
        sister_names_count = 2;
        sister_names = new string[sister_names_count];
        sister_names[0] = sister;
        sister_names[1] = extra_sister;
    }else if(sister != ""){
        sister_names_count = 1;
        sister_names = new string[sister_names_count];
        sister_names[0] = sister;
    }else{
        sister_names_count = 0;
        return;
    }
}

//get the bootstrap value for the sisters of the target node
double SiClE::bootstrap(){
    if (target->parent == phylo->root) {
        if (target->parent->left_child == target) {
            return target->parent->right_child->bootstrap;
        }
        if (target->parent->right_child == target) {
            return target->parent->left_child->bootstrap;
        }
    }else{
        return target->parent->bootstrap;
    }
    return 0;
}

bool SiClE::bootstrapIsNull(){
    if(sister_names_count > 1){ return 0; }
    if (target->parent == phylo->root) {
        if (target->parent->left_child == target) {
            return whole_tree_by_name(target->parent->right_child,sister_names[0]) == target->parent->right_child;
        }
        if (target->parent->right_child == target) {
            return whole_tree_by_name(target->parent->left_child,sister_names[0]) == target->parent->left_child;
        }
    }else{
        return target->parent->leaf;
    }
    return 1;
}

SiClE::SiClE(Phylogeny* p, string s, string group_file){
    groupConfig(group_file);
    phylo = p;
    target = all_match(p->root,s);
    sister_names_count = 0;
    get_sisters();
}

//Find the subtree which is consistant with the names of the family given (or prefix in the general case) and find 
//  the sisters and put them in the sister names array
SiClE::SiClE(Phylogeny* p, string s){

    phylo = p;
    target = all_match(p->root,s);
    if(target != NULL){
        cerr << "Target: " << target->print() << endl;
        sister_names_count = 0;
        get_sisters();
    }else{
        cerr << "CLADE NOT FOUND" << endl;
    }
}