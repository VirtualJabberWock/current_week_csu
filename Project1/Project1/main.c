#include <stdio.h>
#include "StringExtendedU.h"
#include "testModule.h"

#define SKIP_TEST 0 // SET TO 1 to skip test mode

int main()
{
	if (SKIP_TEST == 0) {
		int passedTheTest = testModule();
		if (passedTheTest == 0) return printf("\n\nTest Failed\n");
		else return printf("\n\nAll tests are passed!\n");
	}
	printf("\nReplacing!\n\n");
	string_t example = "For writing i use <color> pencil, <color> is my favorite, sky is <color> and water is <color> too!";
	string_t result = SEU_replaceAll(example, "<color>", "blue"); //7(<color>) -> 4(blue)
	printf(example);
	printf("\n");  
	printf(result);
	free(result);
	printf("\n");
	example = "His name is <?>, <?> actually kind man!";
	result = SEU_replaceAll(example, "<?>", "Nathan"); //3 (<?>) -> 6(Nathan)
	printf(example);
	printf("\n");
	printf(result);
	free(result);

	printf("\n\nCounting\n\n");
	example = "AbababAABBABABABAaBBAAB"; // 5 'BA'
	int count = SEU_countSubstring(example, "BA");
	printf("%s contains %d [%s]\n\n", example, count, "BA");
}
