#include "note.hpp"
#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

dirko::Note::Note(std::string name):
  name(name),
  desc(),
  links()
{}

dirko::linkIt_t dirko::find(linkIt_t start, linkIt_t end, std::string name)
{
  for (; start != end; ++start) {
    if (!start->expired() && start->lock()->name == name) {
      return start;
    }
  }
  return end;
}

void dirko::addNote(std::istream &is, std::ostream &, notes_t &db)
{
  std::string name;
  is >> name;
  if (db.find(name) == db.end()) {
    db[name] = std::make_shared< Note >(name);
  } else {
    throw std::logic_error("This note already exists");
  }
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
    if (!db.at(name)->desc.size()) {
      os << '\n';
    }
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
  if (db.find(name) == db.end()) {
    throw std::logic_error("no note with this name");
  }
  db.erase(name);
}

void dirko::linkNote(std::istream &is, std::ostream &, notes_t &db)
{
  std::string from, to;
  is >> from >> to;
  try {
    if (dirko::find(db.at(from)->links.begin(), db.at(from)->links.end(), to) == db.at(from)->links.end()) {
      db.at(from)->links.push_back(db.at(to));
    } else {
      throw std::logic_error("This link already exists");
    }
  } catch (const std::out_of_range &) {
    throw std::logic_error("Cant link");
  }
}
void dirko::removeLink(std::istream &is, std::ostream &, notes_t &db)
{
  std::string from, to;
  is >> from >> to;
  try {
    dirko::linkIt_t it = dirko::find(db.at(from)->links.begin(), db.at(from)->links.end(), to);
    if (it == db.at(from)->links.end()) {
      throw std::logic_error("No link with this name");
    }
    db.at(from)->links.erase(it);
  } catch (const std::out_of_range &) {
    throw std::logic_error("No note with this name");
  }
}
void dirko::printLinks(std::istream &is, std::ostream &os, notes_t &db)
{
  std::string name;
  is >> name;
  bool print = true;
  try {
    for (const std::weak_ptr< Note > &link : db.at(name)->links) {
      if (!link.expired()) {
        os << link.lock()->name << '\n';
        print = false;
      }
    }
  } catch (const std::out_of_range &) {
    throw std::logic_error("No note with this name");
  }
  if (print) {
    os << '\n';
  }
}

void dirko::countExpired(std::istream &is, std::ostream &os, notes_t &db)
{
  std::string name;
  size_t expired = 0;
  is >> name;
  try {
    for (const std::weak_ptr< Note > &link : db.at(name)->links) {
      if (link.expired()) {
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
  std::vector< std::weak_ptr< Note > > vec;
  is >> name;
  try {
    for (std::weak_ptr< Note > &link : db.at(name)->links) {
      if (!link.expired()) {
        vec.push_back(std::move(link));
      }
    }
  } catch (const std::out_of_range &) {
    throw std::logic_error("No note with this name");
  }
  db.at(name)->links.swap(vec);
}
