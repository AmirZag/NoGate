/****************************************************************************
** Meta object code from reading C++ file 'QrServerManager.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "QrServerManager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QrServerManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Afc__QrServerManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      36,   22,   21,   21, 0x05,

 // slots: signature, parameters, type, tag, flags
     112,   21,   21,   21, 0x08,
     132,   21,   21,   21, 0x08,
     156,  152,   21,   21, 0x08,
     200,   21,   21,   21, 0x08,
     218,   21,   21,   21, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Afc__QrServerManager[] = {
    "Afc::QrServerManager\0\0ticket,result\0"
    "validationReady(QSharedPointer<QrTicket>,QrServerManager::ValidationRe"
    "sult)\0"
    "onSocketConnected()\0onSocketReadyRead()\0"
    "err\0onSocketError(QAbstractSocket::SocketError)\0"
    "connectToServer()\0onTimerNetwork()\0"
};

void Afc::QrServerManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QrServerManager *_t = static_cast<QrServerManager *>(_o);
        switch (_id) {
        case 0: _t->validationReady((*reinterpret_cast< const QSharedPointer<QrTicket>(*)>(_a[1])),(*reinterpret_cast< const QrServerManager::ValidationResult(*)>(_a[2]))); break;
        case 1: _t->onSocketConnected(); break;
        case 2: _t->onSocketReadyRead(); break;
        case 3: _t->onSocketError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 4: _t->connectToServer(); break;
        case 5: _t->onTimerNetwork(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Afc::QrServerManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Afc::QrServerManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Afc__QrServerManager,
      qt_meta_data_Afc__QrServerManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Afc::QrServerManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Afc::QrServerManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Afc::QrServerManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Afc__QrServerManager))
        return static_cast<void*>(const_cast< QrServerManager*>(this));
    return QObject::qt_metacast(_clname);
}

int Afc::QrServerManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void Afc::QrServerManager::validationReady(const QSharedPointer<QrTicket> & _t1, const QrServerManager::ValidationResult & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
