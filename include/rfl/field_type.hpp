
#ifndef RFL_FIELD_TYPE_HPP_
#define RFL_FIELD_TYPE_HPP_

#ifndef REFLECTCPP_MODULE
#include <tuple>
#include <type_traits>
#include <variant>
#endif

#include "internal/StringLiteral.hpp"
#include "internal/field_type.hpp"

namespace rfl {

template <internal::StringLiteral _field_name, class T>
using field_type_t = typename internal::FieldType<_field_name, T>::Type;

}  // namespace rfl

#endif
