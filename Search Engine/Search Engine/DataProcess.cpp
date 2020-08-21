#include "DataProcess.h"

// ========== TRIE IMPLEMENTION ==========
trieNode* initTrieNode() {
	trieNode* root = new trieNode;
	root->isEndWord = false;
	for (int i = 0; i < SIZE; i++)
		root->children[i] = nullptr;
	return root;
}

void deleteWholeTrie(trieNode* root) {
	if (root == nullptr) return;
	for (int i = 0; i < SIZE; i++) {
		if (root->children[i] != nullptr)
			deleteWholeTrie(root->children[i]);
	}
	root->documents.clear();
	delete root;
}

// =========== INDEXING DATA ===========
// Insert a single word/number to trie.
void insertToTrie(trieNode* root, string keyword, int docNum) {
	trieNode* cur = root;
	for (int i = 0; i < keyword.length(); i++) {
		int index;
		if (keyword[i] >= 'a')
			index = keyword[i] - 'a';
		else index = keyword[i] - '0' + 26;

		if (!cur->children[index])
			cur->children[index] = initTrieNode();

		cur = cur->children[index];
	}
	cur->isEndWord = true;
	cur->documents.push_back(docNum);
}

// Index the data of a document to trie.
void indexData(trieNode* root, string path, vector<string> docPath) {
	int docNum = docPath.size();
	docPath.push_back(path);

	ifstream in;
	in.open(path);
	if (!in.is_open())
		cout << "Error: Cannot open the document!\n";
	else {
		string singleWord;
		while (in>>singleWord) {
			insertToTrie(root, singleWord, docNum);
		}
		in.close();
	}
}


// =========== SEARCH ===========

vector <int> searchKeyword(trieNode* root, string keyword) {
	vector <int> res;
	trieNode* cur = root;

	for (int i = 0; i < keyword.size(); ++i) {
		int index;
		if (keyword[i] > 'a' && keyword[i] < 'z') index = keyword[i] - 'a';
		else index = keyword[i] - '0';

		if (!cur->children[index]) return res;
		cur = cur->children[index];

	}
	if (cur->isEndWord == true) {
		res = cur->documents;
	}
	return res;
}

vector <int> searchFullText(trieNode* root, string text) {
    int count_doc_appear[1000],count_words=0;
    for (int i=0;i<1000;i++) count_doc_appear[i]=0;

    string tmp="";
    vector <int> res;

    for (int i=0;i<text.length();i++) {
        if (text[i]==' ' || i=text.length()-1) {
            vector <int> doc_temp;
            doc_temp=searchKeyword(root,tmp);

            for (int j=0;j<doc_temp.size();j++)
                count_doc_appear[doc_temp[i]]++;

            count_words++;
            tmp="";
        } else {
            tmp+=text[i];
        }
    }

    for (int i=0;i<1000;i++)
        if (count_doc_appear[i]==count_words)
            res.push_back(i);

    return res;
}
