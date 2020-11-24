# CIo
C-style IO library

A header only library created to ease use of classic cstdio and increase general safety through compile time logic.

# Motivation 

Cstdio is a C library, as such it does not have any form of RAII protection, all checks and formating happens at compile time and has very criptic function names with unclear functionality. Furthermore passing invalid arguments to some function will terminate the program (fopen mode). The C++ iostream library attempts to solve this but at has its own set of problems. While RAII is accounted for and the class is much safer it makes a heavy use of exceptions which might make it impossible to use for embedded systems. It uses many virtual calls and most importantly has a problem with buffer [sizes higher than 1024](https://stackoverflow.com/a/48585805) making it unfit for large buffered outputs.

# Implementation

This library aims to solve all of the problems above by internaly using cstdio but hiding its problematic parts behind abstractions. Classes use templates to safely read and write passed arguments. Openning file instead of mode string argument now takes OpenMode object which in the case of wrong input will simply fail and the function will return false. The checking is done entirely in compile time enabling the user to see potential problmes while writting the code. 
