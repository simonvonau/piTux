
#ifndef __ARRAYPROCESSING_H__
#define __ARRAYPROCESSIN_H__
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../model/block.h"

int* initIntArray(int pSize,int defaultVal);
char ** splitString(char* str,char separator,int sizeMax,int returnArrayMaxQty,int returnMaxSize);
#endif
