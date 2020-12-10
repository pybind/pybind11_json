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

#include "gtest/gtest.h"

#include "pybind11_json/pybind11_json.hpp"

#include "pybind11/embed.h"

namespace py = pybind11;
namespace nl = nlohmann;

using namespace pybind11::literals;

TEST(nljson_serializers_tojson, none)
{
    py::scoped_interpreter guard;
    py::object obj1;
    py::object obj2 = py::none();
    nl::json j1 = obj1;
    nl::json j2 = obj2;

    ASSERT_TRUE(j1.is_null());
    ASSERT_TRUE(j2.is_null());
}

TEST(nljson_serializers_tojson, bool_)
{
    py::scoped_interpreter guard;
    py::bool_ obj(false);
    nl::json j = obj;

    ASSERT_TRUE(j.is_boolean());
    ASSERT_FALSE(j.get<bool>());
}

TEST(nljson_serializers_tojson, integer)
{
    py::scoped_interpreter guard;
    py::int_ obj(36);
    nl::json j = obj;

    ASSERT_TRUE(j.is_number_integer());
    ASSERT_EQ(j.get<int>(), 36);
}

TEST(nljson_serializers_tojson, float_)
{
    py::float_ obj(36.37);
    nl::json j = obj;

    ASSERT_TRUE(j.is_number_float());
    ASSERT_EQ(j.get<double>(), 36.37);

    py::float_ obj_inf(INFINITY);
    nl::json j_inf = obj_inf;

    ASSERT_TRUE(j_inf.is_number_float());
    ASSERT_EQ(j_inf.get<double>(), INFINITY);

    py::float_ obj_nan(NAN);
    nl::json j_nan = obj_nan;

    ASSERT_TRUE(j_nan.is_number_float());
    ASSERT_TRUE(isnan(j_nan.get<double>()));
}

TEST(nljson_serializers_tojson, string)
{
    py::scoped_interpreter guard;
    py::str obj("Hello");
    nl::json j = obj;

    ASSERT_TRUE(j.is_string());
    ASSERT_EQ(j.get<std::string>(), "Hello");
}

TEST(nljson_serializers_tojson, list)
{
    py::scoped_interpreter guard;
    py::list obj;
    obj.append(py::int_(36));
    obj.append(py::str("Hello World"));
    obj.append(py::bool_(false));
    nl::json j = obj;

    ASSERT_TRUE(j.is_array());
    ASSERT_EQ(j[0].get<int>(), 36);
    ASSERT_EQ(j[1].get<std::string>(), "Hello World");
    ASSERT_EQ(j[2].get<bool>(), false);
}

TEST(nljson_serializers_tojson, empty_list)
{
    py::scoped_interpreter guard;
    py::list obj;
    nl::json j = obj;

    ASSERT_TRUE(j.is_array());
    ASSERT_TRUE(j.empty());
}

TEST(nljson_serializers_tojson, tuple)
{
    py::scoped_interpreter guard;
    py::tuple obj = py::make_tuple(1234, "hello", false);
    nl::json j = obj;

    ASSERT_TRUE(j.is_array());
    ASSERT_EQ(j[0].get<int>(), 1234);
    ASSERT_EQ(j[1].get<std::string>(), "hello");
    ASSERT_EQ(j[2].get<bool>(), false);
}

TEST(nljson_serializers_tojson, dict)
{
    py::scoped_interpreter guard;
    py::dict obj("number"_a=1234, "hello"_a="world");
    nl::json j = obj;

    ASSERT_TRUE(j.is_object());
    ASSERT_EQ(j["number"].get<int>(), 1234);
    ASSERT_EQ(j["hello"].get<std::string>(), "world");
}

TEST(nljson_serializers_tojson, empty_dict)
{
    py::scoped_interpreter guard;
    py::dict obj;
    nl::json j = obj;

    ASSERT_TRUE(j.is_object());
    ASSERT_TRUE(j.empty());
}

TEST(nljson_serializers_tojson, nested)
{
    py::scoped_interpreter guard;
    py::dict obj(
        "list"_a=py::make_tuple(1234, "hello", false),
        "dict"_a=py::dict("a"_a=12, "b"_a=13),
        "hello"_a="world",
        "world"_a=py::none()
    );
    nl::json j = obj;

    ASSERT_TRUE(j.is_object());
    ASSERT_EQ(j["list"][0].get<int>(), 1234);
    ASSERT_EQ(j["list"][1].get<std::string>(), "hello");
    ASSERT_EQ(j["list"][2].get<bool>(), false);
    ASSERT_EQ(j["dict"]["a"].get<int>(), 12);
    ASSERT_EQ(j["dict"]["b"].get<int>(), 13);
    ASSERT_EQ(j["world"], nullptr);
}

TEST(nljson_serializers_tojson, handle)
{
    py::scoped_interpreter guard;
    py::list obj;
    obj.append(py::make_tuple(1234, "hello", false));
    obj.append(py::dict("a"_a=12, "b"_a=13));
    obj.append("world");
    obj.append(py::none());

    for (py::handle handle : obj)
    {
        nl::json j = handle;

        ASSERT_TRUE(j.is_array() || j.is_object() || j.is_string() || j.is_null());
    }
}

TEST(nljson_serializers_tojson, list_accessor)
{
    py::scoped_interpreter guard;
    py::list obj;
    obj.append(py::make_tuple(1234, "hello", false));
    obj.append(py::dict("a"_a=12, "b"_a=13));
    obj.append("world");
    obj.append(py::none());

    nl::json j = obj[0];
    ASSERT_TRUE(j.is_array());

    j = obj[1];
    ASSERT_TRUE(j.is_object());

    j = py::make_tuple(1234, "hello", false)[0];
    ASSERT_TRUE(j.is_number_integer());
    ASSERT_EQ(j.get<int>(), 1234);
}

