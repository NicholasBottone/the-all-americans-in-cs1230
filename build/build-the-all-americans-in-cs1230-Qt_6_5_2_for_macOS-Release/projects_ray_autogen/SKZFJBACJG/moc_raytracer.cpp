/****************************************************************************
** Meta object code from reading C++ file 'raytracer.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/raytracer/raytracer.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'raytracer.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSRayTracerENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSRayTracerENDCLASS = QtMocHelpers::stringData(
    "RayTracer",
    "xyRotationChanged",
    "",
    "value",
    "xzRotationChanged",
    "xwRotationChanged",
    "yzRotationChanged",
    "ywRotationChanged",
    "zwRotationChanged"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSRayTracerENDCLASS_t {
    uint offsetsAndSizes[18];
    char stringdata0[10];
    char stringdata1[18];
    char stringdata2[1];
    char stringdata3[6];
    char stringdata4[18];
    char stringdata5[18];
    char stringdata6[18];
    char stringdata7[18];
    char stringdata8[18];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSRayTracerENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSRayTracerENDCLASS_t qt_meta_stringdata_CLASSRayTracerENDCLASS = {
    {
        QT_MOC_LITERAL(0, 9),  // "RayTracer"
        QT_MOC_LITERAL(10, 17),  // "xyRotationChanged"
        QT_MOC_LITERAL(28, 0),  // ""
        QT_MOC_LITERAL(29, 5),  // "value"
        QT_MOC_LITERAL(35, 17),  // "xzRotationChanged"
        QT_MOC_LITERAL(53, 17),  // "xwRotationChanged"
        QT_MOC_LITERAL(71, 17),  // "yzRotationChanged"
        QT_MOC_LITERAL(89, 17),  // "ywRotationChanged"
        QT_MOC_LITERAL(107, 17)   // "zwRotationChanged"
    },
    "RayTracer",
    "xyRotationChanged",
    "",
    "value",
    "xzRotationChanged",
    "xwRotationChanged",
    "yzRotationChanged",
    "ywRotationChanged",
    "zwRotationChanged"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSRayTracerENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   50,    2, 0x06,    1 /* Public */,
       4,    1,   53,    2, 0x06,    3 /* Public */,
       5,    1,   56,    2, 0x06,    5 /* Public */,
       6,    1,   59,    2, 0x06,    7 /* Public */,
       7,    1,   62,    2, 0x06,    9 /* Public */,
       8,    1,   65,    2, 0x06,   11 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Float,    3,

       0        // eod
};

Q_CONSTINIT const QMetaObject RayTracer::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSRayTracerENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSRayTracerENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSRayTracerENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<RayTracer, std::true_type>,
        // method 'xyRotationChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<float, std::false_type>,
        // method 'xzRotationChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<float, std::false_type>,
        // method 'xwRotationChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<float, std::false_type>,
        // method 'yzRotationChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<float, std::false_type>,
        // method 'ywRotationChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<float, std::false_type>,
        // method 'zwRotationChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<float, std::false_type>
    >,
    nullptr
} };

void RayTracer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RayTracer *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->xyRotationChanged((*reinterpret_cast< std::add_pointer_t<float>>(_a[1]))); break;
        case 1: _t->xzRotationChanged((*reinterpret_cast< std::add_pointer_t<float>>(_a[1]))); break;
        case 2: _t->xwRotationChanged((*reinterpret_cast< std::add_pointer_t<float>>(_a[1]))); break;
        case 3: _t->yzRotationChanged((*reinterpret_cast< std::add_pointer_t<float>>(_a[1]))); break;
        case 4: _t->ywRotationChanged((*reinterpret_cast< std::add_pointer_t<float>>(_a[1]))); break;
        case 5: _t->zwRotationChanged((*reinterpret_cast< std::add_pointer_t<float>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (RayTracer::*)(float );
            if (_t _q_method = &RayTracer::xyRotationChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (RayTracer::*)(float );
            if (_t _q_method = &RayTracer::xzRotationChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (RayTracer::*)(float );
            if (_t _q_method = &RayTracer::xwRotationChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (RayTracer::*)(float );
            if (_t _q_method = &RayTracer::yzRotationChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (RayTracer::*)(float );
            if (_t _q_method = &RayTracer::ywRotationChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (RayTracer::*)(float );
            if (_t _q_method = &RayTracer::zwRotationChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
    }
}

const QMetaObject *RayTracer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RayTracer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSRayTracerENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int RayTracer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

// SIGNAL 0
void RayTracer::xyRotationChanged(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void RayTracer::xzRotationChanged(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void RayTracer::xwRotationChanged(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void RayTracer::yzRotationChanged(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void RayTracer::ywRotationChanged(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void RayTracer::zwRotationChanged(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_WARNING_POP
