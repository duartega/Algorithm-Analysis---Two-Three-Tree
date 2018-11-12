#ifndef TWOTHREETREE_H
#define TWOTHREETREE_H

#include<iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class TTT{
    public:
        TTT();
        void contains() const;
        bool isEmpty();
        void printTreeTTT(ostream & out = cout) const;
        void buildTreeTTT(ifstream & input);
    private:
    struct node{
        node(const string &lv, const string &mv, const string &rv, node *l, node * m, node *r, node *p)
        :lval(lv), mval(mv), rval(rv), left(l), middle(m), right(r), parent(p){
            lines.resize(0); // Resizes vector to only allot necessary amount of space
        }
        string lval;
        string mval;
        string rval;
        node * left;
        node * middle;
        node * right;
        node * parent;

        vector<int> lines;
    };
    node * root;
    void insertHelper(const string &X, int line, node *& t, int &distWords);
    void leftInsertHelper(node *& t, const string &X);
    void splitInsertHelper(node *& t, const string &X);
    char directionHelper(node *& t, node *& p);
	bool containsHelper(const string & x, node * t, node* &result) const;
	void printTreeHelper(node *t, ostream & out) const;
	int findHeight(node *t);
};

#endif
