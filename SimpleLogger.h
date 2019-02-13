#ifndef LOGGER_H
#define LOGGER_H

/*
 * Author: Artem Isakov
 * The SimpleLogger is responsible for all message reporting operations.
 */

#include <chrono>
#include <fstream>
#include "../framework/ILogger.h"

namespace ayisakov
{
namespace framework
{
class IMessage;
class SimpleLogger : public ILogger
{
  public:
    class Options
    {
      public:
        Options();
        Options(bool logToStdout,
                bool logToFile,
                const std::string &filePath,
                bool rotate,
                time_t rotationFreqSec);
        virtual ~Options();
        bool logToFile() const;
        bool logToStdout() const;
        const std::string &filePath() const;

      private:
        bool m_writeToFile;
        bool m_writeToStdout;
        std::string m_filePath;
        time_t m_rotationFreqSec;
    };
    SimpleLogger();
    SimpleLogger(const Options &opts);
    virtual ~SimpleLogger();
    /**
     * Configure this logger
     *
     * @return 0 on success
     */
    int configure(const Options &opts);
    // Is the logger ready to process messages?
    virtual bool isReady() override;
    /**
     * Process a message; returns 0 on success
     * NOT thread-safe
     */
    virtual int log(IMessage &msg) override;
    /**
     * Log this string
     *
     * @return 0 on success, nonzero value on failure
     */
    virtual int log(const std::string &str) override;
    /**
     * Syntactic sugar
     */
    //    virtual void operator<<(const std::string &message);
    //    virtual void operator<<(const char *message);
    //    virtual void operator<<(int integer);
    //    virtual void operator<<(double floating);
    //    virtual void operator<<(const char character);
  protected:
    /**
     * rotate the log file
     */
    void rotate();

    void closeFileIfOpen();

  private:
    Options m_options;
    std::ofstream m_fileStream;
};
} // namespace framework
} // namespace ayisakov

#endif // LOGGER_H
