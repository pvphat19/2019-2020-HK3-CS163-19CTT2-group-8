#include "QueryProcess.h"

//============INPUT QUERY================


string inputQuery() {
	cout << "Query: ";
	string query;
	getline(cin, query, '\n');
	//history


	//standardize the query
	string tmp = removeSymbols(query);
	query = removeStopWords(tmp);
	return query;
}

void presentResult(trieNode* root, string query, vector <string> docPath) {
	vector <int> queryTypes;
	queryTypes = queryType(query);

	vector<int> res;


	if (queryTypes[0] == 1) {
		res = searchAnd(root, query);
	}
	else if (queryTypes[0] == 2) {
		res = searchOr(root, query);
	}
	else if (queryTypes[0] == 3) {
		res = searchWithoutaWord(root, query);
	}
	else if (queryTypes[0] == 4) {
		res = searchTitle(root, query);
	}
	else if (queryTypes[0] == 5) {
		res = operator5(root, query, docPath);
	}
	else if (queryTypes[0] == 6) {
		res = searchFiletype(root, query, docPath);
	}
	else if (queryTypes[0] == 7) {
		res = searchForPrice(root, query);
	}
	else if (queryTypes[0] == 8) {
		res = searchHashtag(root, query);
	}
	else if (queryTypes[0] == 9) {
		res = searchExactMatch(root, query, docPath);
	}
	else if (queryTypes[0] == 10) {
		res = searchWildCards(root, query);
	}
	else if (queryTypes[0] == 11) {
		res = searchRangeOfNumber(root, query);
	}
	else {//operator 12
		res = searchSynonyms(root, query);
	}
	//give out result
	for (int i = 0; i < 5; ++i) {
		if (i > res.size() - 1) break;
		string docName = "";

		int j = docPath[i].size() - 1;
		while (j >= 0 && docPath[i][j] != '/') {
			docName += docPath[i][j];
			j--;
		}

		reverse(docName.begin(), docName.end());
		cout << i + 1 << ". " << docName << endl;
	}
	//let the clients choose what to present, not for operator 4, 6
	if (queryTypes[0] != 4 && queryTypes[0] != 6) {
		cout << "Enter the order of document you want to view result(0 for exit): ";
		int choice;
		cin >> choice;
		while (choice < 0 || choice>5 || choice > res.size()) {
			cout << "Please enter another number: ";
			cin >> choice;
		}

		//present the paragraph and highlight keywords
		string path = docPath[choice];
		presentParagraph(path, query, queryTypes[0]);
	}
}

//present the paragraph
void  presentParagraph(string path, string query, int queryType) {
	ifstream in;
	in.open(path);

	if (!in.is_open()) {
		cout << "can not open the file." << endl;
		return;
	}

	if (queryType == 1) {
		searchOperator1(in, query);
	}
	else if (queryType == 2) {
		searchOperator2(in, query);
	}
	else if (queryType == 3) {
		searchGeneral(in, query);
	}
	else if (queryType == 5) {
		searchGeneral(in, query);
	}
	else if (queryType == 7) {
		searchGeneral(in, query);
	}
	else if (queryType == 8) {
		searchGeneral(in, query);
	}
	else if (queryType == 9) {
		searchOperator9(in, query);
	}
	else if (queryType == 10) {
		searchOperator10(in, query);
	}
	else if (queryType == 11) {
		searchOperator11(in, query);
	}
	else {//operator 12
		searchOperator12(in, query);
	}

	in.close();
}

// ========== SUPPORTING FUNCTION ==========

//Search in a document sentences that contain query words
vector <string> searchSentence(ifstream& in, vector<string> queryWords) {
	vector <string> res;//vector that holds the sentence containing query words

	int n = queryWords.size();
	bool* a = new bool[n];//check if the word is contained in the sentences or not
	for (int i = 0; i < n; ++i) {
		a[i] = false;
	}

	string prev = "";
	string cur = "";
	while (!in.eof()) {
		getline(cin, cur, '.');
		if (cur[0] == ' ') {
			stringstream sentence(prev);
			string tmpWord;
			bool t = false;
			while (sentence >> tmpWord) {
				for (int i = 0; i < n; ++i) {
					if (!a[i]) {
						if (tmpWord == queryWords[i]) {
							t = true;
							a[i] = true;
							break;
						}
					}
				}
			}
			if (t == true) {
				res.push_back(prev);
			}
			prev = cur;
		}
		else {
			prev = prev + '.' + cur;
		}
	}
	stringstream sentence(prev);
	string tmpWord;
	bool t = false;
	while (sentence >> tmpWord) {
		for (int i = 0; i < n; ++i) {
			if (!a[i]) {
				if (tmpWord == queryWords[i]) {
					t = true;
					a[i] = true;
					break;
				}
			}
		}
	}
	if (t == true) {
		res.push_back(prev);
	}
	return res;
}

