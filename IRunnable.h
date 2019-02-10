#ifndef IRUNNABLE_H
#define IRUNNABLE_H

#include <function>

namespace ayisakov
{
namespace framework
{
class IRunnable
{
  public:
    IRunnable() {}
    virtual ~IRunnable() {}
    /**
     * Optional. Override this function with any initialization
     * code that does not belong in the constructor or in run().
     * Suggestion: return 0 on successful initialization to work
     * properly with IAsyncRunner and derived classes.
     */
    virtual int initialize() { return 0; }
    /**
     * Run in the calling thread, and block until end of execution.
     * Override this function with the code you wish to run.
     *
     * @return exit code
     */
    virtual int run() = 0;
    /**
     * Optional. Override this function with any post-run
     * operations that should be performed after run() and do not
     * belong in the destructor.
     */
    virtual void postrun() {}
    /**
     * A facility for requesting termination of asynchronous
     * tasks. After calling this function, it is the user's
     * responsibility to confirm that the task was indeed
     * terminated. Override this function with your
     * termination request code.
     */
    virtual void terminate() {}
};
} // namespace framework
} // namespace ayisakov

#endif // IRUNNABLE_H
