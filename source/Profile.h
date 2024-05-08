#pragma once
#ifdef _WIN64
#include <Windows.h>

#endif

#include <qjsonarray.h>
#include <qkeysequence.h>
#include <qlist.h>
#include <qstring.h>
#include <qvector.h>

#include <QObject>
#include <qmap.h>
#include <string>

#include "AttackMethode.hpp"
#include "AutoHuntData.hpp"
#include "JsonClass.hpp"
#include "Key.h"
#include "Profession.hpp"
#include "RestorationMethode.hpp"
#include "Utilities.hpp"
namespace CJ {
class Profile : JsonClass {
 public:
  enum AutoLoot { RIGHT_MOUSE_BUTTON, SHIFT_RIGHT, LEFT_MOUSE_BUTTON };  //enable only if controls
  enum Controls { CLSSIC_CONTROLS, REGULARCONTROLS, LEFT_SMART_CLICK };

  Profile() = default;
  Profile(QJsonObject obj);
  Profile(const Profile& profile);

  //funcs()
  void        clearProfile();
  QJsonObject toJson() const;
  QStringList getManaRestorationNames();
  QStringList getHealthRestorationNames();
  bool        isValid() const;

  //setters
  void setName(QString name);
  void setProfession(Profession profession);
  void setRestMethodesHealth(QVector<RestorationMethode> healthRestMethodes);
  void setRestMethodesMana(QVector<RestorationMethode> manaRestMethodes);
  void setScreenShotKey(Key key);
  void setBars(int left, int right);
  void setControls(int enumCode);
  void setAutoLoot(int enumCode);
  void setRopeKey(Key key);
  void setAutoAttackKey(Key key);
  void setShovelKey(Key key);
  void setAutoHuntData(AutoHuntData autoHuntData);

  //getters
  QString                     getName() const;
  Profession                  getProfession() const;
  QVector<RestorationMethode> getRestMethodesHealth() const;
  QVector<RestorationMethode> getRestMethodesMana() const;
  Key                         getScreenShotKey() const;
  int                         getControls() const;
  AutoLoot                    getAutoLoot() const;
  int                         getBarsLeft() const;
  int                         getBarsRight() const;
  Key                         getRopeKey() const;
  Key                         getAutoAttackKey() const;
  Key                         getShovelKey() const;
  AutoHuntData                getAutoHuntData() const;

 private:
  QVector<RestorationMethode> manaRestorations;
  QVector<RestorationMethode> healthRestorations;
  Profession                  profession;
  QString                     profileName;
  AutoLoot                    autoLoot;
  Controls                    controls;
  Key                         screenShotKey, shovelKey, ropeKey, autoAttackKey;
  int                         barsLeft, barsRight;
  AutoHuntData                autoHuntData = AutoHuntData();
};
}  // namespace CJ
