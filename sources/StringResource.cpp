#include "StringResource.h"

QString StringResource::WindowTitle_CrackerJackProblem()
{
	return "CrackerJack problem";
}

QString StringResource::NewProfileConfigHelp(int page){
	QString toRet = "";
	switch (page)
	{
	case 1:
		toRet = "Choose name for your profile, it can be your character name but it's not mandatory.";
		break;
	case 2:
		toRet = "Choose profession of your character";
		break;
	case 3:
		toRet = "Choose how many methodes of restoring health do want, up to 5 diffrent methodes. ";
		toRet.append("Move sliders to set value of missing health that will be used to auto heal. ");
		toRet.append("First Slider should be used for smallest healing, last for biggest. ");
		toRet.append("If healing would be needed, program will use appropriate healing to your remaining health. ");
		toRet.append("To all used sliders you have to add the same hotkey that use in game. ");
		break;
	case 4:
		toRet = "Choose how many methodes of restoring mana do want, up to 5 diffrent methodes. ";
		toRet.append("Move sliders to set value of missing mana that will be used to auto mana restore. ");
		toRet.append("First Slider should be used for smallest mana regeneration, last for biggest. ");
		toRet.append("If mana will be low, program will use appropriate methode to regenerate it. ");
		toRet.append("To all used sliders you have to add the same hotkey that use in game. ");
		break;
	default:
		break;
	}
	return toRet;
}

QString StringResource::NewProfileConfig_1_TooLongName(){
	return "Profile name can't be longer than 50 characters";
}

QString StringResource::NewProfileConfig_1_TooShortName()
{
	return "Profile name can't be shorter than 3 characters";
}

QString StringResource::NewProfileConfig_1_TooShortExactName()
{
	return "Exact name of character has to be at least 2 characters long";
}

QString StringResource::NewProfileConfig_1_TooLongExactName()
{
	return "Exact name of character has to be shorter than 100 characters";
}

QString StringResource::NewProfileConfig_1_ExactNameHasNewLineChars()
{
	return "Exact name of character can't contain new line character";
}

QString StringResource::NewProfileConfig_1_ForbiddenChars()
{
	return "Profile name can't have special characters, please use only letters, numbers and spaces";
}

QString StringResource::NewProfileConfig_2_anyProfIsChecked()
{
	return "You have to choose profession.";
}

QString StringResource::NewProfileConfig_3_SlidersAreInTheSamePosition()
{
	return "Two or more sliders can't be in the same position.";
}

QString StringResource::NewProfileConfig_3_SlidersAreInWrongOrder()
{
	return "Sliders are in wrong order, please set it from biggest value to lowest.";
}

QString StringResource::NewProfileConfig_3_ShortcutNoValue()
{
	return "at least one of shortcuts field have no key assigned to itself";
}

QString StringResource::NewProfileConfig_3_ShortcutManyValue()
{
	return "at least one of shortcuts field have more than one key assigned to itself";
}

QString StringResource::NewProfileConfig_3_ShortCutHasForbiddenChars()
{	
	QString toRet = "Shortcut can accept only keys";
	toRet.append("\n");
	toRet.append("F1 - F12");
	toRet.append("\n");
	toRet.append("0 - 9");
	toRet.append("\n");
	toRet.append("Insert, Delete, Home, End, Page Up, Page Down, ScrollLock, PauseBreak");
	toRet.append("\n");
	toRet.append("/, *, -, +");
	return toRet;
}

