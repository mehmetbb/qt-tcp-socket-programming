/****************************************************************************
** Meta object code from reading C++ file 'mythread.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../serverApp/mythread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mythread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MyThread_t {
    const uint offsetsAndSize[40];
    char stringdata0[231];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_MyThread_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_MyThread_t qt_meta_stringdata_MyThread = {
    {
QT_MOC_LITERAL(0, 8), // "MyThread"
QT_MOC_LITERAL(9, 5), // "error"
QT_MOC_LITERAL(15, 0), // ""
QT_MOC_LITERAL(16, 23), // "QTcpSocket::SocketError"
QT_MOC_LITERAL(40, 11), // "socketError"
QT_MOC_LITERAL(52, 9), // "readyRead"
QT_MOC_LITERAL(62, 12), // "disconnected"
QT_MOC_LITERAL(75, 12), // "displayError"
QT_MOC_LITERAL(88, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(117, 11), // "sendMessage"
QT_MOC_LITERAL(129, 11), // "QTcpSocket*"
QT_MOC_LITERAL(141, 6), // "socket"
QT_MOC_LITERAL(148, 7), // "message"
QT_MOC_LITERAL(156, 14), // "analyzeMessage"
QT_MOC_LITERAL(171, 18), // "checkAuthorization"
QT_MOC_LITERAL(190, 4), // "user"
QT_MOC_LITERAL(195, 4), // "pass"
QT_MOC_LITERAL(200, 12), // "listToString"
QT_MOC_LITERAL(213, 12), // "QString[][5]"
QT_MOC_LITERAL(226, 4) // "list"

    },
    "MyThread\0error\0\0QTcpSocket::SocketError\0"
    "socketError\0readyRead\0disconnected\0"
    "displayError\0QAbstractSocket::SocketError\0"
    "sendMessage\0QTcpSocket*\0socket\0message\0"
    "analyzeMessage\0checkAuthorization\0"
    "user\0pass\0listToString\0QString[][5]\0"
    "list"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MyThread[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   62,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    0,   65,    2, 0x0a,    3 /* Public */,
       6,    0,   66,    2, 0x0a,    4 /* Public */,
       7,    1,   67,    2, 0x0a,    5 /* Public */,
       9,    2,   70,    2, 0x0a,    7 /* Public */,
      13,    1,   75,    2, 0x0a,   10 /* Public */,
      14,    2,   78,    2, 0x0a,   12 /* Public */,
      17,    1,   83,    2, 0x0a,   15 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    4,
    QMetaType::Void, 0x80000000 | 10, QMetaType::QString,   11,   12,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   15,   16,
    QMetaType::QString, 0x80000000 | 18,   19,

       0        // eod
};

void MyThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MyThread *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->error((*reinterpret_cast< std::add_pointer_t<QTcpSocket::SocketError>>(_a[1]))); break;
        case 1: _t->readyRead(); break;
        case 2: _t->disconnected(); break;
        case 3: _t->displayError((*reinterpret_cast< std::add_pointer_t<QAbstractSocket::SocketError>>(_a[1]))); break;
        case 4: _t->sendMessage((*reinterpret_cast< std::add_pointer_t<QTcpSocket*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 5: _t->analyzeMessage((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->checkAuthorization((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 7: { QString _r = _t->listToString((*reinterpret_cast< std::add_pointer_t<QString[][5]>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
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
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QTcpSocket* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MyThread::*)(QTcpSocket::SocketError );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyThread::error)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject MyThread::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_meta_stringdata_MyThread.offsetsAndSize,
    qt_meta_data_MyThread,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_MyThread_t
, QtPrivate::TypeAndForceComplete<MyThread, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QTcpSocket::SocketError, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QAbstractSocket::SocketError, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QTcpSocket *, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>


>,
    nullptr
} };


const QMetaObject *MyThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MyThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MyThread.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int MyThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void MyThread::error(QTcpSocket::SocketError _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
