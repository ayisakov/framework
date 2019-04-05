#ifndef TESTARGUMENT_H
#define TESTARGUMENT_H

#include <boost/optional.hpp>
//#include "IArgument.h"
#include "TestArgBase.h"

namespace ayisakov
{
namespace framework
{
class TestArgument : public TestArgBase
{
  public:
    TestArgument();
    virtual ~TestArgument();
    static TestArgBase *create();
    virtual bool isStandalone() const override;
    virtual int addValue(const std::string &value) override;
    virtual void apply(TestArgTarget *pTarget) override;
    //    virtual void apply(const std::string &value, TestArgTarget *pTarget) override;

  private:
    boost::optional<int> m_valueInt;
    boost::optional<std::string> m_valueString;
    int m_valuesProvided;
};
} // namespace framework
} // namespace ayisakov

#endif // TESTARGUMENT_H
