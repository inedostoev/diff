#include "Node.h"

Node::Node(Node* parent, double data, dataType type) :
    parent_     (parent),
    dataStr_    (NULL),
    dataNum_    (data),
    type_       (type),
    left_       (NULL),
    right_      (NULL)
{}

Node::Node(Node* parent, char* data, dataType type) :
    parent_	(parent),
    dataStr_	(data),
    dataNum_	(std::numeric_limits<double>::quiet_NaN()),
    type_	(type),
    left_	(NULL),
    right_	(NULL)
{}

Node::~Node() {
	if (dataStr_ == NULL) dataNum_ = -666;
	//if (dataNum_ == NAN) free(dataStr_);
    else free(dataStr_);
    if(left_) delete left_;
    if(right_) delete right_;
    parent_ = left_ = right_ = NULL;
    type_ = ERROR;
}

bool Node::nodeOk() const{
    return (type_ != ERROR);
}

void Node::dotDumpNode(FILE* stream) const{ 
    fprintf(stream, "[label=\""
                    "treeNode_[%p]\\l",
                    this);
    fprintf(stream, "{\\l");
    fprintf(stream, "  parent_ [%p]\\l", parent_);
    if(dataStr_ == NULL) 
        fprintf(stream, "  data_[%p] = %lg\\l", &dataNum_, dataNum_);
    else 
        fprintf(stream, "  data_ [%p] = %s\\l", dataStr_, dataStr_);   
    fprintf(stream, "  type_ = %i\\l", type_);
    fprintf(stream, "  left_ [%p]\\l", left_);
    fprintf(stream, "  right_ [%p]\\l", right_); 
    fprintf(stream, "}\\l");
    fprintf(stream, "\"]\n");

}

void Node::dumpNode(FILE* stream) const{
    if(stream == NULL) {
        printf("\tdumpNode stream == NULL\n");
        return;
    }
    if (this == NULL) return;    
    fprintf(stream, "treeNode[%p]\n", this);
    fprintf(stream, "{\n");
    fprintf(stream, "  parent_ = %p;\n", parent_);
	if (dataStr_ == NULL) 
        fprintf(stream, "  data_[%p] = '%lg';\n", &dataNum_, dataNum_); 
	else
        fprintf(stream, "  data_[%p] = '%s';\n", dataStr_, dataStr_);
    fprintf(stream, "  dataType_ = %d", type_);
    fprintf(stream, "  left_ = %p;\n", left_);
    fprintf(stream, "  right_ = %p;\n", right_);
    fprintf(stream, "}\n");
}
