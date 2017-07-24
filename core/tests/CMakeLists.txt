project(tests)
cmake_minimum_required(VERSION 3.0)

set(PATHES
collection/frcollection.cpp
type/frtype.cpp
factory/frfactory.cpp
)

UnitTests("${PATHES}" FeerieCore)
