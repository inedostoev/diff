#define _CRT_SECURE_NO_WARNINGS

#include "diff.h"

diff::diff() :
    inputFile_      (NULL),
    outputFile_     (NULL),
    Tree_           (NULL)
{
    inputFile_ = fopen("configFile.txt", "r");
	if (inputFile_ == NULL) {
		printf("Can't open inputFile\n");
		exit(1);
	}
    size_t fileSize = getFileSize();
    char *buffer = (char*)calloc(fileSize, sizeof(char));
    fread(buffer, sizeof(char), fileSize, inputFile_);    
    size_t counter = 0;
    Tree_ = buildTree(buffer, &counter, NULL);
	if (Tree_ == NULL) {
		printf("Can't build tree\n");
		exit(1);
	}
    free(buffer);
    fclose(inputFile_);
}

diff::~diff() {
    delete Tree_;
    Tree_ = NULL; 
}

//Darth_Vader = parent;
//Luke        = son;

Node* diff::buildTree(char* buffer, size_t *counter, Node* Darth_Vader) {
    if(buffer[(*counter) + 1] == ')') {
       (*counter) += 2;
        return NULL;
    }
    (*counter)++;
    int len = -1;
    char* data = (char*)malloc(MAX_OPER_OR_FUNC_SIZE * sizeof(char));
    if (data == NULL) {
        printf("Can't allocate memory\n");
    }
    sscanf(buffer + (*counter), "%[^(]%n", data, &len);
    (*counter) += (size_t)len;
	dataType type = getDataType(data);
	Node* Luke = NULL;
	if (type == NUMBER) {
        Luke = new Node(Darth_Vader, atof(data), type);
        free(data);
    }
	else Luke = new Node(Darth_Vader, data, type);
	if (Luke == NULL) {
		printf("Error with Luke\n");
		exit(1);
	}
	Luke->left_ = buildTree(buffer, counter, Luke);
    Luke->right_ = buildTree(buffer, counter, Luke);
    (*counter)++;
    return Luke; 
}

dataType diff::getDataType(char* data) {						//дописать конец, сделать с макросами
	if ((data[0] == '+' || data[0] == '-') && data[1] == '\0')
		return OPERAND;
	if ((data[0] == '*' || data[0] == '/' || data[0] == '^') && data[1] == '\0')
		return OPERAND;
    if (!strcmp("ln", data))
        return FUNCTION;
	if (!strcmp("sin", data))
		return FUNCTION;
	if (!strcmp("cos", data))
		return FUNCTION;
	if (!strcmp("sin", data))
		return FUNCTION;
	if (!strcmp("x", data))
		return VARIABLE;
	return NUMBER;
}


size_t diff::getFileSize() {
    fseek(inputFile_, 0, SEEK_END);
    size_t fileSize = (size_t)ftell(inputFile_) + 1;
	if (fileSize <= 1) {
		printf("configFile is empty\n");
		exit(1);
	}
    rewind(inputFile_);
    return fileSize;
}

Node* diff::copyTree(Node* node, Node* parent) {
    if(node == NULL) return NULL;
    Node* newNode = NULL;
    if(node->dataStr_ == NULL)
        newNode = new Node(parent, node->dataNum_, node->type_);
    else { 
        char* data = (char*)calloc(MAX_OPER_OR_FUNC_SIZE, sizeof(char));
        copyStr(node, data);
        newNode = new Node(parent, data, node->type_);
    }
    newNode->left_ = copyTree(node->left_, newNode);
    newNode->right_ = copyTree(node->right_, newNode);
    return newNode;
}

void diff::copyStr(Node* node, char* copyStr) {
    int i = 0;
    while(node->dataStr_[i] != '\0') {
        copyStr[i] = node->dataStr_[i];
        i++;
    }
}

char *diff::makeOperPtr(char Oper) {
    char* tmpOper = (char*)malloc(2 * sizeof(char));
    tmpOper[0] = Oper;
    tmpOper[1] = '\0';
    return tmpOper;
}

void diff::execute() {
    Node* copTree = copyTree(Tree_, NULL);
    dotDump(copTree);
    dumpTree(stdout, copTree);
    Node* diffTree = differentiation(copTree, NULL); 
    dotDump(diffTree);
    Node* trfmDiffTree = transformDiffTree(diffTree, NULL);
    dotDump(trfmDiffTree);
    Node* tmp = transformDiffTree(trfmDiffTree, NULL);
    dotDump(tmp);
    makeTex(tmp, copTree);
    delete copTree;
    delete diffTree;
    delete trfmDiffTree;
}

