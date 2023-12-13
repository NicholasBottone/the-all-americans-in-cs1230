/****************************************************************************
** Meta object code from reading C++ file 'raytracer.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/raytracer/raytracer.h"
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
    "zwRotationChanged",
    "rotationChanged",
    "cameraPositionChanged",
    "glm::vec3",
    "timeValueChanged",
    "zChanged"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSRayTracerENDCLASS_t {
    uint offsetsAndSizes[28];
    char stringdata0[10];
    char stringdata1[18];
    char stringdata2[1];
    char stringdata3[6];
    char stringdata4[18];
    char stringdata5[18];
    char stringdata6[18];
    char stringdata7[18];
    char stringdata8[18];
    char stringdata9[16];
    char stringdata10[22];
    char stringdata11[10];
    char stringdata12[17];
    char stringdata13[9];
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
        QT_MOC_LITERAL(107, 17),  // "zwRotationChanged"
        QT_MOC_LITERAL(125, 15),  // "rotationChanged"
        QT_MOC_LITERAL(141, 21),  // "cameraPositionChanged"
        QT_MOC_LITERAL(163, 9),  // "glm::vec3"
        QT_MOC_LITERAL(173, 16),  // "timeValueChanged"
        QT_MOC_LITERAL(190, 8)   // "zChanged"
    },
    "RayTracer",
    "xyRotationChanged",
    "",
    "value",
    "xzRotationChanged",
    "xwRotationChanged",
    "yzRotationChanged",
    "ywRotationChanged",
    "zwRotationChanged",
    "rotationChanged",
    "cameraPositionChanged",
    "glm::vec3",
    "timeValueChanged",
    "zChanged"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSRayTracerENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      10,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   74,    2, 0x06,    1 /* Public */,
       4,    1,   77,    2, 0x06,    3 /* Public */,
       5,    1,   80,    2, 0x06,    5 /* Public */,
       6,    1,   83,    2, 0x06,    7 /* Public */,
       7,    1,   86,    2, 0x06,    9 /* Public */,
       8,    1,   89,    2, 0x06,   11 /* Public */,
       9,    1,   92,    2, 0x06,   13 /* Public */,
      10,    1,   95,    2, 0x06,   15 /* Public */,
      12,    1,   98,    2, 0x06,   17 /* Public */,
      13,    1,  101,    2, 0x06,   19 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, 0x80000000 | 11,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,

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
        QtPrivate::TypeAndForceComplete<float, std::false_type>,
        // method 'rotationChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<float, std::false_type>,
        // method 'cameraPositionChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<glm::vec3, std::false_type>,
        // method 'timeValueChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'zChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>
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
        case 6: _t->rotationChanged((*reinterpret_cast< std::add_pointer_t<float>>(_a[1]))); break;
        case 7: _t->cameraPositionChanged((*reinterpret_cast< std::add_pointer_t<glm::vec3>>(_a[1]))); break;
        case 8: _t->timeValueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 9: _t->zChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
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
        {
            using _t = void (RayTracer::*)(float );
            if (_t _q_method = &RayTracer::rotationChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (RayTracer::*)(glm::vec3 );
            if (_t _q_method = &RayTracer::cameraPositionChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (RayTracer::*)(int );
            if (_t _q_method = &RayTracer::timeValueChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (RayTracer::*)(int );
            if (_t _q_method = &RayTracer::zChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 9;
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
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 10;
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

// SIGNAL 6
void RayTracer::rotationChanged(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void RayTracer::cameraPositionChanged(glm::vec3 _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void RayTracer::timeValueChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void RayTracer::zChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}
QT_WARNING_POP
