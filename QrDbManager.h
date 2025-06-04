#ifndef QRDBMANAGER_H
#define QRDBMANAGER_H


#include <Result.h>
#include <QtSql>

namespace Afc
{
class QrTicket;

class QrDbManager
{
public:
  static QrDbManager & getInstance()
  {
    static QrDbManager _instance;

    return _instance;
  }

  Result          insertTicket(const QSharedPointer<QrTicket> &ticket, bool offline);

  Result          checkTicket(const QSharedPointer<QrTicket> &ticket, quint16 stationId);

  void            cleanOldEntries();

  void            removeDb(const QString &name);

  void            rebuildOfflineDb(bool checkSize = true);

  QList<QString>  getOfflineDbs() const;
private:
  QrDbManager();

  void      createOfflineDb();

  void      generateDbFileName();

  QSqlQuery queryOnDb(const QSqlDatabase &db, const QString &q);

  Result    internalInsertTicket(const QSqlDatabase &db, const QSharedPointer<QrTicket> &ticket, const QString &stamp = QString());

  void      saveSession();

  void      loadSession();

private:
  QFile         _fileSession;
  QDataStream   _streamSession;
  QSqlDatabase  _issuerDb, _transactionsDb, _offlineTransactionsDb;
  QString       _offlineDbName;
  qint32        _fileCounter;
};
}

#endif // QRDBMANAGER_H
