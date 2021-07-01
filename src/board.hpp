#ifndef CREV_BOARD_HPP
#define CREV_BOARD_HPP

#include <string>

using std::string;

namespace crev
{

class Board
{
  private:
    unsigned long long black_;
    unsigned long long white_;
    int turn_;
    string record_;
    unsigned long long mobility_;
    void change_turn_();
  public:
    Board();
    int is_black_turn;
    int is_white_turn;
    unsigned long long mobility();
    void play(int x);
    bool is_over();
    void reset();
    string get_record();
    string to_ASCII_art();
    string move_to_str(int x);
    int move_from_str(string pos);
};

} // namespace crev

#endif