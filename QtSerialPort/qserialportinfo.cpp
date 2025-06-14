/****************************************************************************
**
** Copyright (C) 2011-2012 Denis Shienkov <denis.shienkov@gmail.com>
** Copyright (C) 2011 Sergey Belyashov <Sergey.Belyashov@gmail.com>
** Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qserialportinfo.h"
#include "qserialportinfo_p.h"
#include "qserialport.h"

QT_BEGIN_NAMESPACE


/*!
    \class QSerialPortInfo

    @brief Provides information about existing serial ports.

    \ingroup serialport-main
    \inmodule QtSerialPort
    \since 5.1

    Use the static functions to generate a list of QSerialPortInfo
    objects. Each QSerialPortInfo object in the list represents a single
    serial port and can be queried for the port name, system location,
    description, and manufacturer. The QSerialPortInfo class can also be
    used as an input parameter for the setPort() method of the QSerialPort
    class.

    \sa QSerialPort
*/

/*!
    Constructs an empty QSerialPortInfo object.

    \sa isNull()
*/
QSerialPortInfo::QSerialPortInfo()
    : d_ptr(new QSerialPortInfoPrivate)
{
}

/*!
    Constructs a copy of \a other.
*/
QSerialPortInfo::QSerialPortInfo(const QSerialPortInfo &other)
    : d_ptr(other.d_ptr ? new QSerialPortInfoPrivate(*other.d_ptr) : 0)
{
}

/*!
    Constructs a QSerialPortInfo object from serial \a port.
*/
QSerialPortInfo::QSerialPortInfo(const QSerialPort &port)
    : d_ptr(new QSerialPortInfoPrivate)
{
    foreach (const QSerialPortInfo &serialPortInfo, availablePorts()) {
        if (port.portName() == serialPortInfo.portName()) {
            *this = serialPortInfo;
            break;
        }
    }
}

/*!
    Constructs a QSerialPortInfo object from serial port \a name.

    This constructor finds the relevant serial port among the available ones
    according to the port name \a name, and constructs the serial port info
    instance for that port.
*/
QSerialPortInfo::QSerialPortInfo(const QString &name)
    : d_ptr(new QSerialPortInfoPrivate)
{
    foreach (const QSerialPortInfo &serialPortInfo, availablePorts()) {
        if (name == serialPortInfo.portName()) {
            *this = serialPortInfo;
            break;
        }
    }
}

/*!
    Destroys the QSerialPortInfo object. References to the values in the
    object become invalid.
*/
QSerialPortInfo::~QSerialPortInfo()
{
}

/*! \fn void QSerialPortInfo::swap(QSerialPortInfo &other)

    Swaps QSerialPortInfo \a other with this QSerialPortInfo. This operation is
    very fast and never fails.
*/
void QSerialPortInfo::swap(QSerialPortInfo &other)
{
    d_ptr.swap(other.d_ptr);
}

/*!
    Sets the QSerialPortInfo object to be equal to \a other.
*/
QSerialPortInfo& QSerialPortInfo::operator=(const QSerialPortInfo &other)
{
    QSerialPortInfo(other).swap(*this);
    return *this;
}

/*!
    Returns the name of the serial port.
*/
QString QSerialPortInfo::portName() const
{
    Q_D(const QSerialPortInfo);
    return !d ? QString() : d->portName;
}

/*!
    Returns the system location of the serial port.
*/
QString QSerialPortInfo::systemLocation() const
{
    Q_D(const QSerialPortInfo);
    return !d ? QString() : d->device;
}

/*!
    Returns the description string of the serial port,
    if available; otherwise returns an empty string.
*/
QString QSerialPortInfo::description() const
{
    Q_D(const QSerialPortInfo);
    return !d ? QString() : d->description;
}

/*!
    Returns the manufacturer string of the serial port,
    if available; otherwise returns an empty string.
*/
QString QSerialPortInfo::manufacturer() const
{
    Q_D(const QSerialPortInfo);
    return !d ? QString() : d->manufacturer;
}

/*!
    Returns the 16-bit vendor number for the serial port, if available;
    otherwise returns zero.
*/
quint16 QSerialPortInfo::vendorIdentifier() const
{
    Q_D(const QSerialPortInfo);
    return !d ? 0 : d->vendorIdentifier;
}

/*!
    Returns the 16-bit product number for the serial port, if available;
    otherwise returns zero.
*/
quint16 QSerialPortInfo::productIdentifier() const
{
    Q_D(const QSerialPortInfo);
    return !d ? 0 : d->productIdentifier;
}

/*!
    Returns true if there is a valid 16-bit vendor number present; otherwise
    returns false.
*/
bool QSerialPortInfo::hasVendorIdentifier() const
{
    Q_D(const QSerialPortInfo);
    return !d ? false : d->hasVendorIdentifier;
}

/*!
    Returns true if there is a valid 16-bit product number present; otherwise
    returns false.
*/
bool QSerialPortInfo::hasProductIdentifier() const
{
    Q_D(const QSerialPortInfo);
    return !d ? false : d->hasProductIdentifier;
}

/*!
    \fn bool QSerialPortInfo::isNull() const

    Returns whether this QSerialPortInfo object holds a
    serial port definition.
*/

/*!
    \fn bool QSerialPortInfo::isBusy() const

    Returns true if serial port is busy;
    otherwise returns false.
*/

/*!
    \fn bool QSerialPortInfo::isValid() const

    Returns true if serial port is present on system;
    otherwise returns false.
*/

/*!
    \fn QList<qint32> QSerialPortInfo::standardBaudRates()

    Returns a list of available standard baud rates supported by
    the current serial port.
*/

/*!
    \fn QList<QSerialPortInfo> QSerialPortInfo::availablePorts()

    Returns a list of available serial ports on the system.
*/

QT_END_NAMESPACE
