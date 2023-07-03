import cpp_module_numpy
import numpy as np

m = cpp_module_numpy.Matrix(3,4)
n = np.array(m, copy=False)

n[2,2] = 123
print(m)