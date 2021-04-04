# Dictionary

Searches word in the given dictionary

Has a dependence on Qt. Can be built with Qt5 and with Qt6.

## Build
Required CMake version: 3.17
Required

It's required to define `CMAKE_PREFIX_PATH` as math to Qt.\
Run in the repository root:
```shell
mkdir build
cd build
```
and then
```
cmake -DCMAKE_PREFIX_PATH=path\to\Qt\lib\cmake ..
cmake --build . --config Release
```
to build with **Qt5**, *or*
```shell
cmake -DCMAKE_PREFIX_PATH=path\to\Qt\lib\cmake -DQT6=1 ..
cmake --build . --config Release
```
to build with **Qt6**.
## Configuration
To run the program execute the command:
```
Dictionary [dictionary_file]
```
If `[dictionary_file]` is not specified then the program will try to open `words.txt` file.