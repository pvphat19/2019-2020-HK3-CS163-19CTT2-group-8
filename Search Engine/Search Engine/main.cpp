#include "Function.h"

int main()
{
	// Declare
	Console c;
	trieNode* mainTrie = initTrieNode();
	trieNode* titleTrie = initTrieNode();
	trieNode2* queryTrie = initNode2();
	vector<string> docPath;
	
	// Retrieve Data
	buildDocPath(docPath);
	
	ifstream in;
	retrieve(mainTrie, in, "mainTrie.txt");
	retrieve(titleTrie, in, "titleTrie.txt");
	retrieve2(queryTrie);

	// Main program
	menu(c, mainTrie, titleTrie, queryTrie, docPath);
	
	// Save data 
	ofstream out;
	string word;
	/*out.open("mainTrie.txt");
	saveTrieToFile(mainTrie, out, word);
	out.close();

	out.open("titleTrie.txt");
	saveTrieToFile(titleTrie, out, word);
	out.close();*/

	out.open("history.txt");
	saveTrieToFile2(queryTrie, out, word);
	out.close();

	// Delete pointer
	deleteWholeTrie(mainTrie);
	deleteWholeTrie(titleTrie);
	deleteWholeTrie2(queryTrie);

	return 0;
}


// These following lines are for indexing and saving trie:

//indexAllData(mainTrie, titleTrie, docPath);

//ofstream out;
//string word;
//out.open("mainTrie.txt");
//saveTrieToFile(mainTrie, out, word);
//out.close();

//out.open("titleTrie.txt");
//saveTrieToFile(titleTrie, out, word);
//out.close();


