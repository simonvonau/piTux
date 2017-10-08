
#ifndef __ARRAYPROCESSING_H__
#define __ARRAYPROCESSIN_H__
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../model/block.h"

int* initIntArray(int p_size,int p_defaultVal);
void splitString(char* p_str, int p_strMaxSize, char p_separator, char ***p_res, int *p_resSize1, int *p_resSize2, int p_resMaxSize1, int p_resMaxSize2);
#endif
