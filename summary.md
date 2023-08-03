# Summary
## 参数传递
- pybind中绑定某个C++ class（自定义的class），Python中object往C++某个函数中传的时候，就按照参数描述的来传递，传递引用的时候，就可以修改，传递value的时候，就不会修改
- 传递C++中自定义class的指针
  - Python往里传的时候，直接把object扔进去就OK
  - 注意是自定义的class
- STL中常见模板支持和Python的list, dict, set互相转换，但是无论C++中传value还是reference，都不会影响Python中的值。以Python中list传到C++ vector为例，首先转换为vector，这一步是复制操作！！！

- 想要实现Python和C++同步，参考[https://pybind11.readthedocs.io/en/stable/advanced/cast/stl.html#binding-stl-containers](https://pybind11.readthedocs.io/en/stable/advanced/cast/stl.html#binding-stl-containers) 也就是将`vector<int>`转换为一种新的List
## Native type in C++, wrapper in Python
- the underlying data structure is always the original C++ class while the py::class_ wrapper provides a Python interface
- when an object like this is sent from C++ to Python, pybind11 will just add the outer wrapper layer over the native C++ object（C++传Python，加一层wrapper）
- Getting it back from Python is just a matter of peeling off the wrapper（Python传回C++，去掉这层wrapper）
## Wrapper in C++, native type in Python
```C++
void print_list(py::list my_list) {
    for (auto item : my_list)
        std::cout << item << " ";
}
```
- The Python list is not converted in any way – it’s just wrapped in a C++ py::list class. At its core it’s still a Python object.
- **Copying a py::list will do the usual reference-counting like in Python.**

## Converting between native C++ and Python types
- we have native types on both sides and we convert between them
- **pybind11 will construct a new std::vector<int> and copy each element from the Python list**
- The same thing happens in the other direction: a new list is made to match the value returned from C++

## Return Value Policy
- `py::return_value_policy::take_ownership`
  - reference to an existing object
  - Python take the ownership and will call destructor
- `py::return_value_policy::copy`
  - call class constructor
  - copy left value to a new instance
  - Python will take the ownership of the new instance
- `py::return_value_policy::move`
  - call `std::move`
  - move right value to a new instance
  - Python will take the ownership of the new instance
- `py::return_value_policy::reference`
  - reference to an existing object
  - Python does not have ownership
- `py::return_value_policy::reference_internal`
  - reference to an existing object
  - Indicates that the lifetime of the return value is tied to the lifetime of a parent object, namely the implicit `this`, or `self` argument of the called method or property
  - `def_property`
  - `def_readwrite`
- `py::return_value_policy::automatic`
  - pointer: take_ownership
  - lvalue: copy
  - rvalue: move


## Inheritance
### 非多态
- [https://pybind11.readthedocs.io/en/stable/classes.html#inheritance-and-automatic-downcasting](https://pybind11.readthedocs.io/en/stable/classes.html#inheritance-and-automatic-downcasting)
- 基类正常绑定需要的函数
- 子类需要绑定1）子类的构造函数，2）新增的属性和方法
### 多态非ABC
- 有虚函数，但是没有纯虚函数
```C++
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
};


class Derived: public Base{
public:
    Derived(){cout << "Derived::Derived" << endl;}
    ~Derived(){cout << "Derived::~Derived" << endl;}
    virtual void func_virtual(){cout << "Derived::func_virtual" << endl;}
    void func_new(){cout << "Derived::func_new" << endl;}
};


PYBIND11_MODULE(cpp_module_inheritance, m){
    py::class_<Base>(m, "Base")
        .def(py::init<>())
        .def("func_normal", &Base::func_normal)
        .def("func_virtual", &Base::func_virtual)
    ;

    py::class_<Derived, Base>(m, "Derived")
        .def(py::init<>())
        .def("func_new", &Derived::func_new)
    ;
}
```
```Python
import cpp_module_inheritance as lib

class Derived2(lib.Base):
    def __init__(self):
        lib.Base.__init__(self)
        pass

b = lib.Base()
b.func_normal()
b.func_virtual()
print('*'*30)

d1 = lib.Derived()
d1.func_normal()
d1.func_virtual()
d1.func_new()
print('*'*30)


d2 = Derived2()
d2.func_normal()
d2.func_virtual()
print('*'*30)
```
```
Base::Base
Base::func_normal
Base::func_virtual
******************************
Base::Base
Derived::Derived
Base::func_normal
Derived::func_virtual
Derived::func_new
******************************
Base::Base
Base::func_normal
Base::func_virtual
******************************
Base::~Base
Derived::~Derived
Base::~Base
Base::~Base
```
- 可以发现各种功能都是正常的，C++中继承的子类可以使用自己新的virtual方法，同时Python中也能以Base为基类继续实现新的继承
### 多态ABC
- 当基类是ABC（即包含纯虚函数），基类就不能再绑定构造函数了，因为ABC不允许创建object
- 此时其实不需要按照[https://pybind11.readthedocs.io/en/stable/advanced/classes.html#classes](https://pybind11.readthedocs.io/en/stable/advanced/classes.html#classes)中的修改，也能实现C++转Python，但是在Python中就不能根据Base来创建新的子类了，原因是，如果想要在Python中继承Base创建新的子类，pybind11要求需要在新子类的构造函数中调用基类的构造函数，但前面说了，ABC在pybind11中不允许绑定自己的构造函数
- 如果实在要在Python中继承基类创建新的子类，那就去看上面的官方文档吧
- 似乎这样之后，ABC都能被创建了