//Operator 1
void searchOperator1(ifstream& in, string query) {

	vector <string> res;//vector that holds the sentence containing query words

	vector <string> queryWords; //vector that holds words of query

	stringstream words(query);
	string word;
	string tem = "";
	while (words >> word) {
		if (word != "and") {
			queryWords.push_back(word);
		}
	}

	res = searchSentence(in, queryWords);

	//Present result
	for (int i = 0; i < res.size(); ++i) {
		cout << res[i] << endl;
	}
}

//Operator 2
void searchOperator2(ifstream& in, string query) {
	vector <string> res;//vector that holds the sentence containing query words

	vector <string> queryWords; //vector that holds words of query

	stringstream words(query);
	string word;
	string tem = "";
	while (words >> word) {
		if (word != "or") {
			queryWords.push_back(word);
		}
	}

	res = searchSentence(in, queryWords);

	//Present result
	for (int i = 0; i < res.size(); ++i) {
		cout << res[i] << endl;
	}
}

//Used for operator 3, 5, 7, 8
void searchGeneral(ifstream& in, string query) {
	vector <string> res;//vector that holds the sentence containing query words

	vector <string> queryWords; //vector that holds words of query

	stringstream words(query);
	string word;
	string tem = "";
	while (words >> word) {
		queryWords.push_back(word);
	}

	res = searchSentence(in, queryWords);

	//Present result
	for (int i = 0; i < res.size(); ++i) {
		cout << res[i] << endl;
	}
}

//Operator 9
void searchOperator9(ifstream& in, string query) {
	string res = "";

	string tem = "";
	for (int i = 1; i < query.size() - 1; i++)
		tem += query[i];

	string prev = "";
	string cur = "";
	while (!in.eof()) {
		getline(cin, cur, '.');
		if (cur[0] == ' ') {
			if (checkSubstring(prev, tem)) {
				res = prev;
				break;
			}
		}
		else {
			prev = prev + cur;
		}
	}
	if (res == "") {
		if (checkSubstring(prev, tem)) {
			res = prev;
		}
	}

	//present result
	cout << res << endl;
}
//check if str2 is a substring of str1
bool checkSubstring(string str1, string str2) {
	for (int i = 0; i < str1.size() - 1 - str2.size(); ++i) {
		bool t = true;
		for (int j = 0; j < str2.size() - 1; ++j) {
			if (str1[i + j] != str2[j]) {
				t = false;
				break;
			}
		}
		if (t == true) return true;
	}
	return false;
}

//Operator 10
void searchOperator10(ifstream& in, string query) {
	vector <string> res;//vector that holds the sentence containing query words

	vector <string> queryWords; //vector that holds words of query

	stringstream words(query);
	string word;
	string tem = "";
	int i = 0;
	while (i < (int)query.length()) {
		if (query[i] == '*') i += 2;
		else tem += query[i++];
	}
	res = searchSentence(in, queryWords);

	//Present result
	for (int i = 0; i < res.size(); ++i) {
		cout << res[i] << endl;
	}
}

