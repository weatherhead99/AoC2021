#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <fstream>
#include <algorithm>
#include <set>
#include <map>
#include <unordered_map>

using std::string;
using std::tuple;
using std::vector;
using std::cout;
using std::endl;

using PairInsertionRule = std::tuple<string,char>;

template<typename Container, int I=0>
bool compare_element(const typename Container::value_type& i1, const typename Container::value_type& i2)
{
  return std::get<I>(i1) < std::get<I>(i2);
}


tuple<string, vector<PairInsertionRule>> load_input(const string& fname)
{
  std::ifstream ifs(fname);
  string tpl;
  std::getline(ifs, tpl);

  //this one is a blank line
  string line;
  std::getline(ifs, line);

  vector<PairInsertionRule> out;
  while(std::getline(ifs, line))
    {
      string pr = line.substr(0,2);
      out.emplace_back(pr, line[line.size()-1]);
    }

  return {tpl, out};
}

void apply_rules(string& st_in, const vector<PairInsertionRule>& rules)
{
  vector<std::tuple<std::size_t, char>> insertions;
  //might be too much!
  insertions.reserve(st_in.size());
  
  for(auto& [findstr, insertchr] : rules)
    {
      auto loc = st_in.find(findstr,0);
      while(loc != std::string::npos)
	{
	  insertions.emplace_back(loc+1, insertchr);
	  loc = st_in.find(findstr, loc+1);
	}
    }

  std::sort(insertions.begin(), insertions.end(),
	    compare_element<decltype(insertions)>);

  std::size_t loc_offset = 0;
  for(auto& [loc, insertchr] : insertions)
    st_in.insert(loc+(loc_offset++), 1, insertchr);
      
}

std::map<char, std::size_t> count_characters(const string& st_in)
{
  std::set<char> unique_chars(st_in.cbegin(), st_in.cend());
  std::map<char, std::size_t> out;
  for(auto c : unique_chars)
      out[c] = std::count(st_in.begin(), st_in.end(), c);
  return out;
}

using pair_map = std::unordered_map<string, std::size_t>;

pair_map count_pairs(const string& st_in)
{
  pair_map out;
  for(auto it = st_in.cbegin(); it !=(st_in.cend() -1); it++)
    {
      string pairstr;
      pairstr += (*it);
      pairstr += *(it+1);
      auto finditer = out.find(pairstr);
      if(finditer == out.end())
	out[pairstr] = 1;
      else
	finditer->second += 1;
      
    }

  return out;
}

void apply_rules_part2(pair_map& pairs, const vector<PairInsertionRule>& rules,
		       std::map<char, std::size_t>& charcount)
{

  std::map<string, std::size_t> newpairs;
  for(auto& [findpair, replacechr] : rules)
    {      
      auto pairiter = pairs.find(findpair);
      if(pairiter != pairs.end() and pairiter->second > 0)
	{

	  // cout << "matched pair: " << pairiter->first << endl;
	  string newpair1 = {findpair[0],  replacechr};
	  string newpair2 = {replacechr, findpair[1]};

	  // cout << "newpair1: " << newpair1 << endl;
	  // cout << "newpair2: " << newpair2 << endl;
	  newpairs.try_emplace(newpair1, 0);
	  newpairs[newpair1] += pairiter->second;

	  newpairs.try_emplace(newpair2, 0);
	  newpairs[newpair2] += pairiter->second;
	  
	  charcount.try_emplace(replacechr,0);
	  charcount[replacechr] += pairiter->second;

	  pairiter->second =0;
	}
      
    }
  
  for(auto& [pair, count] : newpairs)
    {
      pairs.try_emplace(pair, 0);
      pairs[pair] += count;
    }

  // for(auto& [pair, count] : pairs)
  //   {
  //     cout << "pair: " << pair << " now has count: " << count << endl;
  //   }

  // for(auto& [chr, count] : charcount)
  //   {
  //     cout << "char: " << chr <<  " now has count: " << count << endl;

  //   }
  
}

void calculate_answer(const std::map<char, std::size_t>& charcount)
{
  auto [miniter, maxiter] = std::minmax_element(charcount.begin(),
						charcount.end(),
						compare_element<std::remove_reference_t<decltype(charcount)>,1>);

    cout << "minimum occuring char: " << miniter->first << " with count: " << miniter->second << endl;
  cout << "max occuring char: " << maxiter->first << " with count: " << maxiter->second << endl;

  cout << "max - min: " << (maxiter->second - miniter->second) << endl;
}

  

void part1(const string& fname)
{
    auto [tpl, rules]  =   load_input(fname);
  cout << "template: " << tpl << endl;

  for(int i=0; i<10; i++)
    apply_rules(tpl, rules);

  cout << "string length after 10 steps:" << tpl.size() << endl;
  
  auto charcount = count_characters(tpl);

  auto [miniter, maxiter] = std::minmax_element(charcount.begin(),
						charcount.end(),
						compare_element<decltype(charcount),1>);


  for(auto& [chr, count]  : charcount)
    {
      cout << "char: " << chr << " has count: " << count << endl;
    }

  calculate_answer(charcount);


}


int main(int argc, char** argv)
{
  // part1("example_input.txt");

  auto [tpl, rules] = load_input("input.txt");
  auto paircount = count_pairs(tpl);

  for(auto& [pair, count] : paircount)
    {
      cout << "pair: " << pair << ", count: " << count << endl;
    }

  auto charcount = count_characters(tpl);
  for(auto& [chr, count] : charcount)
    cout << "char: " << chr << " has count: "<< count << endl;

  for(int i=0; i<40; i++)
    apply_rules_part2(paircount, rules, charcount);

    
  for(auto& [chr, count] : charcount)
    cout << "character: " << chr << " has count: " << count << endl;

  calculate_answer(charcount);
  
}


