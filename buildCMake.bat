@echo off

echo cmake_minimum_required(VERSION 3.10) > CMakeLists.txt
echo project(Kits) >> CMakeLists.txt
echo add_executable(KitsCMake Kits.cpp) >> CMakeLists.txt
mkdir KitsCMake
cd KitsCmake
cmake ../
cmake --build .