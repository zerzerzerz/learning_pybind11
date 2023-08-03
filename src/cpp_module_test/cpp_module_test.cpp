#include <pybind11/pybind11.h>
#include <string>
#include <iostream>
#include <vector>

namespace py = pybind11;
using namespace std;

class Base{
    public:
        ~Base(){}
        virtual void show() = 0;
};


class A: public Base{
public:
    void show(){
        cout << "This is A" << endl;
    }
};

class PyBase: public Base{
    using Base::Base;
    void show(){PYBIND11_OVERLOAD_PURE(void, Base, show);}
};



class B{
public:
    vector<Base*>* data;
    B(){
        data = new vector<Base*>;
    }

    void add_A(A* a){
        data->push_back(a);
    }


    void show_all_A(){
        for(uint i=0; i<data->size(); ++i){
            data->at(i)->show();
        }
    }



    ~B(){
        delete data;
    }
};



A* create_A(){
    return new A();
}




PYBIND11_MODULE(cpp_module_test, m){
    py::class_<Base, PyBase>(m, "Base")
        .def(py::init<>())
    ;
    py::class_<A, Base>(m, "A")
        .def(py::init<>())
    ;


    py::class_<B>(m, "B")
        .def(py::init<>())
        .def("add_A", &B::add_A)
        .def("show_all_A", &B::show_all_A)
    ;

    m.def("create_A", &create_A, py::return_value_policy::reference);
}