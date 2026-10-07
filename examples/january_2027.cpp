#include <betacalendars/calendar_layout.hpp>
#include <iostream>
int main(){const auto t=betacalendars::calendar_layout::topology(2027,1);std::cout<<t.days<<" days, "<<t.natural_rows<<" Monday-first rows\n";}
