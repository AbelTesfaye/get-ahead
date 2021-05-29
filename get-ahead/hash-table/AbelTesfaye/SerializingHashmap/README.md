# Introduction
This project is a single header implementation of a hash table that can be used on any data type. It works by exposing a function that allows the user to convert their data type (serialize) to a string, then uses this string as a key in the hash table.

It supports resizing (growing and shrinking) as well as collision handling. The algorithm used for collision handling is a slightly modified version of [Linear Probing]([//TODO](https://en.wikipedia.org/wiki/Linear_probing))

# Motivation
It is learning exercise initiated by [@vterron](https://github.com/vterron) on the Get Ahead Africa 2021 slack channel.

# Example useage
Check the file comments inside `include/SerializingHashMap.h`

# Testing instructions
0) Install gtest. You can find the instructions [here](https://github.com/google/googletest/blob/master/googletest/README.md)

1) `git clone git@github.com:vterron/get-ahead.git`
2) `cd get-ahead/get-ahead/hash-table/AbelTesfaye/SerializingHashmap/`
3) `mkdir build && cd build`
4) `cmake -GNinja -DCMAKE_CXX_FLAGS="-fdiagnostics-color=always" ..`
5) `ninja`
6) `./SerializingHashMap`