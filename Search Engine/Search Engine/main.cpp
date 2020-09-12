#include "Function.h"

int main()
{
	trieNode* mainTrie = initTrieNode();
	trieNode* titleTrie = initTrieNode();
	vector<string> docPath;

	buildDocPath(docPath);
	//indexAllData(mainTrie, titleTrie, docPath);

	//ifstream in;
	//retrieve(mainTrie, in, "mainTrie.txt");
	//retrieve(titleTrie, in, "titleTrie.txt");

	/*ofstream out;
	string word;  
	out.open("mainTrie.txt");
	saveTrieToFile(mainTrie, out, word);
	out.close();

	out.open("titleTrie.txt");
	saveTrieToFile(titleTrie, out, word);
	out.close();*/

	//string query = "san francisco";
	//vector<int> docNum = searchAnd(mainTrie, query);
	//cout << docNum.size();
	//for (int i = 0; i < docNum.size(); i++) {
	//	cout << docNum.at(i) << " ";
	//}

	/*userIndexNewDoc(mainTrie, titleTrie, docPath);

	deleteWholeTrie(mainTrie);
	deleteWholeTrie(titleTrie);*/
	//return 0;

	trieNode2* queryTrie = initNode2();
	retrieve2(queryTrie);
	string query = "";
	getInput(queryTrie, query);

	deleteWholeTrie2(queryTrie);
	return 0;

}