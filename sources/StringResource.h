#pragma once
#include "qstring.h"
class StringResource{
public:
	static QString WindowTitle_CrackerJackProblem();

	static QString NewProfileConfigHelp(int page);
	static QString NewProfileConfig_1_TooLongName();
	static QString NewProfileConfig_1_EmptyName();
	static QString NewProfileConfig_1_ForbiddenChars();
	static QString NewProfileConfig_2_anyProfIsChecked();
	static QString NewProfileConfig_3_SlidersAreInTheSamePosition();
	static QString NewProfileConfig_3_SlidersAreInWrongOrder();
	static QString NewProfileConfig_3_ShortcutNoValue();
	static QString NewProfileConfig_3_ShortcutManyValue();
	static QString NewProfileConfig_3_ShortCutHasForbiddenChars();
};

