#include "diff.h"

void diff::makeTex(Node* root, Node* firstNode) {
    FILE* stream = fopen("./output/Tex/diff.tex", "w");
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
    int i = 0;
    i = chdir("./output/Tex/");
    i = system("pdflatex diff.tex");
    i = system("evince diff.pdf");
    i = system("mv diff.pdf ../");
    i = chdir("./../");
    if (i != 0) printf("makeTex error\n");
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
    FILE* ptrFile = fopen("./output/Dot/dumpFile.gv", "w");
    if (ptrFile == NULL) {
        printf("Error with dumpFile.gv\n");
        exit(1);
    }
    fprintf(ptrFile, "digraph graf {\n");
    dotNodeDump(root, ptrFile);
    fprintf(ptrFile, "}");
    fclose(ptrFile);
    ptrFile = NULL;
    int i = 0;
    i = chdir("./output/");
    i = system("dot ./Dot/dumpFile.gv -Tpng -o dumpFile.png");
    i = system("xdot ./Dot/dumpFile.gv");
    i = chdir("./../");
    if (i != 0) printf("dotDump error\n");
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
