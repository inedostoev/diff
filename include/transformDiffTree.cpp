#include "diff.h"

Node* diff::transformDiffTree(Node* diffNode, Node* parent) {
    if(diffNode == NULL) return NULL;
    Node* newDiffNode = NULL;
//---------------------------------------------------------//    
    if(diffNode->dataStr_ != NULL) {
        if(!strcmp(diffNode->dataStr_, "*") 
                && (diffNode->right_->dataNum_ == 0 || diffNode->left_->dataNum_ == 0)) {
            Node* newTrfNode = new Node(parent, 0.0, NUMBER); 
            newTrfNode->left_ = NULL;
            newTrfNode->right_= NULL;
            return newTrfNode;
        }

        if(!strcmp(diffNode->dataStr_, "*") && diffNode->right_->dataNum_ == 1) {
           Node* newTrfNode = copyTree(diffNode->left_, parent);       
            return newTrfNode;
        }
        else
        if(!strcmp(diffNode->dataStr_, "*") && diffNode->left_->dataNum_ == 1) {
            Node* newTrfNode = copyTree(diffNode->right_, parent);
            return newTrfNode;
        }
        else
        if(!strcmp(diffNode->dataStr_, "+") && diffNode->left_->dataNum_ == 0) {
            Node* newTrfNode = copyTree(diffNode->right_, parent);
            return newTrfNode;
        }   
        else
        if(!strcmp(diffNode->dataStr_, "+") && diffNode->right_->dataNum_ == 0) {
            Node* newTrfNode = copyTree(diffNode->left_, parent);
            return newTrfNode;
        } 
        else
        if(!strcmp(diffNode->dataStr_, "-") && diffNode->right_->dataNum_ == 0) {
            Node* newTrfNode = copyTree(diffNode->left_, parent);
            return newTrfNode;
        }
//-------------------------------------------------------//
    }
    if(diffNode->dataStr_ == NULL)
       newDiffNode  = new Node(parent, diffNode->dataNum_, diffNode->type_);
    else { 
        newDiffNode = new Node(parent, copyStr(diffNode->dataStr_), diffNode->type_);
    }
    newDiffNode->left_ = transformDiffTree(diffNode->left_, newDiffNode);
    newDiffNode->right_ = transformDiffTree(diffNode->right_, newDiffNode);
    return newDiffNode;  
}

