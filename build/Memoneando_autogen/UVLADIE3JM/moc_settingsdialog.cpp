/****************************************************************************
** Meta object code from reading C++ file 'settingsdialog.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/settingsdialog.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'settingsdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_SettingsDialog_t {
    uint offsetsAndSizes[20];
    char stringdata0[15];
    char stringdata1[14];
    char stringdata2[1];
    char stringdata3[14];
    char stringdata4[23];
    char stringdata5[18];
    char stringdata6[28];
    char stringdata7[19];
    char stringdata8[17];
    char stringdata9[12];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_SettingsDialog_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_SettingsDialog_t qt_meta_stringdata_SettingsDialog = {
    {
        QT_MOC_LITERAL(0, 14),  // "SettingsDialog"
        QT_MOC_LITERAL(15, 13),  // "applySettings"
        QT_MOC_LITERAL(29, 0),  // ""
        QT_MOC_LITERAL(30, 13),  // "resetSettings"
        QT_MOC_LITERAL(44, 22),  // "chooseDefaultTextColor"
        QT_MOC_LITERAL(67, 17),  // "chooseDefaultFont"
        QT_MOC_LITERAL(85, 27),  // "chooseCanvasBackgroundColor"
        QT_MOC_LITERAL(113, 18),  // "testUnsplashApiKey"
        QT_MOC_LITERAL(132, 16),  // "testPexelsApiKey"
        QT_MOC_LITERAL(149, 11)   // "saveApiKeys"
    },
    "SettingsDialog",
    "applySettings",
    "",
    "resetSettings",
    "chooseDefaultTextColor",
    "chooseDefaultFont",
    "chooseCanvasBackgroundColor",
    "testUnsplashApiKey",
    "testPexelsApiKey",
    "saveApiKeys"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_SettingsDialog[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   62,    2, 0x08,    1 /* Private */,
       3,    0,   63,    2, 0x08,    2 /* Private */,
       4,    0,   64,    2, 0x08,    3 /* Private */,
       5,    0,   65,    2, 0x08,    4 /* Private */,
       6,    0,   66,    2, 0x08,    5 /* Private */,
       7,    0,   67,    2, 0x08,    6 /* Private */,
       8,    0,   68,    2, 0x08,    7 /* Private */,
       9,    0,   69,    2, 0x08,    8 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject SettingsDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_SettingsDialog.offsetsAndSizes,
    qt_meta_data_SettingsDialog,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_SettingsDialog_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<SettingsDialog, std::true_type>,
        // method 'applySettings'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'resetSettings'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'chooseDefaultTextColor'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'chooseDefaultFont'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'chooseCanvasBackgroundColor'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'testUnsplashApiKey'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'testPexelsApiKey'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'saveApiKeys'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void SettingsDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SettingsDialog *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->applySettings(); break;
        case 1: _t->resetSettings(); break;
        case 2: _t->chooseDefaultTextColor(); break;
        case 3: _t->chooseDefaultFont(); break;
        case 4: _t->chooseCanvasBackgroundColor(); break;
        case 5: _t->testUnsplashApiKey(); break;
        case 6: _t->testPexelsApiKey(); break;
        case 7: _t->saveApiKeys(); break;
        default: ;
        }
    }
    (void)_a;
}

const QMetaObject *SettingsDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SettingsDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SettingsDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int SettingsDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
