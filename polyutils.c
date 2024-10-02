#include "polyutils.h"
#include <stdio.h>
#include <stdlib.h>

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

void printAnnotatedTree(Node* root, int depth) {
    if (root == NULL) return;
    
    // Print indentation
    for (int i = 0; i < depth; i++) {
        printf("==>");
    }
    
    // Print node data and relevant attributes
    printf(" %c", root->data);
    
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

void setatt(Node* root) {
    if (root == NULL) return;

    // Set attributes based on node type
    switch (root->data) {
        case 'S':
            // S node doesn't need any attribute setting
            break;
        case 'P':
            // P inherits sign from parent S or P
            if (root->children && root->children->data == '-') {
                root->inh = -1;
            } else {
                root->inh = 1;
            }
            
            // Pass inherited sign to T
            if (root->children && root->children->data == 'T') {
                root->children->inh = root->inh;
            }
            break;
        case 'T':
            // T inherits sign from parent P
            // Already set when processing P
            break;
        case 'N':
            // N computes its value from its children
            root->val = 0;
            Node* child = root->children;
            while (child) {
                if (child->data >= '0' && child->data <= '9') {
                    root->val = root->val * 10 + (child->data - '0');
                }
                child = child->sibling;
            }
            break;
        case 'M':
            // M inherits from parent and computes its value
            if (root->children) {
                if (root->children->data >= '0' && root->children->data <= '9') {
                    root->val = root->inh * 10 + (root->children->data - '0');
                }
            }
            
            // Pass computed value to child M if it exists
            if (root->children && root->children->sibling && root->children->sibling->data == 'M') {
                root->children->sibling->inh = root->val;
            }
            break;
        case 'X':
            // X doesn't need attribute setting
            break;
        default:
            // Handle leaf nodes (digits)
            if (root->data >= '0' && root->data <= '9') {
                root->val = root->data - '0';
            }
            break;
    }

    // Recursively set attributes for children
    setatt(root->children);
    
    // Recursively set attributes for siblings
    setatt(root->sibling);
}

int main(){
    yyparse();
    setatt(treeroot);
    printAnnotatedTree(treeroot, 0);
    return 0;
}