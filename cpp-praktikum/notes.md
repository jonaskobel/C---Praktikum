cd cpp-praktikum. # zum ordner navigieren
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build 
./build/tag01_taschenrechner

oder 

cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-release
./build-release/tag01_taschenrechner