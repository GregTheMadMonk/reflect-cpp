
#ifndef RFL_INTERNAL_ISVARIANT_HPP_
#define RFL_INTERNAL_ISVARIANT_HPP_

#ifndef REFLECTCPP_MODULE
#include <type_traits>
#include <variant>
#endif

namespace rfl {
namespace internal {

template <class T>
class is_variant;

template <class T>
class is_variant : public std::false_type {};

template <class... T>
class is_variant<std::variant<T...>> : public std::true_type {};

template <class T>
constexpr bool is_variant_v =
    is_variant<std::remove_cvref_t<std::remove_pointer_t<T>>>::value;

}  // namespace internal
}  // namespace rfl

#endif
