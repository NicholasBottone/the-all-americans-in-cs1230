/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/mainwindow.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.5.2. It"
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
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSMainWindowENDCLASS = QtMocHelpers::stringData(
    "MainWindow",
    "onUploadFile",
    "",
    "onSaveImage",
    "onBulkRender",
    "onValChangexySlider",
    "newValue",
    "onValChangexyBox",
    "onValChangexzSlider",
    "onValChangexzBox",
    "onValChangexwSlider",
    "onValChangexwBox",
    "onValChangeyzSlider",
    "onValChangeyzBox",
    "onValChangeywSlider",
    "onValChangeywBox",
    "onValChangezwSlider",
    "onValChangezwBox",
    "onValChangeMaxTimeSlider",
    "onValChangeMaxTimeBox",
    "onRotateNegative",
    "updateXySlider",
    "updateXzSlider",
    "updateXwSlider",
    "updateYzSlider",
    "updateYwSlider",
    "updateZwSlider",
    "updateRotationSlider",
    "onValChangeWSlider",
    "onValChangeWBox",
    "updateTimeValue",
    "onValChangezSlider",
    "onValChangezBox",
    "updateZSlider"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {
    uint offsetsAndSizes[68];
    char stringdata0[11];
    char stringdata1[13];
    char stringdata2[1];
    char stringdata3[12];
    char stringdata4[13];
    char stringdata5[20];
    char stringdata6[9];
    char stringdata7[17];
    char stringdata8[20];
    char stringdata9[17];
    char stringdata10[20];
    char stringdata11[17];
    char stringdata12[20];
    char stringdata13[17];
    char stringdata14[20];
    char stringdata15[17];
    char stringdata16[20];
    char stringdata17[17];
    char stringdata18[25];
    char stringdata19[22];
    char stringdata20[17];
    char stringdata21[15];
    char stringdata22[15];
    char stringdata23[15];
    char stringdata24[15];
    char stringdata25[15];
    char stringdata26[15];
    char stringdata27[21];
    char stringdata28[19];
    char stringdata29[16];
    char stringdata30[16];
    char stringdata31[19];
    char stringdata32[16];
    char stringdata33[14];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSMainWindowENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSMainWindowENDCLASS_t qt_meta_stringdata_CLASSMainWindowENDCLASS = {
    {
        QT_MOC_LITERAL(0, 10),  // "MainWindow"
        QT_MOC_LITERAL(11, 12),  // "onUploadFile"
        QT_MOC_LITERAL(24, 0),  // ""
        QT_MOC_LITERAL(25, 11),  // "onSaveImage"
        QT_MOC_LITERAL(37, 12),  // "onBulkRender"
        QT_MOC_LITERAL(50, 19),  // "onValChangexySlider"
        QT_MOC_LITERAL(70, 8),  // "newValue"
        QT_MOC_LITERAL(79, 16),  // "onValChangexyBox"
        QT_MOC_LITERAL(96, 19),  // "onValChangexzSlider"
        QT_MOC_LITERAL(116, 16),  // "onValChangexzBox"
        QT_MOC_LITERAL(133, 19),  // "onValChangexwSlider"
        QT_MOC_LITERAL(153, 16),  // "onValChangexwBox"
        QT_MOC_LITERAL(170, 19),  // "onValChangeyzSlider"
        QT_MOC_LITERAL(190, 16),  // "onValChangeyzBox"
        QT_MOC_LITERAL(207, 19),  // "onValChangeywSlider"
        QT_MOC_LITERAL(227, 16),  // "onValChangeywBox"
        QT_MOC_LITERAL(244, 19),  // "onValChangezwSlider"
        QT_MOC_LITERAL(264, 16),  // "onValChangezwBox"
        QT_MOC_LITERAL(281, 24),  // "onValChangeMaxTimeSlider"
        QT_MOC_LITERAL(306, 21),  // "onValChangeMaxTimeBox"
        QT_MOC_LITERAL(328, 16),  // "onRotateNegative"
        QT_MOC_LITERAL(345, 14),  // "updateXySlider"
        QT_MOC_LITERAL(360, 14),  // "updateXzSlider"
        QT_MOC_LITERAL(375, 14),  // "updateXwSlider"
        QT_MOC_LITERAL(390, 14),  // "updateYzSlider"
        QT_MOC_LITERAL(405, 14),  // "updateYwSlider"
        QT_MOC_LITERAL(420, 14),  // "updateZwSlider"
        QT_MOC_LITERAL(435, 20),  // "updateRotationSlider"
        QT_MOC_LITERAL(456, 18),  // "onValChangeWSlider"
        QT_MOC_LITERAL(475, 15),  // "onValChangeWBox"
        QT_MOC_LITERAL(491, 15),  // "updateTimeValue"
        QT_MOC_LITERAL(507, 18),  // "onValChangezSlider"
        QT_MOC_LITERAL(526, 15),  // "onValChangezBox"
        QT_MOC_LITERAL(542, 13)   // "updateZSlider"
    },
    "MainWindow",
    "onUploadFile",
    "",
    "onSaveImage",
    "onBulkRender",
    "onValChangexySlider",
    "newValue",
    "onValChangexyBox",
    "onValChangexzSlider",
    "onValChangexzBox",
    "onValChangexwSlider",
    "onValChangexwBox",
    "onValChangeyzSlider",
    "onValChangeyzBox",
    "onValChangeywSlider",
    "onValChangeywBox",
    "onValChangezwSlider",
    "onValChangezwBox",
    "onValChangeMaxTimeSlider",
    "onValChangeMaxTimeBox",
    "onRotateNegative",
    "updateXySlider",
    "updateXzSlider",
    "updateXwSlider",
    "updateYzSlider",
    "updateYwSlider",
    "updateZwSlider",
    "updateRotationSlider",
    "onValChangeWSlider",
    "onValChangeWBox",
    "updateTimeValue",
    "onValChangezSlider",
    "onValChangezBox",
    "updateZSlider"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSMainWindowENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
      31,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  200,    2, 0x08,    1 /* Private */,
       3,    0,  201,    2, 0x08,    2 /* Private */,
       4,    0,  202,    2, 0x08,    3 /* Private */,
       5,    1,  203,    2, 0x08,    4 /* Private */,
       7,    1,  206,    2, 0x08,    6 /* Private */,
       8,    1,  209,    2, 0x08,    8 /* Private */,
       9,    1,  212,    2, 0x08,   10 /* Private */,
      10,    1,  215,    2, 0x08,   12 /* Private */,
      11,    1,  218,    2, 0x08,   14 /* Private */,
      12,    1,  221,    2, 0x08,   16 /* Private */,
      13,    1,  224,    2, 0x08,   18 /* Private */,
      14,    1,  227,    2, 0x08,   20 /* Private */,
      15,    1,  230,    2, 0x08,   22 /* Private */,
      16,    1,  233,    2, 0x08,   24 /* Private */,
      17,    1,  236,    2, 0x08,   26 /* Private */,
      18,    1,  239,    2, 0x08,   28 /* Private */,
      19,    1,  242,    2, 0x08,   30 /* Private */,
      20,    0,  245,    2, 0x08,   32 /* Private */,
      21,    1,  246,    2, 0x08,   33 /* Private */,
      22,    1,  249,    2, 0x08,   35 /* Private */,
      23,    1,  252,    2, 0x08,   37 /* Private */,
      24,    1,  255,    2, 0x08,   39 /* Private */,
      25,    1,  258,    2, 0x08,   41 /* Private */,
      26,    1,  261,    2, 0x08,   43 /* Private */,
      27,    1,  264,    2, 0x08,   45 /* Private */,
      28,    1,  267,    2, 0x08,   47 /* Private */,
      29,    1,  270,    2, 0x08,   49 /* Private */,
      30,    0,  273,    2, 0x08,   51 /* Private */,
      31,    1,  274,    2, 0x08,   52 /* Private */,
      32,    1,  277,    2, 0x08,   54 /* Private */,
      33,    1,  280,    2, 0x08,   56 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Double,    6,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Double,    6,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Double,    6,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Double,    6,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Double,    6,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Double,    6,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Double,    6,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    6,
    QMetaType::Void, QMetaType::Double,    6,
    QMetaType::Void, QMetaType::Double,    6,
    QMetaType::Void, QMetaType::Double,    6,
    QMetaType::Void, QMetaType::Double,    6,
    QMetaType::Void, QMetaType::Double,    6,
    QMetaType::Void, QMetaType::Float,    6,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Double,    6,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Double,    6,
    QMetaType::Void, QMetaType::Double,    6,

       0        // eod
};

Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSMainWindowENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSMainWindowENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSMainWindowENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MainWindow, std::true_type>,
        // method 'onUploadFile'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSaveImage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onBulkRender'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onValChangexySlider'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onValChangexyBox'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'onValChangexzSlider'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onValChangexzBox'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'onValChangexwSlider'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onValChangexwBox'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'onValChangeyzSlider'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onValChangeyzBox'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'onValChangeywSlider'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onValChangeywBox'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'onValChangezwSlider'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onValChangezwBox'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'onValChangeMaxTimeSlider'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onValChangeMaxTimeBox'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'onRotateNegative'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateXySlider'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'updateXzSlider'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'updateXwSlider'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'updateYzSlider'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'updateYwSlider'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'updateZwSlider'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'updateRotationSlider'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<float, std::false_type>,
        // method 'onValChangeWSlider'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onValChangeWBox'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'updateTimeValue'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onValChangezSlider'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onValChangezBox'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'updateZSlider'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>
    >,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onUploadFile(); break;
        case 1: _t->onSaveImage(); break;
        case 2: _t->onBulkRender(); break;
        case 3: _t->onValChangexySlider((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->onValChangexyBox((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 5: _t->onValChangexzSlider((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->onValChangexzBox((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 7: _t->onValChangexwSlider((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 8: _t->onValChangexwBox((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 9: _t->onValChangeyzSlider((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 10: _t->onValChangeyzBox((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 11: _t->onValChangeywSlider((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 12: _t->onValChangeywBox((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 13: _t->onValChangezwSlider((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 14: _t->onValChangezwBox((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 15: _t->onValChangeMaxTimeSlider((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 16: _t->onValChangeMaxTimeBox((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 17: _t->onRotateNegative(); break;
        case 18: _t->updateXySlider((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 19: _t->updateXzSlider((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 20: _t->updateXwSlider((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 21: _t->updateYzSlider((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 22: _t->updateYwSlider((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 23: _t->updateZwSlider((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 24: _t->updateRotationSlider((*reinterpret_cast< std::add_pointer_t<float>>(_a[1]))); break;
        case 25: _t->onValChangeWSlider((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 26: _t->onValChangeWBox((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 27: _t->updateTimeValue(); break;
        case 28: _t->onValChangezSlider((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 29: _t->onValChangezBox((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 30: _t->updateZSlider((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSMainWindowENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 31)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 31;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 31)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 31;
    }
    return _id;
}
QT_WARNING_POP
