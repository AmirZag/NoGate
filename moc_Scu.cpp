/****************************************************************************
** Meta object code from reading C++ file 'Scu.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "Scu.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Scu.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Afc__Scu[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x08,
      28,    9,    9,    9, 0x08,
      42,    9,    9,    9, 0x08,
      59,    9,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Afc__Scu[] = {
    "Afc::Scu\0\0onNewConnection()\0onReadyRead()\0"
    "onDisconnected()\0onTimerTimeout()\0"
};

void Afc::Scu::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Scu *_t = static_cast<Scu *>(_o);
        switch (_id) {
        case 0: _t->onNewConnection(); break;
        case 1: _t->onReadyRead(); break;
        case 2: _t->onDisconnected(); break;
        case 3: _t->onTimerTimeout(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData Afc::Scu::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Afc::Scu::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Afc__Scu,
      qt_meta_data_Afc__Scu, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Afc::Scu::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Afc::Scu::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Afc::Scu::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Afc__Scu))
        return static_cast<void*>(const_cast< Scu*>(this));
    return QObject::qt_metacast(_clname);
}

int Afc::Scu::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
