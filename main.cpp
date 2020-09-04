#define _CRT_SECURE_NO_WARNINGS
#define NDEBUG

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./include/diff.h"

int main()
{   
	//FILE* stream = fopen("configFile.txt", "r");
	diff d;
	d.execute();
	return 0;
}
