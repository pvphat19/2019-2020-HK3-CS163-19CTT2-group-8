#include "DataProcess.h"

// ========== TRIE IMPLEMENTION ==========
trieNode* initTrieNode() {
	trieNode* root = new trieNode;
	root->isEndWord = false;
	for (int i = 0; i < SIZE; i++)
		root->children[i] = nullptr;
	return root;
}

// Insert a single word/number to trie.
void insertToTrie(trieNode* root, string keyword, vector<int> docNum) {
	trieNode* cur = root;
	for (int i = 0; i < keyword.length(); i++) {
		int index;
		if ('z' >= keyword[i] && keyword[i] >= 'a')
			index = keyword[i] - 'a';
		else if (keyword[i] == '\'')
			index = 36;
		else if (keyword[i] == '#')
			index = 37;
		else if (keyword[i] == '$')
			index = 38;
		else index = keyword[i] - '0' + 26;

		if (!cur->children[index])
			cur->children[index] = initTrieNode();

		cur = cur->children[index];
	}
	cur->isEndWord = true;
	for(int i=0; i<docNum.size(); i++)
		cur->documents.push_back(docNum.at(i));
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

// Write each word in the trie and its document num line by line to text file.
void saveTrieToFile(trieNode* root, ofstream &out, string word) {
	if (root == nullptr) return;
	if (root->isEndWord){
		out << word << " ";
		out << root->documents.size() << " ";
		for (int i = 0; i < root->documents.size(); i++)
			out << root->documents.at(i) << " ";
		out << endl;
	}
	for (int i = 0; i < SIZE; i++) {
		if (root->children[i]) {
			char c;
			if (i < 26) c = i + 'a';
			else if (i == 36) c = '\'';
			else if (i == 37) c = '#';
			else if (i == 38) c = '$';
			else c = i - 26 + '0';
			string tem = word;
			word += c;
			saveTrieToFile(root->children[i], out, word);
			word = tem;
		}
	}
}

// Read a trie from a text file.
void retrieve(trieNode*& root, ifstream& in, string address) {
	in.open(address);
	if (in.is_open()) {
		while (!in.eof()) {
			// read data in a line
			string word;
			in >> word;
			int n; // number of docs
			in >> n;
			vector<int> docNum;
			for (int i = 0; i < n; i++) {
				int t;
				in >> t;
				docNum.push_back(i);
			}
			insertToTrie(root, word, docNum);
		}
		in.close();
	}
}

// =========== INDEXING DATA ===========
// Index the data of a document to trie. (and push its path to vector docPath)
void indexData(trieNode* root, string path, vector<string> docPath) {
	int docnum = docPath.size();
	vector<int>docNum;
	docNum.push_back(docnum);
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

// Index the title of a document to title trie.
void indexTitle(trieNode* titleTrie, string path, vector<string> docPath) {
	vector <int> docNum;
	docNum.push_back(docPath.size());

	ifstream in;
	in.open(path);
	if (in.is_open()) {
		string line;
		getline(in, line);
		while (line.size()) {
			stringstream words(line);
			string word;
			while (words >> word) {
				insertToTrie(titleTrie, word, docNum);
			}
			line = "";
			getline(in,line);
		}
		in.close();
	}
}

// Get doc path from user and index it.
// sau khi hoan thien nhung argument nay se la global variables
void userIndexNewDoc(trieNode* mainTrie, trieNode* titleTrie, vector<string>docPath) {
	cout << "Please enter the path to document: \n\t";
	string path; 
	cin >> path;
	// Add document path to file "__index.txt".
	ofstream out;
	out.open("__index.txt", ios::ate);
	if (out.is_open()) {
		out << path;
		out.close();
	}
	// Index to main trie.
	indexData(mainTrie, path, docPath);
	// Index its title to title trie.
	indexTitle(titleTrie, path, docPath);
}

// Read all doc names in "__index.txt" to vector<string>docPath.
void buildDocPath(vector<string> docPath) {
	ifstream in;
	in.open("__index.txt");
	if (in.is_open()) {

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
		else if (keyword[i] == '\'') index = 36;
		else if (keyword[i] == '#') index = 37;
		else if (keyword[i] == '$') index = 38;
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

    for (int i=0; i<text.length() ;i++) {
        if ( text[i]==' ' || i==text.length()-1) {
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

// ==========OPERATOR=============

//Operator 7: Search for a price. Put $ in front of a number. For example: $400
vector <int> searchForPrice(trieNode* root, string price) {
	vector<int> res;
	res = searchKeyword(root, price);
	return res;
}

//Operator 8: Search hashtags. Put # in front of a word. For example: #helloworld
vector <int> searchHashtag(trieNode* root, string hashtag) {
	vector<int> res;
	res = searchKeyword(root, hashtag);
	return res;
}

//Operator 11: Search for a range of number. Put .. between two numbers. For example: $50..$100
vector <int> searchRangeOfNumber(trieNode* root, string range) {
	vector <int> res;



	return res;
}

