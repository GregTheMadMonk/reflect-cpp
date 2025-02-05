#ifndef RFL_PARSING_PARSER_MAP_LIKE_HPP_
#define RFL_PARSING_PARSER_MAP_LIKE_HPP_

#include <map>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>

#include "rfl/Result.hpp"
#include "rfl/always_false.hpp"
#include "rfl/parsing/MapParser.hpp"
#include "rfl/parsing/Parser_base.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, class T>
requires AreReaderAndWriter<R, W, std::map<std::string, T>>
struct Parser<R, W, std::map<std::string, T>>
    : public MapParser<R, W, std::map<std::string, T>> {
};

template <class R, class W, class T>
requires AreReaderAndWriter<R, W, std::unordered_map<std::string, T>>
struct Parser<R, W, std::unordered_map<std::string, T>>
    : public MapParser<R, W, std::unordered_map<std::string, T>> {
};

}  // namespace parsing
}  // namespace rfl

#endif
