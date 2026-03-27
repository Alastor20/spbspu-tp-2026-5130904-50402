#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>

class Note
{
public:
  Note(std::string name):
    name(name),
    desc(""),
    links()
  {}

  std::string name, desc;
  std::unordered_map< std::string, std::weak_ptr< Note > > links;
};

void addNote(std::istream &is, std::unordered_map< std::string, std::shared_ptr< Note > > &db)
{
  std::string name;
  is >> name;
  db[name] = std::make_shared< Note >(name);
}

int main()
{
  std::unordered_map< std::string, std::shared_ptr< Note > > db;
  constexpr std::streamsize streamMax = std::numeric_limits< std::streamsize >::max();
  using func_t = void (*)(std::istream &, std::unordered_map< std::string, std::shared_ptr< Note > > &);
  const std::unordered_map< std::string, func_t > cmds{
      {"note", addNote},
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
