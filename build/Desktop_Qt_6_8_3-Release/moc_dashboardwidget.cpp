/****************************************************************************
** Meta object code from reading C++ file 'dashboardwidget.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../dashboardwidget.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dashboardwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.3. It"
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
struct qt_meta_tag_ZN15DashBoardWidgetE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN15DashBoardWidgetE = QtMocHelpers::stringData(
    "DashBoardWidget",
    "valueChanged",
    "",
    "value",
    "valueIncreased",
    "newValue",
    "valueDecreased",
    "valueReset",
    "animationStarted",
    "oldValue",
    "animationFinished",
    "increaseValue",
    "increment",
    "decreaseValue",
    "decrement",
    "reset",
    "onAnimationFinished",
    "minValue",
    "maxValue",
    "startAngle",
    "endAngle",
    "scaleMajor",
    "scaleMinor",
    "unit",
    "title",
    "arcColor",
    "scaleColor",
    "pointerColor",
    "textColor",
    "backgroundColor",
    "valueColor",
    "titleColor",
    "animationDuration"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN15DashBoardWidgetE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
      17,  114, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   86,    2, 0x06,   18 /* Public */,
       4,    1,   89,    2, 0x06,   20 /* Public */,
       6,    1,   92,    2, 0x06,   22 /* Public */,
       7,    0,   95,    2, 0x06,   24 /* Public */,
       8,    2,   96,    2, 0x06,   25 /* Public */,
      10,    1,  101,    2, 0x06,   28 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      11,    1,  104,    2, 0x0a,   30 /* Public */,
      11,    0,  107,    2, 0x2a,   32 /* Public | MethodCloned */,
      13,    1,  108,    2, 0x0a,   33 /* Public */,
      13,    0,  111,    2, 0x2a,   35 /* Public | MethodCloned */,
      15,    0,  112,    2, 0x0a,   36 /* Public */,
      16,    0,  113,    2, 0x08,   37 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    5,
    QMetaType::Void, QMetaType::Double,    5,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    9,    5,
    QMetaType::Void, QMetaType::Double,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::Double,   12,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,   14,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags, notifyId, revision
       3, QMetaType::Double, 0x00015103, uint(0), 0,
      17, QMetaType::Double, 0x00015103, uint(-1), 0,
      18, QMetaType::Double, 0x00015103, uint(-1), 0,
      19, QMetaType::Double, 0x00015103, uint(-1), 0,
      20, QMetaType::Double, 0x00015103, uint(-1), 0,
      21, QMetaType::Int, 0x00015103, uint(-1), 0,
      22, QMetaType::Int, 0x00015103, uint(-1), 0,
      23, QMetaType::QString, 0x00015103, uint(-1), 0,
      24, QMetaType::QString, 0x00015103, uint(-1), 0,
      25, QMetaType::QColor, 0x00015103, uint(-1), 0,
      26, QMetaType::QColor, 0x00015103, uint(-1), 0,
      27, QMetaType::QColor, 0x00015103, uint(-1), 0,
      28, QMetaType::QColor, 0x00015103, uint(-1), 0,
      29, QMetaType::QColor, 0x00015103, uint(-1), 0,
      30, QMetaType::QColor, 0x00015103, uint(-1), 0,
      31, QMetaType::QColor, 0x00015103, uint(-1), 0,
      32, QMetaType::Int, 0x00015103, uint(-1), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject DashBoardWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_ZN15DashBoardWidgetE.offsetsAndSizes,
    qt_meta_data_ZN15DashBoardWidgetE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN15DashBoardWidgetE_t,
        // property 'value'
        QtPrivate::TypeAndForceComplete<double, std::true_type>,
        // property 'minValue'
        QtPrivate::TypeAndForceComplete<double, std::true_type>,
        // property 'maxValue'
        QtPrivate::TypeAndForceComplete<double, std::true_type>,
        // property 'startAngle'
        QtPrivate::TypeAndForceComplete<double, std::true_type>,
        // property 'endAngle'
        QtPrivate::TypeAndForceComplete<double, std::true_type>,
        // property 'scaleMajor'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // property 'scaleMinor'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // property 'unit'
        QtPrivate::TypeAndForceComplete<QString, std::true_type>,
        // property 'title'
        QtPrivate::TypeAndForceComplete<QString, std::true_type>,
        // property 'arcColor'
        QtPrivate::TypeAndForceComplete<QColor, std::true_type>,
        // property 'scaleColor'
        QtPrivate::TypeAndForceComplete<QColor, std::true_type>,
        // property 'pointerColor'
        QtPrivate::TypeAndForceComplete<QColor, std::true_type>,
        // property 'textColor'
        QtPrivate::TypeAndForceComplete<QColor, std::true_type>,
        // property 'backgroundColor'
        QtPrivate::TypeAndForceComplete<QColor, std::true_type>,
        // property 'valueColor'
        QtPrivate::TypeAndForceComplete<QColor, std::true_type>,
        // property 'titleColor'
        QtPrivate::TypeAndForceComplete<QColor, std::true_type>,
        // property 'animationDuration'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<DashBoardWidget, std::true_type>,
        // method 'valueChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'valueIncreased'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'valueDecreased'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'valueReset'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'animationStarted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'animationFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'increaseValue'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'increaseValue'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'decreaseValue'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'decreaseValue'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'reset'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onAnimationFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void DashBoardWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<DashBoardWidget *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->valueChanged((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 1: _t->valueIncreased((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 2: _t->valueDecreased((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 3: _t->valueReset(); break;
        case 4: _t->animationStarted((*reinterpret_cast< std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[2]))); break;
        case 5: _t->animationFinished((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 6: _t->increaseValue((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 7: _t->increaseValue(); break;
        case 8: _t->decreaseValue((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 9: _t->decreaseValue(); break;
        case 10: _t->reset(); break;
        case 11: _t->onAnimationFinished(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _q_method_type = void (DashBoardWidget::*)(double );
            if (_q_method_type _q_method = &DashBoardWidget::valueChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _q_method_type = void (DashBoardWidget::*)(double );
            if (_q_method_type _q_method = &DashBoardWidget::valueIncreased; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _q_method_type = void (DashBoardWidget::*)(double );
            if (_q_method_type _q_method = &DashBoardWidget::valueDecreased; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _q_method_type = void (DashBoardWidget::*)();
            if (_q_method_type _q_method = &DashBoardWidget::valueReset; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _q_method_type = void (DashBoardWidget::*)(double , double );
            if (_q_method_type _q_method = &DashBoardWidget::animationStarted; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _q_method_type = void (DashBoardWidget::*)(double );
            if (_q_method_type _q_method = &DashBoardWidget::animationFinished; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< double*>(_v) = _t->value(); break;
        case 1: *reinterpret_cast< double*>(_v) = _t->minValue(); break;
        case 2: *reinterpret_cast< double*>(_v) = _t->maxValue(); break;
        case 3: *reinterpret_cast< double*>(_v) = _t->startAngle(); break;
        case 4: *reinterpret_cast< double*>(_v) = _t->endAngle(); break;
        case 5: *reinterpret_cast< int*>(_v) = _t->scaleMajor(); break;
        case 6: *reinterpret_cast< int*>(_v) = _t->scaleMinor(); break;
        case 7: *reinterpret_cast< QString*>(_v) = _t->unit(); break;
        case 8: *reinterpret_cast< QString*>(_v) = _t->title(); break;
        case 9: *reinterpret_cast< QColor*>(_v) = _t->arcColor(); break;
        case 10: *reinterpret_cast< QColor*>(_v) = _t->scaleColor(); break;
        case 11: *reinterpret_cast< QColor*>(_v) = _t->pointerColor(); break;
        case 12: *reinterpret_cast< QColor*>(_v) = _t->textColor(); break;
        case 13: *reinterpret_cast< QColor*>(_v) = _t->backgroundColor(); break;
        case 14: *reinterpret_cast< QColor*>(_v) = _t->valueColor(); break;
        case 15: *reinterpret_cast< QColor*>(_v) = _t->titleColor(); break;
        case 16: *reinterpret_cast< int*>(_v) = _t->animationDuration(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setValue(*reinterpret_cast< double*>(_v)); break;
        case 1: _t->setMinValue(*reinterpret_cast< double*>(_v)); break;
        case 2: _t->setMaxValue(*reinterpret_cast< double*>(_v)); break;
        case 3: _t->setStartAngle(*reinterpret_cast< double*>(_v)); break;
        case 4: _t->setEndAngle(*reinterpret_cast< double*>(_v)); break;
        case 5: _t->setScaleMajor(*reinterpret_cast< int*>(_v)); break;
        case 6: _t->setScaleMinor(*reinterpret_cast< int*>(_v)); break;
        case 7: _t->setUnit(*reinterpret_cast< QString*>(_v)); break;
        case 8: _t->setTitle(*reinterpret_cast< QString*>(_v)); break;
        case 9: _t->setArcColor(*reinterpret_cast< QColor*>(_v)); break;
        case 10: _t->setScaleColor(*reinterpret_cast< QColor*>(_v)); break;
        case 11: _t->setPointerColor(*reinterpret_cast< QColor*>(_v)); break;
        case 12: _t->setTextColor(*reinterpret_cast< QColor*>(_v)); break;
        case 13: _t->setBackgroundColor(*reinterpret_cast< QColor*>(_v)); break;
        case 14: _t->setValueColor(*reinterpret_cast< QColor*>(_v)); break;
        case 15: _t->setTitleColor(*reinterpret_cast< QColor*>(_v)); break;
        case 16: _t->setAnimationDuration(*reinterpret_cast< int*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *DashBoardWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DashBoardWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN15DashBoardWidgetE.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int DashBoardWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 12;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void DashBoardWidget::valueChanged(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DashBoardWidget::valueIncreased(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DashBoardWidget::valueDecreased(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DashBoardWidget::valueReset()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void DashBoardWidget::animationStarted(double _t1, double _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void DashBoardWidget::animationFinished(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_WARNING_POP
