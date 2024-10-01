#include "poly.h"
#include <stdio.h>
#include <stdlib.h>

Node* createNode(char data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->children = NULL;
    newNode->sibling = NULL;
    return newNode;
}

void addChild(Node* parent, Node* child) {
    if (parent->children == NULL) {
        parent->children = child;
    } else {
        Node* temp = parent->children;
        while (temp->sibling != NULL) {
            temp = temp->sibling;
        }
        temp->sibling = child;
    }
}

void printTree(Node* root, int level) {
    if (root == NULL) {
        return;
    }
    for (int i = 0; i < level; i++) {
        printf("  ");
    }
    printf("%c\n", root->data);
    printTree(root->children, level + 1);
    printTree(root->sibling, level);
}

void freeTree(Node* root) {
    if (root == NULL) {
        return;
    }
    freeTree(root->children);
    freeTree(root->children);
    free(root);
}

int main(){
    yyparse();
    return 0;
}