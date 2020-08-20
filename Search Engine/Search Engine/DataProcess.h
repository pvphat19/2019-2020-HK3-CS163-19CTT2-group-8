#ifndef _DataProcess_h_
#define _DataProcess_h_

#include "Function.h"

// ========== TRIE IMPLEMENTION ==========
const int SIZE = 30;

struct trieNode {
	trieNode* children[SIZE];
	bool isEndWord;
	vector <int> documents;
};

trieNode* initTrieNode();
void deleteWholeTrie(trieNode* root);


// =========== INDEXING DATA =========== 
// Insert a single word/number to trie.
void insertToTrie(trieNode* root, string keyword, int docNum);

// Index the data of a document to trie.
void indexData(trieNode* root, string path, vector<string> docPath);

//=============SEARCH===========

vector <int> searchKeyword(trieNode* root, string keyword);

#endif