TEST(nljson_serializers_tojson, tuple_accessor)
{
    py::scoped_interpreter guard;
    py::tuple obj = py::make_tuple(1234, "hello", false);

    nl::json j = obj[0];
    ASSERT_TRUE(j.is_number_integer());
    ASSERT_EQ(j.get<int>(), 1234);

    j = obj[1];
    ASSERT_TRUE(j.is_string());
    ASSERT_EQ(j.get<std::string>(), "hello");
}

TEST(nljson_serializers_tojson, item_accessor)
{
    py::scoped_interpreter guard;
    py::dict obj = py::dict("a"_a=12, "b"_a="hello");

    nl::json j = obj["a"];
    ASSERT_TRUE(j.is_number());
    ASSERT_EQ(j.get<int>(), 12);

    j = obj["b"];
    ASSERT_TRUE(j.is_string());
    ASSERT_EQ(j.get<std::string>(), "hello");
}

TEST(nljson_serializers_tojson, str_attr_accessor)
{
    py::scoped_interpreter guard;
    py::module sys = py::module::import("sys");

    nl::json j = sys.attr("base_prefix");
    ASSERT_TRUE(j.is_string());
}

TEST(nljson_serializers_tojson, obj_attr_accessor)
{
    py::scoped_interpreter guard;
    py::module sys = py::module::import("sys");

    py::str base_prefix = "base_prefix";

    nl::json j = sys.attr(base_prefix);
    ASSERT_TRUE(j.is_string());
}

TEST(nljson_serializers_fromjson, none)
{
    py::scoped_interpreter guard;
    nl::json j = "null"_json;
    py::object obj = j;

    ASSERT_TRUE(obj.is_none());
}

TEST(nljson_serializers_fromjson, bool_)
{
    py::scoped_interpreter guard;
    nl::json j = "false"_json;
    py::object obj = j;

    ASSERT_TRUE(py::isinstance<py::bool_>(obj));
    ASSERT_FALSE(obj.cast<bool>());

    py::bool_ obj2 = j;

    ASSERT_FALSE(obj2.cast<bool>());
}

TEST(nljson_serializers_fromjson, integer)
{
    py::scoped_interpreter guard;
    nl::json j = "36"_json;
    py::object obj = j;

    ASSERT_TRUE(py::isinstance<py::int_>(obj));
    ASSERT_EQ(obj.cast<int>(), 36);

    py::int_ obj2 = j;

    ASSERT_EQ(obj2.cast<int>(), 36);
}

TEST(nljson_serializers_fromjson, float_)
{
    nl::json j = "36.2"_json;
    py::object obj = j;

    ASSERT_TRUE(py::isinstance<py::float_>(obj));
    ASSERT_EQ(obj.cast<double>(), 36.2);

    py::float_ f_obj = j;

    ASSERT_EQ(f_obj.cast<double>(), 36.2);

    nl::json j_inf(INFINITY);
    py::object obj_inf = j_inf;

    ASSERT_TRUE(py::isinstance<py::float_>(obj_inf));
    ASSERT_EQ(obj_inf.cast<double>(), INFINITY);

    py::float_ f_obj_inf = j_inf;

    ASSERT_EQ(f_obj_inf.cast<double>(), INFINITY);

    nl::json j_nan(NAN);
    py::object obj_nan = j_nan;

    ASSERT_TRUE(py::isinstance<py::float_>(obj_nan));
    ASSERT_TRUE(isnan(obj_nan.cast<double>()));

    py::float_ f_obj_nan = j_nan;

    ASSERT_TRUE(isnan(f_obj_nan.cast<double>()));
}

TEST(nljson_serializers_fromjson, string)
{
    py::scoped_interpreter guard;
    nl::json j = "\"Hello World!\""_json;
    py::object obj = j;

    ASSERT_TRUE(py::isinstance<py::str>(obj));
    ASSERT_EQ(obj.cast<std::string>(), "Hello World!");

    py::str obj2 = j;

    ASSERT_EQ(obj2.cast<std::string>(), "Hello World!");
}

TEST(nljson_serializers_fromjson, list)
{
    py::scoped_interpreter guard;
    nl::json j = "[1234, \"Hello World!\", false]"_json;
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

TEST(nljson_serializers_fromjson, dict)
{
    py::scoped_interpreter guard;
    nl::json j = "{\"a\": 1234, \"b\":\"Hello World!\", \"c\":false}"_json;
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

TEST(nljson_serializers_fromjson, nested)
{
    py::scoped_interpreter guard;
    nl::json j = R"({
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

inline const nl::json& test_fromtojson(const nl::json& json)
{
    return json;
}

TEST(pybind11_caster_tojson, dict)
{
    py::scoped_interpreter guard;
    py::module m("test");

    m.def("to_json", &test_fromtojson);

    // Simulate calling this binding from Python with a dictionary as argument
    py::dict obj("number"_a=1234, "hello"_a="world");
    nl::json j = m.attr("to_json")(obj);

    ASSERT_TRUE(j.is_object());
    ASSERT_EQ(j["number"].get<int>(), 1234);
    ASSERT_EQ(j["hello"].get<std::string>(), "world");
}

TEST(pybind11_caster_fromjson, dict)
{
    py::scoped_interpreter guard;
    py::module m("test");

    m.def("from_json", &test_fromtojson);

    // Simulate calling this binding from Python, getting back a py::object
    py::dict obj("number"_a=1234, "hello"_a="world");
    py::dict j = m.attr("from_json")(obj);

    ASSERT_EQ(j["number"].cast<int>(), 1234);
    ASSERT_EQ(j["hello"].cast<std::string>(), "world");
}
