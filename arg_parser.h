#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>

class ArgParser {
public:

  ArgParser() = default;

  void parse(int argc, char* argv[]);

  ArgParser& add(
    const std::string& name, 
    const std::vector<std::string>& prefixs, 
    const bool& subfix = false, 
    const bool& required = false,
    const std::string& help = "No description of this command"
  ); 

  template<typename T>
  T get(const std::string& name, const T& normal) {
    if (m_name_infos.find(name) == m_name_infos.end()) {
      return normal;
    }

    auto& info = m_name_infos[name];

    if (!info.exists) {
      return normal;
    }

    std::stringstream ss;
    T val;

    ss << info.value;
    ss >> val;

    return val;
  }

  bool exists(const std::string& name);

  std::string help();
  
private:

  struct _info {
    std::vector<std::string> prefixs;
    bool subfix;
    bool required;
    bool exists;
    std::string help;
    std::string value;
  };

  std::vector<std::pair<std::string, bool>>     m_tokens;

  std::unordered_map<std::string, _info>        m_name_infos;

  std::unordered_map<std::string, std::string>  m_prefix_name;

  std::vector<std::string>  m_prefixs;

};