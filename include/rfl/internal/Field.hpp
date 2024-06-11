
#ifndef RFL_INTERNAL_FIELD_TUPLE_T_HPP_
#define RFL_INTERNAL_FIELD_TUPLE_T_HPP_

#ifndef REFLECTCPP_MODULE
#include <functional>
#include <tuple>
#include <type_traits>
#endif

#include "copy_to_field_tuple.hpp"

namespace rfl {
namespace internal {

template <class T>
using field_tuple_t =
    typename std::invoke_result<decltype(copy_to_field_tuple<T>), T>::type;

}
}  // namespace rfl

#endif
