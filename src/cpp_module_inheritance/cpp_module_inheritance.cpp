#include <iostream>
#include <string>
#include <pybind11/pybind11.h>
using namespace std;
namespace py = pybind11;

class Base{
public:
    Base(){cout << "Base::Base" << endl;}
    virtual ~Base(){cout << "Base::~Base" << endl;}

    void func_normal(){cout << "Base::func_normal" << endl;}

    virtual void func_virtual(){cout << "Base::func_virtual" << endl;}
    virtual void func_pure_virtual() = 0;
};


class Derived: public Base{
public:
    Derived(){cout << "Derived::Derived" << endl;}
    ~Derived(){cout << "Derived::~Derived" << endl;}

    virtual void func_virtual(){cout << "Derived::func_virtual" << endl;}

    void func_new(){cout << "Derived::func_new" << endl;}
    virtual void func_pure_virtual() override{cout << "Derived::func_pure_virtual" << endl;}
};


class PyBase: public Base{
    using Base::Base;
    void func_virtual(){PYBIND11_OVERRIDE(void, Base, func_virtual);}
    void func_pure_virtual(){PYBIND11_OVERRIDE_PURE(void, Base, func_pure_virtual);}
};


class PyDerived: public Derived{
    using Derived::Derived;
    void func_virtual(){PYBIND11_OVERRIDE(void, Derived, func_virtual);}
    void func_pure_virtual(){PYBIND11_OVERRIDE_PURE(void, Derived, func_pure_virtual);}
};


PYBIND11_MODULE(cpp_module_inheritance, m){
    py::class_<Base, PyBase>(m, "Base")
        .def(py::init<>())
        .def("func_normal", &Base::func_normal)
        .def("func_virtual", &Base::func_virtual)
        .def("func_pure_virtual", &Base::func_pure_virtual)
    ;

    py::class_<Derived, Base, PyDerived>(m, "Derived")
        .def(py::init<>())
        .def("func_new", &Derived::func_new)
    ;
}