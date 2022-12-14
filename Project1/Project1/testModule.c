#include "testModule.h"
#include <stdio.h>

int testModule()
{
	Test_SEU_firstIndexOf();
	Test_SEU_search();
	Test_SEU_replaceFirst();
	Test_SEU_replaceAll();
	return valid;
}

void Test_SEU_firstIndexOf()
{
	printf("\nTest_SEU_firstIndexOf:");
	assert(SEU_firstIndexOf("abcdefg", "defg", 7, 4), 3);
	assert(SEU_firstIndexOf("abcdefg", "abc", 7, 3), 0);
	assert(SEU_firstIndexOf("Later in hour in our hour", "hour", 25, 4), 9);
	assert(SEU_firstIndexOf("abcdefgdasd0", "efg_as",12, 6), -1);
	assert(SEU_firstIndexOf("abcdefgdasd0", "efgdas", 12, 6), 4);
	assert(SEU_firstIndexOf("can you find me", "find",15, 4), 8);
	assert(SEU_firstIndexOf("can you find me", "",15, 0), SUS_getStringLength("can you find me"));
	assert(SEU_firstIndexOf("", "", 0, 0), 0);
	assert(SEU_firstIndexOf("", "mase", 0, 4), -1);
}

void Test_SEU_search()
{
	printf("\nTest_SEU_search:");
	assert(
		SEU_search("abcdefgdasd0", "efg_as", 6)->size,
		0);
	assert(
		SEU_search("abcdefgdasd0", "efg_as", 6)->size,
		0);
	IntV* iv = SEU_search("Okay this is test, and test, just test", "test", 4);
	for (int i = 0; i < iv->size; i++) {
		printf("\n\t%d. %d", i, iv->ptr[i]);
	}
	assert(
		SEU_search("djsaiABABAsdasABsadaAB BA", "AB", 2)->size,
		4);
	iv->clear(iv);
	free(iv);
}

void Test_SEU_replaceFirst()
{
	printf("\nTest_SEU_replaceFirst:");
	char* t = "His name <name>, he is cool guy!";
	t = SEU_replaceFirst(t, "<name>", "Michael");
	assert_Str(t, "His name Michael, he is cool guy!");
}

void Test_SEU_replaceAll()
{
	printf("\nTest_SEU_replaceFirst:");
	char* t = "His name <name>, <name> is cool guy!";
	t = SEU_replaceAll(t, "<name>", "Nathan");
	assert_Str(t, "His name Nathan, Nathan is cool guy!");
}

void assert(int obj, int to)
{
	if (obj == to) printf("\n\tYES (PASSED)");
	else {
		printf("\n\tNO (excepted %d, but there was %d)", to, obj);
		valid = 0;
	}
}

#include <string.h> // just for check!
void assert_Str(string_t obj, string_t to)
{
	if (strcmp(obj, to) == 0) {
		printf("\n\tYES (PASSED)");
	}
	else {
		printf("\n\tNO (there was {%s})", obj);
		valid = 0;
	}
}