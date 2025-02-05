#ifndef RFL_TO_NAMED_TUPLE_HPP_
#define RFL_TO_NAMED_TUPLE_HPP_

#include <iostream>
#include <tuple>
#include <type_traits>

#include "rfl/always_false.hpp"
#include "rfl/internal/copy_to_field_tuple.hpp"
#include "rfl/internal/has_flatten_fields.hpp"
#include "rfl/internal/is_field.hpp"
#include "rfl/internal/is_named_tuple.hpp"
#include "rfl/internal/move_field_tuple_to_named_tuple.hpp"
#include "rfl/internal/move_to_field_tuple.hpp"
#include "rfl/make_named_tuple.hpp"

namespace rfl {

/// Generates the named tuple that is equivalent to the struct _t.
/// If _t already is a named tuple, then _t will be returned.
/// All fields of the struct must be an rfl::Field.
template <class T>
auto to_named_tuple(T&& _t) {
  if constexpr (internal::is_named_tuple_v<std::decay_t<T>>) {
    return _t;
  } else if constexpr (internal::is_field_v<std::decay_t<T>>) {
    return make_named_tuple(std::forward<T>(_t));
  } else if constexpr (std::is_lvalue_reference<T>{}) {
    auto field_tuple = internal::copy_to_field_tuple(_t);
    return internal::move_field_tuple_to_named_tuple(std::move(field_tuple));
  } else {
    auto field_tuple = internal::move_to_field_tuple(_t);
    return internal::move_field_tuple_to_named_tuple(std::move(field_tuple));
  }
}

/// Generates the named tuple that is equivalent to the struct _t.
/// If _t already is a named tuple, then _t will be returned.
/// All fields of the struct must be an rfl::Field.
template <class T>
auto to_named_tuple(const T& _t) {
  if constexpr (internal::is_named_tuple_v<std::decay_t<T>>) {
    return _t;
  } else if constexpr (internal::is_field_v<std::decay_t<T>>) {
    return make_named_tuple(_t);
  } else {
    auto field_tuple = internal::copy_to_field_tuple(_t);
    return internal::move_field_tuple_to_named_tuple(std::move(field_tuple));
  }
}

}  // namespace rfl

#endif
