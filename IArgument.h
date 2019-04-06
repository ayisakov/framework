#ifndef AYI_IARGUMENT_H
#define AYI_IARGUMENT_H

#include <exception>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace ayisakov
{
namespace framework
{
class missing_values : public std::runtime_error
{
  public:
    missing_values(const std::string &argKey, int supplied, int required)
    : missing_values(argKey.c_str(), supplied, required)
    {
    }
    missing_values(const char *argKey, int supplied, int required)
    : std::runtime_error("Missing values for argument \"" +
                         std::string(argKey) + "\". " +
                         std::to_string(supplied) + " supplied but " +
                         std::to_string(required) + " required.")
    {
    }
    virtual ~missing_values() {}
};

class missing_target : public std::runtime_error
{
  public:
    missing_target(const std::string &argKey)
    : missing_target(argKey.c_str())
    {
    }
    missing_target(const char *argKey)
    : std::runtime_error("Missing target for argument " +
                         std::string(argKey) + ".")
    {
    }
};

/**
 * Command line argument interface
 */
template <typename Target> class IArgument
{
  protected:
    typedef IArgument *(*creator)(void);


  public:
    using ArgStore = std::unordered_map<std::string, creator>;
    enum ArgType {
        ARG_TYPE_INT,
        ARG_TYPE_BOOL,
        ARG_TYPE_FLOAT,
        ARG_TYPE_STRING,
        ARG_TYPE_CHAR
    };

    IArgument() {}

    virtual ~IArgument() {}

    /**
     * Returns true for standalone arguments, i.e. those not
     * followed by a value. Example: "--loop" or "-l"
     */
    virtual bool isStandalone() const = 0;

    /**
     * Add a value to an argument that requires additional values before
     * it can be applied. An example of a value is "myfile" in -f "myfile"
     *
     * @return the number of missing values. When this number is 0, apply()
     * can be used to apply the argument.
     */
    virtual int addValue(const std::string &value) = 0;
    virtual int addValue(const char *value)
    {
        return addValue(std::string(value));
    }

    /**
     * Apply the argument, i.e. perform the action associated
     * with the argument. The target is optional, but makes sense
     * for most arguments. Typical use case: IArgument-derived
     * object calls Target::someFunction(value1, value2, etc.).
     *
     * @throw missing_values if not enough values were provided
     * @throw missing_target if a required target was not provided
     */
    virtual void apply(Target *pTarget) = 0;

    /**
     * Convenience function for immediate application of a
     * single argument. There is no need to call addValue
     * when using this function. See void apply(Target *pTarget).
     */
    //    virtual void apply(const std::string &value, Target *pTarget) = 0;

    /**
     * Get argument, given the key. Ownership is transferred to
     * the caller.
     *
     * @return nullptr if no argument is found with the given key
     */
    static IArgument *getArgument(const std::string &key)
    {
        typename ArgStore::const_iterator fret = args()->find(key);
        if(fret == args()->end()) {
            return nullptr;
        }
        return fret->second();
    }

    /**
     * Get the short usage string associated with this argument
     */
    virtual const char *usage() const { return "default usage"; }

    /**
     * Get the (usually) longer help string associated with the argument
     */
    virtual const char *help() const { return "default help"; }

  protected:
    static void registerArg(const std::string &key, creator spawn)
    {
        ArgStore *pStore = args(1);
        auto fret = pStore->find(key);
        if(fret != pStore->end()) {
            args(-1);
            throw std::runtime_error(
                "Attempted to register the already registered "
                "argument \"" +
                key + "\".");
        }
        pStore->operator[](key) = spawn;
    }

    static void unregisterArg(const std::string &key)
    {
        ArgStore *pStore = args();
        if(!pStore) {
            throw std::runtime_error(
                "Attempted to unregister argument \"" + key +
                "\" but no "
                "argument store exists.");
        }
        auto fret = pStore->find(key);
        if(fret != pStore->end()) {
            pStore->erase(fret);
            args(-1);
        }
    }

  public: // not necessary with GCC, but for some reason MSVC cannot compile otherwise
    template <typename Derived> class Registrar
    {
      public:
        Registrar(const std::string &key) : mr_key(key)
        {
            IArgument<Target>::registerArg(key, &Derived::create);
        }
        ~Registrar()
        {
            IArgument<Target>::unregisterArg(mr_key);
        }
        Registrar(const Registrar &original) = delete;

      private:
        const std::string &mr_key;
    };

  private:
    /**
     * Reference-counted initialization on fist use
     * This is required to overcome the static initialization order fiasco.
     * Normal usage of this function does not change the refcount.
     * Registering an argument should inrement the refcount.
     * Unregistering an argument should decrement the refcount.
     * When the refcount goes to 0, the map is deleted and this
     * function will return a null pointer.
     */
    static ArgStore *args(int refCountChange = 0)
    {
        if(refCountChange < -1 || refCountChange > 1) {
            throw std::out_of_range("Invalid refcount change " +
                                    std::to_string(refCountChange) + ".");
        }
        static int refCount(0); // initialized only once
        static ArgStore *pStore(nullptr); // initialized only once
        if(refCount == 0 && refCountChange > 0) {
            if(pStore) {
                throw std::runtime_error(
                    "ArgStore exists but is not expected to "
                    "exist.");
            }
            pStore = new ArgStore();
        }
        refCount += refCountChange;
        if(refCount == 0) {
            delete pStore;
            pStore = nullptr;
        }
        return pStore;
    }
};
} // namespace framework
} // namespace ayisakov

#endif // AYI_IARGUMENT_H
