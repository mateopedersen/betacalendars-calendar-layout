#include <betacalendars/calendar_layout.hpp>
#include <iostream>
int main(){auto m=betacalendars::calendar_layout::measure_month(2027,1,betacalendars::calendar_layout::week_start::monday,betacalendars::calendar_layout::grid_mode::natural);if(!m)return 1;std::cout<<m.value().cell_width_mm<<" × "<<m.value().cell_height_mm<<" mm\n";}
