#include "polyutils.h"
#include <stdio.h>
#include <stdlib.h>

#define POSITIVE 1
#define NEGATIVE -1

Node* treeroot = NULL;

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

void printParsetree(Node* root, int level){
    if(root == NULL) return;
    for(int i = 0; i < level; i++){
        printf("    ");
    }
    printf("%c\n", root->data);
    printParsetree(root->children, level + 1);
    printParsetree(root->sibling, level);
}

void printAnnotatedTree(Node* root, int depth) {
    if (root == NULL) return;
    
    // Print indentation
    for (int i = 0; i < depth; i++) {
        printf("    ");
    }
    
    // Print node data and relevant attributes
    printf("=>%c", root->data);
    
    // Print attributes based on node type
    switch (root->data) {
        case 'S':
            printf(" []");
            break;
        case 'P':
        case 'T':
            printf(" [inh = %d]", root->inh);
            break;
        case 'N':
            printf(" [val = %d]", root->val);
            break;
        case 'M':
            printf(" [inh = %d, val = %d]", root->inh, root->val);
            break;
        case 'X':
            printf(" []");
            break;
        default:
            // For leaf nodes (digits and operators)
            if (root->data >= '0' && root->data <= '9') {
                printf(" [val = %d]", root->val);
            } else {
                printf(" []");
            }
            break;
    }
    
    printf("\n");
    
    // Recursively print children
    printAnnotatedTree(root->children, depth + 1);
    
    // Recursively print siblings
    printAnnotatedTree(root->sibling, depth);
}

void freeTree(Node* root) {
    if (root == NULL) {
        return;
    }
    freeTree(root->children);
    freeTree(root->children);
    free(root);
}

void setatt(Node* node, int inh){
    if(node == NULL) return;
    Node* leftChild = node->children;
    Node* middleChild = leftChild!=NULL? leftChild->sibling : NULL;
    Node* rightChild =  middleChild!=NULL? middleChild->sibling : NULL;
   
    node->inh = inh;  // Set inherited attribute for all nodes

    switch(node->data){
        case 'S':
            if(leftChild->data == 'P'){
                setatt(leftChild, POSITIVE);
            }
            else {
                setatt(middleChild, leftChild->data == '+' ? POSITIVE : NEGATIVE);
            }
            break;
        case 'P':
            setatt(leftChild, inh);
            if(middleChild != NULL){
                setatt(rightChild, middleChild->data == '+' ? POSITIVE : NEGATIVE);
            }
            break;
        case 'T':
            setatt(leftChild, inh);
            if(middleChild) setatt(middleChild, 0);
            break;
        case 'N':
            if(middleChild != NULL){
                setatt(leftChild, 0);
                setatt(middleChild, leftChild->data - '0');
                node->val = middleChild->val;
            } else {
                setatt(leftChild, 0);
                node->val = leftChild->val;
            }
            break;
        case 'M':
            if(middleChild != NULL){
                setatt(leftChild, 0);
                int newInh = 10 * inh + (leftChild->data - '0');
                setatt(middleChild, newInh);
                node->val = middleChild->val;
            } else {
                setatt(leftChild, 0);
                node->val = inh * 10 + (leftChild->data - '0');
            }
            break;
        case 'X':
            setatt(leftChild, 0);
            if(middleChild) setatt(middleChild, 0);
            if(rightChild) setatt(rightChild, 0);
            break;
        case 'x':
        case '+':
        case '-':
        case '^':
        case '*': 
            break;
        default:
            if(node->data >= '0' && node->data <= '9'){
                node->val = node->data - '0';
            }
            break;
    }
    
    // printf("Node %c: inh = %d, val = %d\n", node->data, node->inh, node->val);
}

int main(){
    yyparse();
    setatt(treeroot,POSITIVE);
    printAnnotatedTree(treeroot, 0);
    // printParsetree(treeroot, 0);
    return 0;
}