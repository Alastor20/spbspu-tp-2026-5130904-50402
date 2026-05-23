#include "lab_io.hpp"
#include <iomanip>

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
