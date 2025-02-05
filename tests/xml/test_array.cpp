#include "test_array.hpp"

#include <array>
#include <iostream>
#include <memory>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <string>

#include "write_and_read.hpp"

namespace test_array {

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  std::unique_ptr<std::array<Person, 3>> children = nullptr;
};

void test() {
  std::cout << std::source_location::current().function_name() << std::endl;

  auto bart = Person{.first_name = "Bart"};

  auto lisa = Person{.first_name = "Lisa"};

  auto maggie = Person{.first_name = "Maggie"};

  const auto homer = Person{
      .first_name = "Homer",
      .children = std::make_unique<std::array<Person, 3>>(std::array<Person, 3>{
          std::move(bart), std::move(lisa), std::move(maggie)})};

  write_and_read(homer);
}
}  // namespace test_array
