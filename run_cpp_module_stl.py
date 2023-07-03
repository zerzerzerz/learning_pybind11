import cpp_module_stl

a = [1,2,3,4]
print("Before calling {}, a = {}".format(cpp_module_stl.change_list.__name__, a))
cpp_module_stl.change_list(a)
print("After  calling {}, a = {}".format(cpp_module_stl.change_list.__name__, a))

my_list = cpp_module_stl.MyList(4)
print("\nBefore call list.append in Python, my_list.list is {}".format(my_list.list))
my_list.list.append(5)
my_list.list.extend([6,7])
print("After  call list.append in Python, my_list.list is {}".format(my_list.list))


a = [1,2,3]
print("Wrap Python list to py::list")
print("\nBefore calling {}, a = {}".format(cpp_module_stl.change_pylist.__name__, a))
cpp_module_stl.change_pylist(a)
print("After  calling {}, a = {}".format(cpp_module_stl.change_pylist.__name__, a))


a = [1,2,3]
print("\nBefore calling {}, a = {}".format(cpp_module_stl.change_pylist_reference.__name__, a))
cpp_module_stl.change_pylist_reference(a)
print("After  calling {}, a = {}".format(cpp_module_stl.change_pylist_reference.__name__, a))


cpp_module_stl.print_dict({
    "zhangsan": 18,
    "lisi": 19,
})

print("\nassignment of list")
my_list = cpp_module_stl.MyList(4)
my_list.display()
print(my_list[0])
my_list[0] = 432432
my_list.display()


print("\nAdd of list")
list1 = cpp_module_stl.MyList(4)
list2 = cpp_module_stl.MyList(3)
list3 = list1 + list2
list3[4] = 923
list1.display()
list2.display()
list3.display()



print("\nRead and write entirelty")
my_list = cpp_module_stl.MyList(3)
print("After init is {}".format(my_list.display()))
my_list.list = [1,2,3,4,5]
print("After read and write entirely is {}".format(my_list.display()))
my_list.list.extend([6,7])
print("After Python list.extend() is {}, not change!".format(my_list.display()))
my_list.display()


print("Opaque float vector")
float_vector = cpp_module_stl.FloatVector()
print(dir(float_vector))
float_vector.append(345.1)
print("length is {}".format(len(float_vector)))
print(float_vector[0])
float_vector[0] += 32
print(float_vector[0])
for i in float_vector:
    print(i)