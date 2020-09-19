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

void presentResult(trieNode* root, trieNode* titleTrie, string query, vector <string> docPath, Console& c) {
	vector <int> queryTypes;
	queryTypes = queryType(query);

	vector<int> res;

	if (queryTypes.size() == 0) res = searchFullText(root, query);
	else if (queryTypes[0] == 1) {
		res = searchAnd(root, query);
	}
	else if (queryTypes[0] == 2) {
		res = searchOr(root, query);
	}
	else if (queryTypes[0] == 3) {
		res = searchWithoutaWord(root, query);
	}
	else if (queryTypes[0] == 4) {
		res = searchTitle(titleTrie, query);
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
		res = operator11(root, query);
	}
	else if (queryTypes[0] == 12) {//operator 12
		res = searchSynonyms(root, query);
	}

	// Give out result.
	// if there is no document sastify:
	if (res.size() == 0) {
		cout << "Oops! Cannot find the documents contain your input!\n";
		return;
	}

	c.setForeColor(BLUE).setBackColor(BLACK).write("\t\t\tTop results:\n\n", false);
	if (queryTypes.size() == 0 || queryTypes[0] == 1 || queryTypes[0] == 2 || queryTypes[0] == 3
		|| queryTypes[0] == 5 || queryTypes[0] == 6 || queryTypes[0] == 7 || queryTypes[0] == 8 || queryTypes[0] == 10 ||queryTypes[0]==12) {
		for (int i = 0; i < 5 && i < res.size(); ++i) {
			if (i > res.size() - 1) break;
			string docName = "";
			docName = docPath[res[i]];

			string stt = to_string(i + 1);
			c.setForeColor(GREEN).setBackColor(BLACK).write(stt + ". " + docName, false);
			cout << endl;
			printGeneral(query, docName, c);
			cout << endl << endl;
		}
	}
	else if (queryTypes[0] == 9) {
		for (int i = 0; i < 5 && i < res.size(); ++i) {
			if (i > res.size() - 1) break;
			string docName = "";

			docName = docPath[res[i]];

			string stt = to_string(i + 1);
			c.setForeColor(YELLOW).setBackColor(BLACK).write(stt + ". " + docName, false);
			cout << endl;
			printQueryMatch(query, docName, c);
			cout << endl << endl;
		}
	}
	else if (queryTypes[0] == 4) {
		for (int i = 0; i < 5 && i < res.size(); ++i) {
			if (i > res.size() - 1) break;
			string docName = "";

			docName = docPath[res[i]];

			string stt = to_string(i + 1);
			c.setForeColor(YELLOW).setBackColor(BLACK).write(stt + ". " + docName, false);
			cout << endl;
			printTitle(query, docName, c);
			cout << endl << endl;
		}
	}
	else if (queryTypes[0] == 11) {
		for (int i = 0; i < 5 && i < res.size(); ++i) {
			if (i > res.size() - 1) break;
			string docName = "";

			docName = docPath[res[i]];

			string stt = to_string(i + 1);
			c.setForeColor(YELLOW).setBackColor(BLACK).write(stt + ". " + docName, false);
			cout << endl;
			printRange(query, docName, c);
			cout << endl << endl;
		}
	}
	else return;

	// let the clients choose the document
	c.setBackColor(BLACK).setForeColor(WHITE).write("\n\nPlease enter the document you want to view (1->5): ", false);
	int choice;
	cin >> choice;
	while (choice < 0 || choice>res.size() || choice > 5) {
		c.setBackColor(BLACK).setForeColor(WHITE).write("\n\nError. Please enter again: ", false);
		cin >> choice;
	}
	cout << endl;
	if (choice > 0) printDocument(docPath[res[choice - 1]]);
	else return;
}


// ========== SUPPORTING FUNCTION ==========

