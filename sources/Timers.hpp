#pragma once
#include <QString>
#include <qmap.h>
#include <qdatetime.h>
#include <mutex>
class Timers
{
public:
	Timers() {};
	~Timers() {};
	qint64 getTimeLastItemUsage() const { return timeLastItemUsage; }
	qint64 getTimeLastSpellUsageAttack() const { return timeLastSpellAttack; }
	qint64 getTimeLastSpellUsageHealing() const { return timeLastSpellHealing; }
	qint64 getTimeLastSpellUsageSupport() const { return timeLastSpellSupport; }
	qint64 getTimeLastSpellUsed(QString spellName) {
		lastTimeUsagesMutex.lock();
		qint64 value = lastTimeSpellUsagesMap.value(spellName, 0);
		lastTimeUsagesMutex.unlock();
		return value;
	}
	void setTimeLastItemUsage() { timeLastItemUsage = now(); }
	void setTimeLastSpellUsageAttack() { timeLastSpellAttack = now(); }
	void setTimeLastSpellUsageHealing() { timeLastSpellHealing = now(); }
	void setTimeLastSpellUsageSupport() { timeLastSpellSupport = now(); }
	void setTimeLastSpellUsed(QString spellName) {
		lastTimeUsagesMutex.lock();
		bool alreadyOnList = lastTimeSpellUsagesMap.contains(spellName);
		if (alreadyOnList)
			lastTimeSpellUsagesMap[spellName] = now();
		else
			lastTimeSpellUsagesMap.insert(spellName, now());
		lastTimeUsagesMutex.unlock();
	}
private:
	std::atomic<qint64> timeLastItemUsage, timeLastSpellAttack, timeLastSpellHealing, timeLastSpellSupport;
	std::mutex lastTimeUsagesMutex;
	QMap<QString, qint64> lastTimeSpellUsagesMap;
	qint64 now() { return QDateTime::currentMSecsSinceEpoch(); };
};