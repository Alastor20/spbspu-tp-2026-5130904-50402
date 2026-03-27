#include "note.hpp"
#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

void dirko::addNote(std::istream &is, std::ostream &, notes_t &db)
{
  std::string name;
  is >> name;
  db[name] = std::shared_ptr< NoteBody >(new NoteBody{});
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
void dirko::removeLink(std::istream &is, std::ostream &, notes_t &db)
{
  std::string from, to;
  is >> from >> to;
  try {
    db.at(from)->links.erase(to);
  } catch (const std::out_of_range &) {
    throw std::logic_error("No note with this name");
  }
}

void dirko::printLinks(std::istream &is, std::ostream &os, notes_t &db)
{
  std::string name;
  is >> name;
  try {
    for (const std::pair< const std::string, std::weak_ptr< NoteBody > > &link : db.at(name)->links) {
      if (!link.second.expired()) {
        os << link.first << '\n';
      }
    }
  } catch (const std::out_of_range &) {
    throw std::logic_error("No note with this name");
  }
}

void dirko::countExpired(std::istream &is, std::ostream &os, notes_t &db)
{
  std::string name;
  size_t expired = 0;
  is >> name;
  try {
    for (const std::pair< const std::string, std::weak_ptr< NoteBody > > &link : db.at(name)->links) {
      if (link.second.expired()) {
        ++expired;
      }
    }
  } catch (const std::out_of_range &) {
    throw std::logic_error("No note with this name");
  }
  os << expired << '\n';
}

void dirko::refreshLinks(std::istream &is, std::ostream &, notes_t &db)
{
  std::string name;
  std::vector< std::string > toRemove;
  is >> name;
  try {
    for (const std::pair< const std::string, std::weak_ptr< NoteBody > > &link : db.at(name)->links) {
      if (link.second.expired()) {
        toRemove.push_back(link.first);
      }
    }
  } catch (const std::out_of_range &) {
    throw std::logic_error("No note with this name");
  }
  for (const std::string &note : toRemove) {
    db.at(name)->links.erase(note);
  }
}
