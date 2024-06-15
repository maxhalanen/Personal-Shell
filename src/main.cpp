#include <iostream>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while(true) {
    std::cout << "$ ";
    std::string input;
    std::getline(std::cin, input);
    if(input == "exit 0"){
      exit(0);
    }
    else if(input.compare(0, 4, "echo") == 0) {
      std::cout << input.substr(5) << std::endl;
    }
    else if(input.compare(0, 4, "type") == 0) {
      if(input.substr(5) == "echo" || input.substr(5) == "type" || input.substr(5) == "exit") {
        std::cout << input.substr(5) << " is a shell builtin" << std::endl;
      }
      else{
         std::cout << input.substr(5) << ": not found" << std::endl;
      }
    }
    else{
      std::cout << input << ": command not found" << std::endl;
    }
  }
}
