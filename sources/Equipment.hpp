#pragma once
#include "Utilities.hpp"
#include <qrect.h>
#include <qpoint.h>
#include "ImgEditor.h"
class Equipment
{
public:
	enum STATES { HASTE, BATTLE, PROTECTOR_ZONE, POISONED, PARALYZED, UPGRADED, HUNGER, DRUNKEN };
	enum class EqRect { StateBar, SoulPoints, Capacity, Helmet, Armor, Legs, Boots, Ring, Amulet, Weapon, Shield, Backpack, Torch };
	Equipment(ImgEditor* imgEditor) : imgEditor(imgEditor){};
	~Equipment() {};
	void setStoreRect(const QRect& storeRectToSet) {
		storeRect.setRect(storeRectToSet);
		QPoint storeRectStart = storeRectToSet.topLeft();
	}
	QRect getStoreRect() { return storeRect.getRect(); }

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
			QString code = ImgEditor::binaryLetterImgToCode(img);
			if (!codeStateMap.contains(code))
				continue;
			toRet.append(codeStateMap.value(code));
		}
		return toRet;
	}
	QString getEqRectBottomText(EqRect eqRect) {
		QImage bottomStrImg = getImg(eqRect);
		if (bottomStrImg.isNull())
			return QString();
		ImgEditor::imgToBlackAndWhiteExactColor(bottomStrImg, GREY_COL_OF_EQ_STRINGS);
		QString str = imgEditor->imgWithStrToStr(bottomStrImg);
		return str;
	}
	QRect getRect(EqRect eqRect) {
		const QPoint EQ_FRAME_TOPLEFT = storeRect.getRect().topLeft() - QPoint(74, 0);
		switch (eqRect)
		{
		case EqRect::Armor: {
			QPoint start = EQ_FRAME_TOPLEFT + QPoint(38, 37);
			return QRect(start, EQ_FIELD_SIZE);
		}
		case EqRect::Legs: {
			QPoint start = EQ_FRAME_TOPLEFT + QPoint(38, 73);
			return QRect(start, EQ_FIELD_SIZE);
		}
		case EqRect::Helmet: {
			QPoint start = EQ_FRAME_TOPLEFT + QPoint(38, 1);
			return QRect(start, EQ_FIELD_SIZE);
		}
		case EqRect::Boots: {
			QPoint start = EQ_FRAME_TOPLEFT + QPoint(38, 109);
			return QRect(start, EQ_FIELD_SIZE);
		}
		case EqRect::Ring: {
			QPoint start = EQ_FRAME_TOPLEFT + QPoint(1, 87);
			return QRect(start, EQ_FIELD_SIZE);
		}
		case EqRect::Amulet: {
			QPoint start = EQ_FRAME_TOPLEFT + QPoint(1, 15);
			return QRect(start, EQ_FIELD_SIZE);
		}
		case EqRect::Weapon: {
			QPoint start = EQ_FRAME_TOPLEFT + QPoint(1, 51);
			return QRect(start, EQ_FIELD_SIZE);
		}
		case EqRect::Shield: {
			QPoint start = EQ_FRAME_TOPLEFT + QPoint(75, 51);
			return QRect(start, EQ_FIELD_SIZE);
		}
		case EqRect::Backpack: {
			QPoint start = EQ_FRAME_TOPLEFT + QPoint(75, 15);
			return QRect(start, EQ_FIELD_SIZE); }
		case EqRect::Torch: {
			QPoint start = EQ_FRAME_TOPLEFT + QPoint(75, 87);
			return QRect(start, EQ_FIELD_SIZE); }

		case EqRect::StateBar: {
			QPoint start = EQ_FRAME_TOPLEFT + QPoint(1, 145);
			const QSize STATUS_BAR_SIZE = QSize(106, 11);
			return QRect(start, STATUS_BAR_SIZE);
		}
		case EqRect::SoulPoints: {
			QPoint start = EQ_FRAME_TOPLEFT + QPoint(1, 132);
			const QSize SOUL_AMOUNT_BAR_SIZE = QSize(32, 9);//not whole sqare but only amount!!!
			return QRect(start, SOUL_AMOUNT_BAR_SIZE);
		}
		case EqRect::Capacity: {
			QPoint start = EQ_FRAME_TOPLEFT + QPoint(75, 132);
			const QSize CAP_AMOUNT_BAR_SIZE = QSize(32, 9);//not whole sqare but only amount!!!
			return QRect(start, CAP_AMOUNT_BAR_SIZE);
		}
		default: return QRect();
		}
	}
	void setImg(EqRect eqRect, const QImage& imgToSet) {
		switch (eqRect)
		{
		case EqRect::StateBar: statesBarImg.setImg(imgToSet); return;
		case EqRect::SoulPoints: soulPtsImg.setImg(imgToSet); return;;
		case EqRect::Capacity: capImg.setImg(imgToSet); return;
		case EqRect::Helmet: helmetImg.setImg(imgToSet); return;
		case EqRect::Armor: armorImg.setImg(imgToSet); return;
		case EqRect::Legs: legsImg.setImg(imgToSet); return;
		case EqRect::Boots: bootsImg.setImg(imgToSet); return;
		case EqRect::Ring: ringImg.setImg(imgToSet); return;
		case EqRect::Amulet: amuletImg.setImg(imgToSet); return;
		case EqRect::Weapon: weaponImg.setImg(imgToSet); return;
		case EqRect::Shield: shieldImg.setImg(imgToSet); return;
		case EqRect::Backpack: backpackImg.setImg(imgToSet); return;
		case EqRect::Torch: torchImg.setImg(imgToSet); return;
		default:return;
		}
	}
	QImage getImg(EqRect eqRect) {
		switch (eqRect)
		{
		case EqRect::StateBar: return statesBarImg.getImgCopy();
		case EqRect::SoulPoints: return soulPtsImg.getImgCopy();
		case EqRect::Capacity: return capImg.getImgCopy();
		case EqRect::Helmet: {		
			QImage toRet = helmetImg.getImgCopy();
			helmetImg.clear();
			return toRet;
		}
		case EqRect::Armor: {
			QImage toRet = armorImg.getImgCopy();
			armorImg.clear();
			return toRet; 
		}
		case EqRect::Legs: return legsImg.getImgCopy();
		case EqRect::Boots: return bootsImg.getImgCopy();
		case EqRect::Ring: return ringImg.getImgCopy();
		case EqRect::Amulet: return amuletImg.getImgCopy();
		case EqRect::Weapon: {
			QImage toRet = weaponImg.getImgCopy();
			weaponImg.clear();
			return toRet;
		}
		case EqRect::Shield: return shieldImg.getImgCopy();
		case EqRect::Backpack: backpackImg.getImgCopy();
		case EqRect::Torch:torchImg.getImgCopy();
		default: return QImage();
		}
	}
	bool wearsEquipment(EqRect eqRect, bool& ok) {
		QImage equipmentPiece = getImg(eqRect);
		if (equipmentPiece.isNull()) {
			ok = false;
			return false;
		}
		equipmentPiece = equipmentPiece.copy(0, equipmentPiece.height() / 2, equipmentPiece.width(), 1);
		ImgEditor::imgToBlackAndWhiteOneColor(equipmentPiece, 120);
		for (int x = 0; x < equipmentPiece.width(); x++){
			if (equipmentPiece.pixel(x, 0) == WHITE)
				return true;
		}
		return false;
	}
private:
	MutexImg statesBarImg, soulPtsImg, capImg, helmetImg, armorImg, legsImg, bootsImg, ringImg, amuletImg, weaponImg, shieldImg, backpackImg, torchImg;
	MutexRect storeRect;
	ImgEditor* imgEditor;
	const int toBlackAndWhiteThreshold = 100;
	const QRgb GREY_COL_OF_EQ_STRINGS = qRgb(191, 191, 191);
	const QRgb WHITE = qRgb(255, 255, 255);
	const QSize EQ_FIELD_SIZE = QSize(32, 32);

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
		QString code = ImgEditor::binaryLetterImgToCode(img);
		return code;
	}

};