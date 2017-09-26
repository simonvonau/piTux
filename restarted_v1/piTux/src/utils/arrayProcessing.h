
#ifndef __ARRAYPROCESSING_H__
#define __ARRAYPROCESSIN_H__
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../model/block.h"

int* initIntArray(int p_size,int p_defaultVal);
char ** splitString(char* p_str,char p_separator,int p_sizeMax,int p_returnArrayMaxQty,int p_returnMaxSize);
#endif
