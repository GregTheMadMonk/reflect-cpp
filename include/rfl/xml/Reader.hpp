#ifndef RFL_XML_READER_HPP_
#define RFL_XML_READER_HPP_

#include <array>
#include <exception>
#include <map>
#include <memory>
#include <pugixml.hpp>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <variant>
#include <vector>

#include "rfl/Result.hpp"
#include "rfl/always_false.hpp"

namespace rfl {
namespace xml {

struct Reader {
  struct XMLInputArray {
    XMLInputArray(pugi::xml_node _node) : node_(_node) {}
    pugi::xml_node node_;
  };

  struct XMLInputObject {
    XMLInputObject(pugi::xml_node _node) : node_(_node) {}
    pugi::xml_node node_;
  };

  struct XMLInputVar {
    XMLInputVar() : node_or_attribute_(pugi::xml_node()) {}
    XMLInputVar(pugi::xml_attribute _attr) : node_or_attribute_(_attr) {}
    XMLInputVar(pugi::xml_node _node) : node_or_attribute_(_node) {}
    std::variant<pugi::xml_node, pugi::xml_attribute> node_or_attribute_;
  };

  using InputArrayType = XMLInputArray;
  using InputObjectType = XMLInputObject;
  using InputVarType = XMLInputVar;

  // TODO
  template <class T>
  static constexpr bool has_custom_constructor = false;

  /// XML-only helper function. This is needed because XML distinguishes between
  /// nodes and attributes.
  static rfl::Result<pugi::xml_node> cast_as_node(
      const std::variant<pugi::xml_node, pugi::xml_attribute>&
          _node_or_attribute) {
    const auto cast = [](const auto& _n) -> Result<pugi::xml_node> {
      using Type = std::decay_t<decltype(_n)>;
      if constexpr (std::is_same<Type, pugi::xml_node>()) {
        return _n;
      } else {
        return Error("Field '" + std::string(_n.name()) + "' is an attribute.");
      }
    };
    return std::visit(cast, _node_or_attribute);
  }

  rfl::Result<InputVarType> get_field(
      const std::string& _name, const InputObjectType _obj) const noexcept {
    const auto node = _obj.node_.child(_name.c_str());
    if (!node) {
      return rfl::Error("Object contains no field named '" + _name + "'.");
    }
    return InputVarType(node);
  }

  bool is_empty(const InputVarType _var) const noexcept {
    const auto wrap = [](const auto& _node) { return !_node; };
    return std::visit(cast_as_node, _var.node_or_attribute_)
        .transform(wrap)
        .value_or(false);
  }

  template <class T>
  rfl::Result<T> to_basic_type(const InputVarType _var) const noexcept {
    const auto get_value = [](const auto& _n) -> std::string {
      using Type = std::decay_t<decltype(_n)>;
      if constexpr (std::is_same<Type, pugi::xml_node>()) {
        return std::string(_n.child_value());
      } else {
        return std::string(_n.value());
      }
    };

    if constexpr (std::is_same<std::decay_t<T>, std::string>()) {
      return std::visit(get_value, _var.node_or_attribute_);
    } else if constexpr (std::is_same<std::decay_t<T>, bool>()) {
      return std::visit(get_value, _var.node_or_attribute_) == "true";
    } else if constexpr (std::is_floating_point<std::decay_t<T>>()) {
      const auto str = std::visit(get_value, _var.node_or_attribute_);
      try {
        return static_cast<T>(std::stod(str));
      } catch (std::exception& e) {
        return Error("Could not cast '" + std::string(str) +
                     "' to floating point value.");
      }
    } else if constexpr (std::is_integral<std::decay_t<T>>()) {
      const auto str = std::visit(get_value, _var.node_or_attribute_);
      try {
        return static_cast<T>(std::stoi(str));
      } catch (std::exception& e) {
        return Error("Could not cast '" + std::string(str) + "' to integer.");
      }
    } else {
      static_assert(rfl::always_false_v<T>, "Unsupported type.");
    }
  }

  rfl::Result<InputArrayType> to_array(const InputVarType _var) const noexcept {
    const auto wrap = [](const auto& _node) { return InputArrayType(_node); };
    return std::visit(cast_as_node, _var.node_or_attribute_).transform(wrap);
  }

  template <size_t size, class FunctionType>
  std::array<std::optional<InputVarType>, size> to_fields_array(
      const FunctionType _fct, const InputObjectType _obj) const noexcept {
    std::array<std::optional<InputVarType>, size> f_arr;

    for (auto child = _obj.node_.first_child(); child;
         child = child.next_sibling()) {
      const auto name = child.name();
      const auto ix = _fct(std::string_view(name));
      // XML allows for duplicate node names, but we always want the first
      // sibling.
      if (ix != -1 && !f_arr[ix]) {
        f_arr[ix] = InputVarType(child);
      }
    }

    for (auto attr = _obj.node_.first_attribute(); attr;
         attr = attr.next_attribute()) {
      const auto name = attr.name();
      const auto ix = _fct(std::string_view(name));
      // XML allows for duplicate node names, but we always want the first
      // sibling.
      if (ix != -1 && !f_arr[ix]) {
        f_arr[ix] = InputVarType(attr);
      }
    }

    const auto ix = _fct(std::string_view("xml_content"));
    if (ix != -1) {
      f_arr[ix] = InputVarType(_obj.node_);
    }

    return f_arr;
  }

  std::vector<std::pair<std::string, InputVarType>> to_map(
      const InputObjectType _obj) const noexcept {
    std::vector<std::pair<std::string, InputVarType>> m;
    for (auto child = _obj.node_.first_child(); child;
         child = child.next_sibling()) {
      auto p = std::make_pair(std::string(child.name()), InputVarType(child));
      m.emplace_back(std::move(p));
    }
    return m;
  }

  rfl::Result<InputObjectType> to_object(
      const InputVarType _var) const noexcept {
    const auto wrap = [](const auto& _node) { return InputObjectType(_node); };
    return std::visit(cast_as_node, _var.node_or_attribute_).transform(wrap);
  }

  std::vector<InputVarType> to_vec(const InputArrayType _arr) const noexcept {
    const auto name = _arr.node_.name();
    std::vector<InputVarType> vec;
    for (auto node = _arr.node_; node; node = node.next_sibling(name)) {
      vec.push_back(InputVarType(node));
    }
    return vec;
  }

  template <class T>
  rfl::Result<T> use_custom_constructor(
      const InputVarType _var) const noexcept {
    return rfl::Error("TODO");
  }
};

}  // namespace xml
}  // namespace rfl

#endif
