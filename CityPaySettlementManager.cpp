#include <CityPaySettlementManager.h>
#include <TransactionLog.h>
#include <QFile>
#include <QDir>
#include <QDirIterator>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>

namespace Afc
{
static constexpr auto  MAX_FILE_COUNTER = 100U;
static constexpr auto  DIR_NAME         = "citypay";
static constexpr auto  TEMP_DIR_NAME    = "~citypay";
static constexpr auto  INSTITUDE_CODE   = "12345";

CityPaySettlementManager::CityPaySettlementManager()
{
  if (!QDir(DIR_NAME).exists())
  {
    QDir().mkdir(DIR_NAME);
  }

  if (!QDir(TEMP_DIR_NAME).exists())
  {
    QDir().mkdir(TEMP_DIR_NAME);
  }

  commit();
}

QString  CityPaySettlementManager::generateNewFileName()
{
  const auto &date = QDate::currentDate();
  QString     fileName;

  for (quint32 i = 1U; i < MAX_FILE_COUNTER; ++i)
  {
    fileName = generateFileName(date, i);

    if (!QFile::exists(DIR_NAME + QString("/") + fileName))
    {
      break;
    }
  }

  return fileName;
}

QString  CityPaySettlementManager::generateFileName(const QDate &date, quint32 counter)
{
  return date.toString("yyMMdd") + QString::number(counter).rightJustified(2, '0') + ".csf";
}

CityPaySettlementManager::~CityPaySettlementManager()
{
}

void  CityPaySettlementManager::appendLog(const QSharedPointer<TransactionLog> &log)
{
  const auto &tempFilePath = TEMP_DIR_NAME + QString("/") + generateNewFileName();
  QFile       file(tempFilePath);

  if (file.open(QIODevice::Append | QIODevice::Text))
  {
    file.write(log->serialize());
    file.write("\n");
    file.flush();
  }
}

void  CityPaySettlementManager::commit()
{
  const auto   &now = QDateTime::currentDateTimeUtc();
  QDirIterator  itDir(TEMP_DIR_NAME + QString("/"), QStringList() << "*", QDir::Files);

  while (itDir.hasNext())
  {
    itDir.next();
    const auto &tempFilePath = itDir.filePath();
    QFile       file(tempFilePath);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      QByteArray   buffer(file.readAll());
      QTextStream  inStream(buffer, QIODevice::ReadOnly | QIODevice::Text);
      quint32      transactionCount = 0U;
      quint32      paymentInfoSum   = 0U;

      while (!inStream.atEnd())
      {
        paymentInfoSum += inStream.readLine().split(";")[2].toUInt();
        ++transactionCount;
      }

      file.close();
      const auto &newFilePath = DIR_NAME + QString("/") + itDir.fileName();

      file.setFileName(newFilePath);

      if (file.open(QIODevice::WriteOnly | QIODevice::Text))
      {
        const auto &firstLine = QString::number(transactionCount) + ";" + QString::number(paymentInfoSum) + ";"
                                + now.toString("yyyyMMddhhmmss") + ";" + QString(INSTITUDE_CODE) + "\n";
        file.write((firstLine + buffer).toLatin1());
        file.flush();
        QFile::remove(tempFilePath);
      }
    }
  }
}
}
