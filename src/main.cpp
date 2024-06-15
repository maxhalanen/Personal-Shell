#include <iostream>
#include <string>
#include <sstream>
#include <filesystem>




std::string findPath(std::string dir) {

  std::string pathVar = getenv("PATH");
  std::stringstream ss(pathVar);
  std::string path;

  while(!ss.eof()) {

    std::getline(ss, path, ':');

    std::string fullPath = path + "/" + dir;

    if(std::filesystem::exists(fullPath)) {
      return (dir + " is " + fullPath);
    }
  }
  
  return (dir +  ": not found");
}


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
        std::cout << findPath(input.substr(5)) << std::endl;
      }
    }
    else{
      std::cout << input << ": command not found" << std::endl;
    }
  }
}
