#include "TestApp.h"
#include "TestMessages.h"

ayisakov::framework::TestApp::TestApp() {}

ayisakov::framework::TestApp::TestApp(const std::function<void()> &onSampleMsg)
: m_onSampleMsg(onSampleMsg)
{
}

ayisakov::framework::TestApp::~TestApp() {}

void ayisakov::framework::TestApp::onMsg(const MsgSample &msg)
{
    if(m_onSampleMsg) {
        m_onSampleMsg();
    }
}

void ayisakov::framework::TestApp::onMsg(const MsgQuit &msg)
{
    terminate();
}