void printDocument(string docPath) {
	cout << "\t\t\t\t\t" << docPath << "\n\n";
	ifstream in;
	in.open(docPath);
	if (in.is_open()) {
		cout<<in.rdbuf();
		in.close();
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
	if (query.find("\"") != string::npos) res.push_back(9);
	if (query.find("intittle: ") == (size_t)0) res.push_back(4);
	if (query.find("filetype: ") != string::npos) res.push_back(6);
	if (query.find("-") != string::npos) res.push_back(3);
	if (query.find("..") != string::npos) res.push_back(11);
	if (query.find(" +and") != string::npos) res.push_back(5);
	if (query.find(" or") != string::npos) res.push_back(2);
	if(query[0]=='~') res.push_back(12);
	if (query.find("*") != string::npos) res.push_back(10);
	if (query.find(" and") != string::npos && query.find("+and") == string::npos) res.push_back(1);
	if (query.find("$") != string::npos) res.push_back(7);
	if (query.find("#") != string::npos) res.push_back(8);
	return res;
}


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
			toLower(w);
			int flag = 0;
			if (w == word[0]) {
				for (int j = 1; j < i; j++) {
					in >> w;
					toLower(w);
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
	for (int i = 0; i < 47; i++)
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
		else if (query[i] == '+')
			index = 39;
		else if (query[i] == ':')
			index = 40;
		else if (query[i] == '.')
			index = 41;
		else if (query[i] == '\'')
			index = 42;
		else if (query[i] == '*')
			index = 43;
		else if (query[i] == '-')
			index = 44;
		else if (query[i] == '"')
			index = 45;
		else if (query[i] == '~')
			index = 46;
		else index = query[i] - '0' + 26;

		if (!cur->children[index])
			cur->children[index] = initNode2();

		cur = cur->children[index];
	}

	cur->isEndWord = true;
}

bool isChar(char c) {
	if ('z' >= c && c >= 'a') return true;
	if (c == ' ' || c == '#' || c == '$' || c == '+' || c == ':' ||c=='~'
		|| c == '.' || c == '\'' || c == '*' || c == '-' || c == '"')
		return true;
	if (c >= 65 && c <= 90)return true;
	if (c >= '0' && c <= '9') return true;

	return false;
}

void saveTrieToFile2(trieNode2* root, ofstream& out, string word) {
	if (root == nullptr) return;
	if (root->isEndWord) {
		out << word << " ";
		out << endl;
	}
	for (int i = 0; i < 47; i++) {
		if (root->children[i]) {
			char c;
			if (i == 36) c = ' ';
			else if (i == 37) c = '#';
			else if (i == 38) c = '$';
			else if (i < 26) c = 97 + i;
			else if (i == 39) c = '+';
			else if (i == 40) c = ':';
			else if (i == 41) c = '.';
			else if (i == 42) c = '\'';
			else if (i == 43) c = '*';
			else if (i == 44) c = '-';
			else if (i == 45) c = '"';
			else if (i == 46)c = '~';
			else c=i - 26 + 48;
			string tem = word;
			word += c;
			saveTrieToFile2(root->children[i], out, word);
			word = tem;
		}
	}
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

	for (int i = 0; i < 47; i++) {
		if (root->children[i]) {
			// Append current character to prefix string 
			if (i == 36) prefix.push_back(' ');
			else if (i == 37) prefix.push_back('#');
			else if (i == 38) prefix.push_back('$');
			else if (i < 26) prefix.push_back(97 + i);
			else if (i == 39) prefix.push_back('+');
			else if (i == 40) prefix.push_back(':');
			else if (i == 41) prefix.push_back('.');
			else if (i == 42) prefix.push_back('\'');
			else if (i == 43) prefix.push_back('*');
			else if (i == 44) prefix.push_back('-');
			else if (i == 45) prefix.push_back('"');
			else if(i==46)prefix.push_back('~');
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
		else if (query[i] == '+')
			index = 39;
		else if (query[i] == ':')
			index = 40;
		else if (query[i] == '.')
			index = 41;
		else if (query[i] == '\'')
			index = 42;
		else if (query[i] == '*')
			index = 43;
		else if (query[i] == '-')
			index = 44;
		else if (query[i] == '"')
			index = 45;
		else if (query[i] == '~')
			index = 46;
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
				k = readkey(fn, arrow, ch);
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
					if (ch == '\r') {
						if (cur == -1) {
							return;
						}
						else {
							query = suggests[cur];
							return;
						}
					}
					else {
						
						break;
					}
				}
			} while (1);
		}

		else ch = _getch();
	}
	if (t != 1 && t != -1) insertToTrie2(history, query);
}

