
#ifndef RFL_INTERNAL_TO_PTR_TUPLE_HPP_
#define RFL_INTERNAL_TO_PTR_TUPLE_HPP_

#ifndef REFLECTCPP_MODULE
#include <iostream>
#include <tuple>
#include <type_traits>
#endif

#include "bind_to_tuple.hpp"

namespace rfl {
namespace internal {

template <class T>
constexpr auto to_ptr_tuple(T& _t) {
  if constexpr (std::is_pointer_v<std::remove_cvref_t<T>>) {
    return to_ptr_tuple(*_t);
  } else {
    return bind_to_tuple(_t, [](auto& x) { return &x; });
  }
}

}  // namespace internal
}  // namespace rfl

#endif
