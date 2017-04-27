#include "diff.h"

#define NEWNODE(parent, oper)                        \
        new Node(parent, makeOperPtr(oper), OPERAND) 

Node* diff::differentiation(Node* node, Node* Parent) {
    if(node == NULL) return NULL;
    Node* diffNode = NULL;
    switch(node->type_) {
        case NUMBER:
            diffNode = new Node(Parent, 0.0, NUMBER);
            break;
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
#undef LEFTDIFF
#undef RIGHTDIFF
#undef LEFTCOPY
#undef RIGHTCOPY
                }
                case '/': {
                diffNode = NEWNODE(Parent, '/');
                diffNode->left_ = NEWNODE(diffNode, '-');
                (diffNode->left_)->left_ = NEWNODE(diffNode, '*');
                ((diffNode->left_)->left_)->left_ = differentiation(node->left_, (diffNode->left_)->left_);
                ((diffNode->left_)->left_)->right_ = copyTree(node->right_, (diffNode->left_)->left_);
                                
                (diffNode->left_)->right_= NEWNODE(diffNode,'*');
                ((diffNode->left_)->right_)->left_ = copyTree(node->left_, (diffNode->left_)->right_);
                ((diffNode->left_)->right_)->right_ = differentiation(node->right_,  (diffNode->left_)->right_);

                diffNode->right_= NEWNODE(diffNode, '^');
                (diffNode->right_)->left_ = copyTree(node->right_, diffNode->right_);
                (diffNode->right_)->right_ = new Node(diffNode->left_, 2, NUMBER);
                break;
                }
                case '^': {
                diffNode = NEWNODE(Parent,'*');
                diffNode->left_ = NEWNODE(diffNode,'^');
                (diffNode->left_)->left_ = NEWNODE(diffNode->left_,'*');
                ((diffNode->left_)->left_)->left_ = copyTree(node->right_, (diffNode->left_->left_));
                ((diffNode->left_)->left_)->right_= copyTree(node->left_, (diffNode->left_)->left_);

                (diffNode->left_)->right_ = new Node(diffNode->left_, (node->right_)->dataNum_ - 1, NUMBER);
                diffNode->right_ = differentiation(node->left_, diffNode);
                }
                default:
                    printf("Unknown oper\n");
                    exit(1);
            }
            break;
        case FUNCTION:
            if(!strcmp(node->dataStr_, "ln")) {
                diffNode = new Node(Parent, makeOperPtr('*'), OPERAND);
                diffNode->left_ = new Node(diffNode, makeOperPtr('/'), OPERAND);
                (diffNode->left_)->left_ = new Node(diffNode->left_, 1, NUMBER);
                (diffNode->left_)->right_ = copyTree(node->left_, diffNode->left_);
                diffNode->right_ = differentiation(node->left_, diffNode);
            }
            break;
        default: 
            printf("Unknown type\n");
    }
    return diffNode;
}
#undef NEWNODE