void printGeneral(string query, string docPath, Console& c) {
	// split query
	stringstream words(query);
	string word;
	vector<string>split;
	while (words >> word) {
		split.push_back(word);
	}

	// Open docPath to read
	ifstream in;
	in.open(docPath);
	if (in.is_open()) {
		bool* visited = new bool[split.size()]{ false };
		while (!in.eof()) {
			vector<string>line;
			vector<int> lineNum;
			for (int i = 0; i < 20; i++) {
				string word;
				if (in >> word) {
					line.push_back(word);
					toLower(word);
					for (int j = 0; j < split.size(); j++)
						if (word == split[j] && visited[j]==false) {
							lineNum.push_back(i);
							visited[j] = true;
						}
				}
					
			}
			if (lineNum.size()) {
				c.setBackColor(BLACK).setForeColor(WHITE).write("...", false);
				for (int i = 0; i < 20; i++) {
					int flag = 0;
					for (int j = 0; j < lineNum.size(); j++) {
						if (i == lineNum[j]) {
							c.setBackColor(YELLOW).setForeColor(BLACK).write(line[i], false);
							c.setBackColor(BLACK).setForeColor(WHITE).write(" ", false);
							flag = 1;
							break;
						}
					}
					if (flag == 0) {
						c.setBackColor(BLACK).setForeColor(WHITE).write(line[i] + " ", false);
					}
				}
				c.setBackColor(BLACK).setForeColor(WHITE).write("...", false);
			}

			// if all word in split are visited break
			int count = 0;
			for (int k = 0; k < split.size(); k++)
				if (visited[k] == true) count++;
			if (count == split.size())
				break;
		}

		delete[]visited;
		in.close();
	}
}

void printQueryMatch(string query, string docPath, Console& c) {
	// count word in query
	stringstream words(query);
	vector<string> split;
	string word;
	int count = 0;
	while (words >> word) {
		split.push_back(word);
		count++;
	}

	ifstream in;
	in.open(docPath);
	if(in.is_open()) {
		while (!in.eof()) {
			vector <string> line;
			int flag = false;
			for (int i = 0; i < 15; i++) {
				string word = "";
				if (in >> word) {
					toLower(word);
					if (word == split[0]) {
						for (int j = 0; j < count - 1; j++) {
							string tem;
							if (in >> tem) {
								word = word + " " + tem;
							}
						}
						
						toLower(word);
						line.push_back(word);
					}
					else line.push_back(word);
				}
				
				if (word == query) flag = true;
			}

			if (flag) {
				c.setBackColor(BLACK).setForeColor(WHITE).write("...", false);
				for (int i = 0; i < 15; i++) {
					if (line[i] == query) {
						c.setBackColor(YELLOW).setForeColor(BLACK).write(line[i], false);
						c.setBackColor(BLACK).setForeColor(WHITE).write(" ", false);
					}
					else {
						c.setBackColor(BLACK).setForeColor(WHITE).write(line[i] + " ", false);
					}
				}
				c.setBackColor(BLACK).setForeColor(WHITE).write("...", false);
				break;
			}
		}

		in.close();
	}
}

