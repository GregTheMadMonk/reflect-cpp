#ifndef RFL_PARSING_PARSER_REFERENCE_WRAPPER_HPP_
#define RFL_PARSING_PARSER_REFERENCE_WRAPPER_HPP_

#include <functional>
#include <type_traits>

#include "rfl/Result.hpp"
#include "rfl/always_false.hpp"
#include "rfl/parsing/Parser_base.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, class T>
requires AreReaderAndWriter<R, W, std::reference_wrapper<T>>
struct Parser<R, W, std::reference_wrapper<T>> {
  using InputVarType = typename R::InputVarType;
  using OutputVarType = typename W::OutputVarType;

  static Result<std::reference_wrapper<T>> read(
      const R& _r, const InputVarType& _var) noexcept {
    static_assert(always_false_v<T>,
                  "Reading into std::reference_wrapper is dangerous and "
                  "therefore unsupported. "
                  "Please consider using std::unique_ptr, rfl::Box, "
                  "std::shared_ptr, or"
                  "rfl::Ref instead.");
    return Error("Unsupported.");
  }

  static OutputVarType write(const W& _w,
                             const std::reference_wrapper<T> _ref) noexcept {
    return Parser<R, W, std::decay_t<T>>::write(_w, _ref.get());
  }
};

}  // namespace parsing
}  // namespace rfl

#endif
