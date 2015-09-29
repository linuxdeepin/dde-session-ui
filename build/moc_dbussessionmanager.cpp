/****************************************************************************
** Meta object code from reading C++ file 'dbussessionmanager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../DBus/dbussessionmanager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dbussessionmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_DBusSessionManagerInterface_t {
    QByteArrayData data[27];
    char stringdata[337];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DBusSessionManagerInterface_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DBusSessionManagerInterface_t qt_meta_stringdata_DBusSessionManagerInterface = {
    {
QT_MOC_LITERAL(0, 0, 27), // "DBusSessionManagerInterface"
QT_MOC_LITERAL(1, 28, 12), // "CanHibernate"
QT_MOC_LITERAL(2, 41, 23), // "QDBusPendingReply<bool>"
QT_MOC_LITERAL(3, 65, 0), // ""
QT_MOC_LITERAL(4, 66, 9), // "CanLogout"
QT_MOC_LITERAL(5, 76, 9), // "CanReboot"
QT_MOC_LITERAL(6, 86, 11), // "CanShutdown"
QT_MOC_LITERAL(7, 98, 10), // "CanSuspend"
QT_MOC_LITERAL(8, 109, 11), // "ForceLogout"
QT_MOC_LITERAL(9, 121, 19), // "QDBusPendingReply<>"
QT_MOC_LITERAL(10, 141, 11), // "ForceReboot"
QT_MOC_LITERAL(11, 153, 13), // "ForceShutdown"
QT_MOC_LITERAL(12, 167, 6), // "Logout"
QT_MOC_LITERAL(13, 174, 14), // "PowerOffChoose"
QT_MOC_LITERAL(14, 189, 6), // "Reboot"
QT_MOC_LITERAL(15, 196, 8), // "Register"
QT_MOC_LITERAL(16, 205, 3), // "in0"
QT_MOC_LITERAL(17, 209, 16), // "RequestHibernate"
QT_MOC_LITERAL(18, 226, 11), // "RequestLock"
QT_MOC_LITERAL(19, 238, 13), // "RequestLogout"
QT_MOC_LITERAL(20, 252, 13), // "RequestReboot"
QT_MOC_LITERAL(21, 266, 15), // "RequestShutdown"
QT_MOC_LITERAL(22, 282, 14), // "RequestSuspend"
QT_MOC_LITERAL(23, 297, 13), // "ShowGuideOnce"
QT_MOC_LITERAL(24, 311, 8), // "Shutdown"
QT_MOC_LITERAL(25, 320, 10), // "CurrentUid"
QT_MOC_LITERAL(26, 331, 5) // "Stage"

    },
    "DBusSessionManagerInterface\0CanHibernate\0"
    "QDBusPendingReply<bool>\0\0CanLogout\0"
    "CanReboot\0CanShutdown\0CanSuspend\0"
    "ForceLogout\0QDBusPendingReply<>\0"
    "ForceReboot\0ForceShutdown\0Logout\0"
    "PowerOffChoose\0Reboot\0Register\0in0\0"
    "RequestHibernate\0RequestLock\0RequestLogout\0"
    "RequestReboot\0RequestShutdown\0"
    "RequestSuspend\0ShowGuideOnce\0Shutdown\0"
    "CurrentUid\0Stage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DBusSessionManagerInterface[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       2,  136, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  114,    3, 0x0a /* Public */,
       4,    0,  115,    3, 0x0a /* Public */,
       5,    0,  116,    3, 0x0a /* Public */,
       6,    0,  117,    3, 0x0a /* Public */,
       7,    0,  118,    3, 0x0a /* Public */,
       8,    0,  119,    3, 0x0a /* Public */,
      10,    0,  120,    3, 0x0a /* Public */,
      11,    0,  121,    3, 0x0a /* Public */,
      12,    0,  122,    3, 0x0a /* Public */,
      13,    0,  123,    3, 0x0a /* Public */,
      14,    0,  124,    3, 0x0a /* Public */,
      15,    1,  125,    3, 0x0a /* Public */,
      17,    0,  128,    3, 0x0a /* Public */,
      18,    0,  129,    3, 0x0a /* Public */,
      19,    0,  130,    3, 0x0a /* Public */,
      20,    0,  131,    3, 0x0a /* Public */,
      21,    0,  132,    3, 0x0a /* Public */,
      22,    0,  133,    3, 0x0a /* Public */,
      23,    0,  134,    3, 0x0a /* Public */,
      24,    0,  135,    3, 0x0a /* Public */,

 // slots: parameters
    0x80000000 | 2,
    0x80000000 | 2,
    0x80000000 | 2,
    0x80000000 | 2,
    0x80000000 | 2,
    0x80000000 | 9,
    0x80000000 | 9,
    0x80000000 | 9,
    0x80000000 | 9,
    0x80000000 | 9,
    0x80000000 | 9,
    0x80000000 | 2, QMetaType::QString,   16,
    0x80000000 | 9,
    0x80000000 | 9,
    0x80000000 | 9,
    0x80000000 | 9,
    0x80000000 | 9,
    0x80000000 | 9,
    0x80000000 | 2,
    0x80000000 | 9,

 // properties: name, type, flags
      25, QMetaType::QString, 0x00095001,
      26, QMetaType::Int, 0x00095001,

       0        // eod
};

