#pragma once

#include "capra/core/msg_def.hxx"
#include <functional>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace capra {

using MsgFunc = std::function<void(const Msg &)>;

class MsgBus {
public:
  static std::string register_endpoint(const MsgFunc &&f);

  static void subscribe(const std::string &id, const MsgType &type);

  template<MsgType T, typename... Args>
  static void send(const Args &...args);

  // This should be used sparingly, as it's effectively a
  // disconnected function call that will create some bad spaghetti
  template<MsgType T, typename... Args>
  static void send_nowait(const Args &...args);

  static void poll(const std::string &id);

  static void poll_all();

private:
  static std::unordered_map<std::string, MsgFunc> funcs_;
  static std::unordered_map<MsgType, std::unordered_set<std::string>> subscriptions_;
  static std::unordered_map<std::string, std::queue<Msg>> queues_;
};

template<MsgType T, typename... Args>
void MsgBus::send(const Args &...args) {
  for (const auto &id: subscriptions_[T])
    queues_[id].push(Msg{T, typename Map<T>::type{args...}});
}

template<MsgType T, typename... Args>
void MsgBus::send_nowait(const Args &...args) {
  for (const auto &id : subscriptions_[T])
    funcs_[id](Msg{T, typename Map<T>::type{args...}});
}

} // namespace capra
