#include "Function.h"

int main()
{
	trieNode* mainTrie = initTrieNode();
	trieNode* titleTrie = initTrieNode();
	vector<string> docPath;

	docPath.push_back("Search Engine-Data/000.txt");
	docPath.push_back("Search Engine-Data/1.txt");
	docPath.push_back("Search Engine-Data/001.txt");
	docPath.push_back("Search Engine-Data/2.txt");
	docPath.push_back("Search Enginr-Data/004.txt");
	docPath.push_back("Search Enginr-Data/003.txt");
	docPath.push_back("Search Enginr-Data/002.txt");
	docPath.push_back("Search Enginr-Data/006.txt");
	docPath.push_back("Search Enginr-Data/008.txt");
	docPath.push_back("Search Enginr-Data/007.txt");

	indexAllData(mainTrie, titleTrie, docPath);
	ofstream out;
	out.open("mainTrie.txt");
	string word;
	saveTrieToFile(mainTrie, out, word);
	out.close();
	string query = "human bloodstream";
	vector<int> docNum = searchFullText(mainTrie, query);
	
	deleteWholeTrie(mainTrie);
	deleteWholeTrie(titleTrie);
	return 0;
}