#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include <stdio.h> 
#include <stdlib.h>
#include <limits>

enum dataType {
	NUMBER,
	VARIABLE,
	OPERAND,
	FUNCTION,
	ERROR
};

class Node
{
public:
                    Node(Node* parent, double data, dataType type);
		    Node(Node* parent, char* data, dataType type);
		    ~Node();
	
    bool            nodeOk() const;
    void            dumpNode(FILE* stream) const;     
    void            dotDumpNode(FILE* stream) const;
    
    Node*           parent_;
    char*           dataStr_;
    double          dataNum_;
    dataType        type_; 
    Node*           left_;
    Node*           right_;

    Node&           operator = (const Node&);
                    Node(const Node&);
};

#endif // !NODE_H_INCLUDED
