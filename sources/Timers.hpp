#pragma once
#include <qdatetime.h>
#include <qmap.h>

#include <QString>
class Timers {
 public:
  Timers()  = default;
  ~Timers() = default;
  qint64 getTimeLastItemUsageGeneral() const {
    return timeLastItemUsage;
  }
  qint64 getTimeLastSpellUsageAttack() const {
    return timeLastSpellAttack;
  }
  qint64 getTimeLastSpellUsageHealing() const {
    return timeLastSpellHealing;
  }
  qint64 getTimeLastSpellUsageSupport() const {
    return timeLastSpellSupport;
  }
  qint64 getTimeLastSpellUsed(QString spellName) {
    lastTimeUsagesMutex.lock();
    qint64 value = lastTimeSpellUsagesMap.value(spellName, 0);
    lastTimeUsagesMutex.unlock();
    return value;
  }
  qint64 getTimeLastItemUsage(QString itemName) {
    lastTimeUsagesMutex.lock();
    qint64 value = lastTimeItemUsagesMap.value(itemName, 0);
    lastTimeUsagesMutex.unlock();
    return value;
  }

  void setTimeLastItemUsageGeneral() {
    timeLastItemUsage = now();
  }
  void setTimeLastSpellUsageAttack() {
    timeLastSpellAttack = now();
  }
  void setTimeLastSpellUsageHealing() {
    timeLastSpellHealing = now();
  }
  void setTimeLastSpellUsageSupport() {
    timeLastSpellSupport = now();
  }
  void setTimeLastSpellUsed(QString spellName) {
    lastTimeUsagesMutex.lock();
    bool alreadyOnList = lastTimeSpellUsagesMap.contains(spellName);
    if (alreadyOnList)
      lastTimeSpellUsagesMap[spellName] = now();
    else
      lastTimeSpellUsagesMap.insert(spellName, now());
    lastTimeUsagesMutex.unlock();
  }
  void setTimeLastItemUsed(QString itemName, int additionalTime = 0) {
    lastTimeUsagesMutex.lock();
    bool alreadyOnList = lastTimeItemUsagesMap.contains(itemName);
    if (alreadyOnList)
      lastTimeItemUsagesMap[itemName] = now() + additionalTime;
    else
      lastTimeItemUsagesMap.insert(itemName, now() + additionalTime);
    lastTimeUsagesMutex.unlock();
  }

 private:
  std::atomic<qint64>   timeLastItemUsage, timeLastSpellAttack, timeLastSpellHealing, timeLastSpellSupport;
  QMutex                lastTimeUsagesMutex;
  QMap<QString, qint64> lastTimeSpellUsagesMap;
  QMap<QString, qint64> lastTimeItemUsagesMap;
  qint64                now() {
    return QDateTime::currentMSecsSinceEpoch();
  };
};
