#include <gtest/gtest.h>
#include <memory>
#include <string>
#include "../SimpleEventApp.h"
#include "TestApp.h"
#include "TestMessages.h"

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

TEST_F(SimpleEventAppTest, SimpleEventAppConstruction)
{
    ayisakov::framework::SimpleEventApp<TestMessage> *app = nullptr;
    app = new ayisakov::framework::SimpleEventApp<TestMessage>();
    ASSERT_NE(app, nullptr);
    delete app;
    app = nullptr;
}

TEST_F(SimpleEventAppTest, TestAppConstruction)
{
    ayisakov::framework::TestApp *app = nullptr;
    app = new ayisakov::framework::TestApp();
    ASSERT_NE(app, nullptr);
    delete app;
    app = nullptr;
}

TEST_F(SimpleEventAppTest, TestAppMessageExecutionDeferred)
{
    // Create the object that will supply the callback and bind the callback
    ayisakov::framework::TestFunctor functor;
    ASSERT_EQ(functor.value(), 0);
    std::function<void(void)> callback =
        std::bind(&ayisakov::framework::TestFunctor::increment, &functor);
    // Create the TestApp
    ayisakov::framework::TestApp *app = nullptr;
    app = new ayisakov::framework::TestApp(callback);
    ASSERT_NE(app, nullptr);
    // Send it a message that should trigger the callback and one
    // that should cause it to exit.
    std::unique_ptr<ayisakov::framework::MsgSample> msg1 = nullptr;
    msg1 = std::unique_ptr<ayisakov::framework::MsgSample>(
        new ayisakov::framework::MsgSample());
    ASSERT_NE(msg1, nullptr);
    std::unique_ptr<ayisakov::framework::MsgQuit> msg2 = nullptr;
    msg2 = std::unique_ptr<ayisakov::framework::MsgQuit>(
        new ayisakov::framework::MsgQuit());
    ASSERT_NE(msg2, nullptr);
    app->send(std::move(msg1));
    app->send(std::move(msg2));
    // Run the application, which should now process the two messages.
    int ret = app->run();
    ASSERT_EQ(ret, 0);
    ASSERT_EQ(functor.value(), 1);
    delete app;
    app = nullptr;
}
} // namespace

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
