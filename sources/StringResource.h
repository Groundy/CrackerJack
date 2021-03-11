#pragma once
#include "qstring.h"
class StringResource{
public:
	static QString WindowTitle_CrackerJackProblem();

	static QString NewProfileConfigHelp(int page);
	static QString NewProfileConfig_1_TooLongName();
	static QString NewProfileConfig_1_TooShortName();
	static QString NewProfileConfig_1_TooShortExactName();
	static QString NewProfileConfig_1_TooLongExactName();
	static QString NewProfileConfig_1_ExactNameHasNewLineChars();
	static QString NewProfileConfig_1_ForbiddenChars();
	static QString NewProfileConfig_2_anyProfIsChecked();
	static QString NewProfileConfig_3_SlidersAreInTheSamePosition();
	static QString NewProfileConfig_3_SlidersAreInWrongOrder();
	static QString NewProfileConfig_3_ShortcutNoValue();
	static QString NewProfileConfig_3_ShortcutManyValue();
	static QString NewProfileConfig_3_ShortCutHasForbiddenChars();
};

