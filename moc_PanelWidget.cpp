/****************************************************************************
** Meta object code from reading C++ file 'PanelWidget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "PanelWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PanelWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Afc__PanelWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      44,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   18,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      48,   17,   17,   17, 0x08,
      70,   17,   17,   17, 0x08,
     102,   17,   17,   17, 0x08,
     117,   17,   17,   17, 0x08,
     134,   17,   17,   17, 0x08,
     163,   17,   17,   17, 0x08,
     190,   17,   17,   17, 0x08,
     219,   17,   17,   17, 0x08,
     248,   17,   17,   17, 0x08,
     276,   17,   17,   17, 0x08,
     307,   17,   17,   17, 0x08,
     338,   17,   17,   17, 0x08,
     371,   17,   17,   17, 0x08,
     396,   17,   17,   17, 0x08,
     429,   17,   17,   17, 0x08,
     461,   17,   17,   17, 0x08,
     485,   17,   17,   17, 0x08,
     520,   17,   17,   17, 0x08,
     547,   17,   17,   17, 0x08,
     576,   17,   17,   17, 0x08,
     597,   17,   17,   17, 0x08,
     630,   17,   17,   17, 0x08,
     663,   17,   17,   17, 0x08,
     687,   17,   17,   17, 0x08,
     712,   17,   17,   17, 0x08,
     741,   17,   17,   17, 0x08,
     767,   17,   17,   17, 0x08,
     792,   17,   17,   17, 0x08,
     817,   17,   17,   17, 0x08,
     843,   17,   17,   17, 0x08,
     865,   17,   17,   17, 0x08,
     893,   17,   17,   17, 0x08,
     921,   17,   17,   17, 0x08,
     949,   17,   17,   17, 0x08,
     977,   17,   17,   17, 0x08,
    1005,   17,   17,   17, 0x08,
    1033,   17,   17,   17, 0x08,
    1061,   17,   17,   17, 0x08,
    1089,   17,   17,   17, 0x08,
    1117,   17,   17,   17, 0x08,
    1148,   17,   17,   17, 0x08,
    1171,   17,   17,   17, 0x08,
    1202,   17,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Afc__PanelWidget[] = {
    "Afc::PanelWidget\0\0ip\0qrServerIpChanged(QString)\0"
    "resetBtnSamKeyStyle()\0"
    "resetBtnClearGateCounterStyle()\0"
    "showKeyboard()\0onTimerTimeout()\0"
    "on_btnExitOperator_clicked()\0"
    "on_btnIpSettings_clicked()\0"
    "on_btnDateSettings_clicked()\0"
    "on_btnGateSettings_clicked()\0"
    "on_btnSamSettings_clicked()\0"
    "on_btnFileOperations_clicked()\0"
    "on_btnExitIpSettings_clicked()\0"
    "on_btnExitDateSettings_clicked()\0"
    "on_btnSaveDate_clicked()\0"
    "on_btnExitGateSettings_clicked()\0"
    "on_btnExitSamSettings_clicked()\0"
    "on_btnSaveSam_clicked()\0"
    "on_btnExitFileOperations_clicked()\0"
    "on_btnParameters_clicked()\0"
    "on_btnTransactions_clicked()\0"
    "on_btnLogs_clicked()\0"
    "on_btnExitTransactions_clicked()\0"
    "on_btnSaveTransactions_clicked()\0"
    "on_btnExitLog_clicked()\0"
    "on_btnSaveLogs_clicked()\0"
    "on_btnExitGateMode_clicked()\0"
    "on_btnExitPanel_clicked()\0"
    "on_btnOperator_clicked()\0"
    "on_btnGateMode_clicked()\0"
    "on_btnExitLogin_clicked()\0"
    "on_btnLogin_clicked()\0on_radioGateMode1_clicked()\0"
    "on_radioGateMode2_clicked()\0"
    "on_radioGateMode3_clicked()\0"
    "on_radioGateMode4_clicked()\0"
    "on_radioGateMode5_clicked()\0"
    "on_radioGateMode6_clicked()\0"
    "on_radioGateMode7_clicked()\0"
    "on_radioGateMode8_clicked()\0"
    "on_radioGateMode9_clicked()\0"
    "on_radioGateModeAuto_clicked()\0"
    "on_btnSaveIp_clicked()\0"
    "on_btnExitParameters_clicked()\0"
    "on_btnClearGateCounters_clicked()\0"
};

void Afc::PanelWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        PanelWidget *_t = static_cast<PanelWidget *>(_o);
        switch (_id) {
        case 0: _t->qrServerIpChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->resetBtnSamKeyStyle(); break;
        case 2: _t->resetBtnClearGateCounterStyle(); break;
        case 3: _t->showKeyboard(); break;
        case 4: _t->onTimerTimeout(); break;
        case 5: _t->on_btnExitOperator_clicked(); break;
        case 6: _t->on_btnIpSettings_clicked(); break;
        case 7: _t->on_btnDateSettings_clicked(); break;
        case 8: _t->on_btnGateSettings_clicked(); break;
        case 9: _t->on_btnSamSettings_clicked(); break;
        case 10: _t->on_btnFileOperations_clicked(); break;
        case 11: _t->on_btnExitIpSettings_clicked(); break;
        case 12: _t->on_btnExitDateSettings_clicked(); break;
        case 13: _t->on_btnSaveDate_clicked(); break;
        case 14: _t->on_btnExitGateSettings_clicked(); break;
        case 15: _t->on_btnExitSamSettings_clicked(); break;
        case 16: _t->on_btnSaveSam_clicked(); break;
        case 17: _t->on_btnExitFileOperations_clicked(); break;
        case 18: _t->on_btnParameters_clicked(); break;
        case 19: _t->on_btnTransactions_clicked(); break;
        case 20: _t->on_btnLogs_clicked(); break;
        case 21: _t->on_btnExitTransactions_clicked(); break;
        case 22: _t->on_btnSaveTransactions_clicked(); break;
        case 23: _t->on_btnExitLog_clicked(); break;
        case 24: _t->on_btnSaveLogs_clicked(); break;
        case 25: _t->on_btnExitGateMode_clicked(); break;
        case 26: _t->on_btnExitPanel_clicked(); break;
        case 27: _t->on_btnOperator_clicked(); break;
        case 28: _t->on_btnGateMode_clicked(); break;
        case 29: _t->on_btnExitLogin_clicked(); break;
        case 30: _t->on_btnLogin_clicked(); break;
        case 31: _t->on_radioGateMode1_clicked(); break;
        case 32: _t->on_radioGateMode2_clicked(); break;
        case 33: _t->on_radioGateMode3_clicked(); break;
        case 34: _t->on_radioGateMode4_clicked(); break;
        case 35: _t->on_radioGateMode5_clicked(); break;
        case 36: _t->on_radioGateMode6_clicked(); break;
        case 37: _t->on_radioGateMode7_clicked(); break;
        case 38: _t->on_radioGateMode8_clicked(); break;
        case 39: _t->on_radioGateMode9_clicked(); break;
        case 40: _t->on_radioGateModeAuto_clicked(); break;
        case 41: _t->on_btnSaveIp_clicked(); break;
        case 42: _t->on_btnExitParameters_clicked(); break;
        case 43: _t->on_btnClearGateCounters_clicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Afc::PanelWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Afc::PanelWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Afc__PanelWidget,
      qt_meta_data_Afc__PanelWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Afc::PanelWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Afc::PanelWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Afc::PanelWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Afc__PanelWidget))
        return static_cast<void*>(const_cast< PanelWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int Afc::PanelWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 44)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 44;
    }
    return _id;
}

// SIGNAL 0
void Afc::PanelWidget::qrServerIpChanged(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
