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


void toLower(string& text) {
	for (int i = 0; i < text.size(); ++i) {
		if (text[i] >= 'A' && text[i] <= 'Z') {
			text[i] = text[i] - 'A' + 'a';
		}
	}
}

bool isStopWords(string word, string list[], int numStopWords) {
	for (int i = 0; i < numStopWords; ++i) {
		if (word == list[i]) return true;
	}
	return false;
}