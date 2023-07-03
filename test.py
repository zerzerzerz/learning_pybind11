import copy
import numpy as np


class Node:
    def __init__(self) -> None:
        self.data = None

a = [Node(), 2, 3]
b = [0, 0]

# object is reference
# int is immutable
b[0] = a[0]
b[1] = a[1]

# both are True
print(b[0] is a[0])
print(b[1] is a[1])

# int is immutable, they do not point to the same location
a[1] += 1
print(b[1] is a[1])

# object is still reference
b[0].data = 'object in list is reference'
print(b[1] is a[0])

# deepcopy, completely independent
c = copy.deepcopy(a)
c[0].data = "deepcopy with copy.deepcopy()"
print(a[0].data)

# shadow copy
c = copy.copy(a)
c[0].data = "shallow copy with copy.copy()"
print(a[0].data)


# slice is shallow copy
c = a[:]
print("slice is copy? {}".format(not c is a))

# slice is a shallow copy
c[0].data = "slice is shallow copy"
print(a[0].data)

# assignment
d = a
print("assignment is reference? {}".format(d is a))



print("slice in numpy is reference")
a = np.zeros((4,4))
b = a[1:3, 1:3]
b += 1
print(a)