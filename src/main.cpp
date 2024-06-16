#include <iostream>
#include <string>
#include <sstream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <limits.h>

enum availTypes {
  Executable,
  Builtin,
  Nonexistent,
};


struct inputType { 
  availTypes type;
  std::string pathToDir;
};


std::string find_command_in_path(std::string command, std::string path);

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

std::string find_command_executable_path(std::string command) {
  
  char *path = getenv("PATH");

  if (path == NULL) {
    return "";
  }

  std::string path_acc = "";

  char *p = path;

  while (*p != '\0') {

    if (*p == ':') {

      std::string exec_path = find_command_in_path(command, path_acc);

      if (exec_path != "") {

        return exec_path;
      }
      path_acc = "";
    }

    else {
      path_acc += *p;
    }
    p++;
  }

  std::string exec_path = find_command_in_path(command, path_acc);

  if (exec_path != "") {
    return exec_path;
  }

  return "";
}

std::string find_command_in_path(std::string command, std::string path) {

  for (const auto &entry : std::filesystem::directory_iterator(path)) {

    if (entry.path() == (path + "/" + command)) {

      return entry.path();
    }
  }
  return "";
}

inputType findInputType(std::string command) {

  std::vector<std::string> builtin_commands = {"exit", "echo", "type"};

  if(std::find(builtin_commands.begin(), builtin_commands.end(), command) != builtin_commands.end()) { 
    
    inputType inp;
    inp.type = availTypes::Builtin;

    return inp;
  }


  std::string exec_path = find_command_executable_path(command);
  if (exec_path != "") {

    inputType inp;

    inp.type = Executable;

    inp.pathToDir = exec_path;

    return inp;
  }

  inputType inp;
  inp.type = availTypes::Nonexistent;

  return inp;
}

int cdCheck(std::string cdPath) {
  if(std::filesystem::exists(cdPath)) {
    std::filesystem::current_path(cdPath); 
  } else {
    std::cout << "cd: " << cdPath << ": No such file or directory";
    return 1;  
  }
  return 0;
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while(true) {

    std::cout << "$ ";
    std::string input;
    std::getline(std::cin, input);

    inputType inp = findInputType(input.substr(0, input.find(' ')));

    if(input == "exit 0"){
      exit(0);
    }
    else if(input.compare(0, 2, "cd") == 0) {
      cdCheck(input.substr(3));
    }
    else if(input.compare(0, 3, "pwd") == 0) {
      char buffer[PATH_MAX];
      std::cout << getcwd(buffer, sizeof(buffer)) << std::endl;
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

      if (inp.type == Executable) {

      std::string command_with_full_path = inp.pathToDir + input.substr(input.find(' '));

      const char *command_ptr = command_with_full_path.c_str();

      system(command_ptr);

      continue;

    }

      std::cout << input << ": command not found" << std::endl;
    }
  }
}
