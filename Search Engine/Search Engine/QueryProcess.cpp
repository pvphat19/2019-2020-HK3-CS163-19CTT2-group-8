#include "QueryProcess.h"

string removeStopWords(string query) {

	string res;
	ifstream in;
	in.open("stopWords.txt");

	if (!in.is_open()) {
		cout << "Error in loading stop words." << endl;
		return query;
	}

	string stopWords[500];
	int numStopWords = 0;
	while (!in.eof()) {
		getline(in, stopWords[numStopWords], '\n');
		numStopWords++;
	}

	//can xu li them dau cach va cac ki tu dac biet
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

	toLower(res);

	return res;
}


// ========== SUPPORTING FUNCTION ==========
// Turn a string to lowercase.
void toLower(string& text) {
	for (char& c : text)
		c = tolower(c);
}

bool isStopWords(string word, string list[], int numStopWords) {
	for (int i = 0; i < numStopWords; ++i) {
		if (word == list[i]) return true;
	}
	return false;
}

vector<int> queryType(string query) {
	//tra ve loai query tuong ung voi 12 loai cua thay
	vector<int> res;
	if(query.find("and")!=string::npos) res.push_back(1);
	if(query.find("or")!=string::npos) res.push_back(2);
	if(query.find("-")!=string::npos) res.push_back(3);
	if(query.find("intittle:")==(size_t)0) res.push_back(4);
	if(query.find("+")!=string::npos) res.push_back(5);
	//so 6 khong hieu lam
	if(query.find("$")!=string::npos) res.push_back(7);
	if(query.find("#")!=string::npos) res.push_back(8);
	if(query.find("\"")!=string::npos) res.push_back(9);
	if(query.find("*")!=string::npos) res.push_back(10);
	if(query.find("..")!=string::npos) res.push_back(11);
	//12 khong hieu lam
	return res;
}