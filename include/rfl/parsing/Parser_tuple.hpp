#ifndef RFL_PARSING_PARSER_TUPLE_HPP_
#define RFL_PARSING_PARSER_TUPLE_HPP_

#include "rfl/parsing/Parser_base.hpp"
#include "rfl/parsing/TupleParser.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, class... Ts>
requires AreReaderAndWriter<R, W, std::tuple<Ts...>>
struct Parser<R, W, std::tuple<Ts...>>
    : public TupleParser<R, W, /*_ignore_empty_containers=*/false, Ts...> {
};

}  // namespace parsing
}  // namespace rfl

#endif
