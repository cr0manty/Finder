#pragma once
#include "includes.h"

class Objects
{
	void _create_listview();
	void _create_tree();
	void _crete_objects();
	void set_listviw_colum();
protected:
	HWND ListView;
	HWND Edit;
	HWND ComboBox;
	HWND *Button;
	HWND Tree;
	HWND Search;
	HMENU Menu;
	HWND hWnd;
	const int number_colum;

	Objects(HWND, int);

	void resize();
	~Objects();
};