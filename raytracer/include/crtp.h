#pragma once

namespace rt {
/**
 * Helper class for applying the Curiously Recurring Template Pattern (CRTP) for compile time polymorphism (CTP).
 * https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
 * @tparam T Class
 */
template<class T>
class CRTP {
 public:
  T& Actual() { return static_cast<T&>(*this); }
  const T& Actual() const { return static_cast<const T&>(*this); }
 private:
  CRTP() = default;
  friend T;
};
}  // namespace rt
