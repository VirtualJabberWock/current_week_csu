#include "StringUtils.h"
#include "ArrayUtils.h"
#include <stdarg.h>

string_t SUS_clearDublicateSpaces(string_t str)
{
	int i = 0;
	CharList buf = initArray(1, sizeof(char));
	int buf_len = 0;
	char isLastSpace = 0;
	while(str[i] != '\0'){
		if (isLastSpace && str[i] == ' ') {}
		else pushToCharArray(&buf, &buf_len, str[i]);
		isLastSpace = (str[i] == ' ');
		i++;
	}
	return buildString(buf, buf_len);
}

int SUS_getStringLength(string_t str)
{
	int i = 0;
	while (str[i] != '\0') {
		if (i > 2048) return -1;
		i++;
	};
	return i;
}

int SUS_isStringNumber(string_t str) {
	int i = 0;
	if (str[0] == '-') i++;
	while (str[i] != '\0') {
		if (str[i] < '0' || str[i] > '9') return (i + 1);
		i++;
	}
	return 0;
}

int SUS_parseInteger32(string_t str, __int32 *out)
{
	int l = 0;
	__int64 result = 0;
	__int64 tmp;
	__int64 k = 1;
	int sign = 1;
	if (str[0] == '-') {
		sign = -1;
		l = 1;
	}
	int len = SUS_getStringLength(str);
	int isNum = SUS_isStringNumber(str);
	if (isNum != 0) return isNum;
	if (len > 19) return -2;
	long long intMax = INT_MAX + ((long long)max(0, -1 * sign));
	for (int i = len - 1; i >= l; i--) {
		tmp = ((__int64)str[i] - '0') * k;
		k = k * 10;
		if ((__int64)(result + tmp) > intMax) {
			__int64 s = result + tmp;
			return -1;
		}
		result += tmp;
	}
	*out = (__int32) (result * sign);
	return 0;
}

string_t SUS_str_c(string_t str1, string_t str2) {
	int i = 0;
	CharList buf = initArray(1, sizeof(char));
	int buf_len = 0;
	while (str1[i] != '\0') {
		pushToCharArray(&buf, &buf_len, str1[i]); i++;
	}
	while (str2[i] != '\0') {
		pushToCharArray(&buf, &buf_len, str2[i]); i++;
	}
	return buildString(buf, buf_len);
}

string_t SUS_str_f(string_t format, string_t str2) {
	int i = 0;
	CharList buf = initArray(0, sizeof(char));
	buf[0] = '\0';
	int buf_len = 0;
	while (format[i] != '\0') {
		if (format[i] == '%') {
			int j = 0;
			while (str2[j] != '\0') {
				pushToCharArray(&buf, &buf_len, str2[j]);
				j++;
			}
		}
		else {
			pushToCharArray(&buf, &buf_len, format[i]);
		}
		i++;
	}
	return buildString(buf, buf_len);
}

string_t SUS_format1024(string_t format, ...)
{
	va_list arg;
	char* buffer = initArray(1024, sizeof(char));
	/* Write the error message */
	va_start(arg, format);
	vsprintf_s(buffer, 1024, format, arg);
	va_end(arg);
	return buildString(buffer, 1024);
}

void SUS_str_unlock(string_t const_str, int* out_len, CharList* out_buffer)
{
	int i = 0;
	int buf_len = 0;
	while (const_str[i] != '\0') {
		pushToCharArray(out_buffer, &buf_len, const_str[i]); i++;
	}
	*out_len = buf_len;
}

string_t SUS_str_lock(char* buffer, int size)
{
	char* buf0 = initArray(size + 1, sizeof(char));
	for (int i = 0; i < size; i++) buf0[i] = buffer[i];
	buf0[size] = '\0';
	return buf0;
}

string_t SUS_str_bucket_assemble(string_t* bucket, int bsize, char delim)
{
	string_t s = "";
	CharList megaBuffer = initArray(0, sizeof(char));
	int megaBuffer_len = 0;
	for (int i = 0; i < bsize; i++) {
		int j = 0;
		while (bucket[i][j] != '\0') {
			pushToCharArray(&megaBuffer, &megaBuffer_len, bucket[i][j]); j++;
		}
		pushToCharArray(&megaBuffer, &megaBuffer_len, delim);
	}
	return buildString(megaBuffer, megaBuffer_len);
}

string_t SUS_str_copy(string_t str1) {
	int i = 0;
	CharList buf = initArray(1, sizeof(char));
	int buf_len = 0;
	while (str1[i] != '\0') {
		pushToCharArray(&buf, &buf_len, str1[i]); i++;
	}
	return buildString(buf, buf_len);
}

char SUS_alphabetCompare(string_t str, string_t next)
{
	int str_len = SUS_getStringLength(str);
	int next_len = SUS_getStringLength(next);
	int len = max(str_len, next_len);
	for (int i = 0; i < len; i++) {
		if (str[i] == next[i]) continue;
		else return (str[i] < next[i]);
	}
	return (str_len <= next_len); // 1."ab" 2."abc" 3."abcd" ...
	//also bet on the [str] so as not to do swap
}


string_t SUS_trim(string_t str)
{
	string_t str2 = SUS_clearDublicateSpaces(str);
	CharList buf = initArray(1, sizeof(char));
	int buf_s = 0;
	int str_len = SUS_getStringLength(str2);
	int start = 0, end = str_len;
	if (str[0] == ' ') start++;
	if (str[str_len-1] == ' ') end--;
	for (int i = start; i < end; i++) pushToCharArray(&buf, &buf_s, str2[i]);
	return buildString(buf, buf_s);
}

string_t* SUS_split(string_t str, char delimiter, int* count) {
	string_t* a = (string_t*) initBucket(1);
	int bucket_s = 0;
	int i = 0;
	int start = 0, end = 0;
	CharList tmp = initArray(1, sizeof(char));
	tmp[0] = '\0';
	while (str[i] != '\0') {
		if (str[i] == delimiter) {
			tmp = initArray(end-start+1, sizeof(char));
			subCharArray(str, tmp, start, end-1);
			pushToBucket(&a, &bucket_s, buildString(tmp, end-start));
			start = i+1;
			end = i;
		}
		end++;
		i++;
	}
	tmp = initArray(end - start + 1, sizeof(char));
	subCharArray(str, tmp, start, end);
	pushToBucket(&a, &bucket_s, buildString(tmp, end-start));
	*count = bucket_s; // [out]
	return a;
}
