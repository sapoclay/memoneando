/****************************************************************************
** Meta object code from reading C++ file 'memecanvas.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/memecanvas.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'memecanvas.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_MemeCanvas_t {
    uint offsetsAndSizes[34];
    char stringdata0[11];
    char stringdata1[14];
    char stringdata2[1];
    char stringdata3[11];
    char stringdata4[6];
    char stringdata5[23];
    char stringdata6[18];
    char stringdata7[7];
    char stringdata8[14];
    char stringdata9[31];
    char stringdata10[22];
    char stringdata11[23];
    char stringdata12[28];
    char stringdata13[26];
    char stringdata14[25];
    char stringdata15[12];
    char stringdata16[19];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_MemeCanvas_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_MemeCanvas_t qt_meta_stringdata_MemeCanvas = {
    {
        QT_MOC_LITERAL(0, 10),  // "MemeCanvas"
        QT_MOC_LITERAL(11, 13),  // "layerSelected"
        QT_MOC_LITERAL(25, 0),  // ""
        QT_MOC_LITERAL(26, 10),  // "LayerItem*"
        QT_MOC_LITERAL(37, 5),  // "layer"
        QT_MOC_LITERAL(43, 22),  // "layersSelectionChanged"
        QT_MOC_LITERAL(66, 17),  // "QList<LayerItem*>"
        QT_MOC_LITERAL(84, 6),  // "layers"
        QT_MOC_LITERAL(91, 13),  // "layersChanged"
        QT_MOC_LITERAL(105, 30),  // "backgroundColorChangeRequested"
        QT_MOC_LITERAL(136, 21),  // "addTextLayerRequested"
        QT_MOC_LITERAL(158, 22),  // "addImageLayerRequested"
        QT_MOC_LITERAL(181, 27),  // "searchOnlineImagesRequested"
        QT_MOC_LITERAL(209, 25),  // "removeBackgroundRequested"
        QT_MOC_LITERAL(235, 24),  // "textColorChangeRequested"
        QT_MOC_LITERAL(260, 11),  // "selectLayer"
        QT_MOC_LITERAL(272, 18)   // "onSelectionChanged"
    },
    "MemeCanvas",
    "layerSelected",
    "",
    "LayerItem*",
    "layer",
    "layersSelectionChanged",
    "QList<LayerItem*>",
    "layers",
    "layersChanged",
    "backgroundColorChangeRequested",
    "addTextLayerRequested",
    "addImageLayerRequested",
    "searchOnlineImagesRequested",
    "removeBackgroundRequested",
    "textColorChangeRequested",
    "selectLayer",
    "onSelectionChanged"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_MemeCanvas[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   80,    2, 0x06,    1 /* Public */,
       5,    1,   83,    2, 0x06,    3 /* Public */,
       8,    0,   86,    2, 0x06,    5 /* Public */,
       9,    0,   87,    2, 0x06,    6 /* Public */,
      10,    0,   88,    2, 0x06,    7 /* Public */,
      11,    0,   89,    2, 0x06,    8 /* Public */,
      12,    0,   90,    2, 0x06,    9 /* Public */,
      13,    0,   91,    2, 0x06,   10 /* Public */,
      14,    0,   92,    2, 0x06,   11 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      15,    1,   93,    2, 0x0a,   12 /* Public */,
      16,    0,   96,    2, 0x08,   14 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject MemeCanvas::staticMetaObject = { {
    QMetaObject::SuperData::link<QGraphicsView::staticMetaObject>(),
    qt_meta_stringdata_MemeCanvas.offsetsAndSizes,
    qt_meta_data_MemeCanvas,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_MemeCanvas_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MemeCanvas, std::true_type>,
        // method 'layerSelected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<LayerItem *, std::false_type>,
        // method 'layersSelectionChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QList<LayerItem*> &, std::false_type>,
        // method 'layersChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'backgroundColorChangeRequested'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'addTextLayerRequested'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'addImageLayerRequested'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'searchOnlineImagesRequested'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'removeBackgroundRequested'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'textColorChangeRequested'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'selectLayer'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<LayerItem *, std::false_type>,
        // method 'onSelectionChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void MemeCanvas::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MemeCanvas *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->layerSelected((*reinterpret_cast< std::add_pointer_t<LayerItem*>>(_a[1]))); break;
        case 1: _t->layersSelectionChanged((*reinterpret_cast< std::add_pointer_t<QList<LayerItem*>>>(_a[1]))); break;
        case 2: _t->layersChanged(); break;
        case 3: _t->backgroundColorChangeRequested(); break;
        case 4: _t->addTextLayerRequested(); break;
        case 5: _t->addImageLayerRequested(); break;
        case 6: _t->searchOnlineImagesRequested(); break;
        case 7: _t->removeBackgroundRequested(); break;
        case 8: _t->textColorChangeRequested(); break;
        case 9: _t->selectLayer((*reinterpret_cast< std::add_pointer_t<LayerItem*>>(_a[1]))); break;
        case 10: _t->onSelectionChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MemeCanvas::*)(LayerItem * );
            if (_t _q_method = &MemeCanvas::layerSelected; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MemeCanvas::*)(const QList<LayerItem*> & );
            if (_t _q_method = &MemeCanvas::layersSelectionChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MemeCanvas::*)();
            if (_t _q_method = &MemeCanvas::layersChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (MemeCanvas::*)();
            if (_t _q_method = &MemeCanvas::backgroundColorChangeRequested; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (MemeCanvas::*)();
            if (_t _q_method = &MemeCanvas::addTextLayerRequested; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (MemeCanvas::*)();
            if (_t _q_method = &MemeCanvas::addImageLayerRequested; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (MemeCanvas::*)();
            if (_t _q_method = &MemeCanvas::searchOnlineImagesRequested; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (MemeCanvas::*)();
            if (_t _q_method = &MemeCanvas::removeBackgroundRequested; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (MemeCanvas::*)();
            if (_t _q_method = &MemeCanvas::textColorChangeRequested; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 8;
                return;
            }
        }
    }
}

const QMetaObject *MemeCanvas::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MemeCanvas::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MemeCanvas.stringdata0))
        return static_cast<void*>(this);
    return QGraphicsView::qt_metacast(_clname);
}

int MemeCanvas::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void MemeCanvas::layerSelected(LayerItem * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MemeCanvas::layersSelectionChanged(const QList<LayerItem*> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MemeCanvas::layersChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void MemeCanvas::backgroundColorChangeRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void MemeCanvas::addTextLayerRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void MemeCanvas::addImageLayerRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void MemeCanvas::searchOnlineImagesRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void MemeCanvas::removeBackgroundRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void MemeCanvas::textColorChangeRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
