// KeyboardInput.h: interface for the CKeyboardInput class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KEYBOARDINPUT_H__BD002013_C56A_11D4_AA26_000000000000__INCLUDED_)
#define AFX_KEYBOARDINPUT_H__BD002013_C56A_11D4_AA26_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CKeyboardInput  
{
public:
	int KeyToNoteNum(int key);
	CKeyboardInput();
	virtual ~CKeyboardInput();
};

#endif // !defined(AFX_KEYBOARDINPUT_H__BD002013_C56A_11D4_AA26_000000000000__INCLUDED_)
