#ifndef NOTE_HPP
#define NOTE_HPP
#include <iosfwd>
#include <limits>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace dirko
{
  class NoteBody
  {
  public:
    std::vector< std::string > desc;
    std::unordered_map< std::string, std::weak_ptr< NoteBody > > links;
  };
  constexpr std::streamsize streamMax = std::numeric_limits< std::streamsize >::max();
  using notes_t = std::unordered_map< std::string, std::shared_ptr< NoteBody > >;
  using func_t = void (*)(std::istream &, std::ostream &, notes_t &);
  void addNote(std::istream &is, std::ostream &, notes_t &db);
  void addDesc(std::istream &is, std::ostream &, notes_t &db);
  void printNote(std::istream &is, std::ostream &os, notes_t &db);
  void dropNote(std::istream &is, std::ostream &, notes_t &db);
  void linkNote(std::istream &is, std::ostream &, notes_t &db);
  void removeLink(std::istream &is, std::ostream &, notes_t &db);
  void printLinks(std::istream &is, std::ostream &os, notes_t &db);
  void countExpired(std::istream &is, std::ostream &os, notes_t &db);
  void refreshLinks(std::istream &is, std::ostream &, notes_t &db);
}
#endif
