![Tests](https://github.com/pybind/pybind11_json/workflows/Tests/badge.svg)

# pybind11_json
`pybind11_json` is an `nlohmann::json` to `pybind11` bridge, it allows you to automatically convert `nlohmann::json` to `py::object` and the other way around. Simply include the header, and the automatic conversion will be enabled.

## C++ API: Automatic conversion between `nlohmann::json` and `pybind11` Python objects

```cpp
#include "pybind11_json/pybind11_json.hpp"

#include "nlohmann/json.hpp"

#include "pybind11/pybind11.h"

namespace py = pybind11;
namespace nl = nlohmann;
using namespace pybind11::literals;

py::dict obj = py::dict("number"_a=1234, "hello"_a="world");

// Automatic py::dict->nl::json conversion
nl::json j = obj;

// Automatic nl::json->py::object conversion
py::object result1 = j;
// Automatic nl::json->py::dict conversion
py::dict result2 = j;
```

## Making bindings

You can easily make bindings for C++ classes/functions that make use of `nlohmann::json`.

For example, making a binding for the following two functions is automatic, thanks to `pybind11_json`

```cpp
void take_json(const nlohmann::json& json) {
    std::cout << "This function took an nlohmann::json instance as argument: " << s << std::endl;
}

nlohmann::json return_json() {
    nlohmann::json j = {{"value", 1}};

    std::cout << "This function returns an nlohmann::json instance: "  << j << std::endl;

    return j;
}
```

Bindings:

```cpp
PYBIND11_MODULE(my_module, m) {
    m.doc() = "My awesome module";

    m.def("take_json", &take_json, "pass py::object to a C++ function that takes an nlohmann::json");
    m.def("return_json", &return_json, "return py::object from a C++ function that returns an nlohmann::json");
}
```

You can then use your functions Python side:

```python
import my_module

my_module.take_json({"value": 2})
j = my_module.return_json()

print(j)
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
Download the "pybind11_json.hpp" single file into your project, and install/download `pybind11` and `nlohmann_json` or use as git submodule.


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
|  0.2.10        | >=3.2.0,<4.0    | >=2.2.4,<3.0    |
|  0.2.9         | >=3.2.0,<4.0    | >=2.2.4,<3.0    |
|  0.2.8         | >=3.2.0,<4.0    | >=2.2.4,<3.0    |
|  0.2.7         | >=3.2.0,<4.0    | >=2.2.4,<3.0    |
|  0.2.6         | >=3.2.0,<4.0    | >=2.2.4,<3.0    |
|  0.2.5         | >=3.2.0,<4.0    | >=2.2.4,<3.0    |
|  0.2.4         | >=3.2.0,<4.0    | >=2.2.4,<3.0    |
|  0.2.3         | >=3.2.0,<4.0    | >=2.2.4,<3.0    |
|  0.2.2         | >=3.2.0,<4.0    | >=2.2.4,<3.0    |
|  0.2.1         | >=3.2.0,<4.0    | >=2.2.4,<3.0    |
|  0.2.0         | >=3.2.0,<4.0    | >=2.2.4,<3.0    |


# License

We use a shared copyright model that enables all contributors to maintain the
copyright on their contributions.

This software is licensed under the BSD-3-Clause license. See the [LICENSE](LICENSE) file for details.
