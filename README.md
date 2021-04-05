# Dictionary

Searches entries of a substring in a given dictionary

Has a dependence on Qt. Can be built with **Qt5** or **Qt6**.

## Build
Required CMake version: **3.17**

Run in the repository root:
```shell
mkdir build
cd build
```
and then generate the project with
```shell
cmake ..
```
You can specify `-DUSE_QT6=ON` if you want to build the project with **Qt6** instead of **Qt5**.\
In some cases, you will have to explicitly specify `-DUSE_QT6=OFF`
(e.g. if CMake cache is already generated with the option enabled)\
Also, you may need to set the `CMAKE_PREFIX_PATH` variable.\
`-DCMAKE_PREFIX_PATH="path/to/Qt/lib/cmake"` or\
`-DCMAKE_PREFIX_PATH=/usr/include/{host}/qt{version}/` on Ubuntu.

```shell
cmake --build . --config Release
```
to build the project.


## Run the app
To run the program execute the command:
```shell
./Dictionary [--dont_store] [dictionary_file]
```
If the `--dont_store` argument is present then the application doesn't store the dictionary in memory
and reads it from file for each search.\
If `dictionary_file` is not specified then the program will try to open the `./words.txt` file.\
The order of arguments is important.\
[This file](https://raw.githubusercontent.com/dwyl/english-words/master/words.txt) was used in tests.

### Tested
* on Windows 10 with Qt 5.15.2 and Qt 6.0.3
* on Ubuntu 20.04 with Qt 6.0.3
  * with Qt 5, if Qt installed with `apt` manager (`sudo apt install qt5-default`)

## Algorithm
The program uses a simplified version of the Boyer-Moore-Horspool algorithm for substring detection.
