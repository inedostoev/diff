#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include "./Node/Node.h"

const int MAX_OPER_OR_FUNC_SIZE = 16;
enum Traverse {
    INORDER,
    PREORDER,
    POSTORDER
};

enum Priority {
    LOW_PRIOR,
    ADD_AND_SUB_PRIOR,
    MUL_AND_DIV_PROIR,
    EXP_PRIOR,
};

class diff
{
public:
                    diff();
                    ~diff();
    FILE            *inputFile_;
    FILE            *outputFile_;
    
	diff& operator= (const diff&);
    		        diff(const diff&);
    void            execute();
                
private:
    Node*           Tree_; 

    Node*           Counting(Node* parent);
    Node*           getSubAdd(Node* parent);
    Node*           getMulDiv(Node* parent);
    Node*           getBracketsFunc(Node* parent);
    Node*           getNumber(Node* parent);
    Node*           getPower(Node* parent);
    
    char*           str_;
    size_t          counter_;

    Node*           buildTree(char* buffer, size_t *counter, Node* Darth_Vader);
    Node*           copyTree(Node* node, Node* parent);
    Node*           differentiation(Node* node, Node* Parent);
    Node*           transformDiffTree(Node* diffNode, Node* parent);
    
    Node*           transformTree(Node* transformTree);
    bool            transformFlag;
    
    dataType        getDataType(char* data);
    size_t          getFileSize();

    char*           makeFuncPtr(char* func);

    void            deleteNode(Node* node);
    void            deleteTree(Node* node);
    char*           copyStr(char* copyStr);
    char*           makeOperPtr(char Oper);
    void            dumpTree(FILE* stream, Node* node);
    void            dotDump(Node* root);
    void            dotNodeDump(Node* root, FILE* stream);
    void            visitor(Traverse mode, Node* node, void act(Node*));
    void            makeTex(Node* root, Node* first);
    void            makeTexNodeDump(Node* node, FILE* stream);
    Priority        getPriority(Node* node);
};

#endif // !TREE_H_INCLUDED
