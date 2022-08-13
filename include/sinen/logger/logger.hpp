#ifndef SINEN_LOGGER
#define SINEN_LOGGER
#include <iostream>
#include <memory>
#include <string_view>

namespace sinen {
namespace detail {
template <typename... Args>
std::string string_format_internal(const std::string &format, Args &&...args) {
  int str_len =
      std::snprintf(nullptr, 0, format.c_str(), std::forward<Args>(args)...);

  size_t buffer_size = str_len + sizeof(char);
  std::unique_ptr<char[]> buffer(new char[buffer_size]);
  std::snprintf(buffer.get(), buffer_size, format.c_str(), args...);
  return std::string(buffer.get(), buffer.get() + str_len);
}
template <typename T> auto convert(T &&value) {
  if constexpr (std::is_same<std::remove_cv_t<std::remove_reference_t<T>>,
                             std::string>::value) {
    return std::forward<T>(value).c_str();
  } else {
    return std::forward<T>(value);
  }
}
template <typename... Args>
std::string string_format_logger(const std::string &format, Args &&...args) {
  return string_format_internal(format, convert(std::forward<Args>(args))...);
}
} // namespace detail

class logger {
private:
  class interface {
  public:
    virtual void debug(std::string_view) {}
    virtual void info(std::string_view) {}
    virtual void error(std::string_view) {}
    virtual void warn(std::string_view) {}
    virtual void fatal(std::string_view) {}
  };

public:
  static void change_logger(std::unique_ptr<interface> logger);
  template <typename... Args>
  static void debug(std::string_view format, Args &&...args) {
    mLogger->debug(detail::string_format_logger(format.data(),
                                                std::forward<Args>(args)...));
  }
  template <typename... Args>
  static void info(std::string_view format, Args &&...args) {
    mLogger->info(detail::string_format_logger(std::string(format),
                                               std::forward<Args>(args)...));
  }
  template <typename... Args>
  static void error(std::string_view format, Args &&...args) {
    mLogger->error(detail::string_format_logger(std::string(format),
                                                std::forward<Args>(args)...));
  }
  template <typename... Args>
  static void warn(std::string_view format, Args &&...args) {
    mLogger->warn(detail::string_format_logger(format.data(),
                                               std::forward<Args>(args)...));
  }
  template <typename... Args>
  static void fatal(std::string_view format, Args &&...args) {
    mLogger->fatal(detail::string_format_logger(format.data(),
                                                std::forward<Args>(args)...));
  }

private:
  static std::unique_ptr<interface> mLogger;

public:
  class default_logger {
  public:
    static std::unique_ptr<interface> CreateConsoleLogger() {
      return std::move(std::make_unique<console_logger>());
    }

  private:
    class console_logger : public interface {
    public:
      virtual void debug(std::string_view) final override;
      virtual void info(std::string_view) final override;
      virtual void error(std::string_view) final override;
      virtual void warn(std::string_view) final override;
      virtual void fatal(std::string_view) final override;
    };
    class file_logger : public interface {
    public:
      virtual void debug(std::string_view) final override;
      virtual void info(std::string_view) final override;
      virtual void error(std::string_view) final override;
      virtual void warn(std::string_view) final override;
      virtual void fatal(std::string_view) final override;
    };
  };
};
} // namespace sinen

#endif // SINEN_LOGGER
