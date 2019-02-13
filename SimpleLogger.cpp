#include "SimpleLogger.h"
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "IMessage.h"

// SimpleLogger::Options
ayisakov::framework::SimpleLogger::Options::Options()
: m_writeToFile(false), m_writeToStdout(true), m_rotationFreqSec(0)
{
}

ayisakov::framework::SimpleLogger::Options::Options(bool logToStdout,
                                                    bool logToFile,
                                                    const std::string &filePath,
                                                    bool rotate,
                                                    time_t rotationFreqSec)
: m_writeToStdout(logToStdout), m_writeToFile(logToFile),
  m_filePath(filePath)
{
}
ayisakov::framework::SimpleLogger::Options::~Options() {}
bool ayisakov::framework::SimpleLogger::Options::logToFile() const
{
    return m_writeToFile;
}

bool ayisakov::framework::SimpleLogger::Options::logToStdout() const
{
    return m_writeToStdout;
}

const std::string &ayisakov::framework::SimpleLogger::Options::filePath() const
{
    return m_filePath;
}

using ayisakov::framework::SimpleLogger;

// SimpleLogger
ayisakov::framework::SimpleLogger::SimpleLogger() {}

ayisakov::framework::SimpleLogger::SimpleLogger(const SimpleLogger::Options &opts)
: m_fileStream(std::ofstream())
{
    if(configure(opts)) {
        throw std::invalid_argument(opts.filePath());
    }
}

ayisakov::framework::SimpleLogger::~SimpleLogger()
{
    closeFileIfOpen();
}

void ayisakov::framework::SimpleLogger::closeFileIfOpen()
{
    if(m_fileStream.is_open()) {
        log("*** Log file closed. ***");
        m_fileStream.close();
    }
}

int ayisakov::framework::SimpleLogger::configure(
    const ayisakov::framework::SimpleLogger::Options &opts)
{
    m_options = opts;
    // close current stream if one is open
    closeFileIfOpen();

    // open a (new) file
    if(m_options.logToFile()) {
        m_fileStream.open(m_options.filePath(),
                          std::ofstream::out | std::ofstream::trunc);
        if(!m_fileStream.is_open()) {
            return -1;
        }
        if(log("*** Log file opened. ***")) {
            return -2;
        }
    }
    return 0;
}

bool ayisakov::framework::SimpleLogger::isReady()
{
    return m_fileStream.is_open();
}

int ayisakov::framework::SimpleLogger::log(const std::string &str)
{
    // don't waste cycles if logging is turned off
    if(!m_options.logToStdout() && !m_options.logToFile()) {
        return 0;
    }

    // get the current time
    std::chrono::system_clock::time_point now_tp =
        std::chrono::system_clock::now();
    std::time_t tnow = std::chrono::system_clock::to_time_t(now_tp);
    std::ostringstream oss;
    // format the log string
    oss << std::put_time(std::localtime(&tnow), "[%Y-%m-%d %X]")
        << "    " << str;
    std::string toLog(oss.str());

    // log to file
    if(m_options.logToFile()) {
        if(!m_fileStream.is_open()) return -1; // error

        m_fileStream << toLog << std::endl;
    }
    // log to stdout
    if(m_options.logToStdout()) {
        std::cout << toLog << std::endl;
    }
    return 0;
}

int ayisakov::framework::SimpleLogger::log(IMessage &msg)
{
    return log(msg.toString());
}
