#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <tuple>

void instruction_part1(std::array<int,2>& pos, std::string instruction,
		       int argint)
{
   if(instruction == "forward")
    pos[0] += argint;
  else if (instruction == "down")
    pos[1] += argint;
  else if (instruction == "up")
    pos[1] -= argint;
  else
      throw std::runtime_error("invalid instruction -" + instruction + "-");
}

void instruction_part2(std::array<int,3>& pos, std::string instruction,
		       int argint)
{
  if(instruction == "forward")
    {
      pos[0] += argint;
      pos[1] += pos[2] * argint;
    }
  else if (instruction == "down")
    pos[2] += argint;
  else if (instruction == "up")
    pos[2] -= argint;
  else
      throw std::runtime_error("invalid instruction -" + instruction + "-");
  
}


template<typename Callable, std::size_t poslen>
void enact_instruction(const std::string& input, std::array<int,poslen>& pos,
		       Callable&& fn)
{
  std::istringstream ifs(input);

  //  std::cout << ifs.str() << std::endl;
  
  std::string instruction;
  std::string arg;
  ifs >> instruction;
  ifs >> arg;

  // std::cout << "instruction: " << instruction << std::endl;
  // std::cout << "arg: " << arg <<  std::endl;
  
  int argint = std::atoi(arg.c_str());
  fn(pos, instruction, argint);
}



int main(int argc, char** argv)
{
  std::ifstream ifs("simple_input.txt");
  std::array<int,2> pos {0,0};

  for(std::string line; std::getline(ifs,line);)
    {
      std::cout << line << ",";
      enact_instruction(line, pos, instruction_part1);
      std::cout << "horizontal: " << pos[0] << ", depth: " << pos[1] << std::endl;
      
    }
  std::cout << std::endl;

  std::cout << "result: " << pos[0] * pos[1] << std::endl;
  

  ifs.close();
  ifs.open("input.txt");
  pos = {0,0};
  for(std::string line; std::getline(ifs, line);)
    enact_instruction(line, pos, instruction_part1);

  std::cout << "real result: " << pos[0] * pos[1] << std::endl;

  std::array<int,3> pos2 = {0,0,0};
  auto part2_logic = [&pos2, &ifs] ( std::string&& fname)
    {
      ifs.close();
      ifs.open(fname);
      pos2 = {0,0,0};
      for(std::string line; std::getline(ifs, line);)
	enact_instruction(line, pos2, instruction_part2);
    };

  part2_logic("simple_input.txt");
  std::cout << "test result part 2: " << pos2[0] * pos2[1] << std::endl;


  part2_logic("input.txt");
  std::cout << "real result part 2: " << pos2[0] * pos2[1] << std::endl;;


  
}
