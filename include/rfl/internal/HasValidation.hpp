
#ifndef RFL_INTERNAL_HASVALIDATION_HPP_
#define RFL_INTERNAL_HASVALIDATION_HPP_

#ifndef REFLECTCPP_MODULE
#include <type_traits>
#endif

#include "../Result.hpp"

namespace rfl {
namespace internal {

template <class Class, typename T>
concept HasValidation = requires(Class obj, T value) {
  { Class::validate(value) } -> std::same_as<rfl::Result<T>>;
};

}  // namespace internal
}  // namespace rfl

#endif
