#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <unordered_map>

int main()
{
  constexpr std::streamsize streamMax = std::numeric_limits< std::streamsize >::max();
  using func_t = void (*)();
  const std::unordered_map< std::string, func_t > cmds;
  std::string cmd;
  while (std::cin >> cmd) {
    try {
      cmds.at(cmd)();
    } catch (const std::out_of_range &) {
      std::cout << "<UNKNOWN COMMAND>\n";
      std::cin.ignore(streamMax, '\n');
    } catch (const std::logic_error &e) {
      std::cout << e.what() << '\n';
    } catch (...) {
      std::cerr << "<PANIC>\n";
      return 1;
    }
  }
  if (!std::cin.eof()) {
    std::cerr << "<INPUT ERROR>\n";
  }
}
