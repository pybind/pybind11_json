#include "../include/pybind11_json/pybind11_json.hpp"
#include <iostream>

void print_pyobject_as_json(nlohmann::json s) {
    std::cout << "print_pyobject_as_json(): " << s << std::endl;
}
nlohmann::json return_json_as_pyobject() {
    nlohmann::json j = {{"pi", 3.141}};
    std::cout << "return_json_as_pyobject() : "  << j<< std::endl;
    return j;
}

//=================================
PYBIND11_MODULE(pyjson, m) {
    m.doc() = "pyjson module"; // optional module docstring

    /* other choice:  wrap this json C++ type,  but it is a complicate class,    
    py::class_<nlohmann::json>(m, "json")
        .def(py::init<>())
        .def(py::init<py::object>())
    ; */

    //py::implicitly_convertible<py::object, nlohmann::json>();  // error
    //ImportError: implicitly_convertible: Unable to find type nlohmann::basic_json

    // Dummy json
    m.def("dummy_json", []() {
        const nlohmann::json j = {
            {"pi", 3.141},
            {"happy", true},
            {"name", "Niels"},
            {"nothing", nullptr},
            {"answer", {
                {"everything", 42}
            }},
            {"list", {1, 0, 2}},
            {"object", {
                {"currency", "USD"},
                {"value", 42.99}
            }}
        };
        //return j;
        py::object out = j;
        return out;
    }, "returns a dummy json object");

    m.def("print_json", [](const py::object in) {
        const nlohmann::json j = in;
        std::cout << j;
    }, "pass py::object to c++");

    m.def("print_pyobject_as_json", &print_pyobject_as_json, "pass py::object to c++");
    m.def("return_json_as_pyobject", &return_json_as_pyobject, "get py::object from c++");

}