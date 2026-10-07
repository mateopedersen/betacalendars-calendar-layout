#include <betacalendars/calendar_layout.hpp>
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <cstddef>
#include <array>
static std::size_t checks=0;
#define CHECK(expression) do { ++checks; if(!(expression)) { std::cerr << "CHECK failed at " << __FILE__ << ':' << __LINE__ << ": " #expression << '\n'; return 1; } } while(false)
using namespace betacalendars::calendar_layout;

int main() {
  CHECK(!is_leap_year(1900)); CHECK(is_leap_year(2000)); CHECK(is_leap_year(2024));
  CHECK(!is_leap_year(2027)); CHECK(!is_leap_year(2100)); CHECK(is_leap_year(2400));
  CHECK(weekday_of({2027,1,1})==weekday::friday);
  CHECK(weekday_of({2027,2,1})==weekday::monday);
  CHECK(weekday_of({2026,12,31})==weekday::thursday);
  CHECK(weekday_of({1,1,1})==weekday::monday);
  CHECK(weekday_of({1900,1,1})==weekday::monday);
  CHECK(weekday_of({2000,1,1})==weekday::saturday);
  CHECK(weekday_of({2400,1,1})==weekday::saturday);
  CHECK(days_in_month(2027,2)==28 && days_in_month(2024,2)==29);
  CHECK(days_in_month(2027,13)==0 && days_in_month(0,1)==0);
  CHECK(!month_grid(2027,13));
  for(int y=1;y<=9999;++y) {
    int sum=0;
    for(int m=1;m<=12;++m) {
      const int n=days_in_month(y,m); CHECK(n>=28&&n<=31); sum+=n;
      const auto first=weekday_of({y,m,1});
      CHECK(weekday_of({y,m,n})==static_cast<weekday>((weekday_index(first)+n-1)%7));
      for(int s=0;s<7;++s) {
        const auto t=topology(y,m,static_cast<week_start>(s));
        CHECK(t.leading_cells>=0&&t.leading_cells<7);
        CHECK(t.natural_rows>=4&&t.natural_rows<=6);
        CHECK(t.leading_cells+n+t.trailing_cells==t.natural_rows*7);
        CHECK(t.fixed_cells==42);
      }
    }
    CHECK(sum==(is_leap_year(y)?366:365));
  }
  // Exercise materialized cells for every month/week origin in the requested
  // 1900–2100 package-validation window, in both natural and fixed modes.
  for(int y=1900;y<=2100;++y) for(int m=1;m<=12;++m) for(int s=0;s<7;++s) {
    const auto start=static_cast<week_start>(s);
    auto natural=month_grid(y,m,start,grid_mode::natural,adjacent_policy::include_adjacent_dates);
    auto fixed=month_grid(y,m,start,grid_mode::fixed_six_weeks,adjacent_policy::include_adjacent_dates);
    CHECK(natural&&fixed);
    CHECK(natural.value().size()==static_cast<std::size_t>(topology(y,m,start).natural_rows*7));
    CHECK(fixed.value().size()==42);
    int natural_days=0,fixed_days=0;
    std::array<bool,32> natural_seen{},fixed_seen{};
    for(const auto& cell:natural.value()) if(cell.relation==cell_relation::current_month) {
      CHECK(cell.date&&cell.date->year==y&&cell.date->month==m);
      CHECK(cell.date->day>=1&&cell.date->day<=31);
      CHECK(!natural_seen[static_cast<std::size_t>(cell.date->day)]);
      natural_seen[static_cast<std::size_t>(cell.date->day)]=true;
      ++natural_days;
    }
    for(const auto& cell:fixed.value()) if(cell.relation==cell_relation::current_month) {
      CHECK(cell.date&&cell.date->year==y&&cell.date->month==m);
      CHECK(!fixed_seen[static_cast<std::size_t>(cell.date->day)]);
      fixed_seen[static_cast<std::size_t>(cell.date->day)]=true;
      ++fixed_days;
    }
    CHECK(topology(y,m,start).leading_cells>=0&&topology(y,m,start).leading_cells<7);
    CHECK(natural_days==days_in_month(y,m));
    CHECK(fixed_days==days_in_month(y,m));
    for(int d=1;d<=days_in_month(y,m);++d) CHECK(natural_seen[static_cast<std::size_t>(d)]&&fixed_seen[static_cast<std::size_t>(d)]);
  }
  int days_2027=0;for(int month=1;month<=12;++month)days_2027+=days_in_month(2027,month);
  CHECK(days_2027==365&&days_in_month(2027,2)==28);
  CHECK(days_in_month(2026,11)==30&&days_in_month(2026,12)==31);
  CHECK(weekday_of({2026,12,31})==weekday::thursday&&weekday_of({2027,1,1})==weekday::friday);
  CHECK(weekday_of({2027,1,1})==static_cast<weekday>((weekday_index(weekday_of({2026,12,31}))+1)%7));
  const int known_2027_rows[12][2]={{5,6},{4,5},{5,5},{5,5},{6,6},{5,5},{5,5},{6,5},{5,5},{5,6},{5,5},{5,5}};
  for(int m=1;m<=12;++m){CHECK(topology(2027,m,week_start::monday).natural_rows==known_2027_rows[m-1][0]);CHECK(topology(2027,m,week_start::sunday).natural_rows==known_2027_rows[m-1][1]);}
  for(int m=11;m<=12;++m){auto g=month_grid(2026,m,week_start::monday);CHECK(g&&g.value().size()==static_cast<std::size_t>(topology(2026,m).natural_rows*7));}
  auto jan=month_grid(2027,1,week_start::monday,grid_mode::natural,adjacent_policy::include_adjacent_dates);
  CHECK(jan&&jan.value().front().date->year==2026&&jan.value().front().date->month==12&&jan.value().front().date->day==28);
  auto empty=month_grid(2027,1,week_start::monday,grid_mode::natural,adjacent_policy::empty_adjacent);
  CHECK(empty&&!empty.value().front().date&&empty.value().front().relation==cell_relation::empty);
  auto feb=month_grid(2027,2,week_start::sunday,grid_mode::fixed_six_weeks);CHECK(feb&&feb.value().size()==42);
  const paper_dimensions_mm papers[]={{210,297},{148,210},{215.9,279.4},{215.9,355.6}};
  const paper_size kinds[]={paper_size::a4,paper_size::a5,paper_size::us_letter,paper_size::us_legal};
  for(int i=0;i<4;++i){layout_spec spec;spec.paper=kinds[i];auto r=measure(spec);CHECK(r);CHECK(std::abs(r.value().page_width_mm-papers[i].width)<1e-9);CHECK(std::abs(r.value().page_height_mm-papers[i].height)<1e-9);}
  layout_spec landscape;landscape.page_orientation=orientation::landscape;auto lr=measure(landscape);CHECK(lr&&lr.value().page_width_mm==297&&lr.value().page_height_mm==210);
  layout_spec invalid;invalid.margins.left=-1;CHECK(!measure(invalid));
  CHECK(!measure_blank(0,7));
  auto blank=measure_blank(5,7,paper_size::us_letter);CHECK(blank&&blank.value().cell_width_mm>0);
  std::cout<<"Calendar Layout passed "<<checks<<" checks; Gregorian years 1-9999 verified.\n";
}
