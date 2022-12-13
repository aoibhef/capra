#include "capra/core/msg_bus.hpp"

#include "capra/util/log.hpp"
#include "capra/util/rnd.hpp"

namespace capra {

std::unordered_map<std::string, MsgFunc> MsgBus::funcs_{};
std::unordered_map<MsgType, std::unordered_set<std::string>> MsgBus::subscriptions_{};
std::unordered_map<std::string, std::queue<Msg>> MsgBus::queues_{};

std::string MsgBus::register_endpoint(const MsgFunc &&f) {
  std::string id;
  do {
    id = rnd::base58(11);
  } while (funcs_.contains(id));

  funcs_[id] = f;
  queues_[id] = std::queue<Msg>();

  return id;
}

void MsgBus::subscribe(const std::string &id, const MsgType &type) {
  subscriptions_[type].insert(id);
}

void MsgBus::poll(const std::string &id) {
  while (!queues_[id].empty()) {
    funcs_[id](queues_[id].front());
    queues_[id].pop();
  }
}

void MsgBus::poll_all() {
  for (auto &q: queues_)
    poll(q.first);
}

} // namespace capra
