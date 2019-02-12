#include <gtest/gtest.h>
#include <memory>
#include <string>
#include "../SimpleEventApp.h"
#include "TestApp.h"
#include "TestMessages.h"
#include "../ThreadRunner.h"

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
    ayisakov::framework::SimpleEventApp<TestMessage> *app = nullptr;
    app = new ayisakov::framework::SimpleEventApp<TestMessage>();
    ASSERT_NE(app, nullptr);
    delete app;
    app = nullptr;
}

TEST_F(SimpleEventAppTest, AppConstruction)
{
    ayisakov::framework::TestApp *app = nullptr;
    app = new ayisakov::framework::TestApp();
    ASSERT_NE(app, nullptr);
    delete app;
    app = nullptr;
}

TEST_F(SimpleEventAppTest, AppMessageExecutionDeferred)
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
    std::unique_ptr<ayisakov::framework::IAppMessage> msg1 = nullptr;
    msg1 = std::unique_ptr<ayisakov::framework::IAppMessage>(
        new ayisakov::framework::MsgSample());
    ASSERT_NE(msg1, nullptr);
    std::unique_ptr<ayisakov::framework::IAppMessage> msg2 = nullptr;
    msg2 = std::unique_ptr<ayisakov::framework::IAppMessage>(
        new ayisakov::framework::MsgQuit());
    ASSERT_NE(msg2, nullptr);
    app->send(msg1);
    app->send(msg2);
    // Run the application, which should now process the two messages.
    int ret = app->run();
    ASSERT_EQ(ret, 0);
    ASSERT_EQ(functor.value(), 1);
    delete app;
    app = nullptr;
}

TEST_F(SimpleEventAppTest, ThreadLaunchAsyncMsgProcAndJoin)
{
    // create the functor whose state will change asynchronously
    ayisakov::framework::TestFunctor functor;
    // create the callback that will be given to the app
    std::function<void(void)> callback =
        std::bind(&ayisakov::framework::TestFunctor::increment, &functor);
    // create the app
    ayisakov::framework::TestApp app(callback);
    // create the runner that will run the app in its own thread
    ayisakov::framework::ThreadRunner runner(app);
    // test successful thread creation
    ASSERT_EQ(runner.launch(), 0);

    // send a message that will cause the functor to change state when processed
    std::unique_ptr<ayisakov::framework::IAppMessage> msg1(new ayisakov::framework::MsgSample());
    app.send(msg1);
    // make sure that the pointer was moved to the receiver
    ASSERT_FALSE(msg1);
    // make sure that the callback was called
    ASSERT_EQ(functor.waitUntilCalled(1), 0);
    // and that it caused the state of functor to change
    ASSERT_EQ(functor.value(), 1);
    // send another message that will cause the functor to change state when processed
    std::unique_ptr<ayisakov::framework::IAppMessage> msg2(new ayisakov::framework::MsgSample());
    app.send(msg2);
    // make sure that the callback was called
    ASSERT_EQ(functor.waitUntilCalled(1), 0);
    // and that it caused the state of functor to change
    ASSERT_EQ(functor.value(), 2);

    // send it a message that will cause the app to exit
    std::unique_ptr<ayisakov::framework::IAppMessage> msgquit(new ayisakov::framework::MsgQuit());
    app.send(msgquit);
    // wait for thread to end and make sure it terminated gracefully
    ASSERT_EQ(runner.waitExit(), 0);
}
} // namespace

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