//Operator 11
void searchOperator11(ifstream& in, string query) {
	vector <string> res;//vector that holds the sentence containing query words

	vector <string> queryWords; //vector that holds words of query
	vector <string> range;

	stringstream words(query);
	string word;
	string tem = "";
	while (words >> word) {
		if (!checkRange(word)) {
			queryWords.push_back(word);
		}
		else {
			range.push_back(word);
		}
	}

	int n = queryWords.size();
	bool* a = new bool[n];//check if the word is contained in the sentences or not
	for (int i = 0; i < n; ++i) {
		a[i] = false;
	}

	int m = range.size();
	bool* b = new bool[m];
	for (int i = 0; i < m; ++i) {
		b[i] = false;
	}

	string prev = "";
	string cur = "";
	while (!in.eof()) {
		getline(cin, cur, '.');
		if (cur[0] == ' ') {
			stringstream sentence(prev);
			string tmpWord;
			bool t = false;
			while (sentence >> tmpWord) {
				for (int i = 0; i < n; ++i) {
					if (!a[i]) {
						if (tmpWord == queryWords[i]) {
							t = true;
							a[i] = true;
							break;
						}
					}
				}
				for (int i = 0; i < m; ++i) {
					if (!b[i]) {
						if (checkInRange(range[i], tmpWord)) {
							t = true;
							b[i] = true;
							break;
						}
					}
				}
			}
			if (t == true) {
				res.push_back(prev);
			}
			prev = cur;
		}
		else {
			prev = prev + '.' + cur;
		}
	}
	stringstream sentence(prev);
	string tmpWord;
	bool t = false;
	while (sentence >> tmpWord) {
		for (int i = 0; i < n; ++i) {
			if (!a[i]) {
				if (tmpWord == queryWords[i]) {
					t = true;
					a[i] = true;
					break;
				}
			}
		}
		for (int i = 0; i < m; ++i) {
			if (!b[i]) {
				if (checkInRange(range[i], tmpWord)) {
					t = true;
					b[i] = true;
					break;
				}
			}
		}
	}
	if (t == true) {
		res.push_back(prev);
	}

	//Present result
	for (int i = 0; i < res.size(); ++i) {
		cout << res[i] << endl;
	}
}

