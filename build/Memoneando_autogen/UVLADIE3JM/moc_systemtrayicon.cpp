/****************************************************************************
** Meta object code from reading C++ file 'systemtrayicon.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/systemtrayicon.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'systemtrayicon.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_SystemTrayIcon_t {
    uint offsetsAndSizes[14];
    char stringdata0[15];
    char stringdata1[14];
    char stringdata2[1];
    char stringdata3[34];
    char stringdata4[7];
    char stringdata5[15];
    char stringdata6[16];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_SystemTrayIcon_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_SystemTrayIcon_t qt_meta_stringdata_SystemTrayIcon = {
    {
        QT_MOC_LITERAL(0, 14),  // "SystemTrayIcon"
        QT_MOC_LITERAL(15, 13),  // "iconActivated"
        QT_MOC_LITERAL(29, 0),  // ""
        QT_MOC_LITERAL(30, 33),  // "QSystemTrayIcon::ActivationRe..."
        QT_MOC_LITERAL(64, 6),  // "reason"
        QT_MOC_LITERAL(71, 14),  // "showMainWindow"
        QT_MOC_LITERAL(86, 15)   // "quitApplication"
    },
    "SystemTrayIcon",
    "iconActivated",
    "",
    "QSystemTrayIcon::ActivationReason",
    "reason",
    "showMainWindow",
    "quitApplication"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_SystemTrayIcon[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   32,    2, 0x08,    1 /* Private */,
       5,    0,   35,    2, 0x08,    3 /* Private */,
       6,    0,   36,    2, 0x08,    4 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject SystemTrayIcon::staticMetaObject = { {
    QMetaObject::SuperData::link<QSystemTrayIcon::staticMetaObject>(),
    qt_meta_stringdata_SystemTrayIcon.offsetsAndSizes,
    qt_meta_data_SystemTrayIcon,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_SystemTrayIcon_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<SystemTrayIcon, std::true_type>,
        // method 'iconActivated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QSystemTrayIcon::ActivationReason, std::false_type>,
        // method 'showMainWindow'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'quitApplication'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void SystemTrayIcon::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SystemTrayIcon *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->iconActivated((*reinterpret_cast< std::add_pointer_t<QSystemTrayIcon::ActivationReason>>(_a[1]))); break;
        case 1: _t->showMainWindow(); break;
        case 2: _t->quitApplication(); break;
        default: ;
        }
    }
}

const QMetaObject *SystemTrayIcon::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SystemTrayIcon::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SystemTrayIcon.stringdata0))
        return static_cast<void*>(this);
    return QSystemTrayIcon::qt_metacast(_clname);
}

int SystemTrayIcon::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSystemTrayIcon::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
