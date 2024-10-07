/****************************************************************************
** Meta object code from reading C++ file 'tiva_remotelink.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../tiva_remotelink.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tiva_remotelink.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSTivaRemoteLinkENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSTivaRemoteLinkENDCLASS = QtMocHelpers::stringData(
    "TivaRemoteLink",
    "statusChanged",
    "",
    "status",
    "message",
    "messageReceived",
    "uint8_t",
    "message_type",
    "parameter",
    "startRemoteLink",
    "puerto",
    "sendMessage",
    "int32_t",
    "parameter_size",
    "processIncommingSerialData",
    "TivaStatus",
    "TivaConnected",
    "TivaDisconnected",
    "OpenPortError",
    "BaudRateError",
    "DataBitError",
    "ParityError",
    "StopError",
    "FlowControlError",
    "UnexpectedPacketError",
    "FragmentedPacketError",
    "CRCorStuffError",
    "ReceivedDataError",
    "TivaIsDisconnected"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSTivaRemoteLinkENDCLASS_t {
    uint offsetsAndSizes[58];
    char stringdata0[15];
    char stringdata1[14];
    char stringdata2[1];
    char stringdata3[7];
    char stringdata4[8];
    char stringdata5[16];
    char stringdata6[8];
    char stringdata7[13];
    char stringdata8[10];
    char stringdata9[16];
    char stringdata10[7];
    char stringdata11[12];
    char stringdata12[8];
    char stringdata13[15];
    char stringdata14[27];
    char stringdata15[11];
    char stringdata16[14];
    char stringdata17[17];
    char stringdata18[14];
    char stringdata19[14];
    char stringdata20[13];
    char stringdata21[12];
    char stringdata22[10];
    char stringdata23[17];
    char stringdata24[22];
    char stringdata25[22];
    char stringdata26[16];
    char stringdata27[18];
    char stringdata28[19];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSTivaRemoteLinkENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSTivaRemoteLinkENDCLASS_t qt_meta_stringdata_CLASSTivaRemoteLinkENDCLASS = {
    {
        QT_MOC_LITERAL(0, 14),  // "TivaRemoteLink"
        QT_MOC_LITERAL(15, 13),  // "statusChanged"
        QT_MOC_LITERAL(29, 0),  // ""
        QT_MOC_LITERAL(30, 6),  // "status"
        QT_MOC_LITERAL(37, 7),  // "message"
        QT_MOC_LITERAL(45, 15),  // "messageReceived"
        QT_MOC_LITERAL(61, 7),  // "uint8_t"
        QT_MOC_LITERAL(69, 12),  // "message_type"
        QT_MOC_LITERAL(82, 9),  // "parameter"
        QT_MOC_LITERAL(92, 15),  // "startRemoteLink"
        QT_MOC_LITERAL(108, 6),  // "puerto"
        QT_MOC_LITERAL(115, 11),  // "sendMessage"
        QT_MOC_LITERAL(127, 7),  // "int32_t"
        QT_MOC_LITERAL(135, 14),  // "parameter_size"
        QT_MOC_LITERAL(150, 26),  // "processIncommingSerialData"
        QT_MOC_LITERAL(177, 10),  // "TivaStatus"
        QT_MOC_LITERAL(188, 13),  // "TivaConnected"
        QT_MOC_LITERAL(202, 16),  // "TivaDisconnected"
        QT_MOC_LITERAL(219, 13),  // "OpenPortError"
        QT_MOC_LITERAL(233, 13),  // "BaudRateError"
        QT_MOC_LITERAL(247, 12),  // "DataBitError"
        QT_MOC_LITERAL(260, 11),  // "ParityError"
        QT_MOC_LITERAL(272, 9),  // "StopError"
        QT_MOC_LITERAL(282, 16),  // "FlowControlError"
        QT_MOC_LITERAL(299, 21),  // "UnexpectedPacketError"
        QT_MOC_LITERAL(321, 21),  // "FragmentedPacketError"
        QT_MOC_LITERAL(343, 15),  // "CRCorStuffError"
        QT_MOC_LITERAL(359, 17),  // "ReceivedDataError"
        QT_MOC_LITERAL(377, 18)   // "TivaIsDisconnected"
    },
    "TivaRemoteLink",
    "statusChanged",
    "",
    "status",
    "message",
    "messageReceived",
    "uint8_t",
    "message_type",
    "parameter",
    "startRemoteLink",
    "puerto",
    "sendMessage",
    "int32_t",
    "parameter_size",
    "processIncommingSerialData",
    "TivaStatus",
    "TivaConnected",
    "TivaDisconnected",
    "OpenPortError",
    "BaudRateError",
    "DataBitError",
    "ParityError",
    "StopError",
    "FlowControlError",
    "UnexpectedPacketError",
    "FragmentedPacketError",
    "CRCorStuffError",
    "ReceivedDataError",
    "TivaIsDisconnected"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSTivaRemoteLinkENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       1,   76, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,   50,    2, 0x06,    2 /* Public */,
       5,    2,   55,    2, 0x06,    5 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       9,    1,   60,    2, 0x0a,    8 /* Public */,
      11,    3,   63,    2, 0x0a,   10 /* Public */,
      11,    2,   70,    2, 0x0a,   14 /* Public */,
      14,    0,   75,    2, 0x08,   17 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    3,    4,
    QMetaType::Void, 0x80000000 | 6, QMetaType::QByteArray,    7,    8,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, 0x80000000 | 6, QMetaType::VoidStar, 0x80000000 | 12,    7,    8,   13,
    QMetaType::Void, 0x80000000 | 6, QMetaType::QByteArray,    7,    8,
    QMetaType::Void,

 // enums: name, alias, flags, count, data
      15,   15, 0x0,   13,   81,

 // enum data: key, value
      16, uint(TivaRemoteLink::TivaConnected),
      17, uint(TivaRemoteLink::TivaDisconnected),
      18, uint(TivaRemoteLink::OpenPortError),
      19, uint(TivaRemoteLink::BaudRateError),
      20, uint(TivaRemoteLink::DataBitError),
      21, uint(TivaRemoteLink::ParityError),
      22, uint(TivaRemoteLink::StopError),
      23, uint(TivaRemoteLink::FlowControlError),
      24, uint(TivaRemoteLink::UnexpectedPacketError),
      25, uint(TivaRemoteLink::FragmentedPacketError),
      26, uint(TivaRemoteLink::CRCorStuffError),
      27, uint(TivaRemoteLink::ReceivedDataError),
      28, uint(TivaRemoteLink::TivaIsDisconnected),

       0        // eod
};

Q_CONSTINIT const QMetaObject TivaRemoteLink::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSTivaRemoteLinkENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSTivaRemoteLinkENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSTivaRemoteLinkENDCLASS_t,
        // enum 'TivaStatus'
        QtPrivate::TypeAndForceComplete<TivaRemoteLink::TivaStatus, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<TivaRemoteLink, std::true_type>,
        // method 'statusChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'messageReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<uint8_t, std::false_type>,
        QtPrivate::TypeAndForceComplete<QByteArray, std::false_type>,
        // method 'startRemoteLink'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'sendMessage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<uint8_t, std::false_type>,
        QtPrivate::TypeAndForceComplete<void *, std::false_type>,
        QtPrivate::TypeAndForceComplete<int32_t, std::false_type>,
        // method 'sendMessage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<uint8_t, std::false_type>,
        QtPrivate::TypeAndForceComplete<QByteArray, std::false_type>,
        // method 'processIncommingSerialData'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void TivaRemoteLink::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TivaRemoteLink *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->statusChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 1: _t->messageReceived((*reinterpret_cast< std::add_pointer_t<uint8_t>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[2]))); break;
        case 2: _t->startRemoteLink((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->sendMessage((*reinterpret_cast< std::add_pointer_t<uint8_t>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<void*>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int32_t>>(_a[3]))); break;
        case 4: _t->sendMessage((*reinterpret_cast< std::add_pointer_t<uint8_t>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[2]))); break;
        case 5: _t->processIncommingSerialData(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TivaRemoteLink::*)(int , QString );
            if (_t _q_method = &TivaRemoteLink::statusChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (TivaRemoteLink::*)(uint8_t , QByteArray );
            if (_t _q_method = &TivaRemoteLink::messageReceived; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject *TivaRemoteLink::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TivaRemoteLink::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSTivaRemoteLinkENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int TivaRemoteLink::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void TivaRemoteLink::statusChanged(int _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TivaRemoteLink::messageReceived(uint8_t _t1, QByteArray _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
