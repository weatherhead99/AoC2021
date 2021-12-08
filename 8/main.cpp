#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>
#include "data.hh"
#include <cmath>

using std::string;
using std::vector;
using std::tuple;
using std::cout;
using std::endl;


using segset = std::set<segment>;
using seg_unknown_map = std::map<segment, segset>;

vector<segset> string_to_segset_vector(const string& s)
{
  std::istringstream iss(s);
  vector<segset> out;
  
  while(iss)
    {
      string ssetstr;
      iss >> ssetstr;
      if(ssetstr.size() == 0)
	continue;
      std::set<segment> segset;
      for(auto& c : ssetstr)
	  switch(c)
	    {
	    case('a') : segset.insert(segment::a); break;
	    case('b') : segset.insert(segment::b); break;
	    case('c') : segset.insert(segment::c); break;
	    case('d') : segset.insert(segment::d); break;
	    case('e') : segset.insert(segment::e); break;
	    case('f') : segset.insert(segment::f); break;
	    case('g') : segset.insert(segment::g); break;
	    default:
	      throw std::runtime_error("invalid character for segment");
	    }
      out.push_back(segset);
    }
  
  return out;
}

tuple<vector<segset>, vector<segset>> load_line(const string& line)
{
  std::istringstream iss(line);

  string firstpart;
  string secondpart;
  std::getline(iss, firstpart, '|');
  std::getline(iss, secondpart);

  auto firstset = string_to_segset_vector(firstpart);
  auto secondset = string_to_segset_vector(secondpart);
  return {firstset, secondset};
}


vector<tuple<vector<segset>, vector<segset>>> load_file(const string& fname)
{
  vector<tuple<vector<segset>, vector<segset>>> out;

  std::ifstream ifs(fname);
  string line;
  while(std::getline(ifs, line))
    {
      out.push_back(load_line(line));
    }

  return out;

}


void print_segs(const vector<segset>& in)
{
  for(auto& set: in)
    {
      for(auto& seg: set)
	cout << static_cast<char>(seg);
      cout << " ";
    }
}

void print_segs(const segset& in)
{
  for(auto& seg: in)
    cout << static_cast<char>(seg);
}


template<typename Segments>
typename Segments::const_iterator find_digit_by_set_length(const Segments& segments, int len)
{

  auto cond = [len](auto& v) { return v.size() == len;};
  
  auto iter = std::find_if(segments.begin(), segments.end(), cond);

  if(iter == segments.end())
    throw std::runtime_error("couldn't find digit!");
  
  auto iter2 = std::find_if(iter+1, segments.end(), cond);
  if(iter2 != segments.end())
    {
      throw std::runtime_error("condition not unique!");
    }
  
  return iter;
}


template<typename Segments>
typename Segments::const_iterator find_subset(const Segments& segs, const segset& subset, bool inv)
{
  auto oper = [subset, inv] (const auto& v)
    { return inv ^ std::includes(v.begin(), v.end(), subset.begin(), subset.end());};

  auto iter = std::find_if(segs.begin(), segs.end(), oper);
  if(iter == segs.end())
    throw std::runtime_error("couldn't find digit!");

  auto iter2 = std::find_if(iter +1 , segs.end(), oper);
  if(iter2 != segs.end())
    throw std::runtime_error("condition not unique!");
  
  return iter;
}


std::map<digits, segset> decode_segments(const vector<segset>& unique_inputs)
{

  vector<segset> unknown_digits = unique_inputs;
  std::map<digits, segset> out;
  //first, find the digits we know by unique lengths

  //    cout << "sizes: " << endl;

  auto digit_process = [&unknown_digits, &out] (auto&& iter, digits digit, int expected_size)
    { out[digit] = *iter;
      unknown_digits.erase(iter);

      if(out[digit].size() != expected_size)
	throw std::runtime_error("digit not of expected size!");

    };
  
  //  cout << "identifying 1, 4, 7, 8..." << endl;
  digit_process(find_digit_by_set_length(unknown_digits,2), digits::one, 2);
  digit_process(find_digit_by_set_length(unknown_digits,4), digits::four, 4);
  digit_process(find_digit_by_set_length(unknown_digits,3), digits::seven, 3);
  digit_process(find_digit_by_set_length(unknown_digits,7), digits::eight, 7);
  
  //  cout << "finding 6... " << endl;

  //the set difference of digits 1 & 8 is a subset only of digit 6
  segset diff_1_8;
  std::set_symmetric_difference(out[digits::one].begin(), out[digits::one].end(),
				out[digits::eight].begin(), out[digits::eight].end(),
				std::inserter(diff_1_8, diff_1_8.end()));


  digit_process(find_subset(unknown_digits, diff_1_8, false), digits::six, 6);

  
  //the intersection of digits 1 & 6 is only NOT a subset of digit 2
  
  segset int_1_6;
  std::set_intersection(out[digits::one].begin(), out[digits::one].end(),
			out[digits::six].begin(), out[digits::six].end(),
			std::inserter(int_1_6, int_1_6.end()));
  
  digit_process(find_subset(unknown_digits, int_1_6, true), digits::two, 5);


  digit_process(find_subset(unknown_digits, out[digits::one], true), digits::five, 5);

  digit_process(find_digit_by_set_length(unknown_digits, 5), digits::three, 5);

  digit_process(find_subset(unknown_digits, out[digits::three], false), digits::nine, 6);

  out[digits::zero] = unknown_digits[0];
  
  
  return out;

}


vector<digits> decode_line( const vector<segset>& unique_digits, const vector<segset>& unknown_digits)
{
  auto segmap = decode_segments(unique_digits);
  std::map<segset, digits> rev_lookup_map;
  for(auto& [k,v] : segmap)
    rev_lookup_map[v] = k;

  vector<digits> decoded;
  decoded.reserve(unknown_digits.size());
  for(auto& d : unknown_digits)
    decoded.push_back(rev_lookup_map[d]);
  
  return decoded;
}


int digit_vector_to_number(const vector<digits>& digs)
{

  cout << "number printed: ";
  for(auto& d : digs)
    cout << static_cast<char>(d);
  cout << endl;

  
  int out =0;
  for(int i=0; i < digs.size(); i++)
    {
      int pow_of_10 = digs.size() - i -1;
      int multiplier = int(std::round(std::pow(10, pow_of_10)));

      int basenum = static_cast<char>(digs[i]) - static_cast<char>('0');
      out += basenum * multiplier;
    }
  return out;
}


int main(int argc, char** argv)
{
  auto problem =   load_file("input.txt");

  int part_1_count = 0;
  int part_2_count = 0;

  std::set<digits> part_1_set{ digits::one, digits::four, digits::eight, digits::seven};
  
  for(const auto& [unique, unknown]  : problem)
    {
      auto decoded = decode_line(unique, unknown);

      int part_1_add = std::count_if(decoded.begin(), decoded.end(),		     
				     [&part_1_set] (auto& v)
				     {return part_1_set.count(v);});

      part_1_count += part_1_add;


      cout << "decoded number: "<< digit_vector_to_number(decoded) << endl;
      part_2_count += digit_vector_to_number(decoded);
      
    }


  cout << "part 1 count: " << part_1_count << endl;
  cout << "part 2 count: " << part_2_count << endl;
  
  // print_segs(unique_digits);

  // auto segmap = decode_segments(unique_digits);

  // cout << endl;
  // for(auto& [digit, segs] : segmap)
  //   {
  //     cout << "digit: " << static_cast<char>(digit) << "\t";
  //     print_segs(segs);
  //     cout << endl;
  //   }


  // std::map<segset, digits> rev_lookup_map;
  // for(auto& [k, v] : segmap)
  //   rev_lookup_map[v] = k;

  // std::vector<char> decoded;
  // for(auto& d : unknown_digits)
  //   {
  //     decoded.push_back(static_cast<char>(rev_lookup_map[d]));
      
  //   }

  // for(auto& d : decoded)
  //   cout << d << ",";
  // cout << endl;
  
  


  
}
