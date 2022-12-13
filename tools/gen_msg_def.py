import pathlib
import re

STRUCT_DEF_PAT = r'struct\s+(\w+)\s+{'


def make_specialization(name):
    return f"""
template<> struct Map<MsgType::{name}> {{
  using type = {name};
}};
"""[1:]


def main():
    capra_dir = pathlib.Path(__file__).parent.parent.resolve()

    with open(f"{capra_dir}/include/capra/core/msg_data.hpp", 'r') as f:
        msg_data_contents = f.read()

    struct_names = re.findall(STRUCT_DEF_PAT, msg_data_contents)

    enum_fields = ',\n  '.join(struct_names)
    map_specializations = '\n'.join(make_specialization(name) for name in struct_names)[:-1]
    variant_fields = ',\n    '.join(struct_names)
    fmt_specialization = '\n      '.join(f"STRINGIFY({name})" for name in struct_names)

    with open(f"{capra_dir}/include/capra/core/msg_def.hxx", 'w') as f:
        f.write(f"""
// AUTOGENERATED FILE, DO NOT EDIT
// See: tools/gen_msg_def.py
//      include/capra/core/msg_data.hpp

#pragma once

#include "capra/core/msg_data.hpp"
#include "fmt/format.h"
#include <variant>

namespace capra {{

enum class MsgType {{
  {enum_fields}
}};

template<MsgType>
struct Map;

{map_specializations}

struct Msg {{
  MsgType type;
  std::variant<
    {variant_fields}
  > data;
}};

}} // namespace capra

template<> struct fmt::formatter<capra::MsgType>: formatter<string_view> {{
  template <typename FormatContext>
  auto format(capra::MsgType t, FormatContext& ctx) const {{
    #define STRINGIFY(e) case (e): name = #e; break;
    string_view name = "capra::MsgType::?";
    switch (t) {{
      using enum capra::MsgType;
      {fmt_specialization}
    }}
    return formatter<string_view>::format(name, ctx);
  }}
}};
"""[1:])


if __name__ == '__main__':
    main()