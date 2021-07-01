#include "board.hpp"
#include <string>

using std::string;

namespace crev
{

Board::Board()
{
  black_ = 0x0000000810000000;
  white_ = 0x0000001008000000;
  turn_ = 1;
  record_ = "";
  is_black_turn = 1;
  is_white_turn = 0;
  mobility_ = mobility();
}

unsigned long long Board::mobility()
{
  unsigned long long p, o, oh, ov, oa, fl, mob;
  p = turn_ ? black_ : white_;
  o = turn_ ? white_ : black_;
  oh = o & 0x7e7e7e7e7e7e7e7e;
  ov = o & 0x00ffffffffffff00;
  oa = o & 0x007e7e7e7e7e7e00;
  mob = 0;

  fl = oh & (p << 1);fl |= oh & (fl << 1);fl |= oh & (fl << 1);fl |= oh & (fl << 1);fl |= oh & (fl << 1);fl |= oh & (fl << 1);mob |= fl << 1;
  fl = oh & (p >> 1);fl |= oh & (fl >> 1);fl |= oh & (fl >> 1);fl |= oh & (fl >> 1);fl |= oh & (fl >> 1);fl |= oh & (fl >> 1);mob |= fl >> 1;
  fl = ov & (p << 8);fl |= ov & (fl << 8);fl |= ov & (fl << 8);fl |= ov & (fl << 8);fl |= ov & (fl << 8);fl |= ov & (fl << 8);mob |= fl << 8;
  fl = ov & (p >> 8);fl |= ov & (fl >> 8);fl |= ov & (fl >> 8);fl |= ov & (fl >> 8);fl |= ov & (fl >> 8);fl |= ov & (fl >> 8);mob |= fl >> 8;
  fl = oa & (p << 7);fl |= oa & (fl << 7);fl |= oa & (fl << 7);fl |= oa & (fl << 7);fl |= oa & (fl << 7);fl |= oa & (fl << 7);mob |= fl << 7;
  fl = oa & (p >> 7);fl |= oa & (fl >> 7);fl |= oa & (fl >> 7);fl |= oa & (fl >> 7);fl |= oa & (fl >> 7);fl |= oa & (fl >> 7);mob |= fl >> 7;
  fl = oa & (p << 9);fl |= oa & (fl << 9);fl |= oa & (fl << 9);fl |= oa & (fl << 9);fl |= oa & (fl << 9);fl |= oa & (fl << 9);mob |= fl << 9;
  fl = oa & (p >> 9);fl |= oa & (fl >> 9);fl |= oa & (fl >> 9);fl |= oa & (fl >> 9);fl |= oa & (fl >> 9);fl |= oa & (fl >> 9);mob |= fl >> 9;

  return mob & ~(p|o);
}

void Board::play(int x)
{
  if (x == 64)
  {
    change_turn_();
    return;
  }

  unsigned long long *pptr = turn_ ? &black_ : &white_;
  unsigned long long *optr = turn_ ? &white_ : &black_;
  unsigned long long p, o, m, oh, ov, oa, tmp, rev;
  p = *pptr;
  o = *optr;
  m = 1ULL << x;
  oh = o & 0x7e7e7e7e7e7e7e7e;
  ov = o & 0x00ffffffffffff00;
  oa = o & 0x007e7e7e7e7e7e00;
  rev = 0;

  tmp = oh & (m << 1);tmp |= oh & (tmp << 1);tmp |= oh & (tmp << 1);tmp |= oh & (tmp << 1);tmp |= oh & (tmp << 1);tmp |= oh & (tmp << 1);tmp |= oh & (tmp << 1);if (p & (tmp << 1)) rev |= tmp;
  tmp = oh & (m >> 1);tmp |= oh & (tmp >> 1);tmp |= oh & (tmp >> 1);tmp |= oh & (tmp >> 1);tmp |= oh & (tmp >> 1);tmp |= oh & (tmp >> 1);tmp |= oh & (tmp >> 1);if (p & (tmp >> 1)) rev |= tmp;
  tmp = ov & (m << 8);tmp |= ov & (tmp << 8);tmp |= ov & (tmp << 8);tmp |= ov & (tmp << 8);tmp |= ov & (tmp << 8);tmp |= ov & (tmp << 8);tmp |= ov & (tmp << 8);if (p & (tmp << 8)) rev |= tmp;
  tmp = ov & (m >> 8);tmp |= ov & (tmp >> 8);tmp |= ov & (tmp >> 8);tmp |= ov & (tmp >> 8);tmp |= ov & (tmp >> 8);tmp |= ov & (tmp >> 8);tmp |= ov & (tmp >> 8);if (p & (tmp >> 8)) rev |= tmp;
  tmp = oa & (m << 7);tmp |= oa & (tmp << 7);tmp |= oa & (tmp << 7);tmp |= oa & (tmp << 7);tmp |= oa & (tmp << 7);tmp |= oa & (tmp << 7);tmp |= oa & (tmp << 7);if (p & (tmp << 7)) rev |= tmp;
  tmp = oa & (m >> 7);tmp |= oa & (tmp >> 7);tmp |= oa & (tmp >> 7);tmp |= oa & (tmp >> 7);tmp |= oa & (tmp >> 7);tmp |= oa & (tmp >> 7);tmp |= oa & (tmp >> 7);if (p & (tmp >> 7)) rev |= tmp;
  tmp = oa & (m << 9);tmp |= oa & (tmp << 9);tmp |= oa & (tmp << 9);tmp |= oa & (tmp << 9);tmp |= oa & (tmp << 9);tmp |= oa & (tmp << 9);tmp |= oa & (tmp << 9);if (p & (tmp << 9)) rev |= tmp;
  tmp = oa & (m >> 9);tmp |= oa & (tmp >> 9);tmp |= oa & (tmp >> 9);tmp |= oa & (tmp >> 9);tmp |= oa & (tmp >> 9);tmp |= oa & (tmp >> 9);tmp |= oa & (tmp >> 9);if (p & (tmp >> 9)) rev |= tmp;

  *pptr ^= rev | m;
  *optr ^= rev;

  record_ += move_to_str(x);
  change_turn_();
}

bool Board::is_over()
{
  if (mobility_ != 0) return false;

  unsigned long long p, o, oh, ov, oa, fl, mob;
  p = turn_ ? white_ : black_;
  o = turn_ ? black_ : white_;
  oh = o & 0x7e7e7e7e7e7e7e7e;
  ov = o & 0x00ffffffffffff00;
  oa = o & 0x007e7e7e7e7e7e00;
  mob = 0;

  fl = oh & (p << 1);fl |= oh & (fl << 1);fl |= oh & (fl << 1);fl |= oh & (fl << 1);fl |= oh & (fl << 1);fl |= oh & (fl << 1);mob |= fl << 1;
  fl = oh & (p >> 1);fl |= oh & (fl >> 1);fl |= oh & (fl >> 1);fl |= oh & (fl >> 1);fl |= oh & (fl >> 1);fl |= oh & (fl >> 1);mob |= fl >> 1;
  fl = ov & (p << 8);fl |= ov & (fl << 8);fl |= ov & (fl << 8);fl |= ov & (fl << 8);fl |= ov & (fl << 8);fl |= ov & (fl << 8);mob |= fl << 8;
  fl = ov & (p >> 8);fl |= ov & (fl >> 8);fl |= ov & (fl >> 8);fl |= ov & (fl >> 8);fl |= ov & (fl >> 8);fl |= ov & (fl >> 8);mob |= fl >> 8;
  fl = oa & (p << 7);fl |= oa & (fl << 7);fl |= oa & (fl << 7);fl |= oa & (fl << 7);fl |= oa & (fl << 7);fl |= oa & (fl << 7);mob |= fl << 7;
  fl = oa & (p >> 7);fl |= oa & (fl >> 7);fl |= oa & (fl >> 7);fl |= oa & (fl >> 7);fl |= oa & (fl >> 7);fl |= oa & (fl >> 7);mob |= fl >> 7;
  fl = oa & (p << 9);fl |= oa & (fl << 9);fl |= oa & (fl << 9);fl |= oa & (fl << 9);fl |= oa & (fl << 9);fl |= oa & (fl << 9);mob |= fl << 9;
  fl = oa & (p >> 9);fl |= oa & (fl >> 9);fl |= oa & (fl >> 9);fl |= oa & (fl >> 9);fl |= oa & (fl >> 9);fl |= oa & (fl >> 9);mob |= fl >> 9;

  if ((mob & ~(p|o)) != 0) return false;

  return true;
}

void Board::reset()
{
  black_ = 0x0000000810000000;
  white_ = 0x0000001008000000;
  turn_ = 1;
  record_ = "";
  is_black_turn = 1;
  is_white_turn = 0;
  mobility_ = mobility();
}

string Board::to_ASCII_art()
{
  string ascii = "  a b c d e f g h";
  for (int i = 0; i < 64; i++)
  {
    if (i%8 == 0)
    {
      ascii += "\n" + std::to_string(i/8 + 1);
    }

    if ((black_ >> i) & 1)
    {
      ascii += " o";
    }
    else if ((white_ >> i) & 1)
    {
      ascii += " x";
    }
    else if ((mobility_ >> i) & 1)
    {
      ascii += " *";
    }
    else
    {
      ascii += "  ";
    }
  }
  return ascii;
}

string Board::move_to_str(int x)
{
  string m2s[] = {
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
    "pass"
  };
  return m2s[x];
}

int Board::move_from_str(string pos)
{
  if (pos == "pass") return 64;
  
  int i, j;
  i = pos[0] - 'a';
  j = pos[1] - '1';
  return i * 8 + j;
}

void Board::change_turn_()
{
  turn_ = turn_ ? 0 : 1;
  is_black_turn = turn_ ? 1 : 0;
  is_white_turn = turn_ ? 0 : 1;
  mobility_ = mobility();
}

} // namespace crev