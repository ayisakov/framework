#include <gtest/gtest.h>
#include <memory>
#include <string>
#include "../SimpleEventApp.h"

typedef std::unique_ptr<std::string> TestMessage;

namespace
{
class SimpleEventAppTest : public ::testing::Test
{
  protected:
    SimpleEventAppTest() {}
    ~SimpleEventAppTest() override {}
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(SimpleEventAppTest, Construction)
{
    ayisakov::framework::SimpleEventApp<TestMessage> *app =
        new ayisakov::framework::SimpleEventApp<TestMessage>();
    ASSERT_NE(app, nullptr);
}

} // namespace

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
