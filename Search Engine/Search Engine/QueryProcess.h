#ifndef _QueryProcess_h_
#define _QueryProcess_h_

#include "Function.h"


string removeStopWords(string query);


// ========== SUPPORTING FUNCTION ==========
// Turn a string to lowercase.
void toLower(string& text);

// Check whether a word is a stop word.
bool isStopWords(string word, string list[], int numStopWords);

#endif


