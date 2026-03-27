#ifndef NOTE_HPP
#define NOTE_HPP
#include <iosfwd>
#include <memory>
#include <string>
#include <unordered_map>
#include <limits>
#include <vector>

namespace dirko
{
  class Note
  {
  public:
    Note(std::string name);

    std::string name;
    std::vector< std::string > desc;
    std::unordered_map< std::string, std::weak_ptr< Note > > links;
  };
  constexpr std::streamsize streamMax = std::numeric_limits< std::streamsize >::max();
  using func_t = void (*)(std::istream &, std::ostream &, std::unordered_map< std::string, std::shared_ptr< Note > > &);
  using notes_t = std::unordered_map< std::string, std::shared_ptr< Note > >;
  void addNote(std::istream &is, std::ostream &, notes_t &db);
  void addDesc(std::istream &is, std::ostream &, notes_t &db);
  void printNote(std::istream &is, std::ostream &os, notes_t &db);
  void dropNote(std::istream &is, std::ostream &, notes_t &db);
  void linkNote(std::istream &is, std::ostream &, notes_t &db);
  void removeNote(std::istream &is, std::ostream &, notes_t &db);
}
#endif