bool checkInRange(string range, string text) {

	//check if the text is a number or not
	string number;
	if (text.size() == 0) return false;
	if (text[0] == '$') {
		for (int i = 1; i < text.size(); ++i) {
			if ((text[i] < '0' || text[i]>'9') && text[i] != '.') return false;
			number += text[i];
		}
	}
	else {
		for (int i = 0; i < text.size(); ++i) {
			if ((text[i] < '0' || text[i]>'9') && text[i] != '.') return false;
			number += text[i];
		}
	}

	//check in range

	string num1;
	string num2;
	if (range[0] == '$') {
		if (text[0] != '$') return false;
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

	}
	else {
		if (text[0] == '$') return false;
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

	}

	if (stof(number) > stof(num2)) return false;
	if (stof(number) < stof(num1)) return false;
	return true;
}
//Operator 12
void searchOperator12(ifstream& in, string query) {
	//remove the '~' character
	query = query.substr(1, (int)query.length() - 1);

	vector<string> store;
	ifstream synonyms;
	synonyms.open("synonyms.txt");
	if (!synonyms.is_open()) {
		cout << "Cannot open synonyms.txt!\n";
		return;
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
	vector <string> res;

	vector <string> queryWords; //vector that holds words of query

	stringstream words(newQuery);
	string word;
	string tem = "";
	while (words >> word) {
		queryWords.push_back(word);
	}

	res = searchSentence(in, queryWords);

	//Present result
	for (int i = 0; i < res.size(); ++i) {
		cout << res[i] << endl;
	}
}

// ===========STANDARDIZE QUERY===========

//remove symbols and punctuation
//not remove '.', '#', '$', '-', '+', '*', '~', '\'' and space
string removeSymbols(string query) {
	string tmp = query;

	for (int i = 0; i < tmp.size();) {
		if (tmp[i] >= 'a' && tmp[i] <= 'z');
		else if (tmp[i] >= 'A' && tmp[i] <= 'Z');
		else if (tmp[i] >= '0' && tmp[i] <= '9');
		else if (tmp[i] == '.' || tmp[i] == '#' || tmp[i] == '$' || tmp[i] == '-' || tmp[i] == '+' || tmp[i] == '*' || tmp[i] == '~' || tmp[i] == '\'');
		else if (tmp[i] == ' ') {
			if (tmp[i + 1] == ' ') {
				tmp.erase(i, 1);
				i--;
			}
		}
		else {
			tmp.erase(i, 1);
			i--;//i will not increase in that case, so we need to minus 1 so that it will increase 1 in the end
		}
		i++;
	}

	return tmp;
}

//remove all stop words, redundant spaces and turn query into lowercase
string removeStopWords(string query) {

	string res;
	ifstream in;
	in.open("stopWords.txt");

	if (!in.is_open()) {
		cout << "Error in loading stop words." << endl;
		return query;
	}

	string stopWords[300];
	int numStopWords = 0;
	while (!in.eof()) {
		getline(in, stopWords[numStopWords], '\n');
		numStopWords++;
	}

	toLower(res);

	//can xu li them cac ki tu dac biet
	string tmp;
	for (int i = 0; i < query.size(); ++i) {
		if (query[i] == ' ' || i == query.size() - 1) {
			if (i == query.size() - 1) {
				if (query[i] != ' ') tmp += query[i];
			}
			if (!isStopWords(tmp, stopWords, numStopWords)) {
				if (res == "") {
					res += tmp;
				}
				else {
					res += " ";
					res += tmp;
				}
			}
			tmp = "";
		}
		else {
			tmp += query[i];
		}
	}

	return res;
}


vector<int> queryType(string query) {
	//tra ve loai query tuong ung voi 12 loai cua thay
	vector<int> res;
	if (query.find("and") != string::npos) res.push_back(1);
	if (query.find("or") != string::npos) res.push_back(2);
	if (query.find("-") != string::npos) res.push_back(3);
	if (query.find("intittle:") == (size_t)0) res.push_back(4);
	if (query.find("+") != string::npos) res.push_back(5);
	if (query.find("filetype:") != string::npos) res.push_back(6);
	if (query.find("$") != string::npos) res.push_back(7);
	if (query.find("#") != string::npos) res.push_back(8);
	if (query.find("\"") != string::npos) res.push_back(9);
	if (query.find("*") != string::npos) res.push_back(10);
	if (query.find("..") != string::npos) res.push_back(11);
	if(query[0]=='~') res.push_back(12);
	return res;
}

void autoComplete();

// ========== SUPPORTING FUNCTION ==========
// Turn a string to lowercase.
void toLower(string& text) {
	for (int i = 0; i < text.size(); ++i) {
		if (text[i] >= 'A' && text[i] <= 'Z') {
			text[i] = text[i] - 'A' + 'a';
		}
	}
}

//check if a word is stop word or not
bool isStopWords(string word, string list[], int numStopWords) {
	for (int i = 0; i < numStopWords; ++i) {
		if (word == list[i]) return true;
	}
	return false;
}


bool isExactlyMatch(int docNum, vector<string> docPath, string query) {
	string path = docPath.at(docNum);
	stringstream words(query);
	string word[100];
	int i = 0;
	while (getline(words, word[i], ' '))
		i++;
	ifstream in;
	in.open(path);
	if (in.is_open()) {
		while (!in.eof()) {
			string w;
			in >> w;
			int flag = 0;
			if (w == word[0]) {
				for (int j = 1; j < i; j++) {
					in >> w;
					if (w == word[j])
						flag++;
				}
				if (flag == i - 1) return true;
			}
		}
		in.close();
		return false;
	}
}

// ============= HISTORY =============
trieNode2* initNode2() {
	trieNode2* root = new trieNode2;
	for (int i = 0; i < 39; i++)
		root->children[i] = nullptr;
	root->isEndWord = false;
	return root;
}

void insertToTrie2(trieNode2* root, string query) {
	trieNode2* cur = root;
	for (int i = 0; i < query.length(); i ++) {
		int index;
		if ('z' >= query[i] && query[i] >= 'a')
			index = query[i] - 'a';
		else if (query[i] == ' ')
			index = 36;
		else if (query[i] == '#')
			index = 37;
		else if (query[i] == '$')
			index = 38;
		else index = query[i] - '0' + 26;

		if (!cur->children[index])
			cur->children[index] = initNode2();

		cur = cur->children[index];
	}

	cur->isEndWord = true;
}

bool isChar(char c) {
	if ('z' >= c && c >= 'a') return true;
	if (c ==' ' || c =='#' || c=='$' ) return true;
	if (c >= 65 && c <= 90)return true;
	return false;
}

void deleteWholeTrie2(trieNode2*& root) {
	if (root == nullptr) return;
	for (int i = 0; i < 39; i++)
		deleteWholeTrie2(root->children[i]);
	delete root;
}

// If current node has a child, return 0; else if all children are NULL, return 1. 
bool isLastNode(trieNode2* root) {
	for (int i = 0; i < 39; i++)
		if (root->children[i])
			return 0;
	return 1;
}

// Print auto-suggestions. (pos to save to position of cursor)
void suggestions(trieNode2* root, string prefix, Console &c, int &pos, vector<string> & suggests)
{
	// Found a string in Trie with the given prefix 
	if (root->isEndWord) {
		c.setCursorPos(pos, 39);
		suggests.push_back(prefix);
		cout << prefix;
		pos++;
	}

	// All children struct node pointers are NULL 
	if (isLastNode(root))
		return;

	for (int i = 0; i < 39; i++) {
		if (root->children[i]) {
			// Append current character to prefix string 
			if (i == 36) prefix.push_back(' ');
			else if (i == 37) prefix.push_back('#');
			else if (i == 38) prefix.push_back('$');
			else if (i < 26) prefix.push_back(97 + i);
			else prefix.push_back(i - 26 + 48);

			// Recur over the rest 
			suggestions(root->children[i], prefix, c, pos, suggests);

			// Back track
			prefix.pop_back();
		}
	}
}

// Print suggestions for given query prefix. 
int printAutoSuggestions(trieNode2* root, string query, Console &c, vector<string> & suggests) {
	trieNode2* cur = root;

	// Check if prefix is present and find the node with 
	// last character of given string. 
	int i;
	int n = query.length();
	for (i = 0; i < n; i++)
	{
		int index;
		if ('z' >= query[i] && query[i] >= 'a')
			index = query[i] - 'a';
		else if (query[i] == ' ')
			index = 36;
		else if (query[i] == '#')
			index = 37;
		else if (query[i] == '$')
			index = 38;
		else index = query[i] - '0' + 26;

		// If given string doesnot exist in the trie return 0.
		if (!cur->children[index])
			return 0;

		cur = cur->children[index];
	}

	if (isLastNode(cur)) {
		
		return -1;
	}

	// If there are are nodes below last matching character. 
	else {
		string prefix = query;
		int pos = 14;
		suggestions(cur, prefix, c, pos, suggests);
		return 1;
	}
}

void retrieve2(trieNode2* root) {
	ifstream in;
	in.open("history.txt");
	if (in.is_open()) {
		string query;
		getline(in, query);
		while (query.size()) {
			insertToTrie2(root, query);
			query = "";
			getline(in, query);
		}
		in.close();
	}
}


void getInput(trieNode2* history, string &query, Console &c) {
	char ch;
	SEARCH_GUI(c, query);
	int t = 0;
	ch=_getch();

	while (ch != '\r') {
		system("cls");
		SEARCH_GUI(c, query);
		if (ch == '\r') break;
		if (ch == '\b') {
			query = query.substr(0, query.size() - 1);
		}
		else if (isChar(ch)) query += ch;
		SEARCH_GUI(c, query);
		vector <string> suggests;
		toLower(query);
		t = printAutoSuggestions(history, query, c, suggests);

		if (t == 1) {
			c.setCursorPos(11, 39 + query.size());
			int k, cur = -1;
			int line = suggests.size();
			bool fn, arrow;
			do {
				k = readkey(fn, arrow);
				if (arrow) {
					if (k == UP_ARROW) {
						if (cur == -1) break;
						else {
							// delete >
							c.setCursorPos(14 + cur, 37);
							cout << " ";

							cur = cur - 1;
							if (cur != -1) {
								c.setCursorPos(14 + cur, 37);
								cout << ">";
							}
							else c.setCursorPos(11, 39 + query.size());
						}
					}
					else if (k == DOWN_ARROW) {
						if (cur == line - 1) {
							c.setCursorPos(14 + cur, 37);
							cout << " ";
							cur = -1;
							c.setCursorPos(11, 39 + query.size());
						}
						else {
							if (cur != -1) {
								c.setCursorPos(14 + cur, 37);
								cout << " ";
							}
							
							cur = cur + 1;
							if (cur == line) {
								cur = -1;
								c.setCursorPos(11, 39 + query.size());
							}
							else {
								c.setCursorPos(14 + cur, 37);
								cout << ">";
							}
						}
					}
				}
				else {
					if (k == '\r') {
						if (cur == -1) {
							return;
						}
						else {
							query = suggests[cur];
							return;
						}
					}
					else {
						ch = (char)k;
						break;
					}
				}
			} while (1);
		}

		else ch = _getch();
	}
	if (t != 1 && t != -1) insertToTrie2(history, query);
}