#include "note.hpp"
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

dirko::Note::Note(std::string name):
  name(name),
  desc(),
  links()
{}

void dirko::addNote(std::istream &is, std::ostream &, notes_t &db)
{
  std::string name;
  is >> name;
  db[name] = std::make_shared< Note >(name);
}

void dirko::addDesc(std::istream &is, std::ostream &, notes_t &db)
{
  std::string name, desc;
  is >> name;
  is >> std::quoted(desc);
  try {
    db.at(name)->desc.push_back(desc);
  } catch (const std::out_of_range &) {
    throw std::logic_error("no note with this name");
  }
}

void dirko::printNote(std::istream &is, std::ostream &os, notes_t &db)
{
  std::string name;
  is >> name;
  try {
    for (const std::string &line : db.at(name)->desc) {
      os << line << '\n';
    }
  } catch (const std::out_of_range &) {
    throw std::logic_error("no note with this name");
  }
}

void dirko::dropNote(std::istream &is, std::ostream &, notes_t &db)
{
  std::string name;
  is >> name;
  db.erase(name);
}

void dirko::linkNote(std::istream &is, std::ostream &, notes_t &db)
{
  std::string from, to;
  is >> from >> to;
  try {
    db.at(from)->links[to] = db.at(to);
  } catch (const std::out_of_range &) {
    throw std::logic_error("Cant link");
  }
}
void dirko::removeNote(std::istream &is, std::ostream &, notes_t &db)
{
  std::string from, to;
  is >> from >> to;
  try {
    db.at(from)->links.erase(to);
  } catch (const std::out_of_range &) {
    throw std::logic_error("No note with this name");
  }
}
