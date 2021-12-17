#include <iostream>
#include <tuple>
#include <cmath>
#include <vector>
#include <set>

using std::tuple;

const std::tuple<int, int> example_limits_y = {-10, -5};
const std::tuple<int, int> limits_y = {-171, -136};

const std::tuple<int,int> example_limits_x = {20, 30};
const std::tuple<int, int> limits_x = {60,94};

using std::cout;
using std::endl;


std::tuple<double, double> solve_quadratic2(int vely0, int y)
{
  auto f = (vely0 + 0.5);
  auto det = std::sqrt( f*f - 2*y);
  return {f + det, f-det};
}

double velocity_lowerlim_determinant(int ymin)
{
  return std::sqrt(-2*ymin);
}

int y_n(int v, int n)
{
  return n * v - n*(n-1)/2;
}


void part1() 
{
  // {{{ 

  //auto test = example_limits_y;
  auto test = limits_y;
  auto v_lowlim = velocity_lowerlim_determinant(std::get<0>(test));
  cout << "minimum valid velocity: " << v_lowlim << endl;
  cout << "maximum valid velocity: " <<velocity_lowerlim_determinant(std::get<1>(test));
  int tryvel = std::ceil(v_lowlim);
  int thisvel = tryvel;
  int targetn = 0;
  double diff = 2.0;
  while(diff > 0.1)
    {
      cout << "velocity: " << thisvel << endl;
      auto [t1, t2] = solve_quadratic2(thisvel, std::get<0>(test));
      auto [t3, t4] = solve_quadratic2(thisvel, std::get<1>(test));
      cout << "t1: " << t1 << " t3: " << t3  << endl;
      diff = std::abs(t1 - t3);
      cout << "diff: " << diff << endl;
      auto cond = std::abs(std::floor(t1) - std::floor(t3));
      if( cond >= 1)
	{
	  tryvel = thisvel;
	  auto [tn1, tn2] = solve_quadratic2(tryvel, std::get<1>(test));
	  targetn = std::ceil(tn1);
	}
      
      thisvel += 1.;

      
    }

  cout << "maximum valid y velocity is: " << tryvel << endl;
  auto nmax = std::round(tryvel + 0.25);
  cout << "nmax: " << nmax << endl;
  cout << "maximum height is: " << y_n(tryvel, nmax) << endl;
  cout << "targetn: " << targetn << endl;

  for(int i=targetn-3; i < targetn+3; i++)
    cout << "n: " << i << " y: "<< y_n(tryvel, i) << endl;

  cout << "-----------------------" << endl;
  for(int i=nmax-1; i < nmax+3; i++)
    cout <<"i: " << i << " maxheight: " << y_n(tryvel, i) << endl;

  
};

// }}}

template<typename Func>
std::vector<int>  search_ys(const std::tuple<int,int>& test, Func&& fun)
{
  int thisvel = 0;
  int targetn = 0;
  double diff = 2.0;
  
  std::vector<int> valid_ys;
  while(diff > 0.001)
    {
      auto [t1, t2] = solve_quadratic2(thisvel, std::get<0>(test));
      auto [t3, t4] = solve_quadratic2(thisvel, std::get<1>(test));
      diff = std::abs(t1 - t3);
      auto cond = std::abs(std::floor(t1) - std::floor(t3));
      if( cond >= 1)
	{
	  valid_ys.push_back(thisvel);
	}
      
      fun(thisvel);
    }
  return valid_ys;
}

auto incr = [](int& v) { v++;};
auto decr = [](int& v) { v--;};


int x_n(int u, int n)
{
  return n < u ? n*u - n*(n-1)/2 : u*u - u*(u-1)/2;
}

int x_max(int u)
{
  return u*u - u*(u-1)/2;
}

std::vector<int> search_xs(const std::tuple<int,int>& test)
{
  int u = 0;
  std::vector<int> out;
  cout << "x_max: " << x_max(u) << endl;
  while(x_max(u) < std::get<0>(test))
    u++;

  cout << "new u: " << u << endl;
  while(x_n(u,1) <= std::get<1>(test))
    {
      //solve for when it hits the minimum
      auto [t1, t2] = solve_quadratic2(u, std::get<0>(test));
      auto [t3, t4] = solve_quadratic2(u, std::get<1>(test));

      cout << "u: " << u << endl;
      cout << "t1: " << t1 << " t3: " << t3 << endl;
      cout << "lowest: " << x_n(u, std::ceil(t1)) << endl;
      if(x_n(u, std::ceil(t1)) >= std::get<0>(test))
	{
	  cout << "matches!" << endl;
	  cout << "u: " << u << endl;
	  out.push_back(u);
	}

      u += 1;
    }
  return out;

}


int main(int argc, char** argv)
{
  auto test = example_limits_y;
  //auto test = limits_y;
  int tryvel = 0;
  int thisvel = tryvel;
  int targetn = 0;
  double diff = 2.0;

  std::vector<int> valid_ys = search_ys(test, incr);

  std::vector<int> vy2s = search_ys(test, decr);

  std::copy(vy2s.begin(), vy2s.end(), std::back_insert_iterator(valid_ys));

  std::set<int> unique_ys(valid_ys.begin(), valid_ys.end());
  cout << "valid ys:" << endl;
  for(auto& y : unique_ys)
    cout << y << ", ";
  cout << endl;


  cout << "-----" << endl;
  for(int i=0; i < 10; i++)
    {
      cout << "i: " << i << " x: " << x_max(i) << endl;
    }


  auto testx = example_limits_x;
  std::vector<int> valid_xs = search_xs(testx);
  cout << "valid xs: " << endl;
  for(auto& x : valid_xs)
    cout << x << ",";
  cout << endl;

  std::set<int> unique_xs(valid_xs.begin(), valid_xs.end());

  cout << "total combinations: " << unique_ys.size() * unique_xs.size() << endl;
  
}
