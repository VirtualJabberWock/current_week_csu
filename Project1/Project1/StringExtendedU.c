#include "StringExtendedU.h"
#include "../../aquaUtils/utils/DebugUtils.h"
#include "../../aquaUtils/utils/ArrayUtils.h"

int SEU_Equals(string_t str, string_t anStr)
{
	int i = 0;
	if (str == NULL) 
		return panic_NPE(SEU_Equals, "<Stirng> str (SEU_Equals)");
	if (anStr == NULL) 
		return panic_NPE(SEU_Equals, "<Stirng> anStr (SEU_Equals)");
	if (str[0] != anStr[0]) return 0; //case: str = "" and anStr = "."
	while (str[i] != '\0') {
		if (anStr[i] == '\0') return 0;
		if (str[i] != anStr[i]) return 0;
		i++;
	}
	return 1;
}

/*
@return Found: index of substring, Not found: -1
match time : O(m-n), worst O(n)
faster than java String.contains and C# String.contains
faster than string::find() c++
and also faster than KMP alg
*/
#define NOT_FOUND -1
int SEU_firstIndexOf(string_t str, string_t sub, int strLen, int subLen) 
{
	if (str == NULL || sub == NULL)
		return panic_NPE(SEU_firstIndexOf, "(SEU_firstIndexOf)");
	if (subLen == 0) return strLen; // Java feature!
	if (subLen > strLen) return NOT_FOUND;
	if (subLen == strLen) 
		if (SEU_Equals(str, sub)) return 0;
		else return NOT_FOUND;
	char flag = 0;
	int maybe = 0;
	for (int i = 0; i < strLen; i++) {
		if (!flag && (i + subLen > strLen)) return NOT_FOUND;
		if (str[i] == sub[0] && str[i + subLen - 1] == sub[subLen - 1]) {
			flag = ((maybe = i) > -1); //...
			continue;
		}
		if (!flag) continue;
		if (str[i] != sub[i - maybe]) {
			flag = 0; continue;
		}
		if (i - maybe == subLen - 1) return maybe;
	}
	if (flag != 0) return maybe;
	return NOT_FOUND;
}

int SEU_Contains(string_t str, string_t sub)
{
	int subLen = SUS_getStringLength(sub);
	int strLen = SUS_getStringLength(str);
	return (SEU_firstIndexOf(str, sub, strLen, subLen) != NOT_FOUND);
}
//
#undef NOT_FOUND // ==============================

//yes it dublicates firstIndexOf, but it all for optimization...
IntV* SEU_search(string_t str, string_t sub, int subLen)
{
	IntV* v_ptr = malloc(sizeof(IntV));
	if (v_ptr == 0) return panic("Memory error!");
	InitIntV(v_ptr);
	if (str == NULL || sub == NULL)
		return panic_NPE(SEU_firstIndexOf, "(SEU_search)");
	int strLen = SUS_getStringLength(str);
	if (subLen == 0) return strLen; // Java feature!
	if (subLen > strLen) return v_ptr;
	if (subLen == strLen)
		if (SEU_Equals(str, sub)) return 0;
		else return v_ptr;
	char flag = 0;
	int maybe = 0;
	for (int i = 0; i < strLen; i++) {
		if (!flag && (i + subLen > strLen)) return v_ptr;
		if (str[i] == sub[0] && str[i + subLen - 1] == sub[subLen - 1]) {
			flag = ((maybe = i) > -1); //...
			continue;
		}
		if (!flag) continue;
		if (str[i] != sub[i - maybe]) {
			flag = 0; continue;
		}
		if (i - maybe == subLen - 1) {
			v_ptr->put(v_ptr, maybe);
			flag = 0;
		};
	}
	return v_ptr;
}

int SEU_countSubstring(string_t str, string_t sub)
{
	int subLen = SUS_getStringLength(sub);
	IntV* iv = SEU_search(str, sub, subLen);
	int c = iv->size;
	free(iv->ptr);
	free(iv);
	return c;
}

StringV* SEU_split(string_t str, char c)
{
	StringV* sv = malloc(sizeof(StringV));
	if (sv == 0) return panic("Memory error!");
	InitStringV(sv);
	sv->ptr = SUS_split(str, c, &sv->size);
	return sv;
}

string_t SEU_replaceFirst(char* str, string_t pattern, string_t to)
{
	int subLen = SUS_getStringLength(pattern);
	int strLen = SUS_getStringLength(str);
	int toLen = SUS_getStringLength(to);
	char* shadow = malloc(sizeof(char) * (strLen - subLen + toLen + 1));
	if (shadow == 0) return panic("Memory error!");
	shadow[strLen - subLen + toLen] = '\0';
	int index = SEU_firstIndexOf(str, pattern, strLen, subLen);
	if (index == -1) return str;
	memcpy(shadow, str, sizeof(char) * index);
	memcpy(shadow + index, to, toLen);
	memcpy(shadow + index + toLen, str + index + subLen, sizeof(char) * strLen - index - subLen);
	return shadow;
}

string_t SEU_replaceAll(char* str, string_t pattern, string_t to)
{
	int subLen = SUS_getStringLength(pattern);
	int strLen = SUS_getStringLength(str);
	IntV* iv = SEU_search(str, pattern, subLen);
	if (iv->size == 0) return str;
	int toLen = SUS_getStringLength(to);
	int cast = strLen - subLen * iv->size + toLen * iv->size;
	char* shadow = malloc(sizeof(char) * (cast + 1));
	if (shadow == 0) return panic("Memory error!");
	int origin = 0, alpha = 0;
	int k = 0;
	int flag = 0;
	for (int i = 0; i < cast; i++) {
		if (origin == iv->ptr[k]) {
			origin += subLen;
			flag = ((alpha = 0) == 0);
			k++;
		}
		if (flag) {
			shadow[i] = to[alpha];
			if (alpha + 1 >= toLen) flag = 0;
			alpha++;
			continue;
		}
		shadow[i] = str[origin];
		origin++;
	}
	shadow[cast] = '\0';
	return shadow;
}
