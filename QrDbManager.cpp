#include <QrDbManager.h>
#include <QrTicket.h>
#include <Settings.h>
#include <Utilities.h>
#include <ActivityManager.h>


namespace Afc
{
static constexpr auto SESSION_FILE_NAME       = "~qrDbSession";
static const QString  ISSUER_TABLE_NAME       = "Issuer";
static const QString  TRANSACTION_TABLE_NAME  = "QR_Info";
static const QString  OFFLINE_DB_FOLDER       = "/offline";

static QString quoteString(const QString &str)
{
  return "'" + str + "'";
}

QrDbManager::QrDbManager(): _offlineDbName("QrOfflineTransactions.db"), _fileCounter(1)
{
  if (!QDir(Settings::QR_DATA_DIR).exists())
  {
    QDir().mkdir(Settings::QR_DATA_DIR);
    Utilities::setOwner(Settings::QR_DATA_DIR , Settings::USER_NAME);
  }

  if (!QDir(Settings::QR_DATA_DIR + OFFLINE_DB_FOLDER).exists())
  {
    QDir().mkdir(Settings::QR_DATA_DIR + OFFLINE_DB_FOLDER);
    Utilities::setOwner(Settings::QR_DATA_DIR + OFFLINE_DB_FOLDER, Settings::USER_NAME);
  }

  _issuerDb               = QSqlDatabase::addDatabase("QSQLITE", "CxnIssuer");
  _transactionsDb         = QSqlDatabase::addDatabase("QSQLITE", "CxnQrTransactions");
  _offlineTransactionsDb  = QSqlDatabase::addDatabase("QSQLITE", "CxnOfflineQrTransactions");

  loadSession();

  _issuerDb.setDatabaseName(Settings::QR_DATA_DIR + QString("/") + "Issuer.db");

  if (!_issuerDb.open())
  {
    qDebug() << "Failed to open DB" << _issuerDb.databaseName() << ":" << _issuerDb.lastError().text();
  }

  _transactionsDb.setDatabaseName(Settings::QR_DATA_DIR + QString("/") + "QrTransactions.db");

  if (!_transactionsDb.open())
  {
    qDebug() << "Failed to open DB" << _transactionsDb.databaseName() << ":" << _transactionsDb.lastError().text();
  }

  auto const q = "create table if not exists " + TRANSACTION_TABLE_NAME + " "
                 + "("
                 + "Crypto_Info varchar(80), "
                 + "Qr_Serial varchar(80), "
                 + "Spare varchar(80), "
                 + "Issue_Date varchar(80), "
                 + "Valid_Day_Counter varchar(80), "
                 + "Price varchar(80), "
                 + "Ticket_Type varchar(80), "
                 + "Type varchar(80), "
                 + "Issue_Ticket_Title varchar(80), "
                 + "Issue_Device_Serial varchar(80), "
                 + "Counter_Per_Device varchar(80), "
                 + "MAC varchar(80), "
                 + "Key_NO varchar(80), "
                 + "Reader_IP varchar(80), "
                 + "Date_Used varchar(80), "
                 + "Time_Used varchar(80), "
                 + "Trip_Orgin varchar(80), "
                 + "Trip_Remining varchar(80), "
                 + "Station_Issue varchar(80), "
                 + "Station_Used varchar(80), "
                 + "Status varchar(80), "
                 + "timeStamp integer"
                 + ")";


  queryOnDb(_transactionsDb, q);

  createOfflineDb();
}

void QrDbManager::createOfflineDb()
{
  if (_offlineTransactionsDb.isOpen())
  {
    _offlineTransactionsDb.close();
  }

  _offlineTransactionsDb.setDatabaseName(Settings::QR_DATA_DIR + OFFLINE_DB_FOLDER + "/" + _offlineDbName);

  if (!_offlineTransactionsDb.open())
  {
    qDebug() << "Failed to open DB" << _offlineTransactionsDb.databaseName() << ":" << _offlineTransactionsDb.lastError().text();
  }

  auto const q = "create table if not exists " + TRANSACTION_TABLE_NAME + " "
                 + "("
                 + "Crypto_Info varchar(80), "
                 + "Qr_Serial varchar(80), "
                 + "Spare varchar(80), "
                 + "Issue_Date varchar(80), "
                 + "Valid_Day_Counter varchar(80), "
                 + "Price varchar(80), "
                 + "Ticket_Type varchar(80), "
                 + "Type varchar(80), "
                 + "Issue_Ticket_Title varchar(80), "
                 + "Issue_Device_Serial varchar(80), "
                 + "Counter_Per_Device varchar(80), "
                 + "MAC varchar(80), "
                 + "Key_NO varchar(80), "
                 + "Reader_IP varchar(80), "
                 + "Date_Used varchar(80), "
                 + "Time_Used varchar(80), "
                 + "Trip_Orgin varchar(80), "
                 + "Trip_Remining varchar(80), "
                 + "Station_Issue varchar(80), "
                 + "Station_Used varchar(80), "
                 + "Status varchar(80)"
                 + ")";

  queryOnDb(_offlineTransactionsDb, q);
}

void QrDbManager::generateDbFileName()
{
  auto const  now = QDateTime::currentDateTimeUtc();
  auto const  dateStr = now.toString("yyyyMMdd");

  for (qint32 i = _fileCounter; i < 10000; ++i)
  {
    auto const fileName = dateStr + QString::number(i).rightJustified(7, '0') + ".db";
    auto const dbName = Settings::QR_DATA_DIR + OFFLINE_DB_FOLDER + "/" + fileName;

    if (!QFile::exists(dbName))
    {
      _fileCounter    = i;
      _offlineDbName  = fileName;

      break;
    }
  }

  qDebug() << "generateDbFileName" << _offlineDbName;
}

Result QrDbManager::insertTicket(const QSharedPointer<QrTicket> &ticket, bool offline)
{
  if (offline)
  {
    internalInsertTicket(_offlineTransactionsDb, ticket);
  }

  return internalInsertTicket(_transactionsDb, ticket, ", " + QString::number(QDateTime::currentMSecsSinceEpoch()));
}

Result QrDbManager::checkTicket(const QSharedPointer<QrTicket> &ticket, quint16 stationId)
{
  Result res;

  auto const condition = "IP_Issuer=" + quoteString(ticket->getIssuerIpv4())
                         + " AND Issuer_Title=" + quoteString(ticket->getIssuerType())
                         + " AND Station_ID=" + quoteString(QString::number(stationId));

  qDebug() << condition;

  auto const q1 = "select * from " + ISSUER_TABLE_NAME + " where " + condition;
  auto r1 = queryOnDb(_issuerDb, q1);

  auto const q2 = "select * from " + TRANSACTION_TABLE_NAME + " where "
                  + "Qr_Serial=" + quoteString(ticket->getSerialNumber());
  auto r2 = queryOnDb(_transactionsDb, q2);

  qDebug() << "MGR1";
  if (r1.next())
  {
    qDebug() << "MGR2";
    if (!r2.next())
    {
      qDebug() << "MGR3";
      res.succeed = true;
    }
  }

  return res;
}

void QrDbManager::cleanOldEntries()
{
  auto const removeOldEntries = "delete from " + TRANSACTION_TABLE_NAME + " where "
                                + "timeStamp < "
                                + QString::number(QDateTime::currentMSecsSinceEpoch() - 10 * 1000 * 3600 * 24);

  queryOnDb(_transactionsDb, removeOldEntries);
}

void QrDbManager::removeDb(const QString &name)
{
  QFile::remove(Settings::QR_DATA_DIR + OFFLINE_DB_FOLDER + QString("/") + name + ".db");
}

void QrDbManager::rebuildOfflineDb(bool checkSize)
{
  if (checkSize)
  {
    if (4096 < QFileInfo(_offlineDbName).size())
    {
      generateDbFileName();
      createOfflineDb();
      saveSession();
    }
  }
}

QList<QString> QrDbManager::getOfflineDbs() const
{
  QList<QString>  res;
  QDirIterator    itDir(Settings::QR_DATA_DIR + OFFLINE_DB_FOLDER + QString("/"), QStringList() << "*.db", QDir::Files);

  while(itDir.hasNext())
  {
    auto const fileName = itDir.next();
    if (!(_offlineTransactionsDb.isOpen() && _offlineTransactionsDb.databaseName() == fileName))
    {
      res.append(fileName);
    }
  }

  return res;
}


QSqlQuery QrDbManager::queryOnDb(const QSqlDatabase &db, const QString &q)
{
  qDebug() << q;
  QSqlQuery query(db);

  query.exec(q);

  auto const err = query.lastError().text().trimmed();

  if (!err.isEmpty())
  {
    qDebug() << "QrDbManager" << err;
  }

  return query;
}

Result QrDbManager::internalInsertTicket(const QSqlDatabase &db, const QSharedPointer<QrTicket> &ticket, const QString &stamp)
{
  Result res;
  auto const now = QDateTime::currentDateTimeUtc();
  auto const stationId = QString::number(ActivityManager::getInstance()->getStationId());
  auto const q = "insert into " + TRANSACTION_TABLE_NAME + " values"
                 + "("
                 + quoteString(ticket->getEncryptedSection()) + ", "
                 + quoteString(ticket->getSerialNumber()) + ", "
                 + quoteString(ticket->getSpare()) + ", "
                 + quoteString(ticket->getIssuanceDateString()) + ", "
                 + quoteString(ticket->getValidityDays()) + ", "
                 + quoteString(ticket->getPrice()) + ", "
                 + quoteString(QString::number(ticket->getTicketType())) + ", "
                 + quoteString(ticket->getQrType()) + ", "
                 + quoteString(ticket->getIssuerType()) + ", "
                 + quoteString(ticket->getIssuerId()) + ", "
                 + quoteString(ticket->getIssuerCounter()) + ", "
                 + quoteString(ticket->getMac()) + ", "
                 + quoteString(ticket->getKeyId()) + ", "
                 + quoteString(Utilities::getIpAddress().toString()) + ", "
                 + quoteString(Utilities::miladiToShamsi(now.date()).toString("yyyy/MM/dd")) + ", "
                 + quoteString(now.time().toString("hh:mm:ss")) + ", "
                 + quoteString("1") + ", "
                 + quoteString("0") + ", "
                 + quoteString(stationId) + ", "
                 + quoteString(stationId) + ", "
                 + quoteString("7")
                 + stamp
                 + ")";

  res.message = queryOnDb(db, q).lastError().text().trimmed();
  res.succeed = res.message.isEmpty();

  return res;
}

void QrDbManager::saveSession()
{
  _fileSession.seek(0);

  _streamSession.setDevice(&_fileSession);
  _streamSession << QDateTime::currentDateTimeUtc() << _fileCounter;

  _fileSession.flush();
}

void QrDbManager::loadSession()
{
  _fileSession.setFileName(SESSION_FILE_NAME);
  bool  hasSession = _fileSession.exists();

  if (_fileSession.open(QIODevice::ReadWrite))
  {
    _streamSession.setDevice(&_fileSession);

    if (hasSession)
    {
      auto const now = QDateTime::currentDateTimeUtc().date();
      QDateTime sessionDate;

      _streamSession >> sessionDate >> _fileCounter;

      if (now != sessionDate.date())
      {
        _fileCounter  = 1;
      }
    }
  }

  generateDbFileName();
}
}
