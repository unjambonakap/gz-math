/*
 * Copyright (C) 2021 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#ifndef GZ_MATH_PYTHON__MATRIX3_HH_
#define GZ_MATH_PYTHON__MATRIX3_HH_

#include <sstream>
#include <string>

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

#include <gz/math/Matrix3.hh>

namespace py = pybind11;
using namespace pybind11::literals;

namespace gz
{
namespace math
{
namespace python
{
/// Define a pybind11 wrapper for a gz::math::Matrix3
/**
 * \param[in] module a pybind11 module to add the definition to
 * \param[in] typestr name of the type used by Python
 */
void defineMathMatrix3(py::module &m, const std::string &typestr);

/// Help define a pybind11 wrapper for a gz::math::Matrix3
/**
 * \param[in] module a pybind11 module to add the definition to
 * \param[in] typestr name of the type used by Python
 */
template<typename T>
void helpDefineMathMatrix3(py::module &m, const std::string &typestr)
{
  using Class = gz::math::Matrix3<T>;
  static constexpr int mat_size = 3;
  auto toString = [](const Class &si) {
    std::stringstream stream;
    stream << si;
    return stream.str();
  };
  std::string pyclass_name = typestr;
  py::class_<Class>(m,
                    pyclass_name.c_str(),
                    py::buffer_protocol(),
                    py::dynamic_attr())
    .def(py::init<>())
    .def(py::init<Class>())
    .def(py::init<T, T, T, T, T, T, T, T, T>())
    .def(py::init<const gz::math::Quaternion<T>&>())
    .def(py::self - py::self)
    .def(py::self + py::self)
    .def(py::self * py::self)
    .def(py::self * float())
    .def(py::self * gz::math::Vector3<T>())
    // .def(py::self * py::self)
    // .def(py::self += py::self)
    // .def(-py::self)
    // .def(py::self -= py::self)
    .def(py::self == py::self)
    .def(py::self != py::self)
    .def("__call__",
         py::overload_cast<size_t, size_t>(&Class::operator()),
         py::return_value_policy::reference_internal)
    // .def(py::self *= py::self)
    .def("set",
         py::overload_cast<size_t, size_t, T>(&Class::Set),
         "Set a single value")
    .def("set",
         py::overload_cast<T, T, T, T, T, T, T, T, T>(&Class::Set),
         "Set values")
    .def("set_axes",
         &Class::SetAxes,
         "Set the matrix from three axis (1 per column)")
    .def("set_from_axis_angle",
        &Class::SetFromAxisAngle,
        "Set the matrix from an axis and angle")
    .def("set_from_2_axes",
         &Class::SetFrom2Axes,
         "Set the matrix to represent rotation from "
         "vector _v1 to vector _v2, so that")
    .def("set_col",
         &Class::SetCol,
         "Set a column.")
    .def("equal",
         &Class::Equal,
         "Equality test operator")
    .def("determinant",
         &Class::Determinant,
         "Return the determinant of the matrix")
    .def("inverse",
         &Class::Inverse,
         "Return the inverse matrix")
    .def("transpose",
         &Class::Transpose,
         "Transpose this matrix.")
    .def("transposed",
         &Class::Transposed,
         "Return the transpose of this matrix")
    .def("__copy__", [](const Class &self) {
      return Class(self);
    })
    .def("__deepcopy__", [](const Class &self, py::dict) {
      return Class(self);
    }, "memo"_a)
    .def_readonly_static("IDENTITY", &Class::Identity, "Identity matrix")
    .def_readonly_static("ZERO", &Class::Zero, "Zero matrix")
    .def_buffer([](Class &self) -> py::buffer_info {
        return py::buffer_info(
            self.Data(),
            sizeof(T),
            py::format_descriptor<T>::format(),
            2,
            { mat_size, mat_size },
            { mat_size * sizeof(T), sizeof(T) }
        );
    })
    .def("__str__", toString)
    .def("__repr__", toString);
}
}  // namespace python
}  // namespace math
}  // namespace gz

#endif  // GZ_MATH_PYTHON__MATRIX3_HH_
