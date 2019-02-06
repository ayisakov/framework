#ifndef LOGGER_H
#define LOGGER_H

/*
 * Author: Artem Isakov
 * The Logger is responsible for all message reporting operations.
 */

namespace neu {
namespace lustiglab {
namespace framework {
class Logger {
  public:
    class Options {
      public:
        Options() Options(bool logToStdout, bool logToFile, const std::string &filePath = "");
        bool logToFile() { return m_writeToFile }
        bool logToStdout() { return m_writeToStdout }
        virtual ~Options();
      private:
        bool m_writeToFile;
        bool m_writeToStdout;
    };
    Logger();
    int configure(const Options &opts);
    // Is the logger ready to process messages?
    virtual bool isReady();
    // process a message; returns 0 on success
    virtual int report(const IMessage &msg);
    virtual ~Logger();
};
} // namespace framework
} // namespace lustiglab
} // namespace neu

#endif // LOGGER_H
