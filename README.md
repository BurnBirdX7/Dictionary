# Dictionary

Searches word in the given dictionary

Has a dependence on Qt. Can be built with **Qt5** and with **Qt6**.

## Build
Required CMake version: 3.17

It's required to define `CMAKE_PREFIX_PATH` as math to Qt.\
Run in the repository root:
```shell
mkdir build
cd build
```
and then generate project with
```shell
cmake ..
```
You can specify `-DUSE_QT6=ON` if you want to build the project with **Qt6** instead of **Qt5**.\
Sometimes you have to explicitly specify `-DUSE_QT6=OFF`.

Also you may need to set `CMAKE_PREFIX_PATH` variable.\
`-DCMAKE_PREFIX_PATH="path/to/Qt/lib/cmake"` or\
`-DCMAKE_PREFIX_PATH=/usr/include/{host}/qt{version}/` on Ubuntu.

```shell
cmake --build . --config Release
```
to build the project.


## Configuration
To run the program execute the command:
```shell
./Dictionary [--dont_store] [dictionary_file]
```
If `--dont_store` argument is present then the application doesn't store the dictionary in memory
and reads it from file for each search.\
If `dictionary_file` is not specified then the program will try to open `./words.txt` file.\
Order of arguments is important.\
[This file](https://raw.githubusercontent.com/dwyl/english-words/master/words.txt) was used in tests.

## Tested
* on Windows 10 with Qt 5.15.2 and Qt 6.0.3
* on Ubuntu 20.04 with Qt 6.0.3
  * with Qt 5, if Qt installed with `apt` manager (`sudo apt install qt5-default`)
    
## Algorithm
The program uses the simplified Boyer-Moore-Horspool algorithm for substring detection.