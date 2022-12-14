#pragma once

#include "StringExtendedU.h"

static int valid = 1;

void assert(int obj, int to);
void assert_Str(string_t obj, string_t to);
int testModule();

/* TESTS OF StringExtendedU */

void Test_SEU_firstIndexOf();
void Test_SEU_search();
void Test_SEU_replaceFirst();
void Test_SEU_replaceAll();

