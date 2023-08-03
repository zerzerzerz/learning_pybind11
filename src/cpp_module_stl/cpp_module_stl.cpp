#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <vector>
#include <map>
#include <string>
#include <cassert>

// make this type opaque
// PYBIND11_MAKE_OPAQUE(std::vector<float>);

using namespace std;
namespace py = pybind11;


// this will copy data from Python list!!!
void print_list(const vector<int>& v){
    cout << "Automatic fron Python list to C++ vector" << endl;
    cout << "This will copy data completely" << endl;
    for(int i:v) cout << i << " ";
    cout << endl;
}

void print_dict(const map<string, int>& m){
    for(const auto& p:m){
        cout << p.first << " -> " << p.second << endl;
    }
}

void change_list(vector<int>& v){
    cout << "Change data in C++ vector, but not reflected in Python list!" << endl;
    for(uint i=0; i<v.size(); ++i){
        v[i] = -1;
    }
}


class MyList{
    public:
        vector<int> list;
        MyList(const int& n){
            for(int i=0; i<n; ++i){
                list.push_back(0);
            }
        }

        int& operator[](const int& i){return list[i];}


        MyList operator+(const MyList& n){
            MyList res(list.size() + n.list.size());
            int i = 0;
            for(int j=0; j<list.size(); ++j){
                res[i] = list[j];
                ++i;
            }
            for(int j=0; j<n.list.size(); ++j){
                res[i] = n.list[j];
                ++i;
            }
            return res;
        }

        
        void display(){
            for(const auto& i: list){
                cout << i << " ";
            }
            cout << endl;
        }
};


void change_pylist(py::list a){
    cout << "Change data of py::list, pass by value" << endl;
    a[0] = -999;
}

void change_pylist_reference(py::list& a){
    cout << "Change data of py::list, pass by reference" << endl;
    a[0] = -999;
}



    


PYBIND11_MODULE(cpp_module_stl, m){
    m.def("print_list", &print_list);
    m.def("print_dict", &print_dict);
    m.def("change_list", &change_list);
    m.def("change_pylist", &change_pylist);
    m.def("change_pylist_reference", &change_pylist_reference);
    
    py::class_<MyList>(m, "MyList")
        .def(py::init<int>())
        .def("display", &MyList::display)

        // operator [ ] overloading, via __setitem__ and __getitem__
        .def("__getitem__", &MyList::operator[])
        .def("__setitem__", [](MyList* mylist, const int& idx, const int& v){(*mylist)[idx]=v;})

        // operator +, which is __add__ for Python
        .def("__add__", &MyList::operator+)
        .def_readwrite("list", &MyList::list)
        ;

    py::class_< vector<float> >(m, "FloatVector")
        .def(py::init<>())
        .def("__len__", [](const vector<float>& v){return v.size();})
        .def("append", [](vector<float>& v, const float& x){
            v.push_back(x);
        })
        .def("__getitem__", [](const vector<float>& v, int i) -> const float&{
            assert(i < v.size());
            return v[i];
        })
        .def("__setitem__", [](vector<float>& v, int i, float n){
            assert(i < v.size());
            v[i]=n;
        })
        .def("__iter__", [](std::vector<float> &v) {
            return py::make_iterator(v.begin(), v.end());
        }, py::keep_alive<0, 1>())
        ;
    

}