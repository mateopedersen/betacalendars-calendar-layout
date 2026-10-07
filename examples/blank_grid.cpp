#include <betacalendars/calendar_layout.hpp>
#include <iostream>
int main(){auto m=betacalendars::calendar_layout::measure_blank(5,7);if(!m)return 1;std::cout<<"Undated cells: "<<m.value().cell_width_mm<<" × "<<m.value().cell_height_mm<<" mm\n";}
