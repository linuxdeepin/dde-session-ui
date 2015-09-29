/****************************************************************************
** Meta object code from reading C++ file 'sessionmanagement.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../sessionmanagement.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sessionmanagement.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_SessionManagement_t {
    QByteArrayData data[18];
    char stringdata[197];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SessionManagement_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SessionManagement_t qt_meta_stringdata_SessionManagement = {
    {
QT_MOC_LITERAL(0, 0, 17), // "SessionManagement"
QT_MOC_LITERAL(1, 18, 13), // "DirectKeyLeft"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 14), // "DirectKeyRight"
QT_MOC_LITERAL(4, 48, 10), // "pressEnter"
QT_MOC_LITERAL(5, 59, 18), // "setBackgroundImage"
QT_MOC_LITERAL(6, 78, 8), // "imageUrl"
QT_MOC_LITERAL(7, 87, 13), // "keyPressEvent"
QT_MOC_LITERAL(8, 101, 10), // "QKeyEvent*"
QT_MOC_LITERAL(9, 112, 1), // "e"
QT_MOC_LITERAL(10, 114, 17), // "mouseReleaseEvent"
QT_MOC_LITERAL(11, 132, 12), // "QMouseEvent*"
QT_MOC_LITERAL(12, 145, 7), // "setMode"
QT_MOC_LITERAL(13, 153, 4), // "mode"
QT_MOC_LITERAL(14, 158, 11), // "powerAction"
QT_MOC_LITERAL(15, 170, 6), // "action"
QT_MOC_LITERAL(16, 177, 11), // "initConnect"
QT_MOC_LITERAL(17, 189, 7) // "testing"

    },
    "SessionManagement\0DirectKeyLeft\0\0"
    "DirectKeyRight\0pressEnter\0setBackgroundImage\0"
    "imageUrl\0keyPressEvent\0QKeyEvent*\0e\0"
    "mouseReleaseEvent\0QMouseEvent*\0setMode\0"
    "mode\0powerAction\0action\0initConnect\0"
    "testing"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SessionManagement[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x06 /* Public */,
       3,    0,   65,    2, 0x06 /* Public */,
       4,    0,   66,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   67,    2, 0x0a /* Public */,
       7,    1,   70,    2, 0x0a /* Public */,
      10,    1,   73,    2, 0x0a /* Public */,
      12,    1,   76,    2, 0x0a /* Public */,
      14,    1,   79,    2, 0x0a /* Public */,
      16,    0,   82,    2, 0x0a /* Public */,
      17,    0,   83,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, 0x80000000 | 11,    9,
    QMetaType::Void, QMetaType::Int,   13,
    QMetaType::Void, QMetaType::QString,   15,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SessionManagement::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SessionManagement *_t = static_cast<SessionManagement *>(_o);
        switch (_id) {
        case 0: _t->DirectKeyLeft(); break;
        case 1: _t->DirectKeyRight(); break;
        case 2: _t->pressEnter(); break;
        case 3: _t->setBackgroundImage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->keyPressEvent((*reinterpret_cast< QKeyEvent*(*)>(_a[1]))); break;
        case 5: _t->mouseReleaseEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 6: _t->setMode((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->powerAction((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->initConnect(); break;
        case 9: _t->testing(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (SessionManagement::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SessionManagement::DirectKeyLeft)) {
                *result = 0;
            }
        }
        {
            typedef void (SessionManagement::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SessionManagement::DirectKeyRight)) {
                *result = 1;
            }
        }
        {
            typedef void (SessionManagement::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SessionManagement::pressEnter)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject SessionManagement::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_SessionManagement.data,
      qt_meta_data_SessionManagement,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *SessionManagement::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SessionManagement::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_SessionManagement.stringdata))
        return static_cast<void*>(const_cast< SessionManagement*>(this));
    return QFrame::qt_metacast(_clname);
}

int SessionManagement::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void SessionManagement::DirectKeyLeft()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void SessionManagement::DirectKeyRight()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void SessionManagement::pressEnter()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
