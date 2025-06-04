#include <Utilities.h>
#include <Settings.h>
#include <QDateTime>
#include <QProcess>
#include <QNetworkInterface>
#include <QDir>
#include <QFile>
#include <stack>
#include <random>


namespace Afc
{
static bool  isNumber(char ch)
{
  const auto &temp = static_cast<quint8>(ch);

  return ((temp >= 48) && (temp <= 57)) || ((temp >= 128) && (temp <= 137)) || ((temp >= 213) && (temp <= 215));
}

quint16  Utilities::crc16ModBus(const QByteArray &data)
{
  static const quint16  crcTable[] = {
    0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
    0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
    0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
    0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
    0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
    0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
    0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
    0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
    0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
    0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
    0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
    0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
    0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
    0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
    0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
    0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
    0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
    0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
    0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
    0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
    0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
    0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
    0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
    0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
    0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
    0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
    0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
    0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
    0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
    0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
    0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
    0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040 };
  quint8                temp;
  quint16               crcWord = 0xFFFF;

  for (qint32 i = 0; i < data.size(); ++i)
  {
    temp      = data.at(i) ^ crcWord;
    crcWord >>= 8;
    crcWord  ^= crcTable[temp];
  }

  return crcWord;
}

QByteArray  Utilities::generateRandom(quint32 size)
{
  std::random_device                     rd;
  std::uniform_int_distribution<quint8>  dist(0, 0xFF);
  QByteArray                             res(size, Qt::Uninitialized);

  for (quint32 i = 0; i < size; ++i)
  {
    res[i] = static_cast<char>(dist(rd));
  }

  return res;
}

QByteArray  Utilities::convertDateToBcd(const QDateTime &dateTime)
{
  QByteArray     result;
  unsigned char  bcd;
  int            y = dateTime.date().year();

  bcd  = (y / 100) % 10;
  bcd |= (y / 1000) << 4;
  result.append((char)bcd);

  bcd  = y % 10;
  bcd |= ((y / 10) % 10) << 4;
  result.append((char)bcd);

  bcd  = dateTime.date().month() % 10;
  bcd |= ((dateTime.date().month() / 10) % 10) << 4;
  result.append(bcd);

  bcd  = dateTime.date().day() % 10;
  bcd |= ((dateTime.date().day() / 10) % 10) << 4;
  result.append(bcd);

  bcd  = dateTime.time().hour() % 10;
  bcd |= ((dateTime.time().hour() / 10) % 10) << 4;
  result.append(bcd);

  bcd  = dateTime.time().minute() % 10;
  bcd |= ((dateTime.time().minute() / 10) % 10) << 4;
  result.append(bcd);

  bcd  = dateTime.time().second() % 10;
  bcd |= ((dateTime.time().second() / 10) % 10) << 4;
  result.append(bcd);

  return result;
}

QString  Utilities::prettyPrice(double price)
{
  return QString("%L1 ریال").arg(price, 0, 'f', 0);
}

void  Utilities::setSystemDateTime(const QDateTime &date)
{
  if (date.isValid())
  {
    QProcess  proc;

    proc.start("date", QStringList() << "-s" << date.toString("yyyy-MM-dd hh:mm:ss"));
    proc.waitForFinished();
  }
}

QString  Utilities::convertToQString(char *str)
{
  std::wstring      farsiCode[] = {
    /*0*/ L"",

    /*1*/ L"",

    /*2*/ L"",

    /*3*/ L"",

    /*4*/ L"",

    /*5*/ L"",

    /*6*/ L"",

    /*7*/ L"",

    /*8*/ L"",

    /*9*/ L"	",

    /*10*/ L"",

    /*11*/ L"",

    /*12*/ L"",

    /*13*/ L"\r\n",

    /*14*/ L"",

    /*15*/ L"",

    /*16*/ L"",

    /*17*/ L"",

    /*18*/ L"",

    /*19*/ L"",

    /*20*/ L"م",

    /*21*/ L"م", // mim avval

    /*22*/ L"",

    /*23*/ L"",

    /*24*/ L"",

    /*25*/ L"",

    /*26*/ L"",

    /*27*/ L"",

    /*28*/ L"",

    /*29*/ L"",

    /*30*/ L"",

    /*31*/ L"",

    /*32*/ L" ",

    /*33*/ L"!",

    /*34*/ L"",

    /*35*/ L"#",

    /*36*/ L"$",

    /*37*/ L"%",

    /*38*/ L"&",

    /*39*/ L"'",

    /*40*/ L")",

    /*41*/ L"(",

    /*42*/ L"*",

    /*43*/ L"+",

    /*44*/ L",",

    /*45*/ L"-",

    /*46*/ L"",

    /*47*/ L"/",

    /*48*/ L"0",

    /*49*/ L"1",

    /*50*/ L"2",

    /*51*/ L"3",

    /*52*/ L"4",

    /*53*/ L"5",

    /*54*/ L"6",

    /*55*/ L"7",

    /*56*/ L"8",

    /*57*/ L"9",

    /*58*/ L":",

    /*59*/ L";",

    /*60*/ L"<",

    /*61*/ L"=",

    /*62*/ L">",

    /*63*/ L"?",

    /*64*/ L"@",

    /*65*/ L"A",

    /*66*/ L"B",

    /*67*/ L"C",

    /*68*/ L"D",

    /*69*/ L"E",

    /*70*/ L"F",

    /*71*/ L"G",

    /*72*/ L"H",

    /*73*/ L"I",

    /*74*/ L"J",

    /*75*/ L"K",

    /*76*/ L"L",

    /*77*/ L"M",

    /*78*/ L"N",

    /*79*/ L"O",

    /*80*/ L"P",

    /*81*/ L"Q",

    /*82*/ L"R",

    /*83*/ L"S",

    /*84*/ L"T",

    /*85*/ L"U",

    /*86*/ L"V",

    /*87*/ L"W",

    /*88*/ L"X",

    /*89*/ L"Y",

    /*90*/ L"Z",

    /*91*/ L"[",

    /*92*/ L"\\",

    /*93*/ L"]",

    /*94*/ L"^",

    /*95*/ L"_",

    /*96*/ L"`",

    /*97*/ L"a",

    /*98*/ L"b",

    /*99*/ L"c",

    /*100*/ L"d",

    /*101*/ L"e",

    /*102*/ L"f",

    /*103*/ L"g",

    /*104*/ L"h",

    /*105*/ L"i",

    /*106*/ L"j",

    /*107*/ L"k",

    /*108*/ L"l",

    /*109*/ L"m",

    /*110*/ L"n",

    /*111*/ L"o",

    /*112*/ L"p",

    /*113*/ L"q",

    /*114*/ L"r",

    /*115*/ L"s",

    /*116*/ L"t",

    /*117*/ L"u",

    /*118*/ L"v",

    /*119*/ L"w",

    /*120*/ L"x",

    /*121*/ L"y",

    /*122*/ L"z",

    /*123*/ L"{",

    /*124*/ L"|",

    /*125*/ L"}",

    /*126*/ L"~",

    /*127*/ L"ی",

    /*128*/ L"0",

    /*129*/ L"1",

    /*130*/ L"2",

    /*131*/ L"3",

    /*132*/ L"4",

    /*133*/ L"5",

    /*134*/ L"6",

    /*135*/ L"7",

    /*136*/ L"8",

    /*137*/ L"9",

    /*138*/ L"",

    /*139*/ L"",

    /*140*/ L"",

    /*141*/ L"آ",

    /*142*/ L"ئ",

    /*143*/ L"ئ",

    /*144*/ L"ا",

    /*145*/ L"ا",        // alef chasban

    /*146*/ L"ب" "\u200c",

    /*147*/ L"ب",        // b avval

    /*148*/ L"پ" "\u200c",

    /*149*/ L"پ",        // p avval

    /*150*/ L"ت" "\u200c",

    /*151*/ L"ت",        // t avval

    /*152*/ L"ث" "\u200c",

    /*153*/ L"ث",        // s avval

    /*154*/ L"ج" "\u200c",

    /*155*/ L"ج",        // j avval

    /*156*/ L"چ" "\u200c",

    /*157*/ L"چ",        // ch avval

    /*158*/ L"ح" "\u200c",

    /*159*/ L"ح",        // h avval

    /*160*/ L"خ" "\u200c",

    /*161*/ L"خ",        // kh avval

    /*162*/ L"د",

    /*163*/ L"ذ",

    /*164*/ L"ر",

    /*165*/ L"ز",

    /*166*/ L"ژ",

    /*167*/ L"س",

    /*168*/ L"س",        // sin avval

    /*169*/ L"ش",

    /*170*/ L"ش",        // sh avval

    /*171*/ L"ص",

    /*172*/ L"ص",        // sad avval

    /*173*/ L"ض",

    /*174*/ L"ض",        // zad avval

    /*175*/ L"ط",

    /*176*/ L"ی",

    /*177*/ L"ی",

    /*178*/ L"ی",

    /*179*/ L"ي",

    /*180*/ L"ی",

    /*181*/ L"لا",

    /*182*/ L"ی",        // y avval

    /*183*/ L"لا",

    /*184*/ L"ک",

    /*185*/ L"ی",

    /*186*/ L"ی",

    /*187*/ L"+",

    /*188*/ L"+",

    /*189*/ L"",

    /*190*/ L"",

    /*191*/ L"+",

    /*192*/ L"+",

    /*193*/ L"-",

    /*194*/ L"-",

    /*195*/ L"+",

    /*196*/ L"-",

    /*197*/ L"+",

    /*198*/ L"گ",

    /*199*/ L"ه" "\u200c",        // h akhar

    /*200*/ L"+",

    /*201*/ L"+",

    /*202*/ L"-",

    /*203*/ L"-",

    /*204*/ L"ی",

    /*205*/ L"-",

    /*206*/ L"+",

    /*207*/ L"ه",

    /*208*/ L"ه",

    /*209*/ L"ج" "\u200c",        // j akhar chasban

    /*210*/ L"چ" "\u200c",        // ch akhar chasban

    /*211*/ L"ح" "\u200c",        // h akhar chasban

    /*212*/ L"i",

    /*213*/ L"4",

    /*214*/ L"5",

    /*215*/ L"6",

    /*216*/ L"+",

    /*217*/ L"+",

    /*218*/ L"+",

    /*219*/ L"ی",

    /*220*/ L"_",

    /*221*/ L"ی",

    /*222*/ L"ی",

    /*223*/ L"ک",

    /*224*/ L"ظ",

    /*225*/ L"ع",

    /*226*/ L"ع" "\u200c",        // ein akhar chasban

    /*227*/ L"ع",        // ein vasat

    /*228*/ L"ع",        // ein avval

    /*229*/ L"غ",

    /*230*/ L"غ" "\u200c",        // akhar chasban

    /*231*/ L"غ",        // ghein vasat

    /*232*/ L"غ",        // ghein avval

    /*233*/ L"ف" "\u200c",

    /*234*/ L"ف",        // f avval

    /*235*/ L"ق" "\u200c",

    /*236*/ L"ق",        // gh avval

    /*237*/ L"ک" "\u200c",

    /*238*/ L"ک",        // k avval

    /*239*/ L"گ" "\u200c",

    /*240*/ L"گ",        // g avval

    /*241*/ L"ل" "\u200c",

    /*242*/ L"=",

    /*243*/ L"ل",        // lam avval

    /*244*/ L"م" "\u200c",

    /*245*/ L"م",        // mim avval

    /*246*/ L"ن" "\u200c",

    /*247*/ L"ن",        // noon avval

    /*248*/ L"و",

    /*249*/ L"ه" "\u200c",

    /*250*/ L"ه",        // h vasat

    /*251*/ L"ه",        // h avval

    /*252*/ L"ی" "\u200c",        // ye akhare chasban

    /*253*/ L"ی",

    /*254*/ L"ی",

    /*255*/ L"" };
  QString           res;
  std::stack<char>  tempStack;

  for (qint32 i = 31; i >= 0; i--)
  {
    if (isNumber(str[i]))
    {
      tempStack.push(str[i]);

      continue;
    }
    else
    {
      while (!tempStack.empty())
      {
        res += QString::fromStdWString(farsiCode[static_cast<quint8>(tempStack.top())]);

        tempStack.pop();
      }
    }

    res += QString::fromStdWString(farsiCode[static_cast<quint8>(str[i])]);
  }

  return res;
}

QByteArray  Utilities::uint16ToByteArray(quint16 n)
{
  QByteArray   result;
  QDataStream  ds(&result, QIODevice::WriteOnly);

  ds << n;

  return result;
}

quint16  Utilities::readUint16(const QByteArray &data)
{
  quint16      res;
  QByteArray   temp(data);
  QDataStream  ds(&temp, QIODevice::ReadOnly);

  ds >> res;

  return res;
}

quint32  Utilities::readUint32(const QByteArray &data)
{
  quint32      res;
  QByteArray   temp(data);
  QDataStream  ds(&temp, QIODevice::ReadOnly);

  ds >> res;

  return res;
}

QHostAddress  Utilities::getIpAddress()
{
  QHostAddress  res(QHostAddress::LocalHost);

  for (const auto &address : QNetworkInterface::allAddresses())
  {
    if ((QAbstractSocket::IPv4Protocol == address.protocol()) && (QHostAddress(QHostAddress::LocalHost) != address))
    {
      res = address;
      break;
    }
  }

  return res;
}

QString Utilities::getIpString()
{
  QString  ip, res;

  auto const ipParts = Utilities::getIpAddress().toString().split(".");

  for (auto const &s : ipParts)
  {
    ip += s.rightJustified(3, '0');
  }

  if (12 == ip.size())
  {
    res = ip;
  }

  return res;
}


QString  Utilities::getMacAddress()
{
  QString  mac = "00:00:00:00:00:00";

  for (const auto &interface : QNetworkInterface::allInterfaces())
  {
    if (QNetworkInterface::IsLoopBack != (interface.flags() & QNetworkInterface::IsLoopBack))
    {
      mac = interface.hardwareAddress().toUpper();
      break;
    }
  }

  return mac;
}

bool  Utilities::setIpAddress(const QString &ipv4)
{
  bool   res = false;
  QFile  fileNetwork("/etc/network/interfaces");

  if (fileNetwork.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
  {
    QString  s = "auto lo eth0\n"
                 "iface lo inet loopback\n"
                 "iface eth0 inet static\n";

    s += "      address " + ipv4 + "\n";
    s += "      netmask 255.255.0.0\n"
         "      gateway 192.168.1.1\n";

    fileNetwork.write(s.toLatin1());
    fileNetwork.flush();

    QProcess  proc;
    proc.start("ifconfig", QStringList() << "eth0" << "down");
    proc.waitForFinished();
    proc.start("ifconfig", QStringList() << "eth0" << ipv4);
    proc.waitForFinished();
    proc.start("ifconfig", QStringList() << "eth0" << "up");
    proc.waitForFinished();

    res = true;
  }

  return res;
}

quint16  Utilities::generateSerialId(quint8 seed)
{
  const quint8  ipLowByte = getIpAddress().toIPv4Address() % 256;

  return (static_cast<quint16>(seed) << 8) | ipLowByte;
}

QByteArray  Utilities::generateEquipmentSerialId()
{
  // Total size must be 20 bytes.
  QByteArray  res;
  const auto &ip = getIpAddress().toIPv4Address();

  // res.append("0000000000");
  // res.append(quint8((ip & 0xFF000000) >> 24));
  // res.append(quint8((ip & 0x00FF0000) >> 16));
  // res.append(quint8((ip & 0x0000FF00) >> 8));
  // res.append(quint8(ip & 0x000000FF));
  // res.append(Utilities::macStringToByteArray(Utilities::getMacAddress()));

  res.append("00000");
  res.append(QString::number((ip & 0xFF000000) >> 24).rightJustified(3, '0'));
  res.append('.');
  res.append(QString::number((ip & 0x00FF0000) >> 16).rightJustified(3, '0'));
  res.append('.');
  res.append(QString::number((ip & 0x0000FF00) >> 8).rightJustified(3, '0'));
  res.append('.');
  res.append(QString::number(ip & 0x000000FF)).rightJustified(3, '0');

  return res;
}

QByteArray  Utilities::uint32ToByteArray(quint32 n)
{
  QByteArray   result;
  QDataStream  ds(&result, QIODevice::WriteOnly);

  ds << n;

  return result;
}

bool  Utilities::miladiIsLeap(int year)
{
  if ((((year % 100) != 0) && ((year % 4) == 0)) || (((year % 100) == 0) && ((year % 400) == 0)))
  {
    return true;
  }
  else
  {
    return false;
  }
}

QByteArray  Utilities::macStringToByteArray(const QString &mac)
{
  QByteArray  res;

  res.append(QByteArray::fromHex(mac.mid(0, 2).toLatin1()));
  res.append(QByteArray::fromHex(mac.mid(3, 2).toLatin1()));
  res.append(QByteArray::fromHex(mac.mid(6, 2).toLatin1()));
  res.append(QByteArray::fromHex(mac.mid(9, 2).toLatin1()));
  res.append(QByteArray::fromHex(mac.mid(12, 2).toLatin1()));
  res.append(QByteArray::fromHex(mac.mid(15, 2).toLatin1()));

  return res;
}

void  Utilities::systemReboot()
{
  QProcess  proc;

  proc.execute("killall", {"elin"});
  proc.waitForFinished();
}

bool  Utilities::decompressXz(const QString &path)
{
  QProcess  proc;

  proc.start("xz", QStringList() << "-d" << path);

  return proc.waitForFinished();
}

void  Utilities::initUsers()
{
  const QString user = Settings::USER_NAME;
  const QString pass = Settings::USER_PASS;

  if (!QDir::current().exists("/home/" + user))
  {
    QProcess  proc;
    const auto cmd = QStringList() << "-c" << "adduser -D " + user + "; echo -e \""+ pass + "\\n" + pass
                                      + "\" | passwd " + user + "; mkdir -p /home/" + user
                                      + "; chown " + user + ":" + user + " /home/" + user;

    proc.start("/bin/sh", cmd);
    proc.waitForFinished();
  }
}

void  Utilities::setOwner(const QString &path, const QString &owner)
{
  if (QDir::current().exists(path))
  {
    QProcess  proc;
    const auto cmd = QStringList() << "-c" << "chown " + owner + ":" + owner + " " + path;

    proc.start("/bin/sh", cmd);
    proc.waitForFinished();
  }
}

bool  Utilities::exec(const QString &proc, const QStringList &params)
{
  QProcess  p;

  p.start(proc, params);

  return p.waitForFinished();
}

QDate  Utilities::miladiToShamsi(QDate miladi)
{
  int  shamsiYear, shamsiMonth, shamsiDay;
  int  year = miladi.year();
  int  month = miladi.month();
  int  day = miladi.day();
  int  dayCount, farvardinDayDiff, deyDayDiff;
  int  sumDayMiladiMonth[]     = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };
  int  sumDayMiladiMonthLeap[] = { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335 };

  farvardinDayDiff = 79;

  if (miladiIsLeap(year))
  {
    dayCount = sumDayMiladiMonthLeap[month - 1] + day;
  }
  else
  {
    dayCount = sumDayMiladiMonth[month - 1] + day;
  }

  if ((miladiIsLeap(year - 1)))
  {
    deyDayDiff = 11;
  }
  else
  {
    deyDayDiff = 10;
  }

  if (dayCount > farvardinDayDiff)
  {
    dayCount = dayCount - farvardinDayDiff;

    if (dayCount <= 186)
    {
      switch (dayCount % 31)
      {
      case 0:
        shamsiMonth = dayCount / 31;
        shamsiDay   = 31;
        break;
      default:
        shamsiMonth = (dayCount / 31) + 1;
        shamsiDay   = (dayCount % 31);
        break;
      }

      shamsiYear = year - 621;
    }
    else
    {
      dayCount = dayCount - 186;

      switch (dayCount % 30)
      {
      case 0:
        shamsiMonth = (dayCount / 30) + 6;
        shamsiDay   = 30;
        break;
      default:
        shamsiMonth = (dayCount / 30) + 7;
        shamsiDay   = (dayCount % 30);
        break;
      }

      shamsiYear = year - 621;
    }
  }
  else
  {
    dayCount = dayCount + deyDayDiff;

    switch (dayCount % 30)
    {
    case 0:
      shamsiMonth = (dayCount / 30) + 9;
      shamsiDay   = 30;
      break;
    default:
      shamsiMonth = (dayCount / 30) + 10;
      shamsiDay   = (dayCount % 30);
      break;
    }

    shamsiYear = year - 622;
  }

  return QDate(shamsiYear, shamsiMonth, shamsiDay);
}
}
