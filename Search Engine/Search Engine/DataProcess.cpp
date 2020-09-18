#include "DataProcess.h"

// ========== TRIE IMPLEMENTION ==========
trieNode* initTrieNode() {
	trieNode* root = new trieNode;
	root->isEndWord = false;
	for (int i = 0; i < SSIZE; i++)
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
	if (docNum.size() == 1) {
		if (!isInList(cur->documents, docNum.at(0)))
			cur->documents.push_back(docNum.at(0));
	}
	else {
		cur->documents = docNum;
	}
}

void deleteWholeTrie(trieNode* root) {
	if (root == nullptr) return;
	for (int i = 0; i < SSIZE; i++) {
		if (root->children[i] != nullptr)
			deleteWholeTrie(root->children[i]);
	}
	root->documents.clear();
	delete root;
}

// Write each word in the trie and its document num line by line to text file.
void saveTrieToFile(trieNode* root, ofstream& out, string word) {
	if (root == nullptr) return;
	if (root->isEndWord) {
		out << word << " ";
		out << root->documents.size() << " ";
		for (int i = 0; i < root->documents.size(); i++)
			out << root->documents.at(i) << " ";
		out << endl;
	}
	for (int i = 0; i < SSIZE; i++) {
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
				docNum.push_back(t);
			}
			insertToTrie(root, word, docNum);
		}
		in.close();
	}
}

// =========== INDEXING DATA ===========
// Index the data of a document to trie. (and push its path to vector docPath)
void indexMainTrie(trieNode* mainTrie, string path, int docnum) {
	vector<int> docNum;
	docNum.push_back(docnum);

	ifstream in;
	in.open(path);
	if (in.is_open()) {
		string singleWord;
		while (in >> singleWord) {
			toLower(singleWord);
			removePunctuation(singleWord);
			if (isWord(singleWord))
				insertToTrie(mainTrie, singleWord, docNum);
		}
		in.close();
	}
}


// Index the title of a document to title trie.
void indexTitle(trieNode* titleTrie, string path, int docnum) {
	vector<int>docNum;
	docNum.push_back(docnum);

	ifstream in;
	in.open(path);
	if (in.is_open()) {
		string line;
		getline(in, line);
		while (line.size()) {
			stringstream words(line);
			string word;
			while (words >> word) {
				toLower(word);
				removePunctuation(word);
				if (isWord(word))
					insertToTrie(titleTrie, word, docNum);
			}
			line = "";
			getline(in, line);
		}
		in.close();
	}
}

// Get doc path from user and index it.
void userIndexNewDoc(trieNode* mainTrie, trieNode* titleTrie, vector <string>& docPath) {
	cout << "Please enter the path to document: \n\t";
	string path;
	getline(cin, path);

	// Confirm again.
	cout << "Are you sure to index this document: " << path;
	cout << "\n\tY/N?   ";
	string choice;
	cin >> choice;
	toLower(choice);
	if (choice != "y") {
		cout << "\nCancle indexing new document!\n";
		return;
	}

	// Check whether the path is valid or not.
	ifstream in;
	in.open(path);
	if (!in) {
		cout << "Error: Cannot find the given file!\n";
		return;
	}

	const char* oldPath = path.c_str();

	// Take the name of path only.
	int i = path.size() - 1;
	string tem = "";
	while (i >= 0 && path[i] != '/') {
		tem += path[i];
		i--;
	}

	reverse(tem.begin(), tem.end());

	// Move the file to the Search Engine-Data folder
	string newpath = "Search Engine-Data/" + tem;
	const char* newPath = newpath.c_str();
	rename(oldPath, newPath);

	// Add document path to file "__index.txt".
	ofstream out;
	out.open("Search Engine-Data/___index.txt", ios::app);
	if (out.is_open()) {
		out << tem;
		out.close();
	}
	// Add document path to vector<string>docPath.
	int docnum = docPath.size();
	docPath.push_back(tem);

	// Index to main trie.
	indexMainTrie(mainTrie, newpath, docnum);
	// Index its title to title trie.
	indexTitle(titleTrie, newpath, docnum);

	cout << "The document has been indexed successfully!\n";
}

