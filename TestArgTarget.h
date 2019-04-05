#ifndef TESTARGTARGET_H
#define TESTARGTARGET_H

#include <string>
#include <memory>
#include "TestArgBase.h"

namespace ayisakov
{
namespace framework
{
class TestArgTarget
{
  public:
    TestArgTarget() : m_valueInt(-1) {}
    ~TestArgTarget() {}
    void setValues(int valueInt, const std::string &valueString)
    {
        m_valueInt = valueInt;
        m_valueString = valueString;
    }
    int getIntVal() const { return m_valueInt; }
    const std::string &getStringVal() const
    {
        return m_valueString;
    }

  private:
    int m_valueInt;
    std::string m_valueString;
    std::unique_ptr<TestArgBase> m_pTest;
};
} // namespace framework
} // namespace ayisakov

#endif // TESTARGTARGET_H
