#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <tuple>

namespace hkn {
namespace {

template <class Return, class ...Arg>
class Lazy {
public:
  Lazy(const std::function<Return(Arg...)> &func,
       const Arg &...args)
    : m_func(func),
      m_args(std::make_tuple(args...)) {
  }

  operator Return() const {
    if (m_return_value) {
      return *m_return_value;
    }
    else {
      auto temp = callFunction(std::index_sequence_for<Arg...>{});
      m_return_value = std::make_unique<Return>(temp);
      return *m_return_value;
    }
  }

private:
  template <std::size_t ...I>
  Return callFunction(std::index_sequence<I...>) const {
    return m_func(std::get<I>(m_args)...);
  }

  const std::function<Return(Arg...)> m_func;
  const std::tuple<Arg...> m_args;
  mutable std::unique_ptr<Return> m_return_value;
};

}

template <class Return, class ...Arg>
auto make_lazy(const std::function<Return(Arg...)> &func,
               const Arg &...args) {
  return Lazy<Return, Arg...>(func, args...);
}

template <class Return, class ...Arg>
auto make_lazy(Return(*func)(Arg...), const Arg &...args) {
  const std::function<Return(Arg...)> func_temp = func;
  return Lazy<Return, Arg...>(func_temp, args...);
}

}
