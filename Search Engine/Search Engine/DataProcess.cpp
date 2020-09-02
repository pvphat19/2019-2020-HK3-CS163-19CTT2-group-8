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
void indexMainTrie(trieNode* mainTrie, string path, int docnum) {
	vector<int> docNum;
	docNum.push_back(docnum);

	ifstream in;
	in.open(path);
	if (in.is_open()) {
		string singleWord;
		while (in >> singleWord) {
			toLower(singleWord);
			if(isWord(singleWord))
				insertToTrie(mainTrie, singleWord, docNum);
		}
		in.close();
	}
}

// Check whether a word is right or not.
bool isWord(string word) {
	for (int i = 0; i < word.size(); i++) {
		if (word[i] >= 'a' && word[i] <= 'z');
		else if (word[i] >= '0' && word[i] <= '9');
		else if (word[i] == '#' || word[i] == '$' || word[i] == '\'');
		else return false;
	}
	return true;
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
				if(isWord(word))
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
	cin >> path;
	// Add document path to file "__index.txt".
	ofstream out;
	out.open("Search Engine-Data/___index.txt", ios::ate);
	if (out.is_open()) {
		out << path;
		out.close();
	}
	// Add document path to vector<string>docPath.
	int docnum = docPath.size();
	docPath.push_back(path);

	// Index to main trie.
	indexMainTrie(mainTrie, path, docnum);
	// Index its title to title trie.
	indexTitle(titleTrie, path, docnum);
}

// Index all data.
void indexAllData(trieNode* mainTrie, trieNode* titleTrie, vector <string> & docPath) {
	for (int i = 0; i < docPath.size(); i++) {
		string path = docPath.at(i);
		indexMainTrie(mainTrie, path, i);
		indexTitle(titleTrie, path, i);
	}
}

