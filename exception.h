#ifndef AYI_FRAMEWORK_EXCEPTION_H
#define AYI_FRAMEWORK_EXCEPTION_H

#include <exception>

namespace ayisakov
{
namespace framework
{
class exception : public std::exception
{
public:
    exception() noexcept {}
    
    virtual ~exception() noexcept {}

};
} // namespace framework
} // namespace ayisakov

#endif // AYI_FRAMEWORK_EXCEPTION_H
