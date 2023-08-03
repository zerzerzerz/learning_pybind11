import cpp_module_test

b = cpp_module_test.B()
for _ in range(4):
    b.add_A(cpp_module_test.create_A())
    # b.add_A(cpp_module_test.A())
b.show_all_A()