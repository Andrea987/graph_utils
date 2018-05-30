#ifndef IO_HPP
#define IO_HPP
#include "strtk.hpp"
#include <chrono>

void AddToBuffer(const std::string &s);
void AddToBuffer(char c);

template <typename T> void write(const T &param) {
  AddToBuffer(strtk::type_to_string(param));
}

template <typename First, typename... Args>
void write(const First &first, const Args &... args) {
  write(first);
  write(args...);
}
size_t nextInt(bool comments = false);

class Counter {
public:
  Counter(const std::string &msg, size_t interval = 8192)
      : base_msg_(msg), cnt_(0), interval_(interval),
        start_(std::chrono::high_resolution_clock::now()) {}
  void operator++(int);
  void operator++() { (*this)++; }
  ~Counter();
  Counter(const Counter &) = delete;
  void operator=(const Counter &) = delete;
  Counter(Counter &&) = delete;
  void operator=(const Counter &&) = delete;

private:
  std::string base_msg_;
  size_t cnt_;
  size_t interval_;
  std::chrono::high_resolution_clock::time_point start_;
};

#endif
