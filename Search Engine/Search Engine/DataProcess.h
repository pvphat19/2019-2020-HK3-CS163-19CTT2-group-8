#ifndef _DataProcess_h_
#define _DataProcess_h_

#include "Function.h"

// ========== TRIE IMPLEMENTION ==========
const int SSIZE = 39;
      //a-z: 0-25
      //0-9: 26-35
      //'\'': 36
      //'#': 37
      //'$': 38

struct trieNode {
	trieNode* children[SSIZE];
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

// Index the data of a document to trie. (and push its path to vector docPath)
void indexMainTrie(trieNode* mainTrie, string path, int docnum);

// Index the title of a document to title trie.
void indexTitle(trieNode* titleTrie, string path, int docnum);

// Get doc path from user and index it.
void userIndexNewDoc(trieNode* mainTrie, trieNode* titleTrie, vector <string>& docPath);

// Index all data.
void indexAllData(trieNode* mainTrie, trieNode* titleTrie, vector<string>& docPath);

// Read all doc names in "__index.txt" to vector<string>docPath.
void buildDocPath(vector<string> &docPath);


// ============= SEARCH =============

vector <int> searchKeyword(trieNode* root, string keyword);

// Find out the common documents from a string.
vector <int> searchFullText(trieNode* root, string text);

//Find out the common documents from many strings.
vector <int> searchTextfromVector (trieNode* root, vector <string> t);


// ============= OPERATOR =============
// Operator 1
vector <int> searchAnd(trieNode* root, string query);

// Operator 2
vector <int> searchOr(trieNode* root, string text);

// Operator 3
vector <int> searchWithoutaWord(trieNode* root, string text);

// Operator 4
vector <int> searchTitle(trieNode* titleTrie, string text);

// Operator 5
vector<int> operator5(trieNode* root, string query, vector<string>docPath);

// Operator 7
vector <int> searchForPrice(trieNode* root, string price);

//Operator 6
vector <int> searchFiletype(trieNode* root, string text, vector <string> docPath);

// Operator 8
vector <int> searchHashtag(trieNode* root, string hashtag);

// Operator 9
vector<int> searchExactMatch(trieNode* root, string query, vector<string> docPath);

//Operator 10
vector<int> searchWildCards(trieNode* root, string query);

// Operator 11
vector <int> operator11(trieNode* root, string text);
vector <int> searchRangeOfNumber(trieNode* root, string range);

//Operator 12
vector<int> searchSynonyms(trieNode* root, string query);




// ============ SUPPORT FUNCTION ============
// Check whether a number is in list or not.
bool isInList(vector<int> list, int n);

// Check whether a word is right or not.
bool isWord(string word);

// Remove punctuation.
void removePunctuation(string& word);

bool checkRange(string text);

int compareNumber(string num1, string num2);

void searchRange(trieNode* cur, vector<int> res, string num1, string num2, string currentNumber);

string fileType(string doc_Path);



// ============= OUTPUT =============

#endif
