#include <iostream>

#include "IOListener.h"
#include "IOProvider.h"

ayisakov::framework::IOListener::IOListener()
: m_pProvider(nullptr), m_isRunning(false)
{
}

ayisakov::framework::IOListener::IOListener(ayisakov::framework::IIOProvider *pProvider)
: m_pProvider(pProvider), m_isRunning(false)
{
    // TODO: throw exception if setting this listener fails
    if(m_pProvider) {
        m_pProvider->setListener(this);
    }
}

ayisakov::framework::IOListener::~IOListener()
{
    unsubscribe(m_pProvider);
}

int ayisakov::framework::IOListener::subscribe(ayisakov::framework::IIOProvider *pProvider)
{
    // No changing of provider allowed while dispatching events
    if(m_isRunning) return -1;

    // Check for null pointer
    if(!pProvider) return -2;

    // Check to see if we can listen to the requested provider
    int setRet = pProvider->setListener(this);
    if(setRet) {
        return setRet;
    }

    // All is well
    m_pProvider = pProvider;
    return 0;
}

void ayisakov::framework::IOListener::unsubscribe(IIOProvider *pProvider)
{
    if(m_pProvider && m_pProvider == pProvider) {
        m_pProvider->removeListener(this);
        m_pProvider = nullptr;
    }
}

int ayisakov::framework::IOListener::run()
{
    std::cout << "Entered IOListener::run() with m_pProvider = " << m_pProvider
              << std::endl;
    if(!m_pProvider) return -1;

    // indicate that we are now running
    m_isRunning = true;

    std::cout << "Calling m_pProvider->dispatchEvents(this)" << std::endl;
    int retval = m_pProvider->dispatchEvents(this);

    // indicate that we are no longer running
    std::cout << retval << " from m_pProvider->dispatchEvents(this)"
              << std::endl;
    m_isRunning = false;
    return retval;
}

bool ayisakov::framework::IOListener::isRunning()
{
    return m_isRunning;
}

int ayisakov::framework::IOListener::send(IMessagePtr &message)
{
    // TODO: implement
}
