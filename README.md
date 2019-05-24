[![Travis](https://travis-ci.org/martinRenou/pybind_json.svg?branch=master)](https://travis-ci.org/martinRenou/pybind_json)
[![Build status](https://ci.appveyor.com/api/projects/status/or79ptqnhm37rgm0/branch/master?svg=true)](https://ci.appveyor.com/project/martinRenou/pybind-json/branch/master)

## pybind_json

`pybind_json` is an `nlohmann::json` to `pybind11` bridge, it allows you to automatically convert `nlohmann::json` to `py::object` and the other way around:

```CPP
#include "pybind_json/pybind_json.hpp"

#include "nlohmann/json.hpp"

#include "pybind11/pybind11.h"

namespace py = pybind11;
namespace nl = nlohmann;
using namespace pybind11::literals;

nl::json j = py::dict("number"_a=1234, "hello"_a="world");
py::object o = j;
```

## Installation from source

We encourage you to use conda, but it is not a requirement for `pybind_json` to work.

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

``pybind_json`` depends on

 - [pybind11](https://github.com/pybind/pybind11)
 - [nlohmann_json](https://github.com/nlohmann/json)


| `pybind_json`| `nlohmann_json` | `pybind11`      |
|--------------|-----------------|-----------------|
|  master      | >=3.2.0,<4.0    | >=2.2.4,<3.0    |
|  0.1.0       | >=3.2.0,<4.0    | >=2.2.4,<3.0    |


## License

We use a shared copyright model that enables all contributors to maintain the
copyright on their contributions.

This software is licensed under the BSD-3-Clause license. See the [LICENSE](LICENSE) file for details.
