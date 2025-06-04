/****************************************************************************
** Meta object code from reading C++ file 'ParameterManager.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ParameterManager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ParameterManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Afc__ParameterManager[] = {

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
      23,   22,   22,   22, 0x08,
      33,   22,   22,   22, 0x08,
      46,   22,   22,   22, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Afc__ParameterManager[] = {
    "Afc::ParameterManager\0\0exitApp()\0"
    "loadParams()\0loadINITIATE()\0"
};

void Afc::ParameterManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ParameterManager *_t = static_cast<ParameterManager *>(_o);
        switch (_id) {
        case 0: _t->exitApp(); break;
        case 1: _t->loadParams(); break;
        case 2: _t->loadINITIATE(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData Afc::ParameterManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Afc::ParameterManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Afc__ParameterManager,
      qt_meta_data_Afc__ParameterManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Afc::ParameterManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Afc::ParameterManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Afc::ParameterManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Afc__ParameterManager))
        return static_cast<void*>(const_cast< ParameterManager*>(this));
    return QObject::qt_metacast(_clname);
}

int Afc::ParameterManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
