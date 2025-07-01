/****************************************************************************
** Meta object code from reading C++ file 'imagesearchwidget.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/imagesearchwidget.h"
#include <QtGui/qtextcursor.h>
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'imagesearchwidget.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_ImageSearchWidget_t {
    uint offsetsAndSizes[42];
    char stringdata0[18];
    char stringdata1[14];
    char stringdata2[1];
    char stringdata3[10];
    char stringdata4[16];
    char stringdata5[20];
    char stringdata6[18];
    char stringdata7[25];
    char stringdata8[8];
    char stringdata9[14];
    char stringdata10[6];
    char stringdata11[18];
    char stringdata12[9];
    char stringdata13[21];
    char stringdata14[21];
    char stringdata15[18];
    char stringdata16[7];
    char stringdata17[15];
    char stringdata18[8];
    char stringdata19[9];
    char stringdata20[18];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_ImageSearchWidget_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_ImageSearchWidget_t qt_meta_stringdata_ImageSearchWidget = {
    {
        QT_MOC_LITERAL(0, 17),  // "ImageSearchWidget"
        QT_MOC_LITERAL(18, 13),  // "imageSelected"
        QT_MOC_LITERAL(32, 0),  // ""
        QT_MOC_LITERAL(33, 9),  // "imagePath"
        QT_MOC_LITERAL(43, 15),  // "onSearchClicked"
        QT_MOC_LITERAL(59, 19),  // "onSearchTextChanged"
        QT_MOC_LITERAL(79, 17),  // "onSearchCompleted"
        QT_MOC_LITERAL(97, 24),  // "QList<ImageSearchResult>"
        QT_MOC_LITERAL(122, 7),  // "results"
        QT_MOC_LITERAL(130, 13),  // "onSearchError"
        QT_MOC_LITERAL(144, 5),  // "error"
        QT_MOC_LITERAL(150, 17),  // "onImageDownloaded"
        QT_MOC_LITERAL(168, 8),  // "filePath"
        QT_MOC_LITERAL(177, 20),  // "onImageDownloadError"
        QT_MOC_LITERAL(198, 20),  // "onImageResultClicked"
        QT_MOC_LITERAL(219, 17),  // "ImageSearchResult"
        QT_MOC_LITERAL(237, 6),  // "result"
        QT_MOC_LITERAL(244, 14),  // "onPreviewReady"
        QT_MOC_LITERAL(259, 7),  // "preview"
        QT_MOC_LITERAL(267, 8),  // "imageUrl"
        QT_MOC_LITERAL(276, 17)   // "onProviderChanged"
    },
    "ImageSearchWidget",
    "imageSelected",
    "",
    "imagePath",
    "onSearchClicked",
    "onSearchTextChanged",
    "onSearchCompleted",
    "QList<ImageSearchResult>",
    "results",
    "onSearchError",
    "error",
    "onImageDownloaded",
    "filePath",
    "onImageDownloadError",
    "onImageResultClicked",
    "ImageSearchResult",
    "result",
    "onPreviewReady",
    "preview",
    "imageUrl",
    "onProviderChanged"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_ImageSearchWidget[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   74,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       4,    0,   77,    2, 0x08,    3 /* Private */,
       5,    0,   78,    2, 0x08,    4 /* Private */,
       6,    1,   79,    2, 0x08,    5 /* Private */,
       9,    1,   82,    2, 0x08,    7 /* Private */,
      11,    1,   85,    2, 0x08,    9 /* Private */,
      13,    1,   88,    2, 0x08,   11 /* Private */,
      14,    1,   91,    2, 0x08,   13 /* Private */,
      17,    2,   94,    2, 0x08,   15 /* Private */,
      20,    0,   99,    2, 0x08,   18 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, 0x80000000 | 15,   16,
    QMetaType::Void, QMetaType::QPixmap, QMetaType::QString,   18,   19,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject ImageSearchWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_ImageSearchWidget.offsetsAndSizes,
    qt_meta_data_ImageSearchWidget,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_ImageSearchWidget_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ImageSearchWidget, std::true_type>,
        // method 'imageSelected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onSearchClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSearchTextChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSearchCompleted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QList<ImageSearchResult> &, std::false_type>,
        // method 'onSearchError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onImageDownloaded'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onImageDownloadError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onImageResultClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const ImageSearchResult &, std::false_type>,
        // method 'onPreviewReady'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QPixmap &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onProviderChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void ImageSearchWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ImageSearchWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->imageSelected((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->onSearchClicked(); break;
        case 2: _t->onSearchTextChanged(); break;
        case 3: _t->onSearchCompleted((*reinterpret_cast< std::add_pointer_t<QList<ImageSearchResult>>>(_a[1]))); break;
        case 4: _t->onSearchError((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 5: _t->onImageDownloaded((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->onImageDownloadError((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 7: _t->onImageResultClicked((*reinterpret_cast< std::add_pointer_t<ImageSearchResult>>(_a[1]))); break;
        case 8: _t->onPreviewReady((*reinterpret_cast< std::add_pointer_t<QPixmap>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 9: _t->onProviderChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ImageSearchWidget::*)(const QString & );
            if (_t _q_method = &ImageSearchWidget::imageSelected; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *ImageSearchWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ImageSearchWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ImageSearchWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ImageSearchWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void ImageSearchWidget::imageSelected(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
namespace {
struct qt_meta_stringdata_ImageResultWidget_t {
    uint offsetsAndSizes[10];
    char stringdata0[18];
    char stringdata1[8];
    char stringdata2[1];
    char stringdata3[18];
    char stringdata4[7];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_ImageResultWidget_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_ImageResultWidget_t qt_meta_stringdata_ImageResultWidget = {
    {
        QT_MOC_LITERAL(0, 17),  // "ImageResultWidget"
        QT_MOC_LITERAL(18, 7),  // "clicked"
        QT_MOC_LITERAL(26, 0),  // ""
        QT_MOC_LITERAL(27, 17),  // "ImageSearchResult"
        QT_MOC_LITERAL(45, 6)   // "result"
    },
    "ImageResultWidget",
    "clicked",
    "",
    "ImageSearchResult",
    "result"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_ImageResultWidget[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   20,    2, 0x06,    1 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

Q_CONSTINIT const QMetaObject ImageResultWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_ImageResultWidget.offsetsAndSizes,
    qt_meta_data_ImageResultWidget,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_ImageResultWidget_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ImageResultWidget, std::true_type>,
        // method 'clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const ImageSearchResult &, std::false_type>
    >,
    nullptr
} };

void ImageResultWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ImageResultWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->clicked((*reinterpret_cast< std::add_pointer_t<ImageSearchResult>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ImageResultWidget::*)(const ImageSearchResult & );
            if (_t _q_method = &ImageResultWidget::clicked; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *ImageResultWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ImageResultWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ImageResultWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ImageResultWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void ImageResultWidget::clicked(const ImageSearchResult & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
