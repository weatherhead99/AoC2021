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
  double det = std::sqrt(4*vely0*vely0 + 4*vely0 - 8*y + 1);
  return {-0.5*(det)+vely0+0.5, 0.5*(det + 2*vely0+1)};

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
  auto test = example_limits_y;
  //auto test = limits_y;
  auto v_lowlim = velocity_lowerlim_determinant(std::get<0>(test));
  cout << "minimum valid velocity: " << v_lowlim << endl;

  int tryvel = std::ceil(v_lowlim);
  
  while(true)
    {
      cout << "velocity: " << tryvel << endl;
      auto [t1, t2] = solve_quadratic(tryvel, std::get<0>(test));
      auto [t3, t4] = solve_quadratic(tryvel, std::get<1>(test));
      cout << "t1: " << t1 << " t2: " << t2 << "t3: " << t3 << " t4: " << t4 << endl;
      auto cond = std::abs(std::floor(t1) - std::floor(t3));
      cout << "yn:" << y_n(std::floor(tryvel), std::floor(t1)) << endl;;
      if( std::abs(std::floor(t1) - std::floor(t3))< 1)
	break;
      
      tryvel += 1.;
    }

  cout << "maximum valid y velocity is: " << tryvel << endl;
  auto nmax = std::round(tryvel + 0.25);
  cout << "nmax: " << nmax << endl;
  cout << "maximum height is: " << y_n(tryvel, nmax) << endl;
  cout << y_n(tryvel, 44) << ", " << y_n(tryvel, 45) << endl;

  auto [t1, t2] = solve_quadratic2(9, 15);
  cout << "test solve: " << t1 << ", " << t2 << endl;
  for(int i=0; i < nmax+20; i++)
    cout <<"i: " << i << " maxheight: " << y_n(tryvel, i) << endl;
  
}
