
#ifndef RFL_INTERNAL_FIELDS_HPP_
#define RFL_INTERNAL_FIELDS_HPP_

#ifndef REFLECTCPP_MODULE
#include <array>
#include <cstdint>
#include <string>
#include <string_view>
#include <unordered_map>
#endif

namespace rfl {
namespace internal {

template <int N>
struct Fields {
  std::array<std::string, N> names_;

  std::unordered_map<std::string_view, std::int16_t> indices_;
};

}  // namespace internal
}  // namespace rfl

#endif
