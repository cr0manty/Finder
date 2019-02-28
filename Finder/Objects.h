#pragma once

class Objects
{
	void _create_listview();
	void _create_tree();
	void _crete_objects();
	void _set_listviw_colum();
	void create_hotkey();

	int buttons_amount;
protected:
	HWND ListView;
	HWND Edit;
	HWND ComboBox;
	HWND *Button;
	HWND Tree;
	HMENU CMenu;
	HMENU Main_Menu;
	HWND hWnd;
	HINSTANCE hInst;
	const int number_colum;
	RECT LVrt;
	RECT WindowRT;
	bool mouse_cmenu(const POINT &);

	Objects(HWND, int);
	~Objects();
public:
	bool resize();
	bool minimize() const;
};