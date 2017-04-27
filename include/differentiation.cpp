#include "diff.h"


#define NEWNODE(parent, oper)                        \
        new Node(parent, makeOperPtr(oper), OPERAND) 

Node* diff::differentiation(Node* node, Node* Parent) {
    if(node == NULL) return NULL;
    Node* diffNode = NULL;
    
    switch(node->type_) {
//---------------------------------------------------------------//        
        case NUMBER:
            diffNode = new Node(Parent, 0.0, NUMBER);
            break;
//---------------------------------------------------------------//
#define LEFTDIFF differentiation(node->left_, diffNode)
#define RIGHTDIFF differentiation(node->right_, diffNode);
        case VARIABLE:
            diffNode = new Node(Parent, 1, NUMBER);
            diffNode->left_ = LEFTDIFF; 
            diffNode->right_ = LEFTDIFF;
            break;

        case OPERAND:
            switch(node->dataStr_[0]) {
                case '+': {
                diffNode = NEWNODE(Parent, '+');
                diffNode->left_ = LEFTDIFF;
                diffNode->right_ = RIGHTDIFF;
                break;
                }
                case '-': {
                diffNode = NEWNODE(Parent, '-');
                diffNode->left_ = LEFTDIFF;
                diffNode->right_ = RIGHTDIFF;
                break;
                }
#undef LEFTDIFF
#undef RIGHTDIFF 
//---------------------------------------------------------------//
#define LEFTDIFF differentiation(node->left_, diffNode->left_)
#define RIGHTDIFF differentiation(node->right_, diffNode->right_)
#define LEFTCOPY copyTree(node->left_, diffNode->right_)
#define RIGHTCOPY copyTree(node->right_, diffNode->left_)
                case '*': {
                
                diffNode = NEWNODE(Parent, '+');
                
                diffNode->left_ = NEWNODE(diffNode, '*');
                diffNode->right_ = NEWNODE(diffNode, '*');
 
                (diffNode->left_)->left_ = LEFTDIFF;
                (diffNode->left_)->right_ = RIGHTCOPY;
                
                (diffNode->right_)->left_ = LEFTCOPY;
                (diffNode->right_)->right_ = RIGHTDIFF; 
                
                break;
                }
#undef LEFTDIFF
#undef RIGHTDIFF
#undef LEFTCOPY
#undef RIGHTCOPY
//--------------------------------------------------------------//
#define LEFTDIFF differentiation(node->left_, diffNode->left_->left_)
#define RIGHTDIFF differentiation(node->right_, diffNode->left_->right_)
#define RIGHTCOPY copyTree(node->right_, diffNode->left_->left_)
#define LEFTCOPY copyTree(node->left_, diffNode->left_->right_)
                case '/': {
                diffNode = NEWNODE(Parent, '/');
                diffNode->left_ = NEWNODE(diffNode, '-');
                (diffNode->left_)->left_ = NEWNODE(diffNode, '*');
                ((diffNode->left_)->left_)->left_ = LEFTDIFF;
                ((diffNode->left_)->left_)->right_ = RIGHTCOPY;
                                
                (diffNode->left_)->right_= NEWNODE(diffNode,'*');
                ((diffNode->left_)->right_)->left_ = LEFTCOPY;
                ((diffNode->left_)->right_)->right_ = RIGHTDIFF;

                diffNode->right_= NEWNODE(diffNode, '^');
                diffNode->right_->left_ = copyTree(node->right_, diffNode->right_);
                diffNode->right_->right_ = new Node(diffNode->left_, 2, NUMBER);
                break;
                }
#undef LEFTCOPY
#undef LEFTDIFF
#undef RIGHTCOPY
#undef RIGHTDIFF
//-------------------------------------------------------------//
#define RIGHTCOPY copyTree(node->right_, diffNode->left_->left_)
#define LEFTCOPY copyTree(node->left_, diffNode->left_->left_)
#define LEFTDIFF differentiation(node->left_, diffNode)
#define NEWPOWNODE                                                  \
    new Node(diffNode->left_, (node->right_)->dataNum_ - 1, NUMBER);
                case '^': {
                diffNode = NEWNODE(Parent,'*');
                diffNode->left_ = NEWNODE(diffNode,'^');
                diffNode->left_->left_ = NEWNODE(diffNode->left_,'*');
                ((diffNode->left_)->left_)->left_ = RIGHTCOPY;
                ((diffNode->left_)->left_)->right_= LEFTCOPY;

                (diffNode->left_)->right_ = NEWPOWNODE;
                diffNode->right_ = LEFTDIFF;
                break;
                }
#undef RIGHTCOPY
#undef LEFTCOPY
#undef LEFTDIFF
//-------------------------------------------------------------//
                default:
                    printf("Unknown oper\n");
                    printf("%s\n", node->dataStr_);
                    exit(1);
            }
            break;

#define COPYLEFT copyTree(node->left_, diffNode->left_)
#define LEFTDIFF differentiation(node->left_, diffNode)
        case FUNCTION:
            if(!strcmp(node->dataStr_, "ln")) {
                diffNode = NEWNODE(Parent, '*');
                diffNode->left_ = NEWNODE(diffNode, '/');
                (diffNode->left_)->left_ = 
                            new Node(diffNode->left_, 1, NUMBER);
                (diffNode->left_)->right_ = COPYLEFT;
                diffNode->right_ = LEFTDIFF;
            }
            break;
#undef COPYLEFT
#undef LEFTDIFF
//-------------------------------------------------------------//
        default: 
            printf("Unknown type\n");
    }
    return diffNode;
}
#undef NEWNODE
