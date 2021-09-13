#include "StudentSpellCheck.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

SpellCheck* createSpellCheck()
{
	return new StudentSpellCheck;
}

StudentSpellCheck::~StudentSpellCheck() {
	
	deleteDictionary(root);

}



bool StudentSpellCheck::load(std::string dictionaryFile) {
	ifstream dictionary(dictionaryFile);

	if (!dictionary)
		return false;

	//create a dummy trie
	root = new trie;
	root->cur_letter = '#';


	string c;
	
	//retrieving every line in the dictionary to c and filling the trie with helper function
	while (getline(dictionary, c))
	{
		//check if the word is a word 
		if (isValid(c))
		{

			fillVector(c, root);

		}


	}



	return true;

}


bool StudentSpellCheck::spellCheck(std::string word, int max_suggestions, std::vector<std::string>& suggestions) {
	
	//lower all chars first
	for (int i = 0; i < word.size(); i++)
	{
		word.at(i) = tolower(word.at(i));
	}

	//use helper function to see if word is already in dictionary
	if (checkForWord(word, root))
		return true;


	suggestions.clear();

	

	string test = word;
	int counter = 0;

	//loops through each letter in the word
	for (int i = 0; i < test.size(); i++)
	{
		//up to max suggestions
		if (counter == max_suggestions)
			break;

		string test = word;
		char c = 'a';
		//for each letter, test it with all 26 alphabets + 1 for ' and check if said new string is in the dictionary, and if so, pushback to suggestions
		for (int j = 0; j < 26; j++) 
		{
			if (counter == max_suggestions)
				break;
			test.at(i) = c;
			if (checkForWord(test, root))
			{
				suggestions.push_back(test);
				counter++;
			}

			c++;
		
		}

		if (counter == max_suggestions)
			break;
		c = '\'';
		test.at(i) = c;
		if (checkForWord(test, root))
		{
			suggestions.push_back(test);
			counter++;
		}



	}

	return false;

	


}

void StudentSpellCheck::spellCheckLine(const std::string& line, std::vector<SpellCheck::Position>& problems) {


	//go through the entire line
	for (int i = 0; i < line.size(); i++)
	{
		//starting with the first letter, mark the pos, and loop the rest till you find where it's not an alphabet nor ' and track the length
		if (isalpha(line.at(i)) || line.at(i) == '\'')
		{
			int startPos = i;
			int length = 1;
			for (int j = i + 1; j < line.size(); j++)
			{

				
				if (!isalpha(line.at(j)) && line.at(j) != '\'')
				{
					
					break;
				}
				
				length++;
				
					
			}

			//with the start pos and the length, extract the string and test if it is in the dictionary, if not push said word into suggestions
			string test = line.substr(startPos, length);

			for (int i = 0; i < test.size(); i++)
				test.at(i) = tolower(test.at(i));
			
			if (checkForWord(test, root) == false)
			{

				int endPos = startPos + length;
				problems.push_back({ startPos, endPos });
			}

			i = startPos + length;
			

		}
	}


}


bool StudentSpellCheck::isValid(string& test)
{

	//lower each letter  that is an alphabet or ', otherwise erase it
	for (int i = 0; i < test.size(); i++)
	{
		if (isalpha(test.at(i)) || test.at(i) == '\'')
		{
			test.at(i) = tolower(test.at(i));
		}
		else
		{
			test.erase(i, 1);
		}
	}

	if (test.size() != 0)
		return true;
	else
		return false;

}

void StudentSpellCheck::fillVector(string& word, trie* current)
{
	//base case, check if word is empty and see if current node has a period in its child, if yes return
	if (word.size() == 0)
	{
		if (current->isPotentialEnd == true)
			return;
		else
		{
			current->isPotentialEnd = true;
			trie* Node = new trie;
			Node->cur_letter = '.';
			current->nextLetter.push_back(Node);
			return;
		}
	}


	//if there's already the next letter in the trie, use that branch
	for (int i = 0; i < current->nextLetter.size(); i++)
	{
		if (word.at(0) == current->nextLetter[i]->cur_letter)
		{
			word.erase(0, 1);
			fillVector(word, current->nextLetter[i]);
			return;
		}
	}


	//for when you can't find your letter within the vector
	trie* Node = new trie;
	Node->cur_letter = word.at(0);
	current->nextLetter.push_back(Node);

	word.erase(0, 1);
	fillVector(word, Node);
	return;




}


bool StudentSpellCheck::checkForWord(string word, trie* root)
{

	//if you reach the end of the word and there's a period at the current node, then the word exist
	if (word.size() == 0)
	{
		if (root->isPotentialEnd == true)
			return true;
		else
			return false;

	}
	

	string x = word;

	//go through all possible next letter in the current Node's vector, and if any matches the first, then you return the recusion of it
	for (int i = 0; i < root->nextLetter.size(); i++)
	{
		if (x.at(0) == root->nextLetter[i]->cur_letter)
		{
			x.erase(0, 1);
			return checkForWord(x, root->nextLetter[i]);
		}
	}
	return false;





}





void StudentSpellCheck::deleteDictionary(trie* root)
{

	//go through each branch of the trie before deleting itself
	for (int i = 0; i < root->nextLetter.size(); i++)
	{
		deleteDictionary(root->nextLetter.at(i));
	}


	delete root;


}