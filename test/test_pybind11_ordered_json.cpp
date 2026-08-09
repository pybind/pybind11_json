/***************************************************************************
* Copyright (c) 2019, Martin Renou                                         *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#include <string>
#include <vector>
#include <cmath>
#include <limits>

#include "gtest/gtest.h"

#include "pybind11_json/pybind11_ordered_json.hpp"

#include "pybind11/embed.h"

namespace py = pybind11;
namespace nl = nlohmann;

using namespace pybind11::literals;

inline py::module create_module(const std::string& module_name)
{
    return py::module_::create_extension_module(module_name.c_str(), nullptr, new py::module_::module_def);
}

TEST(nljson_serializers_to_orderedjson, none)
{
    py::scoped_interpreter guard;
    py::object obj1;
    py::object obj2 = py::none();
    nl::ordered_json j1 = obj1;
    nl::ordered_json j2 = obj2;
    ASSERT_TRUE(j1.is_null());
    ASSERT_TRUE(j2.is_null());
}

TEST(nljson_serializers_to_orderedjson, bool_)
{
    py::scoped_interpreter guard;
    py::bool_ obj(false);
    nl::ordered_json j = obj;

    ASSERT_TRUE(j.is_boolean());
    ASSERT_FALSE(j.get<bool>());
}

TEST(nljson_serializers_to_orderedjson, integer)
{
    py::scoped_interpreter guard;
    py::int_ obj(36);
    nl::ordered_json j = obj;

    ASSERT_TRUE(j.is_number_integer());
    ASSERT_EQ(j.get<int>(), 36);

    py::int_ obj_integer_min(std::numeric_limits<nl::ordered_json::number_integer_t>::min());
    nl::ordered_json j_integer_min = obj_integer_min;

    ASSERT_TRUE(j_integer_min.is_number_integer());
    ASSERT_EQ(j_integer_min.get<nl::ordered_json::number_integer_t>(), std::numeric_limits<nl::ordered_json::number_integer_t>::min());

    py::int_ obj_unsigned_max(std::numeric_limits<nl::ordered_json::number_unsigned_t>::max());
    nl::ordered_json j_unsigned_max = obj_unsigned_max;

    ASSERT_TRUE(j_unsigned_max.is_number_unsigned());
    ASSERT_EQ(j_unsigned_max.get<nl::ordered_json::number_unsigned_t>(), std::numeric_limits<nl::ordered_json::number_unsigned_t>::max());

    py::int_ obj_integer_border = py::int_(std::numeric_limits<nl::ordered_json::number_integer_t>::max()).attr("__add__")(1);
    nl::ordered_json j_integer_border = obj_integer_border;

    ASSERT_TRUE(j_integer_border.is_number_unsigned());
    ASSERT_EQ(j_integer_min.get<nl::ordered_json::number_unsigned_t>(), (nl::ordered_json::number_unsigned_t)(std::numeric_limits<nl::ordered_json::number_integer_t>::max()) + 1);

    py::int_ obj_small_outside = obj_integer_min.attr("__sub__")(1);
    ASSERT_THROW(nl::ordered_json j_small_outside = obj_small_outside, std::runtime_error);

    py::int_ obj_large_outside=obj_unsigned_max.attr("__add__")(1);
    ASSERT_THROW(nl::ordered_json j_large_outside = obj_large_outside, std::runtime_error);
}

TEST(nljson_serializers_to_orderedjson, float_)
{
    py::scoped_interpreter guard;
    py::float_ obj(36.37);
    nl::ordered_json j = obj;

    ASSERT_TRUE(j.is_number_float());
    ASSERT_EQ(j.get<double>(), 36.37);

    py::float_ obj_inf(INFINITY);
    nl::ordered_json j_inf = obj_inf;

    ASSERT_TRUE(j_inf.is_number_float());
    ASSERT_EQ(j_inf.get<double>(), INFINITY);

    py::float_ obj_nan(NAN);
    nl::ordered_json j_nan = obj_nan;

    ASSERT_TRUE(j_nan.is_number_float());
    ASSERT_TRUE(isnan(j_nan.get<double>()));
}

TEST(nljson_serializers_to_orderedjson, string)
{
    py::scoped_interpreter guard;
    py::str obj("Hello");
    nl::ordered_json j = obj;

    ASSERT_TRUE(j.is_string());
    ASSERT_EQ(j.get<std::string>(), "Hello");
}

TEST(nljson_serializers_to_orderedjson, list)
{
    py::scoped_interpreter guard;
    py::list obj;
    obj.append(py::int_(36));
    obj.append(py::str("Hello World"));
    obj.append(py::bool_(false));
    nl::ordered_json j = obj;

    ASSERT_TRUE(j.is_array());
    ASSERT_EQ(j[0].get<int>(), 36);
    ASSERT_EQ(j[1].get<std::string>(), "Hello World");
    ASSERT_EQ(j[2].get<bool>(), false);
}

TEST(nljson_serializers_to_orderedjson, empty_list)
{
    py::scoped_interpreter guard;
    py::list obj;
    nl::ordered_json j = obj;

    ASSERT_TRUE(j.is_array());
    ASSERT_TRUE(j.empty());
}

TEST(nljson_serializers_to_orderedjson, tuple)
{
    py::scoped_interpreter guard;
    py::tuple obj = py::make_tuple(1234, "hello", false);
    nl::ordered_json j = obj;

    ASSERT_TRUE(j.is_array());
    ASSERT_EQ(j[0].get<int>(), 1234);
    ASSERT_EQ(j[1].get<std::string>(), "hello");
    ASSERT_EQ(j[2].get<bool>(), false);
}

TEST(nljson_serializers_to_orderedjson, dict)
{
    py::scoped_interpreter guard;
    py::dict obj("number"_a=1234, "hello"_a="world");
    nl::ordered_json j = obj;

    ASSERT_TRUE(j.is_object());
    ASSERT_EQ(j["number"].get<int>(), 1234);
    ASSERT_EQ(j["hello"].get<std::string>(), "world");
}

TEST(nljson_serializers_to_orderedjson, empty_dict)
{
    py::scoped_interpreter guard;
    py::dict obj;
    nl::ordered_json j = obj;

    ASSERT_TRUE(j.is_object());
    ASSERT_TRUE(j.empty());
}

TEST(nljson_serializers_to_orderedjson, nested)
{
    py::scoped_interpreter guard;
    py::dict obj(
        "list"_a=py::make_tuple(1234, "hello", false),
        "dict"_a=py::dict("a"_a=12, "b"_a=13),
        "hello"_a="world",
        "world"_a=py::none()
    );
    nl::ordered_json j = obj;

    ASSERT_TRUE(j.is_object());
    ASSERT_EQ(j["list"][0].get<int>(), 1234);
    ASSERT_EQ(j["list"][1].get<std::string>(), "hello");
    ASSERT_EQ(j["list"][2].get<bool>(), false);
    ASSERT_EQ(j["dict"]["a"].get<int>(), 12);
    ASSERT_EQ(j["dict"]["b"].get<int>(), 13);
    ASSERT_EQ(j["world"], nullptr);
}

TEST(nljson_serializers_to_orderedjson, handle)
{
    py::scoped_interpreter guard;
    py::list obj;
    obj.append(py::make_tuple(1234, "hello", false));
    obj.append(py::dict("a"_a=12, "b"_a=13));
    obj.append("world");
    obj.append(py::none());

    for (py::handle handle : obj)
    {
        nl::ordered_json j = handle;

        ASSERT_TRUE(j.is_array() || j.is_object() || j.is_string() || j.is_null());
    }
}

TEST(nljson_serializers_to_orderedjson, list_accessor)
{
    py::scoped_interpreter guard;
    py::list obj;
    obj.append(py::make_tuple(1234, "hello", false));
    obj.append(py::dict("a"_a=12, "b"_a=13));
    obj.append("world");
    obj.append(py::none());

    nl::ordered_json j = obj[0];
    ASSERT_TRUE(j.is_array());

    j = obj[1];
    ASSERT_TRUE(j.is_object());

    j = py::make_tuple(1234, "hello", false)[0];
    ASSERT_TRUE(j.is_number_integer());
    ASSERT_EQ(j.get<int>(), 1234);
}

TEST(nljson_serializers_to_orderedjson, tuple_accessor)
{
    py::scoped_interpreter guard;
    py::tuple obj = py::make_tuple(1234, "hello", false);

    nl::ordered_json j = obj[0];
    ASSERT_TRUE(j.is_number_integer());
    ASSERT_EQ(j.get<int>(), 1234);

    j = obj[1];
    ASSERT_TRUE(j.is_string());
    ASSERT_EQ(j.get<std::string>(), "hello");
}

TEST(nljson_serializers_to_orderedjson, item_accessor)
{
    py::scoped_interpreter guard;
    py::dict obj = py::dict("a"_a=12, "b"_a="hello");

    nl::ordered_json j = obj["a"];
    ASSERT_TRUE(j.is_number());
    ASSERT_EQ(j.get<int>(), 12);

    j = obj["b"];
    ASSERT_TRUE(j.is_string());
    ASSERT_EQ(j.get<std::string>(), "hello");
}

TEST(nljson_serializers_to_orderedjson, str_attr_accessor)
{
    py::scoped_interpreter guard;
    py::module sys = py::module::import("sys");

    nl::ordered_json j = sys.attr("base_prefix");
    ASSERT_TRUE(j.is_string());
}

TEST(nljson_serializers_to_orderedjson, obj_attr_accessor)
{
    py::scoped_interpreter guard;
    py::module sys = py::module::import("sys");

    py::str base_prefix = "base_prefix";

    nl::ordered_json j = sys.attr(base_prefix);
    ASSERT_TRUE(j.is_string());
}

TEST(nljson_serializers_from_orderedjson, none)
{
    py::scoped_interpreter guard;
    nl::ordered_json j = "null"_json;
    py::object obj = j;

    ASSERT_TRUE(obj.is_none());
}

TEST(nljson_serializers_from_orderedjson, bool_)
{
    py::scoped_interpreter guard;
    nl::ordered_json j = "false"_json;
    py::object obj = j;

    ASSERT_TRUE(py::isinstance<py::bool_>(obj));
    ASSERT_FALSE(obj.cast<bool>());

    py::bool_ obj2 = j;

    ASSERT_FALSE(obj2.cast<bool>());
}

TEST(nljson_serializers_from_orderedjson, integer)
{
    py::scoped_interpreter guard;
    nl::ordered_json j = "36"_json;
    py::object obj = j;

    ASSERT_TRUE(py::isinstance<py::int_>(obj));
    ASSERT_EQ(obj.cast<int>(), 36);

    py::int_ obj2 = j;

    ASSERT_EQ(obj2.cast<int>(), 36);
}

TEST(nljson_serializers_from_orderedjson, integer_large_unsigned)
{
    // Note: if the asserts below error, the large number is printed as "-1" with
    // an overflow error. This is only in the output step in pybind. Calling
    // py::print on the objects shows the correct large unsigned integer.

    py::scoped_interpreter guard;
    uint64_t original = 13625394757606569013ull;
    py::int_ py_orig = original;
    nl::ordered_json j = original;
    py::object obj = j;

    ASSERT_TRUE(py::isinstance<py::int_>(obj));
    ASSERT_EQ(obj.cast<uint64_t>(), original);

    py::int_ obj2 = j;

    // Use .equal to compare values not pointers
    ASSERT_TRUE(obj2.equal(py_orig));
}

TEST(nljson_serializers_from_orderedjson, float_)
{
    py::scoped_interpreter guard;
    nl::ordered_json j = "36.2"_json;
    py::object obj = j;

    ASSERT_TRUE(py::isinstance<py::float_>(obj));
    ASSERT_EQ(obj.cast<double>(), 36.2);

    py::float_ f_obj = j;

    ASSERT_EQ(f_obj.cast<double>(), 36.2);

    nl::ordered_json j_inf(INFINITY);
    py::object obj_inf = j_inf;

    ASSERT_TRUE(py::isinstance<py::float_>(obj_inf));
    ASSERT_EQ(obj_inf.cast<double>(), INFINITY);

    py::float_ f_obj_inf = j_inf;

    ASSERT_EQ(f_obj_inf.cast<double>(), INFINITY);

    nl::ordered_json j_nan(NAN);
    py::object obj_nan = j_nan;

    ASSERT_TRUE(py::isinstance<py::float_>(obj_nan));
    ASSERT_TRUE(isnan(obj_nan.cast<double>()));

    py::float_ f_obj_nan = j_nan;

    ASSERT_TRUE(isnan(f_obj_nan.cast<double>()));
}

TEST(nljson_serializers_from_orderedjson, string)
{
    py::scoped_interpreter guard;
    nl::ordered_json j = "\"Hello World!\""_json;
    py::object obj = j;

    ASSERT_TRUE(py::isinstance<py::str>(obj));
    ASSERT_EQ(obj.cast<std::string>(), "Hello World!");

    py::str obj2 = j;

    ASSERT_EQ(obj2.cast<std::string>(), "Hello World!");
}

TEST(nljson_serializers_from_orderedjson, list)
{
    py::scoped_interpreter guard;
    nl::ordered_json j = "[1234, \"Hello World!\", false]"_json;
    py::object obj = j;

    ASSERT_TRUE(py::isinstance<py::list>(obj));
    ASSERT_EQ(py::list(obj)[0].cast<int>(), 1234);
    ASSERT_EQ(py::list(obj)[1].cast<std::string>(), "Hello World!");
    ASSERT_EQ(py::list(obj)[2].cast<bool>(), false);

    py::list obj2 = j;

    ASSERT_EQ(py::list(obj2)[0].cast<int>(), 1234);
    ASSERT_EQ(py::list(obj2)[1].cast<std::string>(), "Hello World!");
    ASSERT_EQ(py::list(obj2)[2].cast<bool>(), false);
}

TEST(nljson_serializers_from_orderedjson, dict)
{
    py::scoped_interpreter guard;
    nl::ordered_json j = "{\"a\": 1234, \"b\":\"Hello World!\", \"c\":false}"_json;
    py::object obj = j;

    ASSERT_TRUE(py::isinstance<py::dict>(obj));
    ASSERT_EQ(py::dict(obj)["a"].cast<int>(), 1234);
    ASSERT_EQ(py::dict(obj)["b"].cast<std::string>(), "Hello World!");
    ASSERT_EQ(py::dict(obj)["c"].cast<bool>(), false);

    py::dict obj2 = j;

    ASSERT_EQ(py::dict(obj2)["a"].cast<int>(), 1234);
    ASSERT_EQ(py::dict(obj2)["b"].cast<std::string>(), "Hello World!");
    ASSERT_EQ(py::dict(obj2)["c"].cast<bool>(), false);
}

TEST(nljson_serializers_from_orderedjson, nested)
{
    py::scoped_interpreter guard;
    nl::ordered_json j = R"({
        "baz": ["one", "two", "three"],
        "foo": 1,
        "bar": {"a": 36, "b": false},
        "hey": null
    })"_json;
    py::object obj = j;

    ASSERT_TRUE(py::isinstance<py::dict>(obj));
    ASSERT_TRUE(py::isinstance<py::list>(py::dict(obj)["baz"]));
    ASSERT_TRUE(py::isinstance<py::int_>(py::dict(obj)["foo"]));
    py::list baz = py::dict(obj)["baz"];
    py::int_ foo = py::dict(obj)["foo"];
    py::dict bar = py::dict(obj)["bar"];
    ASSERT_EQ(baz[0].cast<std::string>(), "one");
    ASSERT_EQ(baz[1].cast<std::string>(), "two");
    ASSERT_EQ(baz[2].cast<std::string>(), "three");
    ASSERT_EQ(foo.cast<int>(), 1);
    ASSERT_EQ(bar["a"].cast<int>(), 36);
    ASSERT_FALSE(bar["b"].cast<bool>());
    ASSERT_TRUE(py::dict(obj)["hey"].is_none());
}

TEST(nljson_serializers_to_orderedjson, retains_order)
{
    py::scoped_interpreter guard;
    py::dict obj;
    obj["zebra"] = 1;
    obj["apple"] = 2;
    obj["mango"] = 3;
    obj["banana"] = 4;

    nl::ordered_json j = obj;

    ASSERT_TRUE(j.is_object());
    ASSERT_EQ(j.size(), 4);

    auto it = j.begin();
    
    ASSERT_EQ(it.key(), "zebra");
    ASSERT_EQ(it.value().get<int>(), 1);
    ++it;
    
    ASSERT_EQ(it.key(), "apple");
    ASSERT_EQ(it.value().get<int>(), 2);
    ++it;
    
    ASSERT_EQ(it.key(), "mango");
    ASSERT_EQ(it.value().get<int>(), 3);
    ++it;
    
    ASSERT_EQ(it.key(), "banana");
    ASSERT_EQ(it.value().get<int>(), 4);
    ++it;
    
    ASSERT_TRUE(it == j.end());
}

TEST(nljson_serializers_from_orderedjson, retains_order)
{
    py::scoped_interpreter guard;
    
    nl::ordered_json j = nl::ordered_json::parse(R"({
        "zebra": 1,
        "apple": 2,
        "mango": 3,
        "banana": 4
    })");

    py::dict obj = j;

    ASSERT_TRUE(py::isinstance<py::dict>(obj));
    ASSERT_EQ(obj.size(), 4);

    py::list keys = py::list(obj.attr("keys")());
    
    ASSERT_EQ(keys[0].cast<std::string>(), "zebra");
    ASSERT_EQ(keys[1].cast<std::string>(), "apple");
    ASSERT_EQ(keys[2].cast<std::string>(), "mango");
    ASSERT_EQ(keys[3].cast<std::string>(), "banana");
}

inline const nl::ordered_json& test_fromto_orderedjson(const nl::ordered_json& json)
{
    return json;
}

TEST(pybind11_caster_to_orderedjson, dict)
{
    py::scoped_interpreter guard;
    py::module m = create_module("test");

    m.def("to_json", &test_fromto_orderedjson);

    // Simulate calling this binding from Python with a dictionary as argument
    py::dict obj("number"_a=1234, "hello"_a="world");
    nl::ordered_json j = m.attr("to_json")(obj);

    ASSERT_TRUE(j.is_object());
    ASSERT_EQ(j["number"].get<int>(), 1234);
    ASSERT_EQ(j["hello"].get<std::string>(), "world");
}

TEST(pybind11_caster_from_orderedjson, dict)
{
    py::scoped_interpreter guard;
    py::module m = create_module("test");

    m.def("from_json", &test_fromto_orderedjson);

    // Simulate calling this binding from Python, getting back a py::object
    py::dict obj("number"_a=1234, "hello"_a="world");
    py::dict j = m.attr("from_json")(obj);

    ASSERT_EQ(j["number"].cast<int>(), 1234);
    ASSERT_EQ(j["hello"].cast<std::string>(), "world");
}

TEST(pybind11_caster_to_orderedjson, recursive_dict)
{
    py::scoped_interpreter guard;
    py::module m = create_module("test");

    m.def("to_json", &test_fromto_orderedjson);

    // Simulate calling this binding from Python with a dictionary as argument
    py::dict obj_inner("number"_a=1234, "hello"_a="world");
    py::dict obj;
    obj["first"] = obj_inner;
    obj["second"] = obj_inner;

    ASSERT_NO_THROW(m.attr("to_json")(obj));

    obj["second"]["recur"] = obj_inner;
    ASSERT_ANY_THROW(m.attr("to_json")(obj));
}
