/****************************************************************************
** Meta object code from reading C++ file 'imagebutton.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../User/imagebutton.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'imagebutton.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ImageButton_t {
    QByteArrayData data[11];
    char stringdata[91];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ImageButton_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ImageButton_t qt_meta_stringdata_ImageButton = {
    {
QT_MOC_LITERAL(0, 0, 11), // "ImageButton"
QT_MOC_LITERAL(1, 12, 7), // "clicked"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 3), // "nam"
QT_MOC_LITERAL(4, 25, 6), // "hideIn"
QT_MOC_LITERAL(5, 32, 4), // "name"
QT_MOC_LITERAL(6, 37, 7), // "showOut"
QT_MOC_LITERAL(7, 45, 11), // "sendClicked"
QT_MOC_LITERAL(8, 57, 11), // "setIconSize"
QT_MOC_LITERAL(9, 69, 10), // "AvatarSize"
QT_MOC_LITERAL(10, 80, 10) // "avatarsize"

    },
    "ImageButton\0clicked\0\0nam\0hideIn\0name\0"
    "showOut\0sendClicked\0setIconSize\0"
    "AvatarSize\0avatarsize"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ImageButton[] = {

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
       4,    1,   42,    2, 0x0a /* Public */,
       6,    0,   45,    2, 0x0a /* Public */,
       7,    0,   46,    2, 0x0a /* Public */,
       8,    1,   47,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9,   10,

       0        // eod
};

void ImageButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ImageButton *_t = static_cast<ImageButton *>(_o);
        switch (_id) {
        case 0: _t->clicked((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->hideIn((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->showOut(); break;
        case 3: _t->sendClicked(); break;
        case 4: _t->setIconSize((*reinterpret_cast< const AvatarSize(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ImageButton::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ImageButton::clicked)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject ImageButton::staticMetaObject = {
    { &QPushButton::staticMetaObject, qt_meta_stringdata_ImageButton.data,
      qt_meta_data_ImageButton,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ImageButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ImageButton::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ImageButton.stringdata))
        return static_cast<void*>(const_cast< ImageButton*>(this));
    return QPushButton::qt_metacast(_clname);
}

int ImageButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPushButton::qt_metacall(_c, _id, _a);
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
void ImageButton::clicked(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
