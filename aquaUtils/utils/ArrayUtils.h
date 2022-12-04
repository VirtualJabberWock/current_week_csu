#pragma once
#include <stdlib.h>
#include "..\utils\DebugUtils.h"

#define obj_ptr void*

// TODO :: DEFINE SOMETHING -> NOT COPY ALL FUNTCIONS

void* initArray(int size, int b_size);

//INT
void expandIntArray(int** array, int new_size);
void fillIntArray(int* array, int len, int value);
void copyIntArray(int* from, int* to, int begin, int end);
void putToIntArray(int** list, int* list_size, int n);

//INT64
void expand64Array(__int64** array, int new_size);
void fill64Array(__int64* array, int len, __int64 value);
void copy64Array(__int64* from, __int64* to, int begin, int end);
void putTo64Array(__int64** list, int* list_size, __int64 n);
//

//Double
void expandDoubleArray(double** array, int new_size);
void putToDoubleArray(double** list, int* list_size, double n);
//

//Char
void expandCharArray(char** list, int new_size);
void subCharArray(char* from, char* to, int begin, int end);
void pushToCharArray(char** list, int* list_size, char chr);
/* Builds const string from dynamic char array & Dispose resources (free memory) */
const char* buildString(char* list, int list_size);
const char* buildLine(char* list, int list_size);
//

obj_ptr* initBucket(int size);
void expandBucket(obj_ptr** bucket, int new_size);
void pushToBucket(obj_ptr** bucket, int* bucket_size, obj_ptr obj);
void freeBucket(obj_ptr* bucket, int bucket_size);
void _freeBucket(obj_ptr* bucket, int bucket_size);
