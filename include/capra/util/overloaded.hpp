#pragma once

namespace capra {

template<class ...Ts>
struct overloaded : Ts ... {
  using Ts::operator()...;
};

template<class ...Ts> overloaded(Ts...) -> overloaded<Ts...>;

} // namespace capra