// Index all data.
void indexAllData(trieNode* mainTrie, trieNode* titleTrie, vector <string>& docPath) {
	for (int i = 0; i < docPath.size(); i++) {
		string path = docPath.at(i);
		indexMainTrie(mainTrie, path, i);
		indexTitle(titleTrie, path, i);
	}
}

// Read all doc names in "__index.txt" to vector<string>docPath.
void buildDocPath(vector <string>& docPath) {
	ifstream in;
	in.open("Search Engine-Data/___index.txt");
	if (in.is_open()) {
		while (!in.eof()) {
			string file;
			getline(in, file);
			file = "Search Engine-Data/" + file;
			docPath.push_back(file);
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
		if (keyword[i] >= 'a' && keyword[i] <= 'z') index = keyword[i] - 'a';
		else if (keyword[i] == '\'') index = 36;
		else if (keyword[i] == '#') index = 37;
		else if (keyword[i] == '$') index = 38;
		else index = keyword[i] - '0' + 26;

		if (!cur->children[index]) return res;
		cur = cur->children[index];

	}
	if (cur->isEndWord == true) {
		res = cur->documents;
	}
	return res;
}

vector <int> searchFullText(trieNode* root, string text) {
	int count_doc_appear[12000], count_words = 0;
	for (int i = 0; i < 12000; i++) count_doc_appear[i] = 0;

	string tmp = "";
	vector <int> res;

	for (int i = 0; i < text.length(); i++) {
		if (text[i] == ' ' || i == text.length() - 1) {
			if (i == text.length() - 1)
				if (text[i] != ' ')
					tmp += text[i];

			vector <int> doc_temp;
			doc_temp = searchKeyword(root, tmp);

			for (int j = 0; j < doc_temp.size(); j++)
				count_doc_appear[doc_temp[j]]++;

			count_words++;
			tmp = "";
		}
		else {
			tmp += text[i];
		}
	}

	for (int i = 0; i < 12000; i++)
		if (count_doc_appear[i] == count_words)
			res.push_back(i);

	return res;
}


vector <int> searchTextfromVector(trieNode* root, vector <string> t) {
	vector <int> res;
	vector <int>* _doc = new vector <int>[t.size()];
	int count_doc_appear[12000];

	for (int i = 0; i < 12000; i++) count_doc_appear[i] = 0;

	for (int i = 0; i < t.size(); i++)
		_doc[i] = searchFullText(root, t[i]);

	for (int i = 0; i < t.size(); i++) {
		for (int j = 0; j < _doc[i].size(); j++)
			count_doc_appear[_doc[i][j]]++;
	}

	for (int i = 0; i < 12000; i++)
		if (count_doc_appear[i] == t.size())
			res.push_back(i);

	delete[]_doc;
	return res;
}

// ==========OPERATOR=============
// Operator 1: And operator.
vector <int> searchAnd(trieNode* root, string query) {
	stringstream words(query);
	string word;
	string tem = "";
	while (words >> word) {
		if (word != "and") {
			tem = tem + word + " ";
		}
	}
	if (tem != "") {
		tem.erase(tem.size() - 1, 1);
	}
	return searchFullText(root, tem);
}

// Operator 2: Search a text which contains "or". For example: X or C
vector <int> searchOr(trieNode* root, string text) {
	string tmp = "", check_or = "";
	vector <string> split;
	vector <int> res;
	int index_or;

	//check the position of "or"
	for (int i = 0; i < text.length(); i++) {
		if (text[i] == ' ' || i == text.length() - 1) {
			if (i == text.length() - 1)
				if (text[i] != ' ')
					check_or += text[i];

			if (check_or == "or") {
				index_or = i;
				break;
			}
			check_or = "";
		}
		else check_or += text[i];
	}

	//Left phrase of Or
	for (int i = 0; i <= index_or - 4; i++) tmp += text[i];

	split.push_back(tmp);
	tmp = "";

	//Right phrase of Or
	for (int i = index_or + 1; i < text.length(); i++) tmp += text[i];
	split.push_back(tmp);
	tmp = "";

	res = searchTextfromVector(root, split);
	return res;

}

// Operator 3: Search a text which do not contain a word. For example: Manchester -united
// It will search all the document which contain "Manchester" and do not contain "united"
vector <int> searchWithoutaWord(trieNode* root, string text) {
	string tmp = "";
	string eliminate = "";
	vector <string> split;
	stringstream file(text);

	while (file >> tmp) {
		if (tmp[0] == '-') {
			for (int i = 1; i < tmp.size(); i++)
				eliminate += tmp[i];
		}
		else split.push_back(tmp);
	}

	vector<int> temp_res = searchTextfromVector(root, split);

	vector<int> eliminate_doc = searchFullText(root, eliminate);

	vector<int> res;

	for (int k = 0; k < temp_res.size(); k++) {
		int flag = 0;

		for (int t = 0; t < eliminate_doc.size(); t++) {
			if (temp_res[k] == eliminate_doc[t]) {
				flag++;
				break;
			}
		}

		if (flag == 0) res.push_back(temp_res[k]);
	}

	return res;
}

// Operator 4: Search for a title of a document. For example: intitle: hammer nails
vector <int> searchTitle(trieNode* titleTrie, string text) {
	string new_text = "";
	vector <int> res;
	for (int i = 9; i < text.length(); i++) new_text += text[i];
	res = searchFullText(titleTrie, new_text);
	return res;
}


// Operator 5: +and operator.
vector<int> operator5(trieNode* root, string query, vector<string>docPath) {
	stringstream words(query);
	string word;
	string tem = "";
	while (words >> word) {
		if (word == "+and") {
			word = "and";
		}
		tem = tem + word + " ";
	}
	if (tem[tem.size() - 1] == ' ') {
		tem.erase(tem.size() - 1, 1);
	}
	vector<int> res = searchFullText(root, tem);
	for (int i = 0; i < res.size(); i++) {
		if (!isExactlyMatch(res.at(i), docPath, tem)) {
			res.erase(res.begin() + i);
			i--;
		}
		if (i == 5) break;
	}
	return res;
}

//Operator 6: Search for a filetype. For example: filetype: txt + text search
//should be at the first position of the string
vector <int> searchFiletype(trieNode* root, string text, vector <string> docPath) {
	string type, word, temp = "";
	vector <int> res, tmp_res;
	stringstream find(text);
	int countWord = 0;
	while (find >> word) {
		countWord++;
		if (countWord == 2) {
			type = word;
		}
		if (countWord > 2)
			temp = temp + word + " ";
	}
	temp.erase(temp.size() - 1, 1);

	tmp_res = searchFullText(root, temp);

	for (int j = 0; j < tmp_res.size(); j++) {
		string tmp_type = fileType(docPath[tmp_res[j]]);

		if (tmp_type == type) res.push_back(tmp_res[j]);
	}

	return res;
}

//Operator 7: Search for a price. Put $ in front of a number. For example: $400
vector <int> searchForPrice(trieNode* root, string query) {
	vector<int> res;
	res = searchFullText(root, query);
	return res;
}

// Operator 8: Search hashtags. Put # in front of a word. For example: #helloworld
vector <int> searchHashtag(trieNode* root, string query) {
	vector<int> res;
	res = searchFullText(root, query);
	return res;
}

// Operator 9: Search for an exact match.
vector<int> searchExactMatch(trieNode* root, string query, vector<string> docPath) {
	string tem = "";
	for (int i = 1; i < query.size() - 1; i++)
		tem += query[i];
	vector<int>res = searchFullText(root, tem);
	for (int i = 0; i < res.size(); i++) {
		if (!isExactlyMatch(res.at(i), docPath, tem)) {
			res.erase(res.begin() + i);
			i--;
		}
		if (i == 5) break;
	}
	return res;
}

//Operator 10: Search for wild cards or unknown words
//Idea: remove the * character then perform searchFullText as usual
vector<int> searchWildCards(trieNode* root, string query) {
	string tmp = "";
	int i = 0;
	while (i < (int)query.length()) {
		if (query[i] == '*') i += 2;
		else tmp += query[i++];
	}
	return searchFullText(root, tmp);
}

// Operator 11: Search for a range of number. Put .. between two numbers. For example: $50..$100
vector <int> operator11(trieNode* root, string text) {
	vector<int> res;

	string query = "";
	string range;

	stringstream words(text);
	string word;
	int count = 0;
	while (words >> word) {
		if (count == 0 && checkRange(word) ){
			range = word;
			count++;
		}
		else query = query + word + " ";
	}
	if (query != "") {
		query.erase(query.size() - 1, 1);
	}

	if (range[0] == '$') {
		//get two numbers in the range
		string num1;
		string num2;
		int beginNum2 = 0;
		for (int i = 1; i < range.size() - 1; ++i) {
			if (range[i] == '.' && range[i + 1] == '.') {
				beginNum2 = i;
				break;
			}
			else {
				num1 += range[i];
			}
		}
		beginNum2 += 3;
		for (int i = beginNum2; i < range.size(); ++i) {
			num2 += range[i];
		}

		int number1 = toInt(num1), number2 = toInt(num2);

		for (int i = number1; i <= number2; ++i) {
			string tmp = toStr(i);
			tmp = '$' + tmp;
			vector <int> temp_res = searchFullText(root, query + tmp);
			for (int j = 0; j < temp_res.size(); ++j) {
				bool check = true;
				for (int k = 0; k < res.size(); ++k) {
					if (temp_res[j] == res[k]) {
						check = false;
						break;
					}
				}
				if (check == true) res.push_back(temp_res[j]);
				if (res.size() == 5) return res;
			}
		}
	}
	else {
		//get two numbers in the range
		string num1;
		string num2;
		int beginNum2 = 0;
		for (int i = 0; i < range.size() - 1; ++i) {
			if (range[i] == '.' && range[i + 1] == '.') {
				beginNum2 = i;
				break;
			}
			else {
				num1 += range[i];
			}
		}
		beginNum2 += 2;
		for (int i = beginNum2; i < range.size(); ++i) {
			num2 += range[i];
		}
		int number1 = toInt(num1), number2 = toInt(num2);
		
		for (int i = number1; i <= number2; ++i) {
			string tmp = toStr(i);
			vector <int> temp_res;
			if (query != "") {
				temp_res = searchFullText(root, query + " " + tmp);
			}
			else {
				temp_res = searchFullText(root, tmp);
			}
			for (int j = 0; j < temp_res.size(); ++j) {
				bool check = true;
				for (int k = 0; k < res.size(); ++k) {
					if (temp_res[j] == res[k]) {
						check = false;
						break;
					}
				}
				if (check == true) res.push_back(temp_res[j]);
				if (res.size() == 5) return res;
			}
		}
	}
}


//Operator 12: Search for synonyms
//Idea: Use a pre-installed text file of synonyms and perform
//a linear search on the content of that file
//Assuming that the query entered has only one word
vector<int> searchSynonyms(trieNode* root, string query) {
	//remove the '~' character
	query = query.substr(1, (int)query.length() - 1);
	vector<string> store;
	vector<int> ret;
	ifstream synonyms;
	synonyms.open("synonyms.txt");
	if (!synonyms.is_open()) {
		cout << "Cannot open synonyms.txt!\n";
		return ret;
	}
	bool ok = 0;
	while (!synonyms.eof()) {
		vector<string> tmp(3, "");
		synonyms >> tmp[0] >> tmp[1] >> tmp[2];
		if (tmp[0] == query or tmp[1] == query or tmp[2] == query) {
			synonyms.close();
			store = tmp;
			ok = 1;
			break;
		}
	}
	if (!ok) synonyms.close(), store.push_back(query);

	//store now stores all the synonyms together with that word
	string newQuery = "";
	for (int i = 0; i < (int)store.size(); i++) {
		if (i != 0) newQuery = newQuery + " " + store[i];
		else newQuery = store[i];
	}
	return searchFullText(root, newQuery);
}

// ============ SUPPORTING FUNCTION ============

// Check whether a number is already in vector.
bool isInList(vector<int> list, int x) {
	int l = 0, r = list.size() - 1, mid;
	while (l <= r) {
		mid = (l + r) / 2;
		if (list.at(mid) < x) l = mid + 1;
		else if (list.at(mid) > x) r = mid - 1;
		else return true;
	}
	return false;
}


// Check whether a word is right or not.
bool isWord(string word) {
	for (int i = 0; i < word.size(); i++) {
		if (word[i] >= 'a' && word[i] <= 'z');
		else if (word[i] >= '0' && word[i] <= '9');
		else if (word[i] == '#' || word[i] == '$');
		else return false;
	}
	return true;
}

// Remove punctuation.
void removePunctuation(string& word) {
	int n = word.size() - 1;
	if (word[n] == ';' || word[n] == '.' || word[n] == ',' || word[n] == '?' || word[n] == '!') {
		word.erase(n, 1);
	}
}

// Check if a string is a range or not
bool checkRange(string text) {
	if (text[0] != '$') {
		int count = 0;
		for (int i = 0; i < text.size(); i) {
			if ((text[i] < '0' || text[i]>'9') && text[i] != '.') return false;
			if (text[i] == '.') {
				if (count > 0) return false;
				if (i + 1 > text.size() - 2) return false;
				if (text[i + 1] != '.') return false;
				if (text[i + 2] == '.') return false;
				count++;
				i++;
			}
			i++;
		}
	}
	else {
		int count = 0;
		for (int i = 1; i < text.size(); i) {
			if ((text[i] < '0' || text[i]>'9') && text[i] != '.') return false;
			if (text[i] == '.') {
				if (count > 0) return false;
				if (i + 2 > text.size() - 2) return false;
				if (text[i + 1] != '.') return false;
				if (text[i + 2] != '$') return false;
				if (text[i + 3] == '.' || text[i + 3] == '$') return false;
				count++;
				i += 2;
			}
			i++;
		}
	}
	return true;
}

// Search range of number
// Still work with integer only

int toInt(string str) {
	int res = 0;
	for (int i = 0; i <str.size(); ++i) {
		res = res * 10 + str[i] - '0';
	}
	return res;
}

string toStr(int num) {
	string res = "";
	while (num > 0) {
		int r = num % 10;
		num = num / 10;
		char c = '0' + r;
		res = c + res;
	}
	return res;
}

// Compare two number in string form
// Return -1, 0, 1 for smaller than, equal to or bigger than respectively
int compareNumber(string num1, string num2) {
	float number1 = stof(num1);
	float number2 = stof(num2);
	if (number1 - number2 > 0) return 1;
	else if (number1 - number2 < 0) return -1;
	return 0;
}

string fileType(string doc_Path) {
	int mark_point;
	for (int i = doc_Path.length() - 1; i >= 0; i--) {
		if (doc_Path[i] == '.') {
			mark_point = i + 1;
			break;
		}
	}
	string res = "";
	for (int i = mark_point; i < doc_Path.length(); i++) {
		res += doc_Path[i];
	}
	return res;
}
