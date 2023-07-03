# Summary
## 参数传递
- pybind中绑定某个C++ class，Python中object往C++某个函数中传的时候，就按照参数描述的来传递

```C++
// 传递value，Python中的object不会被修改
void add_node(Node n){
    n.value += 1;
}

// 传递reference，Python中的object会被修改
void add_node_reference(Node& n){
    n.value += 1;
}
```
- 传递C++中自定义class的指针
  - Python往里传的时候，直接把object扔进去就OK
  - 注意是自定义的class
- STL中常见模板支持和Python的list, dict, set互相转换，但是无论C++中传value还是reference，都不会影响Python中的值。以Python中list传到C++ vector为例，首先转换为vector，这一步是复制操作！！！
```C++
// 这样不会改变Python中的list
void change_list(vector<int>& v){
    for(int i=0; i<v.size(); ++i){
        v[i] = -1;
    }
}
```
```Python
# 这样不会改变Python中的list
a = [1,2,3,4]
module_stl.change_list(a)
print(a)
```
```C++
// 这样不会改变Python中的list
class MyList{
    public:
        vector<int> list;
        MyList(int n){
            for(int i=0; i<n; ++i){
                list.push_back(i);
            }
        }
};
```
```Python
# 这样不会改变Python中的list
my_list = module_stl.MyList(4)
print("Before append, my_list.list is {}".format(my_list.list))
my_list.list.append(5)
my_list.list.extend([6,7])
print("After  append, my_list.list is {}".format(my_list.list))
```
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