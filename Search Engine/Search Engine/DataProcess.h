#ifndef _DataProcess_h_
#define _DataProcess_h_

#include "Function.h"

// ========== TRIE IMPLEMENTION ==========
const int SIZE = 37;

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

void printDocName(vector<int>docNum, vector<string> docPath) {
	cout << "Top 5 results document: \n";
	for (int i = 0; i < docNum.size(); i++) {
		int docnum = docNum.at(i);
		string docName = docPath.at(docnum);
		cout << "\tDocument " << i + 1 << " :" << docName << "\n";
	}
}

#endif
