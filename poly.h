#ifndef poly_H
#define poly_H

extern int yyparse();
extern int yylex();

struct Node {
    char data;
    struct Node* children;
    struct Node* sibling;
}typedef Node;

Node* createNode(char data);
void addChild(Node* parent, Node* child);
void printTree(Node* root, int level);
void freeTree(Node* root);

#endif 