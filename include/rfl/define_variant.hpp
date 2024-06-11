
#ifndef RFL_DEFINEVARIANT_HPP_
#define RFL_DEFINEVARIANT_HPP_

#ifndef REFLECTCPP_MODULE
#include <variant>
#endif

#include "internal/define_variant.hpp"

namespace rfl {

template <class... Vars>
using define_variant_t = typename internal::define_variant<Vars...>::type;

}  // namespace rfl

#endif  // RFL_DEFINEVARIANT_HPP_
