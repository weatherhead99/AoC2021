#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <numeric>

std::vector<int> test_1 = {199, 200, 208, 210,200, 207, 240, 269, 260, 263};

using std::cout;
using std::endl;

std::vector<int> sliding_window(int sz, const std::vector<int>& ip)
{
  std::vector<int> out;
  out.reserve(ip.size());

  auto startit = ip.begin();
  auto endit = ip.begin() + sz;

  while(endit <= ip.end())
    {
      out.push_back(std::accumulate(startit, endit,0));
      startit += 1;
      endit += 1;
    }

  return out;

}

int total_increasing(const std::vector<int>& ip)
{
  //first calculate adjacent differences
  std::vector<int> adj_diffs(ip.size());
  std::adjacent_difference(ip.begin(), ip.end(), adj_diffs.begin());

  
  // now count them (skip the first which doesn't have a difference)
  return std::count_if(adj_diffs.begin() +1, adj_diffs.end(), [] (auto& v) { return v > 0;});
}

int main(int argc, char** argv)
{
  std::cout << "test input 1 : " << total_increasing(test_1) << std::endl;
  
  std::ifstream ifs("input.txt");
  std::vector<int> test_ip;
  std::string s;
  for(std::string line ; std::getline(ifs, line);)
    test_ip.push_back(std::atoi(line.c_str()));

  std::cout << "test input for real: " << total_increasing(test_ip) << std::endl;

  auto wdw_test = sliding_window(3, test_1);
  std::cout << "windowed input test"  << std::endl;
  for(auto& v : wdw_test)
    std::cout << v << ",";
  std::cout << std::endl;

  std::cout << "windows input count: " << total_increasing(wdw_test) << std::endl;

  auto wdw_real = sliding_window(3, test_ip);
  std::cout << "real input count: " << total_increasing(wdw_real) << std::endl;
  
};