Node* diff::differentiation(Node* node, Node* Parent) {
    if(node == NULL) return NULL;
    Node* diffNode = NULL;
    switch(node->type_) {
        case NUMBER:
            diffNode = new Node(Parent, 0.0, NUMBER);
            break;
        case VARIABLE:
            diffNode = new Node(Parent, 1, NUMBER);
            diffNode->left_ = differentiation(node->left_, diffNode);
            diffNode->right_ = differentiation(node->right_, diffNode);
            break;
//#define LEFTDIFF differentiation(node->left_, diffNode->left_)
//#define LEFTCOPY 
        case OPERAND:
            switch(node->dataStr_[0]) {
                case '+':
                case '-': {
                char* data = (char*)calloc(MAX_OPER_OR_FUNC_SIZE, sizeof(char));
                copyStr(node, data);
                diffNode = new Node(Parent, data, OPERAND);
                diffNode->left_ = differentiation(node->left_, diffNode);
                diffNode->right_ = differentiation(node->right_, diffNode);
                break;
                }
                case '*': {
                diffNode = new Node(Parent, makeOperPtr('+'), OPERAND);
                diffNode->left_ = new Node(diffNode, makeOperPtr('*'), OPERAND);
                diffNode->right_ = new Node(diffNode, makeOperPtr('*'), OPERAND);
                
                (diffNode->left_)->left_ = differentiation(node->left_, diffNode->left_);
                (diffNode->left_)->right_ = copyTree(node->right_, diffNode->left_);
                
                (diffNode->right_)->left_ = copyTree(node->left_, diffNode->right_);
                (diffNode->right_)->right_ = differentiation(node->right_, diffNode->right_);
                break;
                }
                case '/': {
                diffNode = new Node(Parent, makeOperPtr('/'), OPERAND);
                diffNode->left_ = new Node(diffNode, makeOperPtr('-'), OPERAND);
                (diffNode->left_)->left_ = new Node(diffNode, makeOperPtr('*'), OPERAND);
                ((diffNode->left_)->left_)->left_ = differentiation(node->left_, (diffNode->left_)->left_);
                ((diffNode->left_)->left_)->right_ = copyTree(node->right_, (diffNode->left_)->left_);
                                
                (diffNode->left_)->right_= new Node(diffNode, makeOperPtr('*'), OPERAND);
                ((diffNode->left_)->right_)->left_ = copyTree(node->left_, (diffNode->left_)->right_);
                ((diffNode->left_)->right_)->right_ = differentiation(node->right_,  (diffNode->left_)->right_);

                diffNode->right_= new Node(diffNode, makeOperPtr('^'), OPERAND);
                (diffNode->right_)->left_ = copyTree(node->right_, diffNode->right_);
                (diffNode->right_)->right_ = new Node(diffNode->left_, 2, NUMBER);
                break;
                }
                case '^': {
                diffNode = new Node(Parent, makeOperPtr('*'), OPERAND);
                diffNode->left_ = new Node(diffNode, makeOperPtr('^'), OPERAND);
                (diffNode->left_)->left_ = new Node(diffNode->left_, makeOperPtr('*'), OPERAND);
                ((diffNode->left_)->left_)->left_ = copyTree(node->right_, (diffNode->left_->left_));
                ((diffNode->left_)->left_)->right_= copyTree(node->left_, (diffNode->left_)->left_);

                (diffNode->left_)->right_ = new Node(diffNode->left_, (node->right_)->dataNum_ - 1, NUMBER);
                diffNode->right_ = differentiation(node->left_, diffNode);
                }
                default:
                    printf("Unknown oper\n");
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

Priority diff::getPriority(Node* node) {
    if(node == NULL || node->dataStr_ == NULL) return LOW_PRIOR;
    if(!strcmp(node->dataStr_, "+") || !strcmp(node->dataStr_, "-")) return ADD_AND_SUB_PRIOR;
    if(!strcmp(node->dataStr_, "*") || !strcmp(node->dataStr_, "*")) return MUL_AND_DIV_PROIR;
    if(!strcmp(node->dataStr_, "^")) return EXP_PRIOR;
    return LOW_PRIOR;
}

void diff::makeTex(Node* root, Node* firstNode) {
    FILE* stream = fopen("diff.tex", "w");
    if (stream == NULL) {
        printf("Error with diff.tex\n");
        exit(1);
    }
    fprintf(stream, "\\documentclass{article}\n");
    fprintf(stream, "\\usepackage[a4paper, margin=6mm]{geometry}\n");
    fprintf(stream, "\\usepackage{amsmath}\n");
    fprintf(stream, "\\usepackage[utf8x]{inputenc}\n");
    fprintf(stream, "\\usepackage[russian]{babel}\n");
    fprintf(stream, "\\usepackage{hyperref}\n");
    fprintf(stream, "\\title{Берем производную без смс и регистрации}\n");
    fprintf(stream, "\\begin{document}\n");
    fprintf(stream, "\\maketitle\n");
    fprintf(stream, "\\begin{eqnarray}\n");
    fprintf(stream, "\\left( ");
    makeTexNodeDump(firstNode, stream);
    fprintf(stream, "\\right) ' = \n");
    makeTexNodeDump(root, stream);
    printf("\n");
    fprintf(stream, "\\end{eqnarray}\n");
    fprintf(stream, "\\end{document}\n");
    fclose(stream);
    system("pdflatex diff.tex");
    system("evince diff.pdf");
    stream = NULL;
}

void diff::makeTexNodeDump(Node* node, FILE* stream) {
        if (node == NULL) return;
        if(node->type_ == FUNCTION) {
            fprintf(stream, "%s", node->dataStr_);
            fprintf(stream, "\\left(");
            makeTexNodeDump(node->left_, stream);
            makeTexNodeDump(node->right_, stream);
            fprintf(stream, "\\right)");
        }
        if(node->type_ == OPERAND) {
           if(!strcmp(node->dataStr_, "/")) {
                fprintf(stream, "\\frac{");
                makeTexNodeDump(node->left_, stream);
                fprintf(stream, "}{");
                makeTexNodeDump(node->right_, stream);
                fprintf(stream, "}");
           } else
           if(!strcmp(node->dataStr_, "^")) {
                fprintf(stream, "{");
                makeTexNodeDump(node->left_, stream);
                fprintf(stream, "}^{");
                makeTexNodeDump(node->right_, stream);
                fprintf(stream, "}");
           }
           else {
               if(!strcmp(node->dataStr_, "-")) {
                    fprintf(stream, "{");
                    makeTexNodeDump(node->left_, stream);
                    fprintf(stream, "}%s\\left({", node->dataStr_);
                    makeTexNodeDump(node->right_, stream);
                    fprintf(stream, "}\\right)"); 
               }
               else
               if(getPriority(node->parent_) > getPriority(node)) {
                    fprintf(stream, "\\left({");
                    makeTexNodeDump(node->left_, stream);
                    fprintf(stream, "%s", node->dataStr_);
                    makeTexNodeDump(node->right_, stream);
                    fprintf(stream, "}\\right)");
               }
                else {
                    fprintf(stream, "{");
                    makeTexNodeDump(node->left_, stream);
                    fprintf(stream, "%s", node->dataStr_);
                    makeTexNodeDump(node->right_, stream);
                    fprintf(stream, "}");
                }
            }       
        }
        if(node->type_ == VARIABLE) {
            fprintf(stream, "{%s}", node->dataStr_);
            makeTexNodeDump(node->left_, stream);
            makeTexNodeDump(node->right_, stream);
        }
        if(node->type_ == NUMBER) {
            fprintf(stream, "{%lg}", node->dataNum_);
            makeTexNodeDump(node->left_, stream);
            makeTexNodeDump(node->right_, stream);
        }
}

void diff::dumpTree(FILE* stream, Node* node) {
    if(node == NULL) return;
    node->dumpNode(stream);
    dumpTree(stream, node->left_);
    dumpTree(stream, node->right_);
}

void diff::dotDump(Node* root) {
    FILE* ptrFile = fopen("dumpFile.gv", "w");
    if (ptrFile == NULL) {
        printf("Error with dumpFile.gv\n");
        exit(1);
    }
    fprintf(ptrFile, "digraph graf {\n");
    dotNodeDump(root, ptrFile);
    fprintf(ptrFile, "}");
    fclose(ptrFile);
    ptrFile = NULL;
    system("dot dumpFile.gv -Tpng -o dumpFile.png");
    system("xdot dumpFile.gv");
}

void diff::dotNodeDump(Node *root, FILE* stream) {
    fprintf(stream, "treeNode_%p ", root);
    root->dotDumpNode(stream);
    if(root->left_) {
        fprintf(stream, "treeNode_%p->treeNode_%p\n", root, root->left_);
        dotNodeDump(root->left_, stream);
    }
    if(root->right_) {
        fprintf(stream, "treeNode_%p->treeNode_%p\n", root, root->right_);
        dotNodeDump(root->right_, stream);
    }
}

void diff::visitor(Traverse mode, Node* node, void act(Node*)) {
    switch(mode) {
        case INORDER:
            visitor(mode, node->left_, act);
            act(node);
            visitor(mode, node->right_, act);
            break;
        case PREORDER:                          
            act(node);
            visitor(mode, node->left_, act);
            visitor(mode, node->right_, act);
            break;
        case POSTORDER:
            visitor(mode, node->right_, act);
            act(node);
            visitor(mode, node->left_, act);
            break;
        default:
            printf("Unknown Traverse mode\n");
    }
}
