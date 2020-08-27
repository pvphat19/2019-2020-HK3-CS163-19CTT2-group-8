#ifndef _QueryProcess_h_
#define _QueryProcess_h_

#include "Function.h"


string removeStopWords(string query);
vector<int> queryType(string query);


// ========== SUPPORTING FUNCTION ==========

// Turn a string to lowercase.
void toLower(string& text);

// Check whether a word is a stop word.
bool isStopWords(string word, string list[], int numStopWords);

// Check whether a string (many words) is in document.
bool isExactlyMatch(int docNum, vector<string> docPath, string query);

#endif


