#include <pybind11/pybind11.h>
#include <string>

namespace py = pybind11;

int add(int i, int j) {
    return i + j;
}

int add2(int i, int j){
    return i+j;
}

int add3(int i=3, int j=5){
    return i+j;
}

struct Pet{
    std::string name;
    Pet(const std::string& name_): name(name_){}
    void set_name(const std::string name_){name = name_;}
    const std::string& get_name(){ return name;}
};

class Food{
    public:
        Food(const std::string& name_): name(name_) {}
        void set_name(const std::string& name_) {name=name_;}
        const std::string& get_name() {return name;}
    private:
        std::string name;
};



// cpp_module_basic is the module name in Python
// m is a variable of type py::module_
PYBIND11_MODULE(cpp_module_basic, m) {
    // optional module docstring
    m.doc() = "pybind11 example plugin"; 

    // module_::def() generates binding code that exposes the add function in python
    // first arg is function name in Python
    // second arg is the function address in C++
    // third arg is the function string
    m.def("add", &add, "A function that adds two numbers");


    // add keyword arguments
    // invoke add2(i=1, j=2) in python
    m.def("add2", &add2, "A function that adds two numbers", py::arg("i"), py::arg("j"));


    // add default parameters
    // we must specify default args in pybind
    m.def("add3", &add3, "A function that adds two numbers", py::arg("i")=3, py::arg("j")=5);


    // exporting variables as module attributes
    m.attr("age") = 22;
    m.attr("name") = "Ruizhe Zhong";

    // binding struct
    py::class_<Pet>(m, "Pet")
        // binding constructor
        // py::init<args>()
        .def(py::init<const std::string &>())

        // binding class methods
        // function name in Python, function address in C++
        .def("set_name", &Pet::set_name)
        .def("get_name", &Pet::get_name)

        // binding lambda function
        .def("__repr__", [](const Pet& p){
            return "<example.Pet named '" + p.name +  "'>";
        })

        // binding attribute, we can access/modify via pet.name = XXX
        // this only works when binding a public attribute
        .def_readwrite("name", &Pet::name);


    py::class_<Food>(m, "Food")
        .def(py::init<const std::string&>())
        .def("set_name", &Food::set_name)
        .def("get_name", &Food::get_name)
        // In this class, name is a private attribute, so we cannot use the following way
        // .def_readwrite("name", &Food::name)
        // If we want to access/modify this attribute in Python via food.name, we should write like this
        .def_property("name", &Food::get_name, &Food::set_name);
}