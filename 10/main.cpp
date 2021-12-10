#include <stack>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <tuple>
#include <variant>
#include <numeric>
#include <sstream>

using std::cout;
using std::endl;

using std::string;
using std::vector;


enum class chunks : unsigned char
  {
    parens,
    brackets,
    braces,
    angles
  };

using Stack = std::stack<chunks>;

vector<string> load_file(const string& fname)
{
  std::ifstream ifs(fname);
  string line;
  vector<string> out;
  while(std::getline(ifs, line, '\n'))
    {
      out.push_back(line);
    }

  return out;
}


bool process_char(Stack& st, char c)
{
  auto charcheck = [&st, c] (chunks comp) -> bool {
    if(st.top() == comp)
      {
	st.pop();
	return true;
      }
    return false;
  };
  
  switch(c)
    {
    case('('): st.push(chunks::parens); break;
    case('{'): st.push(chunks::braces); break;
    case('['): st.push(chunks::brackets); break;
    case('<'): st.push(chunks::angles); break;
    case(')'): if(!charcheck(chunks::parens)) return false; break;
    case('}'): if(!charcheck(chunks::braces)) return false; break;
    case(']'): if(!charcheck(chunks::brackets)) return false; break;
    case('>'): if(!charcheck(chunks::angles)) return false; break;
    default:
      cout << " ERROR: ---" << c << "--- " << endl;
      throw std::runtime_error("invalid character found!");
    }
  return true;
}

std::variant<std::tuple<char, int, Stack>, Stack> find_invalid_char(const string& line)
{
  Stack st;
  auto invaliditer = std::find_if_not(line.begin(), line.end(),
				      [&st] (char c) { return process_char(st,c);});
  if(invaliditer != line.end())
    return std::make_tuple(*invaliditer, std::distance(line.begin(), invaliditer), st);
  else
    return st;
}

vector<char> get_completion(Stack& st)
{
  vector<char> out;
  out.reserve(st.size());
  while(!st.empty())
    {
      chunks c = st.top();
      st.pop();
      switch(c)
	{
	case(chunks::parens): out.push_back(')'); break;
	case(chunks::brackets): out.push_back(']'); break;
	case(chunks::braces): out.push_back('}'); break;
	case(chunks::angles): out.push_back('>'); break;
	}
      
    }
  return out;
}


int get_score(const vector<char>& invalidchars)
{
  int out=0;
  for(auto c : invalidchars)
      switch(c)
	{
	case(')') : out += 3; break;
	case(']') : out += 57; break;
	case('}') : out += 1197; break;
	case('>') : out += 25137; break;
	}
  return out;
}

void part1(const vector<string>& lines)
{
  vector<char> invalidchars;
  for(auto& line: lines)
    {
      cout << "checking line: " << line << endl;
      auto op = find_invalid_char(line);
      if(std::holds_alternative<std::tuple<char, int, Stack>>(op))
	{
	  auto& [invalidchar, pos, _] = std::get<std::tuple<char,int,Stack>>(op);
	  cout << " --- line is invalid! Char: " << invalidchar << " at position: " << pos << endl;
	  invalidchars.push_back(invalidchar);
	}
      
    }

  cout << "invalid chars: ";
  for(auto& c : invalidchars)
    cout << c;
  cout << endl;
  cout << "score: " << get_score(invalidchars) << endl;
}

unsigned long long score_part2(const vector<char>& chrs)
{
  auto res = std::accumulate(chrs.begin(), chrs.end(), 0ull,
			     [] (unsigned long long sm, char c)
			     {
			       int addition = 0;
			       switch(c)
				 {
				 case(')') : addition = 1; break;
				 case(']') : addition = 2; break;
				 case('}') : addition = 3; break;
				 case('>') : addition = 4; break;
				 default:
				   cout << " ERROR: ----" << c << "---" << endl;
				   throw std::runtime_error("invalid character");
				 }

			       return (sm * 5 ) + addition;
			      
			     });
  return res;

}


int main(int argc, char** argv)
{
  auto lines = load_file("input.txt");
  //part1(lines);

  vector<unsigned long long> scores;
  scores.reserve(lines.size());

  vector<string> completions;
  for(auto& line: lines)
    {
      auto result = find_invalid_char(line);
      if(std::holds_alternative<std::tuple<char,int,Stack>>(result))
	continue;
      cout << "line: " << line << " -- not invalid, completing..." << endl;

      auto completion = get_completion(std::get<Stack>(result));
      std::ostringstream oss;
      for(auto& c : completion)
	oss << c;

      completions.push_back(oss.str());
      
      
      for(auto& c: completion)
	cout << c;
      cout << endl;

      auto sc = score_part2(completion);
      cout << "score: " << sc << endl;
      scores.push_back(sc);
      
    }

  for(auto s : scores)
    cout << s << ",";
  cout << endl;

  auto sorted_scores = scores;
  std::sort(sorted_scores.begin(), sorted_scores.end());

  cout << "sorted scores:----" << endl;
  for(auto& s : sorted_scores)
    cout << s << ",";
  cout << endl;
  cout << "n scores: " << sorted_scores.size() << endl;
  
  
  auto m = scores.begin() + scores.size() / 2;
  std::nth_element(scores.begin(), m, scores.end());
  cout << "middle score is: " << scores[scores.size() / 2] << endl;


  // cout << "completions: " << endl;
  // for(auto& comp : completions)
  //   cout << comp << endl;
  
  
  
}
