#include "diff.h"

Node* diff::transformDiffTree(Node* diffNode, Node* parent) {
    if(diffNode == NULL) return NULL;
    Node* newDiffNode = NULL;
    if(diffNode->dataStr_ != NULL) {
        if(!strcmp(diffNode->dataStr_, "*") 
                && (diffNode->right_->dataNum_ == 0 || diffNode->left_->dataNum_ == 0)) {
            newDiffNode = new Node(parent, 0.0, NUMBER); 
            newDiffNode->left_ = NULL;
            newDiffNode->right_= NULL;
            return transformDiffTree(newDiffNode, parent);
        }
        if(!strcmp(diffNode->dataStr_, "*") && diffNode->right_->dataNum_ == 1) {
            newDiffNode = copyTree(diffNode->left_, parent);       
            return transformDiffTree(newDiffNode, parent);
        }
        else
        if(!strcmp(diffNode->dataStr_, "*") && diffNode->left_->dataNum_ == 1) {
            newDiffNode = copyTree(diffNode->right_, parent);
            return transformDiffTree(newDiffNode, parent);
        }
        else
        if(!strcmp(diffNode->dataStr_, "+") && diffNode->left_->dataNum_ == 0) {
            newDiffNode = copyTree(diffNode->right_, parent);
            return transformDiffTree(newDiffNode, parent);
        }   
        else
        if(!strcmp(diffNode->dataStr_, "+") && diffNode->right_->dataNum_ == 0) {
            newDiffNode = copyTree(diffNode->left_, parent);
            return transformDiffTree(newDiffNode, parent);
        } 
        else
        if(!strcmp(diffNode->dataStr_, "-") && diffNode->right_->dataNum_ == 0) {
            newDiffNode = copyTree(diffNode->left_, parent);
            return transformDiffTree(newDiffNode, parent);
        }
        
    }
    if(diffNode->dataStr_ == NULL)
       newDiffNode  = new Node(parent, diffNode->dataNum_, diffNode->type_);
    else { 
        char* data = (char*)calloc(MAX_OPER_OR_FUNC_SIZE, sizeof(char));
        copyStr(diffNode, data);
        newDiffNode = new Node(parent, data, diffNode->type_);
    }
    newDiffNode->left_ = transformDiffTree(diffNode->left_, newDiffNode);
    newDiffNode->right_ = transformDiffTree(diffNode->right_, newDiffNode);
    return newDiffNode;  
}

