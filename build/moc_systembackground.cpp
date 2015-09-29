/****************************************************************************
** Meta object code from reading C++ file 'systembackground.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../background/systembackground.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'systembackground.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_SystemBackground_t {
    QByteArrayData data[11];
    char stringdata[135];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SystemBackground_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SystemBackground_t qt_meta_stringdata_SystemBackground = {
    {
QT_MOC_LITERAL(0, 0, 16), // "SystemBackground"
QT_MOC_LITERAL(1, 17, 17), // "backgroundChanged"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 16), // "backgroundPixmap"
QT_MOC_LITERAL(4, 53, 15), // "updateBackgroud"
QT_MOC_LITERAL(5, 69, 23), // "handleBackgroundChanged"
QT_MOC_LITERAL(6, 93, 3), // "key"
QT_MOC_LITERAL(7, 97, 7), // "setBlur"
QT_MOC_LITERAL(8, 105, 6), // "isBlur"
QT_MOC_LITERAL(9, 112, 17), // "setBackgroundSize"
QT_MOC_LITERAL(10, 130, 4) // "size"

    },
    "SystemBackground\0backgroundChanged\0\0"
    "backgroundPixmap\0updateBackgroud\0"
    "handleBackgroundChanged\0key\0setBlur\0"
    "isBlur\0setBackgroundSize\0size"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SystemBackground[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   42,    2, 0x0a /* Public */,
       5,    1,   43,    2, 0x0a /* Public */,
       7,    1,   46,    2, 0x0a /* Public */,
       9,    1,   49,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QPixmap,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, QMetaType::QSize,   10,

       0        // eod
};

void SystemBackground::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SystemBackground *_t = static_cast<SystemBackground *>(_o);
        switch (_id) {
        case 0: _t->backgroundChanged((*reinterpret_cast< const QPixmap(*)>(_a[1]))); break;
        case 1: _t->updateBackgroud(); break;
        case 2: _t->handleBackgroundChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->setBlur((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->setBackgroundSize((*reinterpret_cast< QSize(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (SystemBackground::*_t)(const QPixmap & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SystemBackground::backgroundChanged)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject SystemBackground::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SystemBackground.data,
      qt_meta_data_SystemBackground,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *SystemBackground::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SystemBackground::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_SystemBackground.stringdata))
        return static_cast<void*>(const_cast< SystemBackground*>(this));
    return QObject::qt_metacast(_clname);
}

int SystemBackground::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void SystemBackground::backgroundChanged(const QPixmap & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
