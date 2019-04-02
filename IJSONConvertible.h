#ifndef IJSONCONVERTIBLE_H
#define IJSONCONVERTIBLE_H

#include "../json/single_include/nlohmann/json.hpp"

namespace ayisakov
{
namespace framework
{
class IJSONConvertible
{
  public:
    IJSONConvertible() {}

    virtual ~IJSONConvertible() {}

    /**
     * Load object state from JSON string
     *
     * @throw std::runtime_error if unable to parse
     */
    virtual void fromJSON(const nlohmann::json &json) = 0;

    /**
     * Export state as JSON string
     */
    virtual nlohmann::json toJSON() const = 0;
};
} // namespace framework
} // namespace ayisakov

#endif // IJSONCONVERTIBLE_H
