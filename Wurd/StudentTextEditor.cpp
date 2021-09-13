#include "StudentTextEditor.h"
#include "Undo.h"
#include <string>
#include <vector>
#include <fstream>
using namespace std;


TextEditor* createTextEditor(Undo* un)
{
	return new StudentTextEditor(un);
}

StudentTextEditor::StudentTextEditor(Undo* undo)
	: TextEditor(undo), cur_row(0), cur_col(0), total_row(1)
{
	textFile.push_back("");
	cur_row_itr = textFile.begin();
	
}

StudentTextEditor::~StudentTextEditor()
{
	
}

bool StudentTextEditor::load(std::string file) {

	ifstream loadFile(file);

	if (!loadFile)
		return false;


	reset();



	//for each line in the file, push it back into our STL container and update total row
	string k;
	while (getline(loadFile, k))
	{
		for (int i = 0; i < k.size(); i++)
		{
			if (k.at(i) == '\r')
				k.erase(i);
		}
		textFile.push_back(k);
		total_row++;
	}


	


	cur_row = 0;
	cur_col = 0;
	cur_row_itr = textFile.begin();
	return true;




}

bool StudentTextEditor::save(std::string file) {

	ofstream outfile(file);

	if (!outfile)
		return false;

	//go through each row in our STL Container and send it to our File Location
	for (list<string>::iterator itr = textFile.begin(); itr != textFile.end(); itr++)
	{
		outfile << (*itr) << '\n';
	}

	return true;

}

void StudentTextEditor::reset() {


	//delete every row in our STL Container and clear our undo stack 
	for (list<string>::iterator itr = textFile.begin(); itr != textFile.end();)
	{
		itr = textFile.erase(itr);
	}

	getUndo()->clear();




	cur_row_itr = textFile.begin();
	cur_col = 0;
	cur_row = 0;



}

void StudentTextEditor::move(Dir dir) {


	//Helper variables
	list<string>::iterator tempPrev = cur_row_itr;
	list<string>::iterator tempNext = cur_row_itr;
	int cur_size = (*cur_row_itr).size();
	tempNext++;


	//Move Cursor based on Received Direction
	//For each case, make sure it is within the bound of view
	if (dir == UP && cur_row != 0)
	{
		 tempPrev--;
		 if (cur_col >  (*tempPrev).size())
		 {
		
		   cur_col = (*tempPrev).size();
		 }

		cur_row--;
		cur_row_itr--;
		
	}
	else if (dir == DOWN && tempNext != textFile.end())
	{

		if (cur_col > (*tempNext).size())
		{
			cur_col = (*tempNext).size();
		}
			

		cur_row++;
		cur_row_itr++;
	}
	else if (dir == LEFT)
	{
		if (cur_col == 0 && cur_row - 1 >= 0)
		{
			cur_row_itr--;
			cur_row--;
			cur_col = (*cur_row_itr).size();
		}
		else if (cur_col != 0)
		{
			cur_col--;
		}
	}
	else if (dir == RIGHT)
	{
		if (cur_col == cur_size && tempNext != textFile.end())
		{
			cur_row_itr++;
			cur_row++;
			cur_col = 0;
		}
		else if (cur_col < (*cur_row_itr).size())
		{
			cur_col++;
		}
	}
	else if (dir == HOME)
	{
		cur_col = 0;
	}
	else if (dir == END)
	{
		cur_col = (*cur_row_itr).size();   
	}


}

void StudentTextEditor::del() {
	

	//Helper Variables
	list<string>::iterator temp = cur_row_itr;
	temp++;
	string cur = *cur_row_itr;

	
	//Check to see if current Cursor is at the end of the current line, if so, merge with the next line if there's one
	if (cur_col == (*cur_row_itr).size()  &&  temp != textFile.end() )
	{
		string nextline = *temp;
		string result = cur + nextline;
		*cur_row_itr = result;
		textFile.erase(temp);
		total_row--;
		
		getUndo()->submit(Undo::Action::JOIN, cur_row, cur_col);


	}
	//Check to see if current Cursor is anywhere that is less than the current line's size and if so delete the current character
	else if (cur_col < (*cur_row_itr).size() )
	{
		char justDeleted = cur_row_itr->at(cur_col);
		cur.erase(cur_col, 1);
		*cur_row_itr = cur;
		
		getUndo()->submit(Undo::Action::DELETE, cur_row, cur_col, justDeleted);

	}


	
	
	
	





}

void StudentTextEditor::backspace() {
	

	//helper Variables
	list<string>::iterator temp = cur_row_itr;
	string cur = *cur_row_itr;



	//if Cursor is at anywhere that is not beginning of the line, delete the previous character the char is currently pointing at
	if (cur_col > 0)
	{
		cur_col--;
		char justDeleted = cur_row_itr->at(cur_col );
		
		cur.erase(cur_col, 1);
		*cur_row_itr = cur;
		

		getUndo()->submit(Undo::Action::DELETE, cur_row, cur_col, justDeleted);
		
	}
	//if cursor is at the beginning of the line and there is a line above the current line, merge it
	else if (cur_col == 0 && temp != textFile.begin())
	{
		temp--;
		int prev_line_size = (*temp).size();
		
		string prevLine = (*temp);
		string result = prevLine + cur;
		(*temp) = result;
		
		cur_col = prev_line_size;
		cur_row--;
		total_row--;

		textFile.erase(cur_row_itr);
		cur_row_itr = temp;

		getUndo()->submit(Undo::Action::JOIN, cur_row, cur_col);
	
	}

	


}

