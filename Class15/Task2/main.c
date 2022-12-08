#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void fillIn(int* begin, int* end);
int* bubbleSort(int* begin, int* end);
int* merge(int* firstBegin, int* firstEnd, int* secondBegin, int* secondEnd);

//void test(int* begin, int* end);

int main(int argc, const char** argv) {
	srand((__int64)__security_cookie);
	if (argc != 3) return printf("Invalid args!");
	int N = atoi(argv[1]);
	int M = atoi(argv[2]);
	int* a = calloc(N, sizeof(int));
	int* b = calloc(M, sizeof(int));
	fillIn(a, a + N);
	fillIn(b, b + M);
	bubbleSort(a, a + N);
	bubbleSort(b, b + M);
//	test(a, a + N);
//	test(b, b + M);
	int* united = merge(a, a + N, b, b + M);
//	int u_size = N + M;
//	test(united, united + u_size);
}

void fillIn(int* begin, int* end)
{
	int* pointer = begin;
	do {
		if (pointer == 0) return printf("Segmentation fault!");
		*pointer = rand() % 100;
		pointer++;
	}
	while (pointer != end);
}

int* bubbleSort(int* begin, int* end)
{
	for (int* p = begin; p < end; p++) {
		for (int* pj = begin; pj < end-1; pj++) {
			if (*pj > *(pj + 1)) {
				int tmp = *pj;
				*pj = *(pj + 1);
				*(pj + 1) = tmp;
			}
		}
	}
	return begin;
}

int* merge(int* firstBegin, int* firstEnd, int* secondBegin, int* secondEnd)
{
	int first_size = (firstEnd - firstBegin);
	int second_size = (secondEnd - secondBegin);
	int size = first_size + second_size;
	int* result = malloc(sizeof(int) * size);
	int fi = 0, si = 0;
	if (result == 0) return 0;
	for (int i = 0; i < size; i++) {
		if (fi < first_size && si >= second_size) 
			result[i] = firstBegin[fi];
		else if (si < second_size && fi >= first_size)
			result[i] = secondBegin[si];
		else 
			result[i] = (firstBegin[fi] < secondBegin[si]) ? firstBegin[fi] : secondBegin[si];
		if (firstBegin[fi] < secondBegin[si]) fi++;
		else si++;
	}
	return result; 
}

//void test(int* begin, int* end)
//{
//	printf("\nTest:\n");
//	int* pointer = begin;
//	do {
//		if (pointer == 0) break;
//		printf(", %d"+2*(pointer == begin), *pointer);
//		pointer++;
//	} while (pointer != end);
//	printf("\n");
//}
