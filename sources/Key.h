#pragma once
#include <cstddef>
#include <qstring.h>
#include "qsplitter.h"
#include "qmap.h"

class Key {
public:
	static enum KEYS { F1=0x70, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
		n0=0x30, n1, n2, n3, n4, n5, n6, n7, n8, n9 ,
		ESC = 0x1B,
		INSERT = 0x2D,
		DELETE_ = 0x2E,
		HOME = 0x24, 
		END = 0x23,
		PAGEUP = 0x21,
		PAGEDOWN = 0x22,
		ScrollLock = 0x91,
		PauseBreak = 0x13,
		SLASH = 0x6F,
		ASTERIX = 0x6A,
		MINUS = 0x64,
		PLUS = 0x6B,
		endOfEnum = -1
	};	

	int number;

	static bool checkIfnumberIsAloowed(int keyNumber);
	Key();
	Key(int numberToSet);
	Key(QKeySequence qsec);
	QKeySequence toQKeySequence(Key key);

};