void printTitle(string query, string docPath, Console& c) {
	ifstream in;
	in.open(docPath);

	if (in.is_open()) {
		string title;
		getline(in, title, '.');
		vector <string> split;
		stringstream st(query);
		string tmp;
		while (st >> tmp) {
			split.push_back(tmp);
		}


		stringstream at(title);
		while (at >> tmp) {
			string tempWord = tmp;
			toLower(tempWord);
			while (tempWord[tempWord.size() - 1] < 48 || (tempWord[tempWord.size() - 1] > 57 && tempWord[tempWord.size() - 1] < 97) || tempWord[tempWord.size() - 1] > 122) {
				tempWord.erase(tempWord.size() - 1, 1);
			}
			bool t = false;
			for (int i = 0; i < split.size(); ++i) {
				if (tempWord == split[i]) {
					c.setBackColor(YELLOW).setForeColor(BLACK).write(tmp, false);
					c.setBackColor(BLACK).setForeColor(WHITE).write(" ", false);
					t = true;
					break;
				}
			}
			if (t == false) {
				c.setBackColor(BLACK).setForeColor(WHITE).write(tmp + " ", false);
			}
		}

		in.close();
	}
}

void printRange(string query, string docPath, Console& c) {

	vector <string> queryWords; //vector that holds words of query
	string range;// vecor that holds range

	stringstream words(query);
	string word;

	while (words >> word) {
		if (!checkRange(word)) {
			queryWords.push_back(word);
		}
		else {
			range = word;
		}
	}

	ifstream in;
	in.open(docPath);

	bool* visited = new bool[queryWords.size()]{ false };
	bool rangeVisited = false;

	bool* printed = new bool[queryWords.size()]{ false };
	bool rangePrinted = false;
	if (in.is_open()) {
		while (!in.eof()) {
			string line;
			getline(in, line, '.');

			bool containKeyword = false;
			stringstream st(line);
			string tmp;
			while (st >> tmp) {
				string tempWord = tmp;
				toLower(tempWord);
				while (tempWord.size()>0 && (tempWord[tempWord.size() - 1] < 48 || (tempWord[tempWord.size() - 1] > 57 && tempWord[tempWord.size() - 1] < 97) || tempWord[tempWord.size() - 1] > 122)) {
					tempWord.erase(tempWord.size() - 1, 1);
				}
				for (int i = 0; i < queryWords.size(); ++i) {
					if (visited[i] == false && tempWord == queryWords[i]) {
						containKeyword = true;
						visited[i] = true;
					}
				}
				if (rangeVisited == false && checkInRange(range, tempWord)) {
					containKeyword = true;
					rangeVisited = true;
				}
			}
			if (!containKeyword) continue;
			stringstream at(line);
			while (at >> tmp) {
				string tempWord = tmp;
				toLower(tempWord);
				while (tempWord.size()>0 && (tempWord[tempWord.size() - 1] < 48 || (tempWord[tempWord.size() - 1] > 57 && tempWord[tempWord.size() - 1] < 97) || tempWord[tempWord.size() - 1] > 122)) {
					tempWord.erase(tempWord.size() - 1, 1);
				}
				bool isQueryWords = false;
				for (int i = 0; i < queryWords.size(); ++i) {
					if (tempWord == queryWords[i]) {
						isQueryWords = true;
						printed[i] = true;
						break;
					}
				}
				if (checkInRange(range, tempWord)) {
					rangePrinted = true;
					isQueryWords = true;
				}
				if (isQueryWords) {
					c.setBackColor(YELLOW).setForeColor(BLACK).write(tmp, false);
					c.setBackColor(BLACK).setForeColor(WHITE).write(" ", false);
				}
				else {
					c.setBackColor(BLACK).setForeColor(WHITE).write(tmp + " ", false);
				}
				bool stop = true;
				for (int i = 0; i < queryWords.size(); ++i) {
					if (printed[i] == false) {
						stop = false;
						break;
					}
				}
				if (stop == true) {
					if (!rangePrinted) stop = false;
				}
				if (stop) {
					if (queryWords.size() > 0) {
						delete[]visited;
						delete[]printed;
					}
					return;
				}
			}
		}

		in.close();
	}
	if (queryWords.size() > 0) {
		delete[]visited;
		delete[]printed;
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