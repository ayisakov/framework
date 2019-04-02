#ifndef ICONFIG_H
#define ICONFIG_H

#include <stdexcept>

namespace ayisakov
{
namespace framework
{
class parsing_error : public std::runtime_error
{
  public:
    explicit parsing_error(const std::string &info)
    : runtime_error(info)
    {
    }
    explicit parsing_error(const char *info)
    : runtime_error(info)
    {
    }
    virtual ~parsing_error() {}
};

/**
 * The most general configuration interface. Supports
 * loading state from and storing it as std::string.
 */
class IConfig
{
  public:
    IConfig() {}
    virtual ~IConfig() {}

    /**
     * Populate object state from a formatted string.
     * Derived classes should override this function
     * with format-specific versions.
     *
     * @throw parsing_error if unable to parse
     */
    virtual void fromString(const std::string &str) = 0;

    /**
     * Export state as a formatted string. Derived
     * classes should override this function with
     * format-specific versions.
     *
     * @return empty string if nothing to export
     */
    virtual std::string toString() const = 0;
};
} // namespace framework
} // namespace ayisakov

#endif // ICONFIG_H
