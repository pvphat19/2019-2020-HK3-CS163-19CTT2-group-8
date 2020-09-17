#ifndef _QueryProcess_h_
#define _QueryProcess_h_

#include "Function.h"


string removeSymbols(string query);
string removeStopWords(string query);
vector<int> queryType(string query);




// ========== SUPPORTING FUNCTION ==========
// Turn a string to lowercase.
void toLower(string& text);

// Check whether a word is a stop word.
bool isStopWords(string word, string list[], int numStopWords);

// Check whether a string (many words) is in document.
bool isExactlyMatch(int docNum, vector<string> docPath, string query);


// ============= HISTORY =============
struct trieNode2 {
	trieNode2* children[39];
	bool isEndWord;
};

trieNode2* initNode2();

void insertToTrie2(trieNode2* root, string query);

void deleteWholeTrie2(trieNode2*& root);

// If current node has a child, return 0; else if all children are NULL, return 1. 
bool isLastNode(trieNode2* root);

// Print auto-suggestions.
void suggestions(trieNode2* root, string prefix, Console &c, int &pos, vector <string> & suggests);

// Print suggestions for given query prefix. 
int printAutoSuggestions(trieNode2* root, string query, Console &c, vector<string> & suggests);

void retrieve2(trieNode2* root);

void getInput(trieNode2* history, string& query, Console &con);


#endif