// Read all doc names in "__index.txt" to vector<string>docPath.
void buildDocPath(vector <string> &docPath) {
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
    int count_doc_appear[12000],count_words=0;
    for (int i=0;i<12000;i++) count_doc_appear[i]=0;

    string tmp="";
    vector <int> res;

    for (int i=0; i<text.length() ;i++) {
        if ( text[i]==' ' || i==text.length()-1) {
            if (i==text.length()-1)
                if (text[i] != ' ')
                    tmp+=text[i];

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

    for (int i=0;i<12000;i++)
        if (count_doc_appear[i]==count_words)
            res.push_back(i);

    return res;
}

vector <int> searchTextfromVector (trieNode* root, vector <string> t) {
    vector <int> res;
    vector <int> *_doc = new vector <int>[t.size()];
    int count_doc_appear[12000];

    for (int i=0;i<12000;i++) count_doc_appear[i]=0;

    for (int i=0;i<t.size();i++)
        _doc[i]=searchFullText(root,t[i]);

    for (int i=0;i<t.size();i++) {
        for (int j=0;j<_doc[i].size();j++)
            count_doc_appear[j]++;
    }

    for (int i=0;i<12000;i++)
        if (count_doc_appear[i]==t.size())
            res.push_back(i);

    delete[] _doc;

    return res;
}

// ==========OPERATOR=============

// Operator 2: Search a text which contains "or". For example: X or C
vector <int> searchOr (trieNode* root, string text) {
    string tmp="", check_or="";
    vector <string> split;
    vector <int> res,temp_res;

    for (int i=0;i<text.length();i++) {
        if (text[i]==' ' || i==text.length()-1) {
            if (i==text.length()-1)
                if (text[i] != ' ') {
                    tmp+=text[i];
                    check_or+=text[i];
                }

            if (check_or!="or") {
                split.push_back(tmp);
                tmp="";
            }
            check_or="";
        } else {
            tmp+=text[i];
            check_or+=text[i];
        }
    }

    res=searchTextfromVector(root,split);
    return res;
}

// Operator 3: Search a text which do not contain a word. For example: Manchester -united
// It will search all the document which contain "Manchester" and do not contain "united"
vector <int> searchWithoutaWord(trieNode* root, string text) {
    string tmp="";
    vector <string> split;
    vector <int> res,temp_res,eliminate_doc;
    string word_eliminate="";

    int i=0;
    while (i<text.length()-1) {
        if (text[i]==' ') {
            if (text[i+1]='-') {
                int j=i+2;
                while (text[j] != ' ') {
                    word_eliminate+=text[j];
                    j++;
                }
                i=j;
            } else {
                split.push_back(tmp);
                tmp="";
            }
        } else tmp+=text[i];
        i++;
    }

    temp_res=searchTextfromVector(root,split);
    eliminate_doc=searchFullText(root,word_eliminate);

    for (int k=0;k<temp_res.size();k++) {
        int flag=0;

        for (int t=0;t<eliminate_doc.size();t++) {
            if (temp_res[k] == eliminate_doc[t]) {
                flag++;
                break;
            }
        }

        if (flag==0) res.push_back(k);
    }

    return res;
}

//Operator 4: Search for a title of a document. For example: intitle:hammer nails
vector <int> searchTitle (trieNode* titleTrie, string text) {
    string new_text="";
    vector <int> res;
    for (int i=8;i<text.length();i++) new_text+=text[i];
    res=searchFullText(titleTrie,new_text);
    return res;
}

//Operator 6: Search for a filetype. For example: filetype:txt + text search
//should be at the first position of the string
vector <int> searchFiletype (trieNode* root, string text, vector <string> docPath) {
    vector <string> type;
    vector <int> res,tmp_res;
    string tmp="";
    string new_text="";
    int i=9;
    while (text[i]!=' ') {
        if (text[i+1]==' ' || text[i]==',') {
            type.push_back(tmp);
            tmp="";
        } else {
            tmp+=text[i];
        }
        i++;
    }
    i++;

    for (int j=i;j<text.length();j++) new_text += text[j];
    tmp_res=searchFullText(root,new_text);

    for (int j=0;j<tmp_res.size();j++) {
        string tmp_type=fileType(docPath[tmp_res[j]]);
        for (int k=0;k<type.size();k++) {
            if (tmp_type==type[k]) {
                res.push_back(j);
                break;
            }
        }
    }

    return res;
}

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
//range in form of: 100..200 or $100..$200, the smaller must be in front of the larger
vector <int> searchRangeOfNumber(trieNode* root, string range) {
	vector <int> res;

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

		trieNode* cur = root->children[38];
		if (cur == nullptr) return res;
		searchRange(cur, res, num1, num2, "");

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

		trieNode* cur = root;
		if (cur == nullptr) return res;
		searchRange(cur, res, num1, num2, "");
	}

	return res;
}

//============SUPPORTING FUNCTION====================

//search range of number
//still work with integer only
void searchRange(trieNode* cur, vector<int> res, string num1, string num2, string currentNumber) {
	if (cur == nullptr) return;

	if (currentNumber != "") {
		if (stof(currentNumber) > stof(num2)) return;//if the current number is bigger than number 2, stop branch. (stof converts to float)
		if (stoi(currentNumber) < stoi(num1)) return;//if the current number is smaller than number 2, stop branch. (stoi converts to integer)
		if (cur->isEndWord) {
			if (stof(currentNumber) >= stof(num1)) {//check if it is truly greater than number 1 or not
				for (int i = 0; i < cur->documents.size(); ++i) {
					bool t = true;
					for (int j = 0; j < res.size(); ++j) {
						if (cur->documents[i] == res[j]) {
							t = false;
							break;
						}
					}
					if (t == true) {//it has not been add to result list yet
						res.push_back(cur->documents[i]);
					}
				}
			}
		}
	}

	for (int i = 26; i <= 35; ++i) {
		if (cur->children[i] != nullptr) {
			char tmp = '0' + i - 26;
			searchRange(cur, res, num1, num2, currentNumber + tmp);
		}
	}
}


//compare two number in string form
//return -1, 0, 1 for smaller than, equal to or bigger than respectively
int compareNumber(string num1, string num2) {
	float number1 = stof(num1);
	float number2 = stof(num2);
	if (number1 - number2 > 0) return 1;
	else if (number1 - number2 < 0) return -1;
	return 0;
}

string fileType (string doc_Path) {
    int mark_point;
    for (int i=doc_Path.length();i>=0;i--) {
        if (doc_Path[i]=='.') {
            mark_point=i+1;
            break;
        }
    }
    string res="";
    for (int i=mark_point;i<doc_Path.length();i++) {
        res+=doc_Path[i];
    }
    return res;
}

