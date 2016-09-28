#pragma once

template<typename T,
  typename = decltype(std::declval<T>().pprintTo(std::declval<std::ostream&>()))>
std::ostream& operator<<(std::ostream& oss, const T& ref) {
  ref.pprintTo(oss);
  return oss;
}
