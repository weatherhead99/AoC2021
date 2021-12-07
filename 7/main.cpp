#include <iostream>
#include <numeric>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <tuple>

using std::string;
using std::vector;
using std::cout;
using std::endl;

vector<int> get_crabs(const string& fname)
{
  std::ifstream ifs(fname);
  string line;
  vector<int> out;
  while(std::getline(ifs, line, ','))
    out.push_back(std::atoi(line.c_str()));
  return out;
}

int calculate_fuel_cost(const vector<int>& crabs, int vertpos)
{
  int fuel_cost = std::accumulate(crabs.begin(), crabs.end(), 0,
				  [vertpos] (int sum, int crab)
				  { return sum + std::abs(crab - vertpos);});
  return fuel_cost;
}

int calculate_fuel_cost_part2(const vector<int>& crabs, int vertpos)
{
  int fuel_cost = std::accumulate(crabs.begin(), crabs.end(), 0,
				  [vertpos] (int sum, int crab)
				  {
				    auto dist = std::abs(crab - vertpos);
				    auto fuel = dist * (1 + dist) / 2;
				    return sum + fuel;
				  });
  return fuel_cost;

}


template<typename Callable>
std::tuple<int, int> find_minima_brute_force_descent(const vector<int>& crabs, Callable& fuelfunc)
{
  auto [miniter, maxiter] = std::minmax_element(crabs.begin(), crabs.end());
  int mincrab = *miniter;
  int maxcrab = *maxiter;

  int startpos = (mincrab + maxcrab)/2;
  int fuel_startpos = fuelfunc(crabs, startpos);
  int fuel_plus = fuelfunc(crabs, startpos +1 );

  int direction = (fuel_plus > fuel_startpos) ? -1 : 1 ;

  int pos = startpos;
  int fuel_pos = fuelfunc(crabs, pos);
  while(true)
    {
      pos += direction;
      fuel_pos = fuelfunc(crabs, pos);

      int fuel_plus = fuelfunc(crabs, pos+1);
      int fuel_minus = fuelfunc(crabs, pos-1);

      
      if( (fuel_plus > fuel_pos) and (fuel_minus > fuel_pos))
	break;
      
    }

  return {pos, fuel_pos};

}



int main(int argc, char** argv)
{
  auto example_input = get_crabs("example_input.txt");
  for(auto& crab: example_input)
    cout << crab << ",";
  cout << endl;

  auto [pos, fuel_pos] = find_minima_brute_force_descent(example_input, calculate_fuel_cost);

  cout << "pos: " << pos << ", fuel: " << fuel_pos << endl;


  auto input = get_crabs("input.txt");
  auto [pos2, fuel_pos2] = find_minima_brute_force_descent(input, calculate_fuel_cost);
  cout << "pos: " << pos2 << ", fuel: " << fuel_pos2 << " for real" << endl;
  

  auto[pos3, fuel_pos3] = find_minima_brute_force_descent(example_input, calculate_fuel_cost_part2);
  cout << "pos: "<< pos3  << ", fuel: " << fuel_pos3 << endl;

  auto [pos4, fuel_pos4] = find_minima_brute_force_descent(input, calculate_fuel_cost_part2);
  cout << "pos: " << pos4 << ", fuel: " << fuel_pos4 << endl;

  
}
