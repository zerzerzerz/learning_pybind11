import cpp_module_tree

print()
print("Create a local variable in C++ function, but return its address")
print("Python will get None")
n = cpp_module_tree.create_crush_node("crush")
print("Object in Python is {}".format(n))
n = None


print()
print("Create a dynamic variable in C++ function, and return its address")
print("py::return_value_policy::take_ownership")
print("Python side reference to this object, and take its ownership")
n = cpp_module_tree.create_dynamic_node_take_ownership("dynamic_take_ownership")
print(n)
n = None
print("Note that Python takes its ownership and calls destructor")


print()
print("Create a dynamic variable in C++ function, and return its address")
print("py::return_value_policy::reference")
print("Python side references to this object, but does not take its ownership")
n = cpp_module_tree.create_dynamic_node_reference("dynamic_take_reference")
print(n)
n = None
print("Note that Python does not take its ownership, and C++ side does not call destructor, leading to a memory leak")




print()
tree = cpp_module_tree.Tree(7)
print("\ntree.node_list[0] is tree.root? {}".format(tree.node_list[0] is tree.root))
tree.node_list[0].name = "change_root_name_via_node_list[0]"
print(tree.node_list[0].name)
print(tree.root.name)



print("\nAccess root via def_readwrite, py::return_value_policy::reference_internal")
print("This is a reference, e.g., both root.name & tree.root.name changed")
root = tree.root
root.name = "Ruizhe Zhong"
print("tree.root.name = {}".format(tree.root.name))
print("root.name = {}".format(root.name))
root = None

print("\nGet root node via vector")
print("\npy::return_value_policy::reference, Python will not free memory")
root = tree.get_root_via_vector_reference()
root = None

print("\npy::return_value_policy::copy, Python will free memory, but free a copy object")
root = tree.get_root_via_vector_copy()
root = None

print("\npy::return_value_policy::move, Python will free memory, but free a copy object")
root = tree.get_root_via_vector_move()
root = None


print("\npy::return_value_policy::take_ownership, Python will free memory, and this is a reference")
print("~Tree() will also free this memory, core dump!")
print("root is a vector containing Node*, I guess free vector will also free its elements")
root = tree.get_root_via_vector_take_ownership()
root = None