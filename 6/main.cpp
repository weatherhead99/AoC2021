#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <map>

using std::string;
using std::vector;
using std::cout;
using std::endl;

using fish_map = std::map<int, long long unsigned>;

vector<int> read_state(const string& fname)
{
  vector<int> out;
  std::ifstream ifs(fname);
  string line;
  while(std::getline(ifs, line, ','))
    {
      out.push_back(std::atoi(line.c_str()));

    }

  return out;
}

fish_map fish_vec_to_map(const vector<int>& fish)
{
  fish_map out;
  for(int i=0; i<9; i++)
    {
      auto count = std::count(fish.begin(), fish.end(), i);
      out[i] = count;
    }
  
  return out;
}


void process_fish(vector<int>& fish)
{
  std::vector<int> new_fish;
  std::for_each(fish.begin(), fish.end(), [&new_fish] (int& f)
		{
		  if(f ==0)
		    {
		      new_fish.push_back(8);
		      f = 6;
		    }
		  else
		    f--;
		});
  fish.insert(fish.end(), new_fish.begin(), new_fish.end());
}

void process_fish(fish_map& fish)
{
  fish_map new_map;

  for(int i=8; i>0; i--)
      new_map[i-1] = fish[i];
  
  new_map[8] = fish[0];
  new_map[6] += fish[0];

  fish = new_map;
}

long long unsigned count_fish(const fish_map& fish)
{
  auto count = std::accumulate(fish.begin(), fish.end(),
			       0ull, [](long long unsigned sm,  const auto& pair){
				 return sm + pair.second;
				 });
  return count;

}

void print_fish(const vector<int>& fish)
{
  for(const auto& f : fish)
    {
      cout << f  << ",";
    }
  cout << endl;
}

void print_fish(const fish_map& fish)
{
  for(const auto& [k,v] : fish)
    cout << k << ", : " << v << ",";
  cout << endl;
}



void part_1()
{
  auto example_fish = read_state("example_input.txt");

  cout << "initial state: " << endl;
  print_fish(example_fish);

  for(int i=1; i < 19; i++)
    {
      process_fish(example_fish);
      cout << "after " << i << "days: " << endl;
      print_fish(example_fish);
    }

  cout << "total fish after 18 days: " << example_fish.size() << endl;
  for(int i=18; i < 80; i++)
    process_fish(example_fish);

  auto total_fish_80 = example_fish.size();
  cout << "total fish after 80 days: " << total_fish_80 << endl;


  auto real_fish = read_state("input.txt");
  for(int i=1; i < 81; i++)
    {
      process_fish(real_fish);
    }
  cout << "after 80 days, real answer: " << real_fish.size() << endl;
}


int main(int argc, char** argv)
{
  //part_1();
  
  auto example_fish = read_state("example_input.txt");
  auto example_fish_map = fish_vec_to_map(example_fish);


    for(int i=1; i < 19; i++)
    {
      process_fish(example_fish_map);
    }

    cout << "part 1 example check: " << count_fish(example_fish_map) << endl;

    for(int i=18; i < 256; i++)
      {
	process_fish(example_fish_map);
      }

   
    cout << "part 2 example check: " << count_fish(example_fish_map) << endl;


    auto real_fish = read_state("input.txt");
    auto real_fish_map = fish_vec_to_map(real_fish);

    for(int i=0; i < 256; i++)
      process_fish(real_fish_map);

    cout << "part 2 real answer: " << count_fish(real_fish_map) << endl;
    
}
