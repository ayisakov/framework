#ifndef JSON_H
#define JSON_H

#include <stdexcept>
#include <list>
#include "../json/single_include/nlohmann/json.hpp"
#include "IJSONConvertible.h"

namespace ayisakov
{
namespace framework
{
class not_found : public std::runtime_error
{
  public:
    not_found(const std::string &whatArg)
    : std::runtime_error(whatArg)
    {
    }
    virtual ~not_found() {}
};

class JSON : public IJSONConvertible
{
  public:
    JSON();

    /**
     * Construct from a JSON string
     */
    JSON(const std::string &jsonStr);

    virtual ~JSON();

    virtual void fromJSON(const std::string &str) override;

    virtual std::string toJSON() const override;

    /**
     * @throw not_found exception if not found
     */
    virtual std::string valueAsString(const std::list<std::string> &path);

  private:
    nlohmann::json m_json;
};
} // namespace framework
} // namespace ayisakov

#endif // JSON_H
