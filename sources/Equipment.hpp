#pragma once
#include "Utilities.h"
#include <qrect.h>
#include <qpoint.h>
#include "ImgEditor.h"
class Equipment
{
public:
	enum STATES { HASTE, BATTLE, PROTECTOR_ZONE, POISONED, PARALYZED, UPGRADED, HUNGER, DRUNKEN };
	enum class EqRect { StateBar, SoulPoints, Capacity, Helmet, Armor, Legs, Boots, Ring, Amulet, Weapon, Shield, BackPack, Torch };
	Equipment() {};
	~Equipment() {};
	void setStoreRect(const QRect& storeRectToSet) {
		storeRect.setRect(storeRectToSet);
		QPoint storeRectStart = storeRectToSet.topLeft();
	}
	QRect getStoreRect() { return storeRect.getRect(); }
	void setStateBarImg(const QImage& stateBarImgToSet) { statesBarImg.setImg(stateBarImgToSet); }
	QVector<STATES> getCurrentStates(bool clearImg = true) {
		QImage stateBar = statesBarImg.getImgCopy();
		if (stateBar.isNull())
			return {};
		if (clearImg)
			statesBarImg.clear();
		ImgEditor::imgToBlackAndWhiteOneColor(stateBar, toBlackAndWhiteThreshold);
		QList<QImage> imgs;
		ImgEditor::cutImgWithLettersToSingleLettersImgList(stateBar, imgs);
		QVector<STATES> toRet = {};
		for each (QImage img in imgs) {
			ImgEditor::cutBlackBordersOfImg(img);
			QString code = ImgEditor::binaryLetterImgToLetterStr(img);
			if (!codeStateMap.contains(code))
				continue;
			toRet.append(codeStateMap.value(code));
		}
		return toRet;
	}


	QRect getRect(EqRect eqRect) {
		QRect storeRectCpy = storeRect.getRect();
		switch (eqRect)
		{
		case Equipment::EqRect::StateBar: {
			QPoint start = storeRectCpy.topLeft() + QPoint(1753 - 1826, 264 - 119);
			QSize size(106,11);
			return QRect(start, size);
		}
		}
		return QRect();
	}
	void setImg(EqRect eqRect, const QImage& imgToSet) {
		switch (eqRect)
		{
		case Equipment::EqRect::StateBar: statesBarImg.setImg(imgToSet); return;
		default: break;
		}
	}
private:
	MutexImg statesBarImg;
	MutexRect storeRect;
	const int toBlackAndWhiteThreshold = 100;

	QMap<QString, STATES> codeStateMap = populateMap();
	QMap<QString, STATES> populateMap() {
		QMap<QString, STATES> toRect = {};
		toRect.insert(getStateCode("battleIco"), STATES::BATTLE);
		toRect.insert(getStateCode("hungerIco"), STATES::HUNGER);
		toRect.insert(getStateCode("drunkenIco"), STATES::DRUNKEN);
		toRect.insert(getStateCode("hastedIco"), STATES::HASTE);
		toRect.insert(getStateCode("paralyzedIco"), STATES::PARALYZED);
		toRect.insert(getStateCode("pzIco"), STATES::PROTECTOR_ZONE);
		toRect.insert(getStateCode("upgradedIco"), STATES::UPGRADED);
		return toRect;
	}
	QString getStateCode(QString fileName) {
		QImage img(":/statesIcons/" + fileName);
		ImgEditor::imgToBlackAndWhiteOneColor(img, toBlackAndWhiteThreshold);
		ImgEditor::cutBlackBordersOfImg(img);
		QString code = ImgEditor::binaryLetterImgToLetterStr(img);
		return code;
	}

};