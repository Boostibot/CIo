# CIo
C-style IO library

A header only library created to ease use of classic cstdio and increase general safety through compile time logic.

# Motivation 

Cstdio is a C library, as such it does not have any form of RAII protection, all checks and formating happens at compile time and has very criptic function names with unclear functionality. The C++ iostream library attempts to solve this but at has its own set of problems. While RAII is accounted for and the class is alot safer it makes a heavy use of exceptions which might make it impossible to use for some embedded systems.
