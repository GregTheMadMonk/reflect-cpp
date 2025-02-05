#ifndef RFL_INTERNAL_GETFIELDNAMES_HPP_
#define RFL_INTERNAL_GETFIELDNAMES_HPP_

#include <array>
#include <iostream>
#include <memory>
#include <source_location>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

#include "rfl/Literal.hpp"
#include "rfl/internal/fake_object.hpp"
#include "rfl/internal/is_flatten_field.hpp"
#include "rfl/internal/is_rename.hpp"
#include "rfl/internal/num_fields.hpp"
#include "rfl/internal/to_ptr_tuple.hpp"

#if __GNUC__
#ifndef __clang__
#pragma GCC system_header
#endif
#endif

namespace rfl {
namespace internal {

template <class T>
struct Wrapper {
  using Type = T;
  T v;
};

template <class T>
Wrapper(T) -> Wrapper<T>;

// This workaround is necessary for clang.
template <class T>
constexpr auto wrap(const T& arg) noexcept {
  return Wrapper{arg};
}

template <auto ptr>
consteval auto get_field_name_str_view() {
  const auto func_name =
      std::string_view{std::source_location::current().function_name()};
#if defined(__clang__)
  const auto split = func_name.substr(0, func_name.find("}]"));
  return split.substr(split.find_last_of(".") + 1);
#elif defined(__GNUC__)
  const auto split = func_name.substr(0, func_name.find(")}"));
  return split.substr(split.find_last_of(":") + 1);
#elif defined(_MSC_VER)
  const auto split = func_name.substr(0, func_name.find_last_of("}"));
  return split.substr(split.find_last_of(">") + 1);
#else
  static_assert(false,
                "You are using an unsupported compiler. Please use GCC, Clang "
                "or MSVC or switch to the rfl::Field-syntax.");
#endif
}

template <auto ptr>
consteval auto get_field_name_str_lit() {
  constexpr auto name = get_field_name_str_view<ptr>();
  const auto to_str_lit = [&]<auto... Ns>(std::index_sequence<Ns...>) {
    return StringLiteral<sizeof...(Ns) + 1>{name[Ns]...};
  };
  return to_str_lit(std::make_index_sequence<name.size()>{});
}

template <class T>
auto get_field_names();

template <auto ptr>
auto get_field_name() {
  using Type = std::decay_t<std::remove_pointer_t<
      typename std::remove_pointer_t<decltype(ptr)>::Type>>;
  if constexpr (is_rename_v<Type>) {
    using Name = typename Type::Name;
    return Name();
  } else if constexpr (is_flatten_field_v<Type>) {
    return get_field_names<std::decay_t<typename Type::Type>>();
  } else {
    return rfl::Literal<get_field_name_str_lit<ptr>()>();
  }
}

template <StringLiteral... _names1, StringLiteral... _names2>
auto concat_two_literals(const rfl::Literal<_names1...>& _lit1,
                         const rfl::Literal<_names2...>& _lit2) {
  return rfl::Literal<_names1..., _names2...>::template from_value<0>();
}

template <class Head, class... Tail>
auto concat_literals(const Head& _head, const Tail&... _tail) {
  if constexpr (sizeof...(_tail) == 0) {
    return _head;
  } else {
    return concat_two_literals(_head, concat_literals(_tail...));
  }
}

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wundefined-var-template"
#endif

template <class T>
#if __GNUC__
#ifndef __clang__
[[gnu::no_sanitize_undefined]]
#endif
#endif
auto get_field_names() {
  if constexpr (std::is_pointer_v<std::decay_t<T>>) {
    return get_field_names<std::remove_pointer_t<T>>();
  } else {
    constexpr auto ptr_tuple = to_ptr_tuple(fake_object<T>);
    const auto get = [&]<std::size_t... Is>(std::index_sequence<Is...>) {
      return concat_literals(
          get_field_name<wrap(std::get<Is>(ptr_tuple))>()...);
    };
    return get(
        std::make_index_sequence<std::tuple_size_v<decltype(ptr_tuple)>>());
  }
}

#ifdef __clang__
#pragma clang diagnostic pop
#endif

}  // namespace internal
}  // namespace rfl

#endif
