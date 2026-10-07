#include <betacalendars/calendar_layout.hpp>
#include <iostream>
#include <string>
using namespace betacalendars::calendar_layout;
int main(int argc,char** argv){
  if(argc<4||std::string(argv[1])!="month"){
    std::cerr<<"Usage: betacal-layout month YEAR MONTH [monday|sunday]\n";return 2;
  }
  int y=std::stoi(argv[2]),m=std::stoi(argv[3]);week_start ws=week_start::monday;
  if(argc>4&&std::string(argv[4])=="sunday")ws=week_start::sunday;
  auto g=month_grid(y,m,ws);
  if(!g){std::cerr<<"Invalid Gregorian year or month.\n";return 2;}
  const auto t=topology(y,m,ws);
  std::cout<<y<<'-'<<(m<10?"0":"")<<m<<" days="<<t.days<<" rows="<<t.natural_rows
           <<" leading="<<t.leading_cells<<" trailing="<<t.trailing_cells<<"\n";
  for(int c=0;c<7;++c)std::cout<<" "<<weekday_index(g.value()[static_cast<std::size_t>(c)].day_of_week);
  std::cout<<"\n";
  for(const auto& cell:g.value()){if(cell.column==0)std::cout<<"\n";if(cell.date&&cell.relation==cell_relation::current_month)std::cout<<cell.date->day;else std::cout<<".";std::cout<<(cell.column==6?"":"\t");}
  std::cout<<"\n";return 0;
}
