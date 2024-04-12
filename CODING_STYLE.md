# Coding Style rules of the project

We use camelCase for variables names and function names, and PascalCase for classes.

We use 4 spaces to indent the code.

## Functions

### Declaration

```c++
/**
 * Description
 * @param   name1   Argument description
 * @param   name2   Argument description
 * ...
 * @return Description of the returned item
 */
type functionName(type name1, type name2, ...);
```

### Definition

```c++
type functionName(type name1, type name2, ...) {
    // Body
}
```

## Classes

```c++
class ClassName {
public:
    // Things

private:
    // Things
}
```

## Basic instructions

To differenciate from function calls, spaces are used before the parentheses of an `if`, `for` or `while` clause.

```c++
while (/* condition */) {
    // Body
}
```
```c++
switch (/* variable */) {
case A:
    // Things
    break;
default:
    // Things
    break;
}
```

