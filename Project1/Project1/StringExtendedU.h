#pragma once
#include "../../aquaUtils/utils/StringUtils.h"
#include "../../aquaUtils/model/AquaTypes.h"


int SEU_Equals(string_t str, string_t anStr);

/*
* @return last index of substring in string,
* if not found return -1
*/
int SEU_firstIndexOf(string_t str, string_t sub,int strLen, int subLen); // Actually this is base & super fast
/*
* @return array of indexes of pattern in string,
* if not found return empty array
*/
IntV* SEU_search(string_t str, string_t pattern, int subLen);

/*@return 1 - if str contains sub, 0 - if not*/
int SEU_Contains(string_t str, string_t sub); 

int SEU_countSubstring(string_t str, string_t sub);

StringV* SEU_split(string_t str, char delimiter);

string_t SEU_replaceFirst(char* str, string_t pattern, string_t to);
string_t SEU_replaceAll(char* str, string_t pattern, string_t to);
