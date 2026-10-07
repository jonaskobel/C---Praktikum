cmake_minimum_required(VERSION 3.16)
project(CppPraktikum LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(tag01_taschenrechner
    tag01/taschenrechner/main.cpp
    tag01/taschenrechner/taschenrechner.cpp
)

add_executable(tag01_maya_numbers
    tag01/maya_numbers/main.cpp
    tag01/maya_numbers/maya_numbers.cpp
)

add_executable(tag01_verfahren_von_heron
    tag01/verfahren_von_heron/main.cpp 
    tag01/verfahren_von_heron/verfahren_von_heron.cpp
)

add_executable(tag01_palindrome
    tag01/palindrome/main.cpp 
    tag01/palindrome/palindrome.cpp 
)

add_executable(tag01_buchstaben_und_woerter
    tag01/buchstaben_und_woerter/buchstaben_und_woerter.cpp
)

add_executable(tag01_quadratische_funktionen
    tag01/quadratische_funktionen/quadratische_funktionen.cpp 
)

add_executable(tag01_referenzen
    tag01/referenzen/referenzen.cpp
)

add_executable(tag01_speicher_und_arrays
    tag01/speicher_und_arrays/speicher_und_arrays.cpp
)

add_executable(tag01_bubble_sort
    tag01/bubble_sort/bubble_sort.cpp
)