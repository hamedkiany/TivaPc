/****************************************************************************
** Meta object code from reading C++ file 'guipanel.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../guipanel.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'guipanel.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSGUIPanelENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSGUIPanelENDCLASS = QtMocHelpers::stringData(
    "GUIPanel",
    "on_runButton_clicked",
    "",
    "on_rollSlider_valueChanged",
    "value",
    "on_pitchSlider_valueChanged",
    "on_yawDial_valueChanged",
    "on_radioButton_pressed",
    "on_radioButton_2_pressed"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSGUIPanelENDCLASS_t {
    uint offsetsAndSizes[18];
    char stringdata0[9];
    char stringdata1[21];
    char stringdata2[1];
    char stringdata3[27];
    char stringdata4[6];
    char stringdata5[28];
    char stringdata6[24];
    char stringdata7[23];
    char stringdata8[25];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSGUIPanelENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSGUIPanelENDCLASS_t qt_meta_stringdata_CLASSGUIPanelENDCLASS = {
    {
        QT_MOC_LITERAL(0, 8),  // "GUIPanel"
        QT_MOC_LITERAL(9, 20),  // "on_runButton_clicked"
        QT_MOC_LITERAL(30, 0),  // ""
        QT_MOC_LITERAL(31, 26),  // "on_rollSlider_valueChanged"
        QT_MOC_LITERAL(58, 5),  // "value"
        QT_MOC_LITERAL(64, 27),  // "on_pitchSlider_valueChanged"
        QT_MOC_LITERAL(92, 23),  // "on_yawDial_valueChanged"
        QT_MOC_LITERAL(116, 22),  // "on_radioButton_pressed"
        QT_MOC_LITERAL(139, 24)   // "on_radioButton_2_pressed"
    },
    "GUIPanel",
    "on_runButton_clicked",
    "",
    "on_rollSlider_valueChanged",
    "value",
    "on_pitchSlider_valueChanged",
    "on_yawDial_valueChanged",
    "on_radioButton_pressed",
    "on_radioButton_2_pressed"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSGUIPanelENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   50,    2, 0x08,    1 /* Private */,
       3,    1,   51,    2, 0x08,    2 /* Private */,
       5,    1,   54,    2, 0x08,    4 /* Private */,
       6,    1,   57,    2, 0x08,    6 /* Private */,
       7,    0,   60,    2, 0x08,    8 /* Private */,
       8,    0,   61,    2, 0x08,    9 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject GUIPanel::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSGUIPanelENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSGUIPanelENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSGUIPanelENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<GUIPanel, std::true_type>,
        // method 'on_runButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_rollSlider_valueChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_pitchSlider_valueChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_yawDial_valueChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_radioButton_pressed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_radioButton_2_pressed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void GUIPanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GUIPanel *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_runButton_clicked(); break;
        case 1: _t->on_rollSlider_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->on_pitchSlider_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->on_yawDial_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->on_radioButton_pressed(); break;
        case 5: _t->on_radioButton_2_pressed(); break;
        default: ;
        }
    }
}

const QMetaObject *GUIPanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GUIPanel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSGUIPanelENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int GUIPanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
