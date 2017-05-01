#include "diff.h"
#include <math.h>
const double accuracy = 0.001;

Node* diff::transformDiffTree(Node* diffNode, Node* parent) {
    if(diffNode == NULL) return NULL;
    Node* newDiffNode = NULL;    
//*********************************************************//
#define RIGHTCOPY copyTree(diffNode->right_, parent);
#define LEFTCOPY copyTree(diffNode->left_, parent);
#define LEFTZEROEQUAL                                   \
        fabs(diffNode->left_->dataNum_) < accuracy 
#define RIGHTZEROEQUAL                                  \
        fabs(diffNode->right_->dataNum_) < accuracy
    if(diffNode->dataStr_ != NULL) {
#define MUL !strcmp(diffNode->dataStr_, "*")
//---------------------------------------------------------//
    if(MUL && (LEFTZEROEQUAL || RIGHTZEROEQUAL)) {
        Node* newTrfNode = new Node(parent, 0.0, NUMBER); 
        newTrfNode->left_ = NULL;
        newTrfNode->right_= NULL;
        return newTrfNode;
    }
//--------------------------------------------------------//
#define RIGHTONEEQUAL                                   \
        fabs(diffNode->right_->dataNum_ - 1) < accuracy 
        if(MUL && RIGHTONEEQUAL) {
           Node* newTrfNode = LEFTCOPY;       
            return newTrfNode;
        }
#undef RIGHTONEEQUAL
//--------------------------------------------------------//
#define LEFTONEEQUAL                                    \
        fabs(diffNode->left_->dataNum_ - 1) < accuracy
        else
        if(MUL && LEFTONEEQUAL) {
            Node* newTrfNode = RIGHTCOPY;
            return newTrfNode;
        }
#undef LEFTONEEQUAL
#undef MUL        
//--------------------------------------------------------//
#define ADD !strcmp(diffNode->dataStr_, "+")  
        else
        if(ADD && LEFTZEROEQUAL) {
            Node* newTrfNode = RIGHTCOPY;
            return newTrfNode;
        }   
//--------------------------------------------------------//
        else
        if(ADD && RIGHTZEROEQUAL) {
            Node* newTrfNode = LEFTCOPY;
            return newTrfNode;
        } 
#undef ADD
//-------------------------------------------------------//
#define SUB !strcmp(diffNode->dataStr_, "-")    
        else
        if(SUB && RIGHTZEROEQUAL) {
            Node* newTrfNode = LEFTCOPY;
            return newTrfNode;
        }
//-------------------------------------------------------//
        else
        if(SUB && LEFTZEROEQUAL) {
        Node* newTrfNode = 
            new Node(parent, makeOperPtr('*'), OPERAND); 
        newTrfNode->left_ = 
            new Node(newTrfNode, -1, NUMBER);
        newTrfNode->right_ = 
            copyTree(diffNode->right_, newTrfNode);
        return newTrfNode;       
        }
#undef SUB
//-------------------------------------------------------//
    }
#undef LEFTZEROEQUAL
#undef RIGHTZEROEQUAL
#undef RIGHTCOPY
#undef LEFTCOPY
//*******************************************************//
#define NEWNODE(arg)                        \
    new Node(parent, arg, diffNode->type_)
#define TRANSFORM(son)                              \
    transformDiffTree(son, newDiffNode)

    if(diffNode->dataStr_ == NULL)
        newDiffNode  = NEWNODE(diffNode->dataNum_);
    else 
        newDiffNode = NEWNODE(copyStr(diffNode->dataStr_));
    newDiffNode->left_ = TRANSFORM(diffNode->left_);
    newDiffNode->right_ = TRANSFORM(diffNode->right_);
    return newDiffNode;  
}
#undef TRANSFORM
#undef NEWNODE
