import cpp_module_inheritance as lib

class Derived2(lib.Base):
    def __init__(self):
        lib.Base.__init__(self)
        print("Derived2::Derived2 in Python")
        pass


b = lib.Base()
b.func_normal()
b.func_virtual()
# b.func_pure_virtual()
print('*'*30)


d1 = lib.Derived()
d1.func_normal()
d1.func_virtual()
d1.func_new()
d1.func_pure_virtual()
print('*'*30)


d2 = Derived2()
d2.func_normal()
d2.func_virtual()
print('*'*30)
