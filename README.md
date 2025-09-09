# noodle
Standalone Reference Module for Common C++ Utilities

# Motive:

The motive behind this repository is to provide a sufficient means of displaying the common utilities I use across majority of my C++ projects. As such, these utils leverage C++ intrinsics and quality-of-life adjustments that make working within the language a lot easier.

As such, these utilities are often used to mitigate overhead when it comes to the line of projects/works that I do.

noodle is a means to maintain and refactor my core utilities/libraries in a single place with a consistent coding style throughout - hopefully making implementation a lot easier

## Features:

- templated pre-processors for debugging
- fixed size data types
- variadic type assertion for function prototypes
- modularisation of memory operations (read/writes, mapping)

## Usage:

This project utilises [fmt](https://github.com/fmtlib/fmt) so, you will be require to install this should it be applicable

From there, it is just a case of including the relevant header files and building alongside using your provided build scheme

For CMake, it would be a case of:

```cmake
find_package(noodle REQUIRED)

target_link_libraries(project_name PRIVATE noodle::noodle)
``` 

## Testing:

For testing utility adjacent from any preceeding project implementation, it's just a case of running
```
cmake -DNOODLE_TEST=ON ..

make

./noodle_tests.exe
```

<p align="center">
  <img src="https://github.com/user-attachments/assets/e7b0f8d1-f3dc-43cc-9d4b-98aa9e9e0af0" alt="gorillaz-dance">
</p>
