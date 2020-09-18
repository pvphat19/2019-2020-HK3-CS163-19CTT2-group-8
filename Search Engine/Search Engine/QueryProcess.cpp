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

	//give out result
	if (queryTypes.size() == 0 || queryTypes[0] == 1 || queryTypes[0] == 2 || queryTypes[0] == 3
		|| queryTypes[0] == 5 || queryTypes[0] == 6 || queryTypes[0] == 7 || queryTypes[0] == 8 || queryTypes[0] == 10) {
		for (int i = 0; i < 5; ++i) {
			if (i > res.size() - 1) break;
			string docName = "";
			docName = docPath[res[i]];
			cout << i + 1 << ". " << docName << endl;
			printGeneral(query, docName, c);
		}
	}
	else if (queryTypes[0] == 9) {
		for (int i = 0; i < 5; ++i) {
			if (i > res.size() - 1) break;
			string docName = "";

			docName = docPath[res[i]];
			cout << i + 1 << ". " << docName << endl;
			printQueryMatch(query, docName, c);
		}
	}

	// let the clients choose the document
	cout << "\n\n Please enter the document you want to view (1->5): ";
	int choice;
	cin >> choice;
	printDocument(docPath[res[choice - 1]]);

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
			index == 46;
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
	string word;
	int count = 0;
	while (words >> word) {
		count++;
	}

	ifstream in;
	in.open(docPath);
	if(in.is_open()) {
		while (!in.eof()) {
			vector <string> line;
			int flag = false;
			for (int i = 0; i < 9; i++) {
				string word = "";
				for (int j = 0; j < count; j++) {
					string tem;
					if (in >> tem)
						word = word + tem + " ";
				}
				word.erase(word.size() - 1, 1);
				line.push_back(word);
				if (word == query) flag = true;
			}

			if (flag) {
				c.setBackColor(BLACK).setForeColor(WHITE).write("...", false);
				for (int i = 0; i < 9; i++) {
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