#include "TestMessages.h"
#include "ITestApp.h"

void ayisakov::framework::MsgSample::examine(ITestApp &reader) const
{
    reader.onMsg(*this);
}

void ayisakov::framework::MsgQuit::examine(ITestApp &reader) const
{
    reader.onMsg(*this);
}
