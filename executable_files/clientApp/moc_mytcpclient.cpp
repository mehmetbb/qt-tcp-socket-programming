/****************************************************************************
** Meta object code from reading C++ file 'mytcpclient.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../clientApp/mytcpclient.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mytcpclient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MyTcpClient_t {
    const uint offsetsAndSize[32];
    char stringdata0[183];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_MyTcpClient_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_MyTcpClient_t qt_meta_stringdata_MyTcpClient = {
    {
QT_MOC_LITERAL(0, 11), // "MyTcpClient"
QT_MOC_LITERAL(12, 10), // "newMessage"
QT_MOC_LITERAL(23, 0), // ""
QT_MOC_LITERAL(24, 10), // "readSocket"
QT_MOC_LITERAL(35, 13), // "discardSocket"
QT_MOC_LITERAL(49, 12), // "displayError"
QT_MOC_LITERAL(62, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(91, 11), // "socketError"
QT_MOC_LITERAL(103, 14), // "displayMessage"
QT_MOC_LITERAL(118, 3), // "str"
QT_MOC_LITERAL(122, 11), // "sendMessage"
QT_MOC_LITERAL(134, 5), // "login"
QT_MOC_LITERAL(140, 14), // "analyzeMessage"
QT_MOC_LITERAL(155, 7), // "message"
QT_MOC_LITERAL(163, 10), // "operations"
QT_MOC_LITERAL(174, 8) // "userData"

    },
    "MyTcpClient\0newMessage\0\0readSocket\0"
    "discardSocket\0displayError\0"
    "QAbstractSocket::SocketError\0socketError\0"
    "displayMessage\0str\0sendMessage\0login\0"
    "analyzeMessage\0message\0operations\0"
    "userData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MyTcpClient[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   68,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       3,    0,   71,    2, 0x0a,    3 /* Public */,
       4,    0,   72,    2, 0x0a,    4 /* Public */,
       5,    1,   73,    2, 0x0a,    5 /* Public */,
       8,    1,   76,    2, 0x0a,    7 /* Public */,
      10,    1,   79,    2, 0x0a,    9 /* Public */,
      11,    0,   82,    2, 0x0a,   11 /* Public */,
      12,    1,   83,    2, 0x0a,   12 /* Public */,
      14,    1,   86,    2, 0x0a,   14 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void, QMetaType::QString,   15,

       0        // eod
};

void MyTcpClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MyTcpClient *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->newMessage((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->readSocket(); break;
        case 2: _t->discardSocket(); break;
        case 3: _t->displayError((*reinterpret_cast< std::add_pointer_t<QAbstractSocket::SocketError>>(_a[1]))); break;
        case 4: _t->displayMessage((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 5: _t->sendMessage((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->login(); break;
        case 7: _t->analyzeMessage((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 8: _t->operations((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MyTcpClient::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyTcpClient::newMessage)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject MyTcpClient::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_MyTcpClient.offsetsAndSize,
    qt_meta_data_MyTcpClient,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_MyTcpClient_t
, QtPrivate::TypeAndForceComplete<MyTcpClient, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QAbstractSocket::SocketError, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QString &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QString &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>


>,
    nullptr
} };


const QMetaObject *MyTcpClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MyTcpClient::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MyTcpClient.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int MyTcpClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void MyTcpClient::newMessage(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
