#include <iomanip>
#include <iostream>
#include <istream>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

class Note
{
public:
  Note(std::string name):
    name(name),
    desc(),
    links()
  {}

  std::string name;
  std::vector< std::string > desc;
  std::unordered_map< std::string, std::weak_ptr< Note > > links;
};

using notes_t = std::unordered_map< std::string, std::shared_ptr< Note > >;

void addNote(std::istream &is, notes_t &db)
{
  std::string name;
  is >> name;
  db[name] = std::make_shared< Note >(name);
}

void addDesc(std::istream &is, notes_t &db)
{
  std::string name, desc;
  is >> name;
  is >> std::quoted(desc);
  if (is.bad()) {
    throw std::logic_error("not enought args");
  }
  try {
    db.at(name)->desc.push_back(desc);
  } catch (const std::out_of_range &) {
    throw std::logic_error("no note with this name");
  }
}

int main()
{
  notes_t db;
  constexpr std::streamsize streamMax = std::numeric_limits< std::streamsize >::max();
  using func_t = void (*)(std::istream &, std::unordered_map< std::string, std::shared_ptr< Note > > &);
  const std::unordered_map< std::string, func_t > cmds{
      {"note", addNote},
      {"line", addDesc},
  };
  std::string cmd;
  while (std::cin >> cmd) {
    try {
      cmds.at(cmd)(std::cin, db);
    } catch (const std::out_of_range &) {
      std::cout << "<INVALID COMMAND>\n";
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
    return 1;
  }
}
