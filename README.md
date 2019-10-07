[![Travis](https://travis-ci.org/pybind/pybind11_json.svg?branch=master)](https://travis-ci.org/pybind/pybind11_json)
[![Build status](https://ci.appveyor.com/api/projects/status/d31t6dytkfm28tfo?svg=true)](https://ci.appveyor.com/project/pybind/pybind11-json/branch/master)

## pybind11_json

`pybind11_json` is an `nlohmann::json` to `pybind11` bridge, it allows you to automatically convert `nlohmann::json` to `py::object` and the other way around:

```CPP
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

## Installation from source

We encourage you to use conda for installing dependencies, but it is not a requirement for `pybind11_json` to work.

You can install it from the sources, you will first need to install dependencies

```bash
conda install cmake nlohmann_json pybind11 -c conda-forge
```

Then you can install the sources

```bash
cmake -D CMAKE_INSTALL_PREFIX=your_conda_path
make install
```

## Dependencies

``pybind11_json`` depends on

 - [pybind11](https://github.com/pybind/pybind11)
 - [nlohmann_json](https://github.com/nlohmann/json)


| `pybind11_json`| `nlohmann_json` | `pybind11`      |
|----------------|-----------------|-----------------|
|  master        | >=3.2.0,<4.0    | >=2.2.4,<3.0    |
|  0.2.2         | >=3.2.0,<4.0    | >=2.2.4,<3.0    |
|  0.2.1         | >=3.2.0,<4.0    | >=2.2.4,<3.0    |
|  0.2.0         | >=3.2.0,<4.0    | >=2.2.4,<3.0    |
|  0.1.0         | >=3.2.0,<4.0    | >=2.2.4,<3.0    |


## License

We use a shared copyright model that enables all contributors to maintain the
copyright on their contributions.

This software is licensed under the BSD-3-Clause license. See the [LICENSE](LICENSE) file for details.
