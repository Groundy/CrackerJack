#include <qqueue.h>
#include <qrect.h>
#include <qsound.h>

#include <QStringList>
#include <QThread>

#include "AttackMethode.hpp"
#include "GameConnecter.h"
#include "MinimapAnalyzer.h"
#include "Profile.h"
#include "Route.h"
#include "VariablesClass.hpp"
class AutoHunting : public QThread {
  Q_OBJECT
 public:
  AutoHunting(QObject* parent, QSharedPointer<VariablesClass> var, QSharedPointer<GameConnecter> gameConnecto, Route route);
  ~AutoHunting();
  void run();

 signals:
  void updateHeadingPointInGUI(QString);
  void updateEnemiesAmountInGUI(int);

 private:
  qint64                         lastTimeMovedToNextNode       = now();
  qint64                         lastTimePressedAttack         = now();
  qint64                         lastTimeSpecialAttackUsed     = now();
  qint64                         lastTimeAlarmRang             = now();
  const int                      minPeriodBetweenAttackingMob  = 1700;
  const int                      minPeriodBetweenMovingToNodes = 2000;
  const int                      breakBetweenAlarms            = 3000;
  QMap<QString, qint64>          attacksMethodesTimers;
  QSharedPointer<VariablesClass> var;
  QSharedPointer<GameConnecter>  gameConnector;
  Point3D                        currentPos;
  int                            lastAchivedPoint = -1;
  Route                          route;
  const int                      SLEEP_TIME = 20;
  QQueue<QPoint>                 lastPositions;
  bool                           atLastLoopPlayerWasFighting = false;
  QVector<AttackMethode>         attackMethodes              = {};
  int                            lastAnalyzeEnemiesNumber    = 0;
  int                            minEnemiesToStop            = 2;
  int                            minEnemiesToContinue        = 0;

  QStringList alloweNamesOnBattleList;
  Logger&     logger = Logger::instance();

  QPoint getDistFromOnePtToAnother(QPoint start, QPoint end);
  QPoint addTwoPoints(QPoint start, QPoint end);
  void   addNewPosition(QPoint pt);
  bool   playerIsMoving();
  bool   checkIfPlayerIsInDesiredNode();
  void   moveToNextNode();
  bool   updatePlayerCurrentPos();
  void   useAttackMethode(const AttackMethode& methode);
  bool   canUseMethode(const AttackMethode& methode);
  void   useSpecialAttackIfNeeded();
  bool   playerIsFighting();
  qint64 now();
  void   keepAtackingTargetOnBattleList();
  bool   playerFoundOnBattleList();
  void   clickOnMiddleOfCompass();
  void   playSound();
};
