[Русский](./README_ru.md)
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
./Dictionary [--dont-store] [dictionary file]
```
If the `--dont-store` argument is present then the application doesn't store the dictionary in memory
and reads it from file for each search.\
If `dictionary file` is not specified then the program will try to open the `./words.txt` file.\
The order of arguments is important.\
[This file](https://raw.githubusercontent.com/dwyl/english-words/master/words.txt) was used in tests.

### Tested
* on Windows 10, MSVC 2019, with Qt 5.15.2 and Qt 6.0.3
* on Ubuntu 20.04, g++ 9.3.0, with Qt 6.0.3
  * with Qt 5, if Qt installed via `apt`

## Search Algorithms
The program uses a simplified version of ~~the Boyer-Moore-Horspool algorithm~~ **the Quick search algorithm**
  for substring detection.

*m - size of substring (needle), n - size of a string (haystack), s - size of an alphabet.*
 * The algorithm requires preprocessing, with time complexity of *O(m + s)*.
   We need to execute preprocessing for each new *needle*.
 * The algorithm itself has time complexity of *O(mn)*.

Each character is a byte, encoding doesn't matter, s = 2^8 = 256.

Algorithm has some simplifications:
 * Search runs for **one line**, not the full text
 * When one entry is found, the search stops

In source code: preprocessing - `Dictionary::quickSearchPreprocessing`, search - `Dictionary::quickSearchImplementation`

### Search of sequential characters (optional)
This search algorithm does not require preprocessing and it's time complexity is *O(n)*

In source code: search - `Dictionary::sequenceSearchImplementation`