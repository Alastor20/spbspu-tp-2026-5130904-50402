#include <iostream>
#include <istream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include "note.hpp"

int main()
{
  dirko::notes_t db;
  const std::unordered_map< std::string, dirko::func_t > cmds{
      {"note", dirko::addNote},
      {"line", dirko::addDesc},
      {"show", dirko::printNote},
      {"drop", dirko::dropNote},
  };
  std::string cmd;
  while (std::cin >> cmd) {
    try {
      cmds.at(cmd)(std::cin, std::cout, db);
    } catch (const std::out_of_range &) {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.ignore(dirko::streamMax, '\n');
    } catch (const std::logic_error &e) {
      std::cout << e.what() << '\n';
    } catch (...) {
      std::cerr << "<PANIC>\n";
      return 1;
    }
  }
  if (!std::cin.eof()) {
    std::cerr << "<INPUT ERROR>\n";
    return 1;
  }
}
