# lambda expression in C++
```C++
[ capture clause ] (args) -> return_type {
    function definition
}
```
or
```C++
[ capture clause ] (args) {
    function definition
}
```
- capture external variables
  - `[&]` capture all external variables by reference
  - `[=]` capture all external variables by value
  - `[a, &b]` capture all a by value, b by reference
  - `[ ]` cannot capture external variables