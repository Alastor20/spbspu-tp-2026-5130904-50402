#include "lab_io.hpp"
#include <functional>
#include <iomanip>
#include <type_traits>

std::istream &dirko::operator>>(std::istream &in, DelimIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  char c = '\0';
  in >> c;
  if (in && c != dest.exp) {
    in.setstate(std::ios::failbit);
  }
  return in;
}

dirko::IOguard::IOguard(std::basic_ios< char > &s):
  s_(s),
  width_(s.width()),
  precision_(s.precision()),
  fmt_(s.flags()),
  fill_(s.fill())
{}

dirko::IOguard::~IOguard()
{
  s_.width(width_);
  s_.precision(precision_);
  s_.flags(fmt_);
  s_.fill(fill_);
}

std::istream &dirko::operator>>(std::istream &in, LabelIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  std::string label = "";
  in >> label;
  if (in && label != dest.exp) {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream &dirko::operator>>(std::istream &in, CharIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  IOguard guard(in);
  in >> DelimIO{'\''} >> dest.ref >> DelimIO{'\''};
  return in;
}

std::istream &dirko::operator>>(std::istream &in, CompIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  IOguard guard(in);
  double real = 0, imag = 0;
  in >> LabelIO{"#c("} >> real >> imag >> DelimIO{')'};
  dest.ref = {real, imag};
  return in;
}

std::istream &dirko::operator>>(std::istream &in, StringIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  return in >> std::quoted(dest.ref);
}

bool dirko::operator<(const DataStruct &lhs, const DataStruct &rhs)
{
  if (lhs.key1 != rhs.key1) {
    return lhs.key1 < rhs.key1;
  }
  if (lhs.key2.real() != rhs.key2.real()) {
    return lhs.key2.real() < rhs.key2.real();
  }
  if (lhs.key2.imag() != rhs.key2.imag()) {
    return lhs.key2.imag() < rhs.key2.imag();
  }
  return lhs.key3.length() < rhs.key3.length();
}

std::istream &dirko::operator>>(std::istream &in, DataStruct &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  DataStruct tmp;
  bool got1 = false, got2 = false, got3 = false;
  in >> DelimIO{'('};
  while (in && (!got1 || !got2 || !got3)) {
    std::string label;
    in >> label;
    if (label == ":key1" && !got1) {
      in >> CharIO{tmp.key1};
      if (in) {
        got1 = true;
      }
    } else if (label == ":key2" && !got2) {
      in >> CompIO{tmp.key2};
      if (in) {
        got2 = true;
      }
    } else if (label == ":key3" && !got3) {
      in >> StringIO{tmp.key3};
      if (in) {
        got3 = true;
      }
    } else {
      in.setstate(std::ios::failbit);
    }
  }
  in >> LabelIO{":)"};
  if (in && got1 && got2 && got3) {
    dest = tmp;
  }
  return in;
}
