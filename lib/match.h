#pragma once
#include <stdbool.h>
 bool fuzzy_match(const char* filter, const char* text, bool insensitive);
 double fuzzy_score(const char* haystack, const char* needle, bool insensitive);
 int fuzzy_sort(const char* text1, const char* text2, const char* filter, bool insensitive);
