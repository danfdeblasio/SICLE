//
//  SisterFinder.cpp
//  Phylogeny Assesment
//
//  Created by Dan DeBlasio on 12/13/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "Phylogeny.h"
#include "SisterFinder.h"

//Returns the node at the root of the subtree (if one exists which is all of the children of the node are in the same family, assuming the the family name is is the first characters in the name of the leaf nodes 
Phylogeny_Node* SisterFinder::all_match(Phylogeny_Node* n,string s){
    if (n->leaf) {
        if (n->name.substr(0,s.length()) == s) {
            return n;
        } else {
            return NULL;
        }
    }else{
        Phylogeny_Node* l = all_match(n->left_child, s);
        Phylogeny_Node* r = all_match(n->right_child, s);
        if (l == NULL) {
            return r;
        }else if(r == NULL){
            return l;
        }else{
            if(l==n->left_child && r==n->right_child) return n;
        }
    }
    return NULL;
}

//If the tree has a consistant family name (defined as substing before "-" character) return this family name, otherwise return an empty string.
string SisterFinder::print_name_if_same(Phylogeny_Node* n){
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
    Phylogeny_Node* a = all_match(n, name);
    if (a==n) {
        return name;
    }
    return "";
}

//searches from the given find node, and finds sister nodes that are also consistant.  
/*SCHEME:
 * If the parent is the root of the tree, both (of multiple) children of the other branch of the root are sisters
 * if the parent of the parent is the root, and the other child is consistant OR is a leaf (leaves are always consistant) that is a sister as well as the direct sister in the tree (if consistant)
 * otherwise the direct sister is tested for consistancy.
 */
void SisterFinder::get_sisters(){
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
            if (target->parent->parent == phylo->root) {
                Phylogeny_Node* to_print = NULL;
                if (target->parent->parent->left_child == target->parent) {
                    to_print = target->parent->parent->right_child;
                }else if (target->parent->parent->right_child == target->parent) {
                    to_print = target->parent->parent->left_child;
                }
                
                extra_sister = print_name_if_same(to_print);
            }
            if (target->parent->left_child == target) {
                sister = print_name_if_same(target->parent->right_child);
            }else if (target->parent->right_child == target) {
                sister = print_name_if_same(target->parent->left_child);
            }
        }
    }
    if (sister == "" && extra_sister != "") {
        sister = extra_sister;
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
double SisterFinder::bootstrap(){
    if (target->parent == phylo->root) {
        if (target->parent->left_child == target) {
            return target->parent->right_child->bootstrap;
        }
        if (target->parent->right_child == target) {
            return target->parent->left_child->branch_length;
        }
    }else{
        return target->parent->bootstrap;
    }
    return 0;
}

//Find the subtree which is consistant with the names of the family given (or prefix in the general case) and find the sisters and put them in the sister names array
SisterFinder::SisterFinder(Phylogeny* p, string s){
    phylo = p;
    target = all_match(p->root,s);
    sister_names_count = 0;
    get_sisters();
}