#include "StringResource.h"

bool StringResource::languageIsPl(){
	QSettings setttings("settings.ini", QSettings::IniFormat);
	QString readVal = setttings.value("language").toString();
	bool toRet = readVal == QString("pl");
	return toRet;
}

QString StringResource::WindowTitle_CrackerJackProblem()
{
	return "CrackerJack problem";
}

QString StringResource::LoginWindow_WrongLoginPasword(){
	QString pl = QString::fromLocal8Bit("Taka kombinacja loginu i has�a nie istnieje!");
	QString eng = "That combination of login and password doesn't exist!";
	return languageIsPl() ? pl : eng;
}

QString StringResource::NewProfileConfigHelp(int page){
	QString toRet = "";
	bool pl = languageIsPl();
	if (pl) {
		switch (page)
		{
		case 1:
			toRet = QString::fromLocal8Bit("Wybierz nazwe profilu.");
			break;
		case 2:
			toRet = QString::fromLocal8Bit("Wybierz profesie swojej postaci.");
			break;
		case 3:
			toRet = QString::fromLocal8Bit("Wybierz ile metod leczenia checz u�ywa�, mo�esz u�ywa� a� do 5 metod.");
			toRet.append(QString::fromLocal8Bit("Przesuwaj suwaki aby okre�li� zakresy w jakich powinna by� u�ywana dana metoda leczenia."));
			toRet.append(QString::fromLocal8Bit("Pierwszy suwak powinnien by� u�yty dla najslabszej metody leczenia, ostatni dla najbardziej efektywnej."));
			toRet.append(QString::fromLocal8Bit("Program zdecduje kt�ra metoda leczenia jest odpowiednia i jej u�yje."));
			toRet.append(QString::fromLocal8Bit("Do ka�dej metody leczenia nale�y przypisa� klawisz kt�ry jest u�yty w grze."));
			toRet.append(QString::fromLocal8Bit("Je�eli najskuteczniejsza metoda leczenia nie jest dost�pna(brak many, brak mikstury) program u�yje innej, s�abszej metody jaka jest dost�pna"));
			break;
		case 4:
			toRet = QString::fromLocal8Bit("Wybierz ile metod odnawiania many checz u�ywa�, mo�esz u�ywa� a� do 5 metod.");
			toRet.append(QString::fromLocal8Bit("Przesuwaj suwaki aby okre�li� zakresy w jakich powinna by� u�ywana dana metoda odnawiania many."));
			toRet.append(QString::fromLocal8Bit("Pierwszy suwak powinnien by� u�yty dla najslabszej metody odnowienia many, ostatni dla najbardziej efektywnego."));
			toRet.append(QString::fromLocal8Bit("Do ka�dej metody odnawiania many nale�y przypisa� klawisz kt�ry jest u�yty w grze."));
			toRet.append(QString::fromLocal8Bit("Mikstury lecznicze maj� priorytet nad miksturami many."));
			break;
		default:
			break;
		}
	}
	else {
		switch (page)
		{
		case 1:
			toRet = "Choose name for your profile.";
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
			toRet.append("If most effective healing methode is not avaible(no enough mana, no potion), program will try to another, less efficient methode.");
			break;
		case 4:
			toRet = "Choose how many methodes of restoring mana do want, up to 5 diffrent methodes. ";
			toRet.append("Move sliders to set value of missing mana that will be used to auto mana restore. ");
			toRet.append("First Slider should be used for smallest mana regeneration, last for biggest. ");
			toRet.append("If mana will be low, program will use appropriate methode to regenerate it. ");
			toRet.append("To all used sliders you have to add the same hotkey that use in game. ");
			toRet.append("Healing potions have priority over mana potions. ");
			break;
		default:
			break;
		}
	}
	return toRet;
}

QString StringResource::NewProfileConfig_1_TooLongName(){
	QString pl = QString::fromLocal8Bit("Nazwa profilu nie mo�e by� d�u�sza ni� 50 znak�w!");
	QString eng = "Profile name can't be longer than 50 characters!";
	return languageIsPl() ? pl : eng;
}


QString StringResource::NewProfileConfig_1_TooShortName(){
	QString pl = QString::fromLocal8Bit("Nazwa profilu nie mo�e by� kr�tsza ni� 3 znaki!");
	QString eng = "Profile name can't be shorter than 3 characters!";
	return languageIsPl() ? pl : eng;
}

