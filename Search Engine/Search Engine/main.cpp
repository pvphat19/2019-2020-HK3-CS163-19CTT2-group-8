#include "Function.h"

int main()
{
	//Console c;
	//trieNode* mainTrie = initTrieNode();
	//trieNode* titleTrie = initTrieNode();
	//vector<string> docPath;



	//

	//buildDocPath(docPath);
	////indexAllData(mainTrie, titleTrie, docPath);

	//

	///*ofstream out;
	//string word;
	//out.open("mainTrie.txt");
	//saveTrieToFile(mainTrie, out, word);
	//out.close();

	//out.open("titleTrie.txt");
	//saveTrieToFile(titleTrie, out, word);
	//out.close();*/

	//ifstream in;
	//retrieve(mainTrie, in, "mainTrie.txt");
	//retrieve(titleTrie, in, "titleTrie.txt");


	//string query = "steel and concrete";
	//vector <string> a;
	//a.push_back("manchester");
	//a.push_back("united");
	////vector<int> docNum = searchOr(mainTrie, query);
	//

	//presentResult(mainTrie,titleTrie, query, docPath, c);
	//
	//
	///*cout << docNum.size() << endl;
	//for (int i = 0; i < docNum.size(); i++) {
	//	cout << docNum.at(i) << " ";
	//}*/

	////userIndexNewDoc(mainTrie, titleTrie, docPath);



	///*trieNode2* queryTrie = initNode2();
	//retrieve2(queryTrie);
	//*/
	//

	////deleteWholeTrie2(queryTrie);
	//deleteWholeTrie(mainTrie);
	//deleteWholeTrie(titleTrie);
	//return 0;

	Console c;
	string query = "posing as crew";
	string docPath = "Search Engine-Data/000.txt";
	printDocument(docPath);

	return 0;
	
}