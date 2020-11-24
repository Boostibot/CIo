# CIo
C-style IO library

A simple header only library created to ease use of classic cstdio and increase general safety through compile time logic.

# Motivation 

Cstdio is a C library, as such it does not have any form of RAII protection, all checks and formatting happen at compile time and has very cryptic function names with unclear behavior. Furthermore, passing invalid arguments to some function will terminate the program (fopen mode).

The C++ iostream library attempts to solve this but it has its own set of problems. While RAII is accounted for and the class is much safer it makes a heavy use of exceptions which might make it impossible to use for embedded systems. It uses many virtual calls and most importantly has a problem with buffer [sizes higher than 1024](https://stackoverflow.com/a/48585805) making it unfit for large buffered outputs.

# Solutions

This library aims to solve all of the problems above by internally using cstdio but hiding its problematic parts behind abstractions. Classes use templates to safely read and write passed arguments. Opening file instead of mode string argument now takes OpenMode object which in the case of wrong input will simply fail and the function will return false. The checking is done entirely at compile time enabling the user to see potential problems while writing the code.

Apart from OS interaction (opening, creating, renaming, removing files) this library does not differentiate file wide/narrow orientation. All data is simply written to or read from the file. This allows for higher control over the data written and greater flexibility.

# Structure

There are two main classes from which all other classes can be reached. These classes are File and UnsafeFile. File is simply a safety wrapper around UnsafeFile. These classes have all the necessary functionality for working with files. They inherit from FileManager - a basic RAII manager. This allows the user to cast down and access the raw FILE pointer to take full control over the class. Furthermore, there is independent class OpenMode responsible for conversion of OpenModeFlags into usable c-mode string.

All of the listed classes are a template specialization of classes prepended with Basic (BasicFile, BasicOpenMode, BasicFileManager...) with the appropriate char type. As default File and others are char specializations. They can be also used in their wide alternatives which only changes the interaction with the OS. These classes are prepended with W (WFile, WOpenMode, WFileManager...).

# Tests

All functions are extensively tested. This was particularly difficult due to the nature of the project. A lot of effort was made to be as independent on the underlying OS as possible. All temporary files should be automatically cleaned up after each test.
