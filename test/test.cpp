#include <gtest/gtest.h>
#include <fstream>
#include <list>
#include <memory>
#include <string>
#include <stdio.h>
#include "../SimpleEventApp.h"
#include "../SimpleLogger.h"
#include "../ThreadRunner.h"
#include "TestApp.h"
#include "TestMessages.h"
#include "../IOProvider.h"
#include "../ISerialPort.h"

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
    std::unique_ptr<ayisakov::framework::IAppMessage> msg1(
        new ayisakov::framework::MsgSample());
    app.send(msg1);
    // make sure that the pointer was moved to the receiver
    ASSERT_FALSE(msg1);
    // make sure that the callback was called
    ASSERT_EQ(functor.waitUntilCalled(1), 0);
    // and that it caused the state of functor to change
    ASSERT_EQ(functor.value(), 1);
    // send another message that will cause the functor to change state when processed
    std::unique_ptr<ayisakov::framework::IAppMessage> msg2(
        new ayisakov::framework::MsgSample());
    app.send(msg2);
    // make sure that the callback was called
    ASSERT_EQ(functor.waitUntilCalled(1), 0);
    // and that it caused the state of functor to change
    ASSERT_EQ(functor.value(), 2);

    // send it a message that will cause the app to exit
    std::unique_ptr<ayisakov::framework::IAppMessage> msgquit(
        new ayisakov::framework::MsgQuit());
    app.send(msgquit);
    // wait for thread to end and make sure it terminated gracefully
    ASSERT_EQ(runner.waitExit(), 0);
}


TEST_F(SimpleEventAppTest, Logging)
{
    const std::string logFilePath = "testlog.log";
    ayisakov::framework::SimpleLogger::Options logOpts(false, // log to stdout
                                                       true,  // log to file
                                                       logFilePath,
                                                       false, // rotate log [future]
                                                       0      // rotation period
    );
    std::unique_ptr<ayisakov::framework::SimpleLogger> logger(
        new ayisakov::framework::SimpleLogger(logOpts));
    //    ayisakov::framework::SimpleLogger logger;
    //    logger.configure(logOpts);
    ASSERT_TRUE(logger);
    ASSERT_TRUE(logger->isReady());

    ayisakov::framework::MsgSample msg1;
    ayisakov::framework::MsgQuit msg2;
    ASSERT_EQ(logger->log(msg1), 0);
    ASSERT_EQ(logger->log(msg2), 0);

    // destroy the logger, which should close the log file
    logger.reset();

    // check the contents of the log file
    std::ifstream readback;
    readback.open(logFilePath);
    ASSERT_TRUE(readback.is_open());
    std::list<std::string> lines_expected(
        { "    *** Log file opened. ***", "    Sample message",
          "    Quit message", "    *** Log file closed. ***" });
    std::list<std::string> lines_read;
    std::string line;
    while(std::getline(readback, line)) {
        int begin = line.find(']');
        lines_read.push_back(line.substr(begin + 1));
    }
    readback.close();
    ASSERT_FALSE(readback.is_open());
    // delete the temporary log file
    ASSERT_FALSE(remove(logFilePath.c_str()));
    // make sure that the number of read back lines is correct
    ASSERT_EQ(lines_read.size(), lines_expected.size());
    // compare each line
    auto it_exp = lines_expected.begin();
    auto it_read = lines_read.begin();
    while(it_exp != lines_expected.end() && it_read != lines_read.end()) {
        ASSERT_STREQ(it_read->c_str(), it_exp->c_str());
        ++it_exp;
        ++it_read;
    }
}

TEST_F(SimpleEventAppTest, IOProviderInstantiation)
{
    ayisakov::framework::IOProvider provider;
}

TEST_F(SimpleEventAppTest, IOProviderSerialPortCreation)
{
    ayisakov::framework::IOProvider provider;
    ayisakov::framework::ISerialPort *pPort1 = nullptr;
    pPort1 = provider.getPort();
    ASSERT_FALSE(pPort1 == nullptr);
    std::cout << pPort1->id() << std::endl;
    ayisakov::framework::ISerialPort *pPort2 = nullptr;
    pPort2 = provider.getPort();
    ASSERT_FALSE(pPort2 == nullptr);
    std::cout << pPort2->id() << std::endl;
    ASSERT_FALSE(pPort1->id() == pPort2->id());
}
} // namespace

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