void StudentTextEditor::insert(char ch) {


	//helper variables
	
	int ccol = cur_col;


	


	if (cur_row_itr == textFile.end())
		return;

	//if character inserted is a tab, insert space four times and submit to undo four times, incrementing the col each time
	if (ch == '\t')
	{
		
	

		(*cur_row_itr).insert(ccol, 4, ' ');
		
		
		for (int i = 0; i < 4; i++)
		{
			cur_col++;
			getUndo()->submit(Undo::Action::INSERT, cur_row, cur_col, ' ');
		}
	
		
		

		return;
	}
	else
	//for every other insertion
	{
		
		(*cur_row_itr).insert(ccol, 1, ch);
		cur_col++;

		getUndo()->submit(Undo::Action::INSERT, cur_row, cur_col, ch);

		return;
	}





}

void StudentTextEditor::enter() {

	

	//helper variables
	int crow = cur_row;
	int ccol = cur_col;



	string cur = *cur_row_itr;

	//splitting current line into two lines, one before the cursor and one after the cursor
	string preSplit = cur.substr(0, ccol);
	string splitted = cur.substr(ccol);



	//set current row to the before the cursor line
	*cur_row_itr = preSplit;


	list<string>::iterator temp = cur_row_itr;

	temp++;
	//if there is no next line, add a whole new line with post-cursor string
	if (temp == textFile.end())
	{
		textFile.push_back(splitted);

	}
	//if there is a line, insert the post cursor string into the existing line by adding on to it
	else
	{
		textFile.insert(temp, splitted);
		

	}



	cur_row_itr++;
	total_row++;
	cur_row++;
	cur_col = 0;


	getUndo()->submit(Undo::Action::SPLIT, crow, ccol);
	



}

void StudentTextEditor::getPos(int& row, int& col) const {
	row = cur_row;
	col = cur_col;
}

int StudentTextEditor::getLines(int startRow, int numRows, std::vector<std::string>& lines) const {
	
	//checks paramenter
	if (startRow < 0 || numRows < 0 || startRow > total_row)
		return -1;


	lines.clear();

	if (startRow == total_row)
		return 0;

	//checks how much our current line's iterator is off by the start row
	list<string>::iterator temp = cur_row_itr;
	int difference = abs(cur_row - startRow);


	//if there is a difference, our iterator increment or decrement by one until the difference is 0, based on if the start row is larger or current row is larger
	if (difference > 0)
	{
		if (cur_row > startRow)
		{
			while (difference != 0)
			{
				temp--;
				difference--;
			}
		}
		else if (cur_row < startRow)
		{
			while (difference != 0)
			{
				temp++;
				difference--;
			}
		}
	}


	//starting from the new current line's iterator till number of rows left = 0, push it back into our parameter vector
	int count = 0;
	for (int i = 0; i < numRows && temp != textFile.end(); i++, temp++)
	{
		string cur = *temp;
		lines.push_back(cur);
		count++;
	}

	return count;
	






}

void StudentTextEditor::undo() {
	

	string text;
	int row;
	int col;
	int count;


	Undo::Action action = getUndo()->get(row, col, count, text);
	


	if (action == Undo::Action::ERROR)
	{
		return;
	}

	//helper variables
	list<string>::iterator temp = cur_row_itr;
	int difference = abs(cur_row - row);


	//same concept, incrementing or decrementing our pointer to our desired spot until the difference is 0
	if (difference > 0)
	{
		if (cur_row > row)
		{
			while (difference != 0)
			{
				temp--;
				difference--;
			}
		}
		else if (cur_row < row)
		{
			while (difference != 0)
			{
				temp++;
				difference--;
			}
		}
	}

	cur_row_itr = temp;




	//if we need to re-insert, then we insert the string we got from our getfunction at the position we were given
	if (action == Undo::Action::INSERT)
	{

		cur_col = col;


		(*temp).insert(col, text);

		cur_col = col;





		return;
	}
	//if to redelete, delete at the position we were given with and up to count many times
	else if (action == Undo::Action::DELETE)
	{

		string cur = *cur_row_itr;
		cur_col = col;
		cur_row = row;


		cur.erase(cur_col, count);
		*cur_row_itr = cur;


		return;

	/*	(*temp).erase(col, count);
		cur_col = col;
		cur_row = row;*/

	}
	//if to re-split, we have a pre-cursor string and post-cursor string, cursor position given by undo's getfunction. Then, we push it back into a new line
	else if (action == Undo::Action::SPLIT)
	{
		cur_col = col;
		cur_row = row;

	

		int ccol = cur_col;



		string cur = *cur_row_itr;

		string preSplit = cur.substr(0, ccol);
		string splitted = cur.substr(ccol);

		//preSplit += '\n';

		*cur_row_itr = preSplit;


		temp++;
		//if there is no next line, add a whole new line with post-cursor string
		if (temp == textFile.end())
		{
			textFile.push_back(splitted);

		}
		//if there is a line, insert the post cursor string into the existing line by adding on to it
		else
		{
			textFile.insert(temp, splitted);


		}


		

		total_row++;

		return;


	}
	//if to rejoin then at the given col and pos, merge the next line to the current position
	else if (action == Undo::Action::JOIN)
	{
		cur_col = col;
		cur_row = row;


		list<string>::iterator temp = cur_row_itr;
		temp++;
		string cur = *cur_row_itr;

		string nextline = *temp;
		string result = cur + nextline;
		*cur_row_itr = result;
		textFile.erase(temp);
		total_row--;

		return;


	}


}
