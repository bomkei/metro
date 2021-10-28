#pragma once

struct Driver {
  std::string source_code;

  std::vector<std::string> script_path;
  std::string app_path;

  u32 cur_script_index = 0;

  bool unsafe   = false;
  bool optimize = true;

  auto const& get_cur_script_path() const {
    return script_path[cur_script_index];
  }
};
