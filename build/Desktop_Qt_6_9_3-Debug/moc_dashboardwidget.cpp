/****************************************************************************
** Meta object code from reading C++ file 'dashboardwidget.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.3)
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
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.3. It"
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

template <> constexpr inline auto DashBoardWidget::qt_create_metaobjectdata<qt_meta_tag_ZN15DashBoardWidgetE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
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
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'valueChanged'
        QtMocHelpers::SignalData<void(double)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 3 },
        }}),
        // Signal 'valueIncreased'
        QtMocHelpers::SignalData<void(double)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 5 },
        }}),
        // Signal 'valueDecreased'
        QtMocHelpers::SignalData<void(double)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 5 },
        }}),
        // Signal 'valueReset'
        QtMocHelpers::SignalData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'animationStarted'
        QtMocHelpers::SignalData<void(double, double)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 9 }, { QMetaType::Double, 5 },
        }}),
        // Signal 'animationFinished'
        QtMocHelpers::SignalData<void(double)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 3 },
        }}),
        // Slot 'increaseValue'
        QtMocHelpers::SlotData<void(double)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 12 },
        }}),
        // Slot 'increaseValue'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void),
        // Slot 'decreaseValue'
        QtMocHelpers::SlotData<void(double)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 14 },
        }}),
        // Slot 'decreaseValue'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void),
        // Slot 'reset'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onAnimationFinished'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'value'
        QtMocHelpers::PropertyData<double>(3, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'minValue'
        QtMocHelpers::PropertyData<double>(17, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'maxValue'
        QtMocHelpers::PropertyData<double>(18, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'startAngle'
        QtMocHelpers::PropertyData<double>(19, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'endAngle'
        QtMocHelpers::PropertyData<double>(20, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'scaleMajor'
        QtMocHelpers::PropertyData<int>(21, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'scaleMinor'
        QtMocHelpers::PropertyData<int>(22, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'unit'
        QtMocHelpers::PropertyData<QString>(23, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'title'
        QtMocHelpers::PropertyData<QString>(24, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'arcColor'
        QtMocHelpers::PropertyData<QColor>(25, QMetaType::QColor, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'scaleColor'
        QtMocHelpers::PropertyData<QColor>(26, QMetaType::QColor, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'pointerColor'
        QtMocHelpers::PropertyData<QColor>(27, QMetaType::QColor, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'textColor'
        QtMocHelpers::PropertyData<QColor>(28, QMetaType::QColor, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'backgroundColor'
        QtMocHelpers::PropertyData<QColor>(29, QMetaType::QColor, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'valueColor'
        QtMocHelpers::PropertyData<QColor>(30, QMetaType::QColor, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'titleColor'
        QtMocHelpers::PropertyData<QColor>(31, QMetaType::QColor, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'animationDuration'
        QtMocHelpers::PropertyData<int>(32, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<DashBoardWidget, qt_meta_tag_ZN15DashBoardWidgetE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject DashBoardWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15DashBoardWidgetE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15DashBoardWidgetE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN15DashBoardWidgetE_t>.metaTypes,
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
        if (QtMocHelpers::indexOfMethod<void (DashBoardWidget::*)(double )>(_a, &DashBoardWidget::valueChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (DashBoardWidget::*)(double )>(_a, &DashBoardWidget::valueIncreased, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (DashBoardWidget::*)(double )>(_a, &DashBoardWidget::valueDecreased, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (DashBoardWidget::*)()>(_a, &DashBoardWidget::valueReset, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (DashBoardWidget::*)(double , double )>(_a, &DashBoardWidget::animationStarted, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (DashBoardWidget::*)(double )>(_a, &DashBoardWidget::animationFinished, 5))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<double*>(_v) = _t->value(); break;
        case 1: *reinterpret_cast<double*>(_v) = _t->minValue(); break;
        case 2: *reinterpret_cast<double*>(_v) = _t->maxValue(); break;
        case 3: *reinterpret_cast<double*>(_v) = _t->startAngle(); break;
        case 4: *reinterpret_cast<double*>(_v) = _t->endAngle(); break;
        case 5: *reinterpret_cast<int*>(_v) = _t->scaleMajor(); break;
        case 6: *reinterpret_cast<int*>(_v) = _t->scaleMinor(); break;
        case 7: *reinterpret_cast<QString*>(_v) = _t->unit(); break;
        case 8: *reinterpret_cast<QString*>(_v) = _t->title(); break;
        case 9: *reinterpret_cast<QColor*>(_v) = _t->arcColor(); break;
        case 10: *reinterpret_cast<QColor*>(_v) = _t->scaleColor(); break;
        case 11: *reinterpret_cast<QColor*>(_v) = _t->pointerColor(); break;
        case 12: *reinterpret_cast<QColor*>(_v) = _t->textColor(); break;
        case 13: *reinterpret_cast<QColor*>(_v) = _t->backgroundColor(); break;
        case 14: *reinterpret_cast<QColor*>(_v) = _t->valueColor(); break;
        case 15: *reinterpret_cast<QColor*>(_v) = _t->titleColor(); break;
        case 16: *reinterpret_cast<int*>(_v) = _t->animationDuration(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setValue(*reinterpret_cast<double*>(_v)); break;
        case 1: _t->setMinValue(*reinterpret_cast<double*>(_v)); break;
        case 2: _t->setMaxValue(*reinterpret_cast<double*>(_v)); break;
        case 3: _t->setStartAngle(*reinterpret_cast<double*>(_v)); break;
        case 4: _t->setEndAngle(*reinterpret_cast<double*>(_v)); break;
        case 5: _t->setScaleMajor(*reinterpret_cast<int*>(_v)); break;
        case 6: _t->setScaleMinor(*reinterpret_cast<int*>(_v)); break;
        case 7: _t->setUnit(*reinterpret_cast<QString*>(_v)); break;
        case 8: _t->setTitle(*reinterpret_cast<QString*>(_v)); break;
        case 9: _t->setArcColor(*reinterpret_cast<QColor*>(_v)); break;
        case 10: _t->setScaleColor(*reinterpret_cast<QColor*>(_v)); break;
        case 11: _t->setPointerColor(*reinterpret_cast<QColor*>(_v)); break;
        case 12: _t->setTextColor(*reinterpret_cast<QColor*>(_v)); break;
        case 13: _t->setBackgroundColor(*reinterpret_cast<QColor*>(_v)); break;
        case 14: _t->setValueColor(*reinterpret_cast<QColor*>(_v)); break;
        case 15: _t->setTitleColor(*reinterpret_cast<QColor*>(_v)); break;
        case 16: _t->setAnimationDuration(*reinterpret_cast<int*>(_v)); break;
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
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15DashBoardWidgetE_t>.strings))
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
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void DashBoardWidget::valueIncreased(double _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void DashBoardWidget::valueDecreased(double _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void DashBoardWidget::valueReset()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void DashBoardWidget::animationStarted(double _t1, double _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1, _t2);
}

// SIGNAL 5
void DashBoardWidget::animationFinished(double _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1);
}
QT_WARNING_POP
