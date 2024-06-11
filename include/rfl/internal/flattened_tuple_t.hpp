
#ifndef RFL_INTERNAL_FLATTENED_TUPLE_T_HPP_
#define RFL_INTERNAL_FLATTENED_TUPLE_T_HPP_

#ifndef REFLECTCPP_MODULE
#include <functional>
#include <tuple>
#include <type_traits>
#endif

#include "flattened_ptr_tuple_t.hpp"
#include "remove_ptrs_tup.hpp"
#include "../to_named_tuple.hpp"

namespace rfl {
namespace internal {

template <class T>
using flattened_tuple_t =
    typename remove_ptrs_tup<flattened_ptr_tuple_t<T>>::TupleType;

}  // namespace internal
}  // namespace rfl

#endif
