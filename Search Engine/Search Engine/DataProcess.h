#ifndef _DataProcess_h_
#define _DataProcess_h_

#include "Function.h"

// ========== TRIE IMPLEMENTION ==========
const int SIZE = 39;

//a-z: 0-25
//0-9: 26-35
//'\'': 36
//'#': 37
//'$': 38
struct trieNode {
	trieNode* children[SIZE];
	bool isEndWord;
	vector <int> documents;
};

trieNode* initTrieNode();

// Insert a single word/number to trie.
void insertToTrie(trieNode* root, string keyword, vector<int> docNum);

void deleteWholeTrie(trieNode* root);

// Write each word in the trie and its document num line by line to text file.
void saveTrieToFile(trieNode* root, ofstream& out, string word);

// Read a trie from a text file.
void retrieve(trieNode*& root, ifstream& in, string address);



// =========== INDEXING DATA ===========

// Index the data of a document to trie.
void indexData(trieNode* root, string path, vector<string> docPath);




// ============= SEARCH =============

vector <int> searchKeyword(trieNode* root, string keyword);

// Find out the common documents.
vector <int> searchFullText(trieNode* root, string text);







// ============= OUTPUT =============


#endif
