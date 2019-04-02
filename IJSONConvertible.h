#ifndef IJSONCONVERTIBLE_H
#define IJSONCONVERTIBLE_H

#include <string>

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
    virtual void fromJSON(const std::string& json) = 0;

    /**
     * Export state as JSON string
     */
    virtual std::string toJSON() const = 0;
};
} // namespace framework
} // namespace ayisakov

#endif // IJSONCONVERTIBLE_H
