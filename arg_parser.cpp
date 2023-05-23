#include "arg_parser.h"

std::string join(const std::vector<std::string>& list, const std::string s) {
  std::stringstream ss;
  for(auto i = 0; i < list.size(); i++) {
    if (i > 0) 
      ss << s;
    ss << list[i];
  }
  return ss.str();
}

ArgParser& ArgParser::add(
  const std::string& name, 
  const std::vector<std::string>& prefixs, 
  const bool& subfix, 
  const bool& required,
  const std::string& help
) {
  m_name_infos[name] = {
    prefixs,
    subfix,
    required,
    false,
    help,
    ""
  };
  for(auto p : prefixs) {
    m_prefix_name[p] = name;
  }
  m_prefixs.insert(m_prefixs.end(), prefixs.begin(), prefixs.end());
  return *this;
}

void ArgParser::parse(int argc, char* argv[]) {
  m_tokens.clear();

  for(int i = 1; i < argc; i++) {
    m_tokens.push_back(
      std::make_pair(
        std::string(argv[i]), 
        std::find(m_prefixs.begin(), m_prefixs.end(), argv[i]) != m_prefixs.end()
      )
    );
  }

  for(auto i = 0 ; i < m_tokens.size(); ) {
    auto& token = m_tokens[i];

    // is not prefix
    if (!token.second) {
      throw std::runtime_error("Unknown argument: " + token.first);
    }

    if (m_prefix_name.find(token.first) == m_prefix_name.end()) {
      throw std::runtime_error("Unknown argument: " + token.first);
    }

    auto& info = m_name_infos[m_prefix_name[token.first]];

    if (info.exists) {
      throw std::runtime_error("Duplicated argument:"  + token.first);
    }

    info.exists = true;

    if (info.subfix) {
      if (i == m_tokens.size() - 1) {
        throw std::runtime_error("Argument " + join(info.prefixs, ",") + " should have subfix.");
      }

      auto& next = m_tokens[i + 1];

      if (next.second) {
        throw std::runtime_error("Argument " + join(info.prefixs, ",") + " should have subfix.");
      }

      info.value = next.first;
      i += 2;
    } else {
      i += 1;
    }
  }

  for(auto& i : m_name_infos) {
    if (i.second.required && !i.second.exists) {
        throw std::runtime_error("Argument " + join(i.second.prefixs, ",") + " is required.");
    }
  }
}


bool ArgParser::exists(const std::string& name) {
  if (m_name_infos.find(name) == m_name_infos.end()) {
    return false;
  }

  auto& info = m_name_infos[name];
  
  return info.exists;
}

std::string ArgParser::help() {

  std::stringstream ss;

  ss << "Argument list:\n";

  for(auto &i : m_name_infos) {
    ss << join(i.second.prefixs, ",");
    if (i.second.subfix) {
      ss << " <subfix>";
    }
    ss << " : " << i.second.help << '\n';
  }

  return ss.str();
} 