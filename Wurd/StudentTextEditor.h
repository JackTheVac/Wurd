#ifndef STUDENTTEXTEDITOR_H_
#define STUDENTTEXTEDITOR_H_

#include "TextEditor.h"
#include <list>
using namespace std;

class Undo;

class StudentTextEditor : public TextEditor {
public:

	StudentTextEditor(Undo* undo);
	~StudentTextEditor();
	bool load(std::string file);
	bool save(std::string file);
	void reset();
	void move(Dir dir);
	void del();
	void backspace();
	void insert(char ch);
	void enter();
	void getPos(int& row, int& col) const;
	int getLines(int startRow, int numRows, std::vector<std::string>& lines) const;
	void undo();

private:
	int cur_row;
	int cur_col;
	int total_row;
	
	list<string> textFile;

	list<string>::iterator cur_row_itr;

	int undoBatch_PrevRow;
	int undoBatch_PrevCol;


};

#endif // STUDENTTEXTEDITOR_H_
