#include <iostream>
#include <tuple>
#include <cmath>

using std::tuple;

const std::tuple<int, int> example_limits_y = {-10, -5};
const std::tuple<int, int> limits_y = {-171, -136};

using std::cout;
using std::endl;

std::tuple<double, double> solve_quadratic(int vely0, int y)
{
  double determinant = std::sqrt(vely0 * vely0/4 +  y);
  return {vely0/2 + determinant, vely0/2 - determinant};
}

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


int main(int argc, char** argv)
{
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
  
}
