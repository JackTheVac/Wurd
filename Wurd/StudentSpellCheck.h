#ifndef STUDENTSPELLCHECK_H_
#define STUDENTSPELLCHECK_H_

#include "SpellCheck.h"

#include <string>
#include <vector>
using namespace std;

class StudentSpellCheck : public SpellCheck {
public:
    StudentSpellCheck() { }
	virtual ~StudentSpellCheck();
	bool load(std::string dict_file);
	bool spellCheck(std::string word, int maxSuggestions, std::vector<std::string>& suggestions);
	void spellCheckLine(const std::string& line, std::vector<Position>& problems);

private:

	struct trie
	{
		char cur_letter;
		bool isPotentialEnd;

		vector<trie*> nextLetter;

	};

	//helper function
	trie* root;
	bool isValid(string& test);
	void fillVector(string& word, trie* current);
	bool checkForWord(string word, trie* root);
	void deleteDictionary(trie* root);


};

#endif  // STUDENTSPELLCHECK_H_
