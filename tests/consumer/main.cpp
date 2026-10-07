#include <betacalendars/calendar_layout.hpp>
int main() {
  using namespace betacalendars::calendar_layout;
  const auto t = topology(2027, 1, week_start::monday);
  return t.days == 31 && t.natural_rows == 5 ? 0 : 1;
}
