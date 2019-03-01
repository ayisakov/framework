#ifndef AYI_SCOPESETTER
#define AYI_SCOPESETTER

namespace ayisakov
{
namespace framework
{
/**
 * A utility class that can be used to temporarily set a variable
 * to the desired value upon construction and restore the
 * previous value upon destruction
 */
template <typename T> class ScopeSetter
{
  public:
    ScopeSetter(T &variable, const T &tempvalue)
    : m_original(variable), m_var(variable)
    {
        m_var = tempvalue;
    }
    ~ScopeSetter() { m_var = m_original; }

  private:
    T &m_var;
    T m_original;
};
} // namespace framework
} // namespace ayisakov
#endif // AYI_SCOPESETTER
