#include "QueryProcess.h"

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
	//so 6 khong hieu lam
	if (query.find("$") != string::npos) res.push_back(7);
	if (query.find("#") != string::npos) res.push_back(8);
	if (query.find("\"") != string::npos) res.push_back(9);
	if (query.find("*") != string::npos) res.push_back(10);
	if (query.find("..") != string::npos) res.push_back(11);
	//12 khong hieu lam
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

// Print auto-suggestions.
void suggestions(trieNode2* root, string prefix)
{
	// Found a string in Trie with the given prefix 
	if (root->isEndWord) {
		cout << prefix;
		cout << endl;
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
			suggestions(root->children[i], prefix);

			// Back track
			prefix.pop_back();
		}
	}
}

// Print suggestions for given query prefix. 
int printAutoSuggestions(trieNode2* root, string query) {
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
		cout << query << endl;
		return -1;
	}

	// If there are are nodes below last matching character. 
	else {
		string prefix = query;
		suggestions(cur, prefix);
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


void getInput(trieNode2* history, string &query) {
	char c;
	cout << query;
	for (int i = 0; i < 4; i++) {
		c = _getch();
		query += c;
		cout << c;
	}
	cout << endl;
	int t = printAutoSuggestions(history, query);
	_getch();
	while (c != '\r') {
		system("cls");
		cout << query;
		c = _getch();
		system("cls");
		if (c == '\r') break;
		if (c == '\b') {
			query = query.substr(0, query.size() - 1);
		}
		else query += c;
		cout << query << endl;
		t = printAutoSuggestions(history, query);
		if (t == 1) _getch();
	}
	if (t != 1 && t != -1) insertToTrie2(history, query);
}