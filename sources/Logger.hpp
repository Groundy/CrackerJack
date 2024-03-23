#pragma once
#include <qdatetime.h>
#include <qdebug.h>
#include <qdir.h>
#include <qtextstream.h>

#include <QObject>
#include <atomic>
#include <qfile>
namespace CJ {
class Logger : public QObject {
  Q_OBJECT

 signals:
  void sendMsgToUserConsol(QStringList);
  void sendMsgToUserConsolRed(QString);

 public:
  void log(QString msg, bool sendToDebug = true, bool sendToUserConsol = false, bool addTimeSTamp = true) {
    if (addTimeSTamp) {
      msg.push_front(getTimeStamp());
    }
    if (sendToDebug) {
      qDebug() << msg;
    }
    if (sendToUserConsol) {
      emit sendMsgToUserConsol(QStringList() << msg);
    }
  }
  static Logger& instance() {
    static Logger logger;
    return logger;
  }

 private:
  Logger()                      = default;
  ~Logger()                     = default;
  void operator=(const Logger&) = delete;
  Logger(Logger& other)         = delete;

  std::atomic<long long> time = QDateTime::currentDateTime().toMSecsSinceEpoch();

  QString getTimeStamp() {
    QDateTime time = QDateTime::currentDateTime();
    return time.toString("mm:ss:zzz") + " ";
  };

  QString getMSecTillLastCommunicate() {
    qint64 current        = QDateTime::currentDateTime().toMSecsSinceEpoch();
    qint64 timeDifference = current - time;
    time                  = current;
    return QString::number(timeDifference) + "  ";
  }
};
}  // namespace CJ
