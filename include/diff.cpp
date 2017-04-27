#define _CRT_SECURE_NO_WARNINGS

#include "diff.h"

diff::diff() :
    inputFile_      (NULL),
    outputFile_     (NULL),
    Tree_           (NULL)
{
    inputFile_ = fopen("./input/configFile.txt", "r");
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

Priority diff::getPriority(Node* node) {
    if(node == NULL || node->dataStr_ == NULL) return LOW_PRIOR;
    if(!strcmp(node->dataStr_, "+") || !strcmp(node->dataStr_, "-")) return ADD_AND_SUB_PRIOR;
    if(!strcmp(node->dataStr_, "*") || !strcmp(node->dataStr_, "*")) return MUL_AND_DIV_PROIR;
    if(!strcmp(node->dataStr_, "^")) return EXP_PRIOR;
    return LOW_PRIOR;
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
