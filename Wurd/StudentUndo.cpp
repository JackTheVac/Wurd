#include "StudentUndo.h"
#include <stack>
using namespace std;

Undo* createUndo()
{
	return new StudentUndo;
}

void StudentUndo::submit(const Action action, int row, int col, char ch) {
	

	

	//if there is already an Undo in the stack
	if (UndoStack.empty() == false)
	{
		int prevRow = UndoStack.top().row;
		int prevCol = UndoStack.top().col;
		int testCol = UndoStack.top().testCol;

		//if action is to Undo Delete and you were passed in a char and the row is the same as the stack top's row
		if (action == Undo::Action::DELETE && ch != 0 && row == prevRow)
		{
			//if it's the same col, then you batch the result together by adding it to the top's result string
			if ( col == prevCol)  //for DELETE
			{
				UndoStack.top().result += ch;
				
				return;
			}
			//if the previous col is off by 1 of current col, then it is backspaced and so you would have to add the current char to the Front of top's result and update it's col
			else if ( abs(prevCol - col )== 1)  //for BACKSPACE
			{
				UndoStack.top().result = ch + UndoStack.top().result;
			
				UndoStack.top().col = col;
				return;
			}


		}
		//if action to redo is Insert and is in the same row as top's row
		else if (action == Undo::Action::INSERT and row == prevRow)
		{
			//if col is ahead of top's testcol by 1, add the ch to the result and update the testcol to the current's col;
			if (col - testCol== 1)
			{
				UndoStack.top().result += ch;
				
				UndoStack.top().testCol = col;
				return;
			}
		}


	}


	//for general case, create an infoObject and initialize the members with given parameters and push it back to the stack
	UndoInfo infoObject;
	infoObject.action = action;
	infoObject.row = row;
	
	infoObject.col = col;

	if (ch == '\t')
		infoObject.result = "    ";
	else
		infoObject.result += ch;

	
	
	infoObject.testCol = col;

	UndoStack.push(infoObject);



}

StudentUndo::Action StudentUndo::get(int& row, int& col, int& count, std::string& text) {


	//if stack is empty, return error
	if (UndoStack.empty())
	{
		return Undo::Action::ERROR;
	}
		

	//store top's object into helper variables
	UndoInfo testObject = UndoStack.top();

	int curRow = testObject.row;
	int curCol = testObject.col;

	string curString = testObject.result;

	//deleting the top's undo
	



	//for each action, match the return value with the spec by keeping track of what is passed in to each value
	if (testObject.action == Undo::Action::INSERT)
	{
		row = curRow;
		col = curCol - 1;
	
		count = curString.size();
		


		UndoStack.pop();
		return Undo::Action::DELETE;
	}
	else if (testObject.action == Undo::Action::DELETE)
	{

		

		row = curRow;
		col = curCol;;

		count = 1;


		text = curString;
		UndoStack.pop();

		return Undo::Action::INSERT;
	}
	else if (testObject.action == Undo::Action::SPLIT)
	{
		row = curRow;
		col = curCol;

		count = 1;


		UndoStack.pop();

		return Undo::Action::JOIN;
	}
	else if (testObject.action == Undo::Action::JOIN)
	{
		row = curRow;
		col = curCol;
		
		count = 1;
		UndoStack.pop();


		return Undo::Action::SPLIT;
	}

	
	
	
	
	
	
	
	return Action::ERROR;  // TODO
}

void StudentUndo::clear() {
	
	//popping entire stack until stack is empty
	while (!UndoStack.empty())
		UndoStack.pop();

}
