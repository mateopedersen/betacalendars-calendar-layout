#pragma once

#include <array>
#include <cmath>
#include <cstddef>
#include <optional>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

namespace betacalendars::calendar_layout {

enum class weekday : int { monday=0, tuesday, wednesday, thursday, friday, saturday, sunday };
enum class week_start : int { monday=0, tuesday, wednesday, thursday, friday, saturday, sunday };
enum class grid_mode { natural, fixed_six_weeks };
enum class adjacent_policy { empty_adjacent, include_adjacent_dates };
enum class cell_relation { previous_month, current_month, next_month, empty };
enum class paper_size { a4, a5, us_letter, us_legal, custom };
enum class orientation { portrait, landscape };
enum class layout_error { invalid_margin, invalid_grid_dimensions, reserved_area_overflow,
                          non_positive_printable_area, non_positive_cell_geometry,
                          invalid_date, invalid_paper_dimensions };

template<class T, class E> class result {
 public:
  result(T value) : data_(std::move(value)) {}
  result(E error) : data_(error) {}
  bool has_value() const noexcept { return std::holds_alternative<T>(data_); }
  explicit operator bool() const noexcept { return has_value(); }
  const T& value() const { return std::get<T>(data_); }
  T& value() { return std::get<T>(data_); }
  E error() const { return std::get<E>(data_); }
 private: std::variant<T,E> data_;
};

struct civil_date { int year; int month; int day; };
struct month_topology { int days; weekday first_weekday; weekday last_weekday;
  int leading_cells; int trailing_cells; int natural_rows; int fixed_cells; };
struct topology_signature { int days; int week_start_offset; int natural_rows; int trailing_capacity; };
struct grid_cell { int row; int column; weekday day_of_week; std::optional<civil_date> date;
  cell_relation relation; };

constexpr bool is_leap_year(int y) noexcept { return y%400==0 || (y%4==0 && y%100!=0); }
constexpr bool valid_month(int m) noexcept { return m>=1 && m<=12; }
constexpr int days_in_month(int y, int m) noexcept {
  if(y<1||y>9999||!valid_month(m)) return 0;
  return m==2 ? (is_leap_year(y)?29:28) : ((m==4||m==6||m==9||m==11)?30:31);
}
constexpr bool valid_date(civil_date d) noexcept {
  return d.year>=1 && d.year<=9999 && valid_month(d.month) && d.day>=1 && d.day<=days_in_month(d.year,d.month);
}

// Proleptic Gregorian weekday calculation; independent of locale, timezone, and DST.
constexpr weekday weekday_of(civil_date d) noexcept {
  const int prior_year=d.year-1;
  int days=prior_year*365+prior_year/4-prior_year/100+prior_year/400;
  for(int m=1;m<d.month;++m) days+=days_in_month(d.year,m);
  days+=d.day-1;
  return static_cast<weekday>(days%7); // 0001-01-01 was Monday.
}
constexpr int weekday_index(weekday d) noexcept { return static_cast<int>(d); }
constexpr int relative_weekday_index(weekday d, week_start start) noexcept {
  return (weekday_index(d)-static_cast<int>(start)+7)%7;
}
constexpr int offset_from_week_start(weekday first, week_start start) noexcept {
  return relative_weekday_index(first,start);
}
constexpr int natural_row_count(int days, int leading) noexcept { return (leading+days+6)/7; }
constexpr int trailing_cell_count(int days, int leading, int rows) noexcept { return rows*7-leading-days; }
constexpr month_topology topology(int year,int month,week_start start=week_start::monday) noexcept {
  const int days=days_in_month(year,month);
  const weekday first=weekday_of({year,month,1});
  const weekday last=weekday_of({year,month,days});
  const int leading=offset_from_week_start(first,start);
  const int rows=natural_row_count(days,leading);
  return {days,first,last,leading,trailing_cell_count(days,leading,rows),rows,42};
}
constexpr topology_signature signature(int year,int month,week_start start=week_start::monday) noexcept {
  const auto t=topology(year,month,start);
  return {t.days,t.leading_cells,t.natural_rows,t.trailing_cells};
}

inline std::optional<civil_date> shift_month(civil_date first, int delta) {
  int idx=first.year*12+(first.month-1)+delta;
  int y=idx/12, m=idx%12;
  if(m<0){m+=12;--y;} ++m;
  if(y<1 || y>9999) return std::nullopt;
  return civil_date{y,m,1};
}
inline result<std::vector<grid_cell>,layout_error> month_grid(int year,int month,
    week_start start=week_start::monday, grid_mode mode=grid_mode::natural,
    adjacent_policy adjacent=adjacent_policy::include_adjacent_dates) {
  if(year<1||year>9999||!valid_month(month)) return layout_error::invalid_date;
  const auto t=topology(year,month,start);
  const int rows=mode==grid_mode::natural?t.natural_rows:6;
  std::vector<grid_cell> out; out.reserve(static_cast<std::size_t>(rows*7));
  const int first_offset=t.leading_cells;
  for(int i=0;i<rows*7;++i){
    int date_day=i-first_offset+1; std::optional<civil_date> date; cell_relation rel=cell_relation::empty;
    if(date_day>=1&&date_day<=t.days){date=civil_date{year,month,date_day};rel=cell_relation::current_month;}
    else if(adjacent==adjacent_policy::include_adjacent_dates){
      if(date_day<1){auto pm=shift_month({year,month,1},-1);if(pm){const int pd=days_in_month(pm->year,pm->month)+date_day;date=civil_date{pm->year,pm->month,pd};rel=cell_relation::previous_month;}}
      else {auto nm=shift_month({year,month,1},1);if(nm&&date_day-t.days<=days_in_month(nm->year,nm->month)){date=civil_date{nm->year,nm->month,date_day-t.days};rel=cell_relation::next_month;}}
    }
    const auto wd=static_cast<weekday>((static_cast<int>(start)+i%7)%7);
    out.push_back({i/7,i%7,wd,date,rel});
  }
  return out;
}

struct margins_mm { double top=10,right=10,bottom=10,left=10; };
struct paper_dimensions_mm { double width; double height; };
struct layout_spec { paper_size paper=paper_size::a4; orientation page_orientation=orientation::portrait;
  margins_mm margins{}; double title_height=16; double weekday_header_height=8; double notes_height=0;
  int rows=5; int columns=7; paper_dimensions_mm custom{210,297}; };
struct layout_metrics { double page_width_mm,page_height_mm,printable_width_mm,printable_height_mm;
  double grid_width_mm,grid_height_mm,cell_width_mm,cell_height_mm,cell_area_mm2,usable_writing_area_mm2;
  int unused_grid_capacity; };
inline result<layout_metrics,layout_error> measure(const layout_spec& s,int padding_per_side_mm=2) {
  const auto m=s.margins;
  if(!std::isfinite(m.top)||!std::isfinite(m.right)||!std::isfinite(m.bottom)||!std::isfinite(m.left)||
     !std::isfinite(s.title_height)||!std::isfinite(s.weekday_header_height)||!std::isfinite(s.notes_height)||
     m.top<0||m.right<0||m.bottom<0||m.left<0||padding_per_side_mm<0) return layout_error::invalid_margin;
  if(s.rows<=0||s.columns<=0) return layout_error::invalid_grid_dimensions;
  paper_dimensions_mm p{};
  switch(s.paper){case paper_size::a4:p={210,297};break;case paper_size::a5:p={148,210};break;
    case paper_size::us_letter:p={215.9,279.4};break;case paper_size::us_legal:p={215.9,355.6};break;case paper_size::custom:p=s.custom;break;
    default:return layout_error::invalid_paper_dimensions;}
  if(!std::isfinite(p.width)||!std::isfinite(p.height)||p.width<=0||p.height<=0) return layout_error::invalid_paper_dimensions;
  if(s.page_orientation==orientation::landscape) std::swap(p.width,p.height);
  const double pw=p.width-m.left-m.right, ph=p.height-m.top-m.bottom;
  if(pw<=0||ph<=0) return layout_error::non_positive_printable_area;
  const double gh=ph-s.title_height-s.weekday_header_height-s.notes_height;
  if(s.title_height<0||s.weekday_header_height<0||s.notes_height<0||gh<=0) return layout_error::reserved_area_overflow;
  const double cw=pw/s.columns,ch=gh/s.rows,innerw=cw-2.0*padding_per_side_mm,innerh=ch-2.0*padding_per_side_mm;
  if(cw<=0||ch<=0||innerw<=0||innerh<=0) return layout_error::non_positive_cell_geometry;
  return layout_metrics{p.width,p.height,pw,ph,pw,gh,cw,ch,cw*ch,innerw*innerh,0};
}
inline result<layout_metrics,layout_error> measure_month(int y,int m,week_start start,grid_mode mode,
    paper_size paper=paper_size::a4,orientation o=orientation::portrait,margins_mm margins={}) {
  if(y<1||y>9999||!valid_month(m))return layout_error::invalid_date;
  layout_spec s; s.paper=paper;s.page_orientation=o;s.margins=margins;
  s.rows=mode==grid_mode::natural?topology(y,m,start).natural_rows:6;
  auto measured=measure(s); if(!measured)return measured.error();
  auto v=measured.value();v.unused_grid_capacity=(s.rows*7-days_in_month(y,m));return v;
}
inline result<layout_metrics,layout_error> measure_blank(int rows=5,int columns=7,
    paper_size paper=paper_size::a4,orientation o=orientation::portrait,margins_mm margins={}) {
  layout_spec s;s.rows=rows;s.columns=columns;s.paper=paper;s.page_orientation=o;s.margins=margins;
  return measure(s);
}

static_assert(is_leap_year(2000)&&!is_leap_year(1900)&&is_leap_year(2024)&&!is_leap_year(2027)&&!is_leap_year(2100)&&is_leap_year(2400),"Gregorian leap-year rule");
static_assert(topology(2027,1,week_start::monday).natural_rows==5,"January 2027 topology");

} // namespace betacalendars::calendar_layout
