#pragma once
#include "qstring.h"
#include "qsettings.h"
 
class StringResource{
public:
	static bool languageIsPl();

	static QString WindowTitle_CrackerJackProblem();

	static QString LoginWindow_WrongLoginPasword();

	static QString NewProfileConfigHelp(int page);
	static QString NewProfileConfig_1_TooLongName();
	static QString NewProfileConfig_1_TooShortName();
	static QString NewProfileConfig_1_ForbiddenChars();
	static QString NewProfileConfig_2_anyProfIsChecked();
	static QString NewProfileConfig_3_SlidersAreInTheSamePosition();
	static QString NewProfileConfig_3_SlidersAreInWrongOrder();
	static QString NewProfileConfig_3_LastSliderIsZero();
	static QString NewProfileConfig_3_ShortcutNoValue();
	static QString NewProfileConfig_3_ShortcutManyValue();
	static QString NewProfileConfig_3_ShortCutHasForbiddenChars();
	static QString NewProfileConfig_3_comboBoxNoValue();
	static QString NewProfileConfig_3_comboBoxShareTheSameValue();

	static QString ProfileDataBaseManager_DbDoesNotExist();
	static QString ProfileDataBaseManager_NewDBWillBeCreated();

	static QString SelectProfileWindow_sureToDeleteProfile();
	static QString NewProfileConfig_finishCreatingNewProfile_WindowTitle();
	static QString NewProfileConfig_finishCreatingNewProfile_BoxMsg();
	static QString NewProfileConfig_cancelCreatingNewProfile_WindowTitle();
	static QString NewProfileConfig_cancelCreatingNewProfile_BoxMsg();
};

