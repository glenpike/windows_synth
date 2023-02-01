// KeyboardInput.cpp': implementation of the CKeyboardInput class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "synth1.h"
#include "KeyboardInput.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKeyboardInput::CKeyboardInput()
{
}

CKeyboardInput::~CKeyboardInput()
{

}

int CKeyboardInput::KeyToNoteNum(int key)
{
	int NoteVal = -1;
	switch(key)
	{
		case 'A':
		case 'a':
			NoteVal = 0;
			break;
		case 'W':
		case 'w':
			NoteVal = 1;
			break;
		case 'S':
		case 's':
			NoteVal = 2;		
			break;
		case 'E':
		case 'e':
			NoteVal = 3;
			break;
		case 'D':
		case 'd':
			NoteVal = 4;
			break;
		case 'F':
		case 'f':
			NoteVal = 5;
			break;
		case 'T':
		case 't':
			NoteVal = 6;
			break;
		case 'G':
		case 'g':
			NoteVal = 7;
			break;
		case 'Y':
		case 'y':
			NoteVal = 8;
			break;
		case 'H':
		case 'h':
			NoteVal = 9;
			break;
		case 'U':
		case 'u':
			NoteVal = 10;
			break;
		case 'J':
		case 'j':
			NoteVal = 11;
			break;
		case 'K':
		case 'k':	
			NoteVal = 12;
			break;
		case 'O':
		case 'o':
			NoteVal = 13;
			break;
		case 'L':
		case 'l':
			NoteVal = 14;
			break;
		case 'P':
		case 'p':
			NoteVal = 15;
			break;
		case 186://';'
		case 189://';'
			NoteVal = 16;
			break;
		case 219:
			NoteVal = 17;
		break;
		case 192://'''
			NoteVal = 18;
			break;
		case 222:
			NoteVal = 19;
			break;
		default:
			NoteVal = -1;
			break;
	}
	return NoteVal;
}
