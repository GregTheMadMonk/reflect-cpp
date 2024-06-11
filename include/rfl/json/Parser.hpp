#ifndef RFL_JSON_PARSER_HPP_
#define RFL_JSON_PARSER_HPP_

#ifndef REFLECTCPP_MODULE
#include "../parsing/Parser.hpp"
#endif
#include "Reader.hpp"
#include "Writer.hpp"

namespace rfl::json {

template <class T, class ProcessorsType>
using Parser = parsing::Parser<Reader, Writer, T, ProcessorsType>;

}  // namespace rfl::json

#endif
