#ifndef TESTARGUMENT_H
#define TESTARGUMENT_H

#include <boost/optional.hpp>
#include "IArgument.h"

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
};

using TestArgBase = IArgument<TestArgTarget>;

class TestArgument : public TestArgBase
{
  public:
    TestArgument();
    virtual ~TestArgument();
    static TestArgBase *create();
    virtual bool isStandalone() const override;
    virtual int addValue(const std::string &value) override;
    virtual void apply(TestArgTarget *pTarget) override;

  private:
    boost::optional<int> m_valueInt;
    boost::optional<std::string> m_valueString;
    int m_valuesProvided;
};
} // namespace framework
} // namespace ayisakov

#endif // TESTARGUMENT_H