void DBusSessionManagerInterface::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DBusSessionManagerInterface *_t = static_cast<DBusSessionManagerInterface *>(_o);
        switch (_id) {
        case 0: { QDBusPendingReply<bool> _r = _t->CanHibernate();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<bool>*>(_a[0]) = _r; }  break;
        case 1: { QDBusPendingReply<bool> _r = _t->CanLogout();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<bool>*>(_a[0]) = _r; }  break;
        case 2: { QDBusPendingReply<bool> _r = _t->CanReboot();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<bool>*>(_a[0]) = _r; }  break;
        case 3: { QDBusPendingReply<bool> _r = _t->CanShutdown();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<bool>*>(_a[0]) = _r; }  break;
        case 4: { QDBusPendingReply<bool> _r = _t->CanSuspend();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<bool>*>(_a[0]) = _r; }  break;
        case 5: { QDBusPendingReply<> _r = _t->ForceLogout();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 6: { QDBusPendingReply<> _r = _t->ForceReboot();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 7: { QDBusPendingReply<> _r = _t->ForceShutdown();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 8: { QDBusPendingReply<> _r = _t->Logout();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 9: { QDBusPendingReply<> _r = _t->PowerOffChoose();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 10: { QDBusPendingReply<> _r = _t->Reboot();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 11: { QDBusPendingReply<bool> _r = _t->Register((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<bool>*>(_a[0]) = _r; }  break;
        case 12: { QDBusPendingReply<> _r = _t->RequestHibernate();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 13: { QDBusPendingReply<> _r = _t->RequestLock();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 14: { QDBusPendingReply<> _r = _t->RequestLogout();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 15: { QDBusPendingReply<> _r = _t->RequestReboot();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 16: { QDBusPendingReply<> _r = _t->RequestShutdown();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 17: { QDBusPendingReply<> _r = _t->RequestSuspend();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 18: { QDBusPendingReply<bool> _r = _t->ShowGuideOnce();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<bool>*>(_a[0]) = _r; }  break;
        case 19: { QDBusPendingReply<> _r = _t->Shutdown();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObject DBusSessionManagerInterface::staticMetaObject = {
    { &QDBusAbstractInterface::staticMetaObject, qt_meta_stringdata_DBusSessionManagerInterface.data,
      qt_meta_data_DBusSessionManagerInterface,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *DBusSessionManagerInterface::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DBusSessionManagerInterface::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_DBusSessionManagerInterface.stringdata))
        return static_cast<void*>(const_cast< DBusSessionManagerInterface*>(this));
    return QDBusAbstractInterface::qt_metacast(_clname);
}

int DBusSessionManagerInterface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractInterface::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 20)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 20;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = currentUid(); break;
        case 1: *reinterpret_cast< int*>(_v) = stage(); break;
        default: break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::WriteProperty) {
        _id -= 2;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
