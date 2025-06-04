/****************************************************************************
** Meta object code from reading C++ file 'MetroBusiness.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "MetroBusiness.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MetroBusiness.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Afc__MetroBusiness[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      20,   19,   19,   19, 0x08,
      37,   19,   19,   19, 0x08,
      75,   61,   19,   19, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Afc__MetroBusiness[] = {
    "Afc::MetroBusiness\0\0onTimerTimeout()\0"
    "onTimerSessionTimeout()\0ticket,result\0"
    "onQrValidationReady(QSharedPointer<QrTicket>,QrServerManager::Validati"
    "onResult)\0"
};

void Afc::MetroBusiness::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MetroBusiness *_t = static_cast<MetroBusiness *>(_o);
        switch (_id) {
        case 0: _t->onTimerTimeout(); break;
        case 1: _t->onTimerSessionTimeout(); break;
        case 2: _t->onQrValidationReady((*reinterpret_cast< const QSharedPointer<QrTicket>(*)>(_a[1])),(*reinterpret_cast< const QrServerManager::ValidationResult(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Afc::MetroBusiness::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Afc::MetroBusiness::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Afc__MetroBusiness,
      qt_meta_data_Afc__MetroBusiness, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Afc::MetroBusiness::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Afc::MetroBusiness::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Afc::MetroBusiness::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Afc__MetroBusiness))
        return static_cast<void*>(const_cast< MetroBusiness*>(this));
    if (!strcmp(_clname, "Business"))
        return static_cast< Business*>(const_cast< MetroBusiness*>(this));
    return QObject::qt_metacast(_clname);
}

int Afc::MetroBusiness::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
