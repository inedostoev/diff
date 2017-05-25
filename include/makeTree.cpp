#include "diff.h"                                           

Node* diff::Counting(Node* parent) {
    Node* val = getSubAdd(parent);
    //assert(str_[counter_] == '\0');
    return val;
}

Node* diff::getSubAdd(Node* parent) {
    Node* newNode = getMulDiv(parent);
    while(str_[counter_] == '+' || str_[counter_] == '-') {
        int op = str_[counter_];
        counter_++;
        Node* rightNode = getMulDiv(parent);
        if(op == '+') {
            Node* leftNode = newNode;
            newNode = new Node(parent, makeOperPtr('+'), OPERAND);
            newNode->left_ = leftNode;
            newNode->right_ = rightNode;
            leftNode->parent_ = newNode;
            rightNode->parent_ = newNode;
        }
       else {
            Node* leftNode = newNode;
            newNode = new Node(parent, makeOperPtr('+'), OPERAND);
            newNode->left_ = leftNode;
            newNode->right_ = rightNode;
            leftNode->parent_ = newNode;
            rightNode->parent_ = newNode;
        }
    }
    return newNode;
}

Node* diff::getMulDiv(Node* parent) {
    Node* newNode = getPower(parent);  
    while(str_[counter_] == '*' || str_[counter_] == '/') {
        int op = str_[counter_];
        counter_++;
        Node* rightNode = getPower(parent);
        if(op == '*') {
            Node* leftNode = newNode;
            newNode = new Node(parent, makeOperPtr('*'), OPERAND);
            newNode->left_ = leftNode;
            newNode->right_ = rightNode;
            leftNode->parent_ = newNode;
            rightNode->parent_ = newNode;
        }
        else {
            Node* leftNode = newNode;
            newNode = new Node(parent, makeOperPtr('/'), OPERAND);
            newNode->left_ = leftNode;
            newNode->right_ = rightNode;
            leftNode->parent_ = newNode;
            rightNode->parent_ = newNode;
        }
    }
    return newNode;
}

Node *diff::getPower(Node* parent) {
    Node* newNode = getBracketsFunc(parent);  
    while(str_[counter_] == '^') {
        int op = str_[counter_];
        counter_++;
        Node* rightNode = getBracketsFunc(parent);
        Node* leftNode = newNode;
        newNode = new Node(parent, makeOperPtr('^'), OPERAND);
        newNode->left_ = leftNode;
        newNode->right_ = rightNode;
        leftNode->parent_ = newNode;
        rightNode->parent_ = newNode;
    }
    return newNode;
}

Node* diff::getBracketsFunc(Node* parent) {
    if (str_[counter_] == '(') {
        counter_++;
        Node* val = getSubAdd(parent);
        assert(str_[counter_] == ')');
        counter_++;
        return val;
    }
    else if (str_[counter_] == 'x') {
        counter_++;
        Node* newNode = new Node(parent, makeOperPtr('x'), VARIABLE);
        newNode->left_ = NULL;
        newNode->right_ = NULL;
        return newNode;
    }
    else if (str_[counter_] == 'l' && str_[counter_ + 1] == 'n') {
        counter_ += 2;
        char* funcPtr = (char*)calloc(3, sizeof(char));
        funcPtr[0] = 'l';
        funcPtr[1] = 'n';
        funcPtr[2] = '\0';
        Node* newNode = new Node(parent, funcPtr, FUNCTION);
        newNode->left_ = getSubAdd(newNode);
        newNode->right_ = NULL;
        return newNode;
    }
   
    else return getNumber(parent);
}

Node* diff::getNumber(Node* parent) {
    double val = 0.0;
    while('0' <= str_[counter_] && str_[counter_] <= '9') {
        val = val * 10 + str_[counter_] - '0';
        counter_++;
    }
    Node* newNode = new Node(parent, val, NUMBER);
    newNode->left_ = NULL;
    newNode->right_ = NULL;
    return newNode;
}
