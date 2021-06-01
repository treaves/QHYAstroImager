# QHYAstroImager
An application for imaging with QHYCCD cameras.

#Building
![example workflow](https://github.com/treaves/QHYAstroImager/actions/workflows/cmake.yml/badge.svg)
## General Requirements
Install the following with your platforms package manager.  The QHYCCD driver may need to be installed manually.
- CMake >=3.15
- Qt >= 5.10
- QHYCCD SDK
- cfitsio

Once you've cloned the repository, from the root, run the following commands:
```sh
mkdir -p builds/unix
cd builds/unix
cmake ../..
make
```
Feel free to replace `unix` with any name you want; I juzt use that to mean _not IDE based_.

##Mac/Linux
If the dependencies are installed in non-standard locations, you may need to update the `CMAKE_MODULE_PATH` in the `Dependencies` section of the root `CMakeLists.txt` file. 

##Windows
Until I test on Windows, I've no idea where CMake will look for the files it needs.  If packages are not found, you may need to update the `CMAKE_MODULE_PATH` in the `Dependencies` section of the root `CMakeLists.txt` file.