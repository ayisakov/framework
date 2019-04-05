#include <gtest/gtest.h>
#include <stdio.h>
#include <chrono>
#include <fstream>
#include <list>
#include <memory>
#include <string>
#include <thread>
#include "../IOListener.h"
#include "../IOProvider.h"
#include "../ISerialPort.h"
#include "../SimpleEventApp.h"
#include "../SimpleLogger.h"
#include "../ThreadRunner.h"
#include "TestApp.h"
#include "TestMessages.h"
#include "../TestArgument.h"


typedef std::unique_ptr<std::string> TestMessage;
namespace ayif = ayisakov::framework;

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
                                                       true, // log to file
                                                       logFilePath,
                                                       false, // rotate log [future]
                                                       0 // rotation period
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
    ayisakov::framework::IOProvider provider(nullptr);
}

TEST_F(SimpleEventAppTest, IOProviderSerialPortCreation)
{
    ayisakov::framework::IOProvider provider(nullptr);
    ayisakov::framework::ISerialPort *pPort1 = nullptr;
    pPort1 = provider.getSerialPort();
    ASSERT_FALSE(pPort1 == nullptr);

    ayisakov::framework::ISerialPort *pPort2 = nullptr;
    pPort2 = provider.getSerialPort();
    ASSERT_FALSE(pPort2 == nullptr);

    std::string port1Id(pPort1->id());
    std::string port2Id(pPort2->id());
    ASSERT_FALSE(port1Id == port2Id);

    pPort1->release();
    ayisakov::framework::ISerialPort *pAlsoPort1 = nullptr;
    pAlsoPort1 = provider.getSerialPort();
    ASSERT_FALSE(pAlsoPort1 == nullptr);
    ASSERT_TRUE(port1Id == pAlsoPort1->id());

    ayisakov::framework::ISerialPort *pPort3 = nullptr;
    pPort3 = provider.getSerialPort();
    ASSERT_FALSE(pPort3 == nullptr);
    std::string port3Id = pPort3->id();
    ASSERT_FALSE(port3Id == port1Id);
    ASSERT_FALSE(port3Id == port2Id);
}

TEST_F(SimpleEventAppTest, IOListenerInstantiation)
{
    std::unique_ptr<ayif::IIOListener> pListener(new ayif::IOListener());
    ASSERT_TRUE(pListener);
    ASSERT_FALSE(pListener->isRunning());

    // Create an I/O provider and make the listener a subscriber
    std::unique_ptr<ayif::IIOProvider> pProvider(new ayif::IOProvider(nullptr));
    ASSERT_TRUE(pProvider.get());
    ASSERT_EQ(pListener->subscribe(pProvider.get()), 0);


    // Start the listener thread
    ayif::ThreadRunner listenerRunner(*pListener.get());
    ASSERT_FALSE(listenerRunner.launch());
    ASSERT_TRUE(pListener);
    // Wait for the listener to start
    int waitCount = 5;
    auto waitStartStop = [&](int count, bool start) {
        ASSERT_TRUE(pListener);
        while(start ? !pListener->isRunning() : pListener->isRunning()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            if(--count <= 0) {
                break;
            }
        }
    };
    waitStartStop(waitCount, true);
    ASSERT_TRUE(pListener->isRunning());

    // Listener should terminate when its IOProvider is destroyed
#ifdef FRAMEWORK_TASK_DEBUG
    std::cout << "Resetting pProvider" << std::endl;
#endif // FRAMEWORK_TASK_DEBUG
    pProvider.reset();
    waitStartStop(waitCount, false);
    ASSERT_FALSE(pListener->isRunning());
}
TEST_F(SimpleEventAppTest, IOListenerIOProviderSafeDestruction)
{
    // reverse construction and destruction order with subscribed listener
    std::unique_ptr<ayif::IIOProvider> pProvider(new ayif::IOProvider(nullptr));
    std::unique_ptr<ayif::IIOListener> pListener1(
        new ayif::IOListener(pProvider.get()));
    // Listener destruction should be safe and allow another listener to subscribe.
    std::unique_ptr<ayif::IIOListener> pListener2(new ayif::IOListener());
    pListener1.reset();
    ASSERT_EQ(pListener2->subscribe(pProvider.get()), 0);
}
//TEST_F(SimpleEventAppTest, JSONParsing)
//{
//    std::string name("RheoRaman configuration file");
//    std::string author("Artem Isakov");
//    std::string version("20181130");
//    int test_int(5);
//    double test_flt(3.14);
//    std::ostringstream oss;
//    oss << "{ \"file_info\":{\"name\":\"" << name
//        << "\",\"author\":\"" << author << "\",\"version\":\""
//        << version << "\",\"integer\":" << test_int
//        << ",\"float\":" << test_flt << "} }";
//    std::string jsonStr = oss.str();
//    ayif::JSON json(jsonStr);
//    std::string nameOut =
//        json.valueAsString({ "file_info", "name" });
//    std::string authorOut =
//        json.valueAsString({ "file_info", "author" });
//    std::string versionOut = json.valueAsString({ "file_info", "version" });
//    int intOut = json.valueAsInt({"file_info", "integer"});
//    double floatOut = json.valueAsDouble({"file_info", "float"});
//    ASSERT_STREQ(name.c_str(), nameOut.c_str());
//    ASSERT_STREQ(author.c_str(), authorOut.c_str());
//    ASSERT_STREQ(version.c_str(), versionOut.c_str());
//    ASSERT_EQ(test_int, intOut);
//    ASSERT_NEAR(test_flt, floatOut, 0.0001);
//}

TEST_F(SimpleEventAppTest, CmdLineArguments)
{
    using ArgBase = ayif::IArgument<ayif::TestArgTarget>;
    const std::string arg_short("t");
    const std::string arg_long("-test");
    const std::string arg_unrecognized("tree");
    const int input_short_int = 500;
    const int input_long_int = 122;
    const std::string input_short_string = "Marco";
    const std::string input_long_string = "Polo";

    std::unique_ptr<ArgBase> pArgS(ArgBase::getArgument(arg_short));
    ASSERT_TRUE(pArgS);
    std::unique_ptr<ArgBase> pArgL(ArgBase::getArgument(arg_long));
    ASSERT_TRUE(pArgL);
    std::unique_ptr<ArgBase> pArgU(ArgBase::getArgument(arg_unrecognized));
    ASSERT_FALSE(pArgU);

    ayif::TestArgTarget targetS;
    ayif::TestArgTarget targetL;

    ASSERT_EQ(pArgS->addValue(std::to_string(input_short_int)), 1);
    ASSERT_EQ(pArgS->addValue(input_short_string), 0);
    pArgS->apply(&targetS);

    ASSERT_EQ(pArgL->addValue(std::to_string(input_long_int)), 1);
    ASSERT_EQ(pArgL->addValue(input_long_string), 0);
    pArgL->apply(&targetL);

    ASSERT_EQ(targetS.getIntVal(), input_short_int);
    ASSERT_STREQ(targetS.getStringVal().c_str(), input_short_string.c_str());
    ASSERT_EQ(targetL.getIntVal(), input_long_int);
    ASSERT_STREQ(targetL.getStringVal().c_str(), input_long_string.c_str());
}
} // namespace

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
