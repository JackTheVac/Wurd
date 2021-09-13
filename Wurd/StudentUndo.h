#ifndef STUDENTUNDO_H_
#define STUDENTUNDO_H_

#include <string>
#include <stack>
#include "Undo.h"
using namespace std;

class StudentUndo : public Undo {
public:

	void submit(Action action, int row, int col, char ch = 0);
	Action get(int& row, int& col, int& count, std::string& text);
	void clear();

private:


private:
	struct UndoInfo
	{
		Undo::Action action;
		int row;
		int col;
		
		int testCol;
		
		

		string result;
	};

	
	stack<UndoInfo> UndoStack;

};

#endif // STUDENTUNDO_H_
