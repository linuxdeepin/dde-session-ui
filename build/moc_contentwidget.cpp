/****************************************************************************
** Meta object code from reading C++ file 'contentwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../contentwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'contentwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ShutDownFrame_t {
    QByteArrayData data[12];
    char stringdata[166];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ShutDownFrame_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ShutDownFrame_t qt_meta_stringdata_ShutDownFrame = {
    {
QT_MOC_LITERAL(0, 0, 13), // "ShutDownFrame"
QT_MOC_LITERAL(1, 14, 7), // "keyLeft"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 8), // "keyRight"
QT_MOC_LITERAL(4, 32, 16), // "pressEnterAction"
QT_MOC_LITERAL(5, 49, 20), // "ShutDownFrameActions"
QT_MOC_LITERAL(6, 70, 8), // "buttonId"
QT_MOC_LITERAL(7, 79, 21), // "setPreviousChildFocus"
QT_MOC_LITERAL(8, 101, 17), // "setNextChildFocus"
QT_MOC_LITERAL(9, 119, 11), // "initConnect"
QT_MOC_LITERAL(10, 131, 14), // "ShutDownAction"
QT_MOC_LITERAL(11, 146, 19) // "setButtonGroupMutex"

    },
    "ShutDownFrame\0keyLeft\0\0keyRight\0"
    "pressEnterAction\0ShutDownFrameActions\0"
    "buttonId\0setPreviousChildFocus\0"
    "setNextChildFocus\0initConnect\0"
    "ShutDownAction\0setButtonGroupMutex"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ShutDownFrame[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x06 /* Public */,
       3,    0,   60,    2, 0x06 /* Public */,
       4,    0,   61,    2, 0x06 /* Public */,
       5,    1,   62,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   65,    2, 0x0a /* Public */,
       8,    0,   66,    2, 0x0a /* Public */,
       9,    0,   67,    2, 0x0a /* Public */,
      10,    0,   68,    2, 0x0a /* Public */,
      11,    1,   69,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,

       0        // eod
};

void ShutDownFrame::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ShutDownFrame *_t = static_cast<ShutDownFrame *>(_o);
        switch (_id) {
        case 0: _t->keyLeft(); break;
        case 1: _t->keyRight(); break;
        case 2: _t->pressEnterAction(); break;
        case 3: _t->ShutDownFrameActions((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->setPreviousChildFocus(); break;
        case 5: _t->setNextChildFocus(); break;
        case 6: _t->initConnect(); break;
        case 7: _t->ShutDownAction(); break;
        case 8: _t->setButtonGroupMutex((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ShutDownFrame::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ShutDownFrame::keyLeft)) {
                *result = 0;
            }
        }
        {
            typedef void (ShutDownFrame::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ShutDownFrame::keyRight)) {
                *result = 1;
            }
        }
        {
            typedef void (ShutDownFrame::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ShutDownFrame::pressEnterAction)) {
                *result = 2;
            }
        }
        {
            typedef void (ShutDownFrame::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ShutDownFrame::ShutDownFrameActions)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject ShutDownFrame::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_ShutDownFrame.data,
      qt_meta_data_ShutDownFrame,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ShutDownFrame::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ShutDownFrame::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ShutDownFrame.stringdata))
        return static_cast<void*>(const_cast< ShutDownFrame*>(this));
    return QFrame::qt_metacast(_clname);
}

int ShutDownFrame::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void ShutDownFrame::keyLeft()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void ShutDownFrame::keyRight()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void ShutDownFrame::pressEnterAction()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void ShutDownFrame::ShutDownFrameActions(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
struct qt_meta_stringdata_LoginFrame_t {
    QByteArrayData data[4];
    char stringdata[29];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LoginFrame_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LoginFrame_t qt_meta_stringdata_LoginFrame = {
    {
QT_MOC_LITERAL(0, 0, 10), // "LoginFrame"
QT_MOC_LITERAL(1, 11, 7), // "keyLeft"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 8) // "keyRight"

    },
    "LoginFrame\0keyLeft\0\0keyRight"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LoginFrame[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x06 /* Public */,
       3,    0,   25,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void LoginFrame::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        LoginFrame *_t = static_cast<LoginFrame *>(_o);
        switch (_id) {
        case 0: _t->keyLeft(); break;
        case 1: _t->keyRight(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (LoginFrame::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LoginFrame::keyLeft)) {
                *result = 0;
            }
        }
        {
            typedef void (LoginFrame::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LoginFrame::keyRight)) {
                *result = 1;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject LoginFrame::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_LoginFrame.data,
      qt_meta_data_LoginFrame,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *LoginFrame::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LoginFrame::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_LoginFrame.stringdata))
        return static_cast<void*>(const_cast< LoginFrame*>(this));
    return QFrame::qt_metacast(_clname);
}

int LoginFrame::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void LoginFrame::keyLeft()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void LoginFrame::keyRight()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}
struct qt_meta_stringdata_QuickShutDownFrame_t {
    QByteArrayData data[7];
    char stringdata[94];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QuickShutDownFrame_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QuickShutDownFrame_t qt_meta_stringdata_QuickShutDownFrame = {
    {
QT_MOC_LITERAL(0, 0, 18), // "QuickShutDownFrame"
QT_MOC_LITERAL(1, 19, 7), // "keyLeft"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 8), // "keyRight"
QT_MOC_LITERAL(4, 37, 16), // "pressEnterAction"
QT_MOC_LITERAL(5, 54, 21), // "setPreviousChildFocus"
QT_MOC_LITERAL(6, 76, 17) // "setNextChildFocus"

    },
    "QuickShutDownFrame\0keyLeft\0\0keyRight\0"
    "pressEnterAction\0setPreviousChildFocus\0"
    "setNextChildFocus"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QuickShutDownFrame[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,
       3,    0,   40,    2, 0x06 /* Public */,
       4,    0,   41,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   42,    2, 0x0a /* Public */,
       6,    0,   43,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void QuickShutDownFrame::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QuickShutDownFrame *_t = static_cast<QuickShutDownFrame *>(_o);
        switch (_id) {
        case 0: _t->keyLeft(); break;
        case 1: _t->keyRight(); break;
        case 2: _t->pressEnterAction(); break;
        case 3: _t->setPreviousChildFocus(); break;
        case 4: _t->setNextChildFocus(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QuickShutDownFrame::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QuickShutDownFrame::keyLeft)) {
                *result = 0;
            }
        }
        {
            typedef void (QuickShutDownFrame::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QuickShutDownFrame::keyRight)) {
                *result = 1;
            }
        }
        {
            typedef void (QuickShutDownFrame::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QuickShutDownFrame::pressEnterAction)) {
                *result = 2;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject QuickShutDownFrame::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_QuickShutDownFrame.data,
      qt_meta_data_QuickShutDownFrame,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QuickShutDownFrame::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QuickShutDownFrame::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QuickShutDownFrame.stringdata))
        return static_cast<void*>(const_cast< QuickShutDownFrame*>(this));
    return QFrame::qt_metacast(_clname);
}

int QuickShutDownFrame::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
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
void QuickShutDownFrame::keyLeft()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void QuickShutDownFrame::keyRight()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void QuickShutDownFrame::pressEnterAction()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