QString StringResource::NewProfileConfig_1_ForbiddenChars(){
	QString pl = QString::fromLocal8Bit("Nazwa profilu nie mo�e zawiera� znak�w specjalnych, nale�y u�ywa� tylko liter, cyfr lub odst�p�w.");
	QString eng = "Profile name can't have any special characters, please use only letters, numbers or spaces";
	return languageIsPl() ? pl : eng;
}

QString StringResource::NewProfileConfig_2_anyProfIsChecked(){
	QString pl = QString::fromLocal8Bit("Nale�y wybra� profesie. ");
	QString eng = "You have to choose profession.";
	return languageIsPl() ? pl : eng;
}

QString StringResource::NewProfileConfig_3_SlidersAreInTheSamePosition(){
	QString pl = QString::fromLocal8Bit("Dwa suwaki nie mog� by� w tym samym miejscu.");
	QString eng = "Two sliders can't be in the same position.";
	return languageIsPl() ? pl : eng;
}

QString StringResource::NewProfileConfig_3_SlidersAreInWrongOrder(){
	QString pl = QString::fromLocal8Bit("Suwaki s� ustawione w z�ej kolejno�ci, nale�y je ustawi� od najwi�kszego do najmniejszego.");
	QString eng = "Sliders are in wrong order, please set it from biggest value to lowest.";
	return languageIsPl() ? pl : eng;
}

QString StringResource::NewProfileConfig_3_ShortcutNoValue(){
	QString pl = QString::fromLocal8Bit("Jedno z p�l klawiszowych nie jest wype�nione.");
	QString eng = "One of key field doesn't have hotkey assigned to itself.";
	return languageIsPl() ? pl : eng;
}

QString StringResource::NewProfileConfig_3_ShortcutManyValue(){
	QString pl = QString::fromLocal8Bit("Jedno z p�l klawiszowych ma przypisane do siebie wi�cej ni� jedn� warto��.");
	QString eng = "One of key field has more than one hotkey assigned to itself.";
	return languageIsPl() ? pl : eng;
}

QString StringResource::NewProfileConfig_3_LastSliderIsZero(){
	QString pl = QString::fromLocal8Bit("Ostatni suwak musi mie� warto�� wi�ksz� ni� zero.");
	QString eng = "Last slider has to have value above zero.";
	return languageIsPl() ? pl : eng;
}

QString StringResource::NewProfileConfig_3_ShortCutHasForbiddenChars(){	
	bool pl = languageIsPl();
	QString toRet;
	if(pl)
		toRet = QString::fromLocal8Bit("Pola klawiszowe moga przyjmowac jedynie poni�sze warto�ci: \n");
	else
		toRet = "Key fields can accpet only those values: \n";
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

QString StringResource::NewProfileConfig_3_comboBoxNoValue(){
	QString pl = QString::fromLocal8Bit("Pole wyboru metody nie mo�e by� puste.");
	QString eng = "Methode field can't be empty.";
	return languageIsPl() ? pl : eng;
}

QString StringResource::NewProfileConfig_3_comboBoxShareTheSameValue(){
	QString pl = QString::fromLocal8Bit("Dwa pola klawiszowe nie moga miec przypisanego tego samego klawisza.");
	QString eng = "Two key fields can't share the same key.";
	return languageIsPl() ? pl : eng;
}

QString StringResource::ProfileDataBaseManager_DbDoesNotExist(){
	QString pl = QString::fromLocal8Bit("Nie znaleziono bazy danych z profilami w ");
	QString eng = "Database with profiles not found in ";
	return languageIsPl() ? pl : eng;
}

QString StringResource::ProfileDataBaseManager_NewDBWillBeCreated(){
	QString pl = QString::fromLocal8Bit("\nNowa baza danych zostanie utworzona.");
	QString eng = "\nNew and empty database will be create.";
	return languageIsPl() ? pl : eng;
}

QString StringResource::SelectProfileWindow_sureToDeleteProfile(){
	QString pl = QString::fromLocal8Bit("Czy na pewno chcesz usun�� profil ");
	QString eng = "Do you really want to delete profile ";
	return languageIsPl() ? pl : eng;
}
