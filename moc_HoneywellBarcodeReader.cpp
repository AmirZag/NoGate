/****************************************************************************
** Meta object code from reading C++ file 'HoneywellBarcodeReader.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "HoneywellBarcodeReader.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'HoneywellBarcodeReader.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Afc__HoneywellBarcodeReader[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      29,   28,   28,   28, 0x08,
      45,   28,   28,   28, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Afc__HoneywellBarcodeReader[] = {
    "Afc::HoneywellBarcodeReader\0\0"
    "onTimerStatus()\0onSerialDataReady()\0"
};

void Afc::HoneywellBarcodeReader::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        HoneywellBarcodeReader *_t = static_cast<HoneywellBarcodeReader *>(_o);
        switch (_id) {
        case 0: _t->onTimerStatus(); break;
        case 1: _t->onSerialDataReady(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData Afc::HoneywellBarcodeReader::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Afc::HoneywellBarcodeReader::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Afc__HoneywellBarcodeReader,
      qt_meta_data_Afc__HoneywellBarcodeReader, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Afc::HoneywellBarcodeReader::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Afc::HoneywellBarcodeReader::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Afc::HoneywellBarcodeReader::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Afc__HoneywellBarcodeReader))
        return static_cast<void*>(const_cast< HoneywellBarcodeReader*>(this));
    if (!strcmp(_clname, "BarcodeReader"))
        return static_cast< BarcodeReader*>(const_cast< HoneywellBarcodeReader*>(this));
    return QObject::qt_metacast(_clname);
}

int Afc::HoneywellBarcodeReader::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
