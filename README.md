[![Travis](https://travis-ci.org/pybind/pybind11_json.svg?branch=master)](https://travis-ci.org/pybind/pybind11_json)
[![Build status](https://ci.appveyor.com/api/projects/status/d31t6dytkfm28tfo?svg=true)](https://ci.appveyor.com/project/pybind/pybind11-json/branch/master)

# pybind11_json

## Automatic conversion between `nlohmann::json` and `pybind11::object`
`pybind11_json` is an `nlohmann::json` to `pybind11` bridge, it allows you to automatically convert `nlohmann::json` to `py::object` and the other way around:

```cpp
#include "pybind11_json/pybind11_json.hpp"

#include "nlohmann/json.hpp"

#include "pybind11/pybind11.h"

namespace py = pybind11;
namespace nl = nlohmann;
using namespace pybind11::literals;

py::object obj = py::dict("number"_a=1234, "hello"_a="world");

nl::json j = obj;  // Automatic py::object->nl::json conversion
py::object o = j;  // Automatic nl::json->py::object conversion
```

##  conversion between nl::json and python dict object

C++ functions in [test/pybind11_json_module.cpp](test/pybind11_json_module.cpp)

```
void print_pyobject_as_json(nlohmann::json s) {
    std::cout << "print_pyobject_as_json(): " << s << std::endl;
}
nlohmann::json return_json_as_pyobject() {
    nlohmann::json j = {{"value", 1}};
    std::cout << "return_json_as_pyobject() : "  << j<< std::endl;
    return j;
}
```

pybind11 binding code, see more in [test/pybind11_json_module.cpp](test/pybind11_json_module.cpp)
```cpp
m.def("print_pyobject_as_json", &print_pyobject_as_json, "pass py::object to c++");
m.def("return_json_as_pyobject", &return_json_as_pyobject, "get py::object from c++");
```

on python side, see [test/test_pybind11_json.py](test/test_pybind11_json.py)
```python
import pyjson
pyjson.print_pyobject_as_json({"value": 2})
dd = pyjson.return_json_as_pyobject()
print(dd)
```

# Installation

## Using conda

You can install `pybind11_json` using conda

```bash
conda install -c conda-forge pybind11 nlohmann_json pybind11_json
```

## From sources

We encourage you to use conda for installing dependencies, but it is not a requirement for `pybind11_json` to work

```bash
conda install cmake nlohmann_json pybind11 -c conda-forge
```

Then you can install the sources

```bash
cmake -D CMAKE_INSTALL_PREFIX=your_conda_path
make install
```

## Header only usage
Download the "pybind11_json.hpp", "json.hpp" single file into your project, and install/download pybind11 or use as git submodule. 

Check header include path for `json.hpp` and `pybind11`, then you are ready to go.


## Run tests

You can compile and run tests locally doing

```bash
cmake -D CMAKE_INSTALL_PREFIX=$CONDA_PREFIX -D DOWNLOAD_GTEST=ON ..
make
./test/test_pybind11_json
```

# Dependencies

``pybind11_json`` depends on

 - [pybind11](https://github.com/pybind/pybind11)
 - [nlohmann_json](https://github.com/nlohmann/json)


| `pybind11_json`| `nlohmann_json` | `pybind11`      |
|----------------|-----------------|-----------------|
|  master        | >=3.2.0,<4.0    | >=2.2.4,<3.0    |
|  0.2.3         | >=3.2.0,<4.0    | >=2.2.4,<3.0    |
|  0.2.2         | >=3.2.0,<4.0    | >=2.2.4,<3.0    |
|  0.2.1         | >=3.2.0,<4.0    | >=2.2.4,<3.0    |
|  0.2.0         | >=3.2.0,<4.0    | >=2.2.4,<3.0    |
|  0.1.0         | >=3.2.0,<4.0    | >=2.2.4,<3.0    |


# License

We use a shared copyright model that enables all contributors to maintain the
copyright on their contributions.

This software is licensed under the BSD-3-Clause license. See the [LICENSE](LICENSE) file for details.
