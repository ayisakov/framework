#ifndef AYI_IARGUMENT_H
#define AYI_IARGUMENT_H

#include <exception>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <list>
#include <sstream>

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
    class ArgRecord
    {
      public:
        ArgRecord(std::list<std::string> keys, creator spawner,
                         const char *usage, const char *help)
            : m_keys(keys), m_spawner(spawner), m_usage(usage), m_help(help)
        {}
        ~ArgRecord() {}
        std::list<std::string> m_keys; // argument strings, including all aliases
        creator m_spawner;   // factory method
        const char *m_usage; // short usage string
        const char *m_help;  // longer help string
    };
    using ArgFactory = std::unordered_map<std::string, creator>; // key, factory method
    using ArgInfo = std::unordered_map<creator, ArgRecord>; // (unique) factory method, record
    class ArgStore
    {
    public:
        ArgStore() {}
        ~ArgStore() {}
        ArgFactory creators;
        ArgInfo info;
    };
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
        ArgFactory &creators = args()->creators;
        typename ArgFactory::const_iterator fret = creators.find(key);
        if(fret == creators.end()) {
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

    enum EArgInfo {
        EARGINFOHELP = 0,
        EARGINFOUSAGE
    };

    /**
     * Get usage information from all registered arguments
     */
    static std::string usageAll()
    {
        return printAll(EArgInfo::EARGINFOUSAGE);
    }

    /**
     * Get help information from all registered arguments
     */
    static std::string helpAll()
    {
        return printAll(EArgInfo::EARGINFOHELP);
    }

    static std::string printAll(EArgInfo whichInfo)
    {
        std::ostringstream oss;
        ArgStore *pStore = args();
        if(!pStore) {
            throw std::runtime_error(
                "Attempting to dump argument information "
                "without an argument store.");
        }
        ArgInfo &info = pStore->info;
        for(const auto &argpair : info) {
            const ArgRecord &arg = argpair.second;
            int keyCount = 0;
            for(const auto &key : arg.m_keys) {
                if(++keyCount > 1) {
                    oss << " or ";
                }
                oss << "-" << key;
            }
            oss << " ";
            switch (whichInfo) {
            case EARGINFOHELP:
                if(arg.m_help) {
                    oss << arg.m_help;
                }
                oss << std::endl;
                break;
            case EARGINFOUSAGE:
                if(arg.m_usage) {
                    oss << arg.m_usage;
                }
                oss << " ";
                break;
            default:
                throw std::runtime_error(
                    "Unrecognized argument information enum "
                    "value " +
                    std::to_string(int(whichInfo)));
                break;
            }
        }
        return oss.str();
    }

  protected:
    static void registerArg(const ArgRecord &rec)
    {
        if(!rec.m_spawner) {
            throw std::runtime_error(
                "Attempted to register argument with a null "
                "creator pointer");
        }
        ArgStore *pStore = args(1);
        ArgFactory &creators = pStore->creators;
        ArgInfo &info = pStore->info;
        for(const auto &key : rec.m_keys) {
            auto fret = creators.find(key);
            if(fret != creators.end()) {
                args(-1);
                throw std::runtime_error(
                    "Attempted to register the already registered "
                    "argument \"" +
                    key + "\".");
            }
            creators[key] = rec.m_spawner;
        }
        //        info[rec.m_spawner] = rec;
        info.emplace(rec.m_spawner, rec);
    }

    static void unregisterArg(const ArgRecord &arg)
    {
        ArgStore *pStore = args();
        if(!pStore) {
            throw std::runtime_error(
                "Attempted to unregister argument"
                " but no "
                "argument store exists.");
        }
        ArgFactory &creators = pStore->creators;
        ArgInfo &info = pStore->info;
        auto fret = info.find(arg.m_spawner);
        if(fret != info.end()) {
            for(const auto &key : arg.m_keys) {
                creators.erase(key);
            }
            info.erase(fret);
            args(-1);
        }
    }

  public: // not necessary with GCC, but for some reason MSVC cannot compile otherwise
    /*template <typename Derived> */class Registrar
    {
      public:
        Registrar(const ArgRecord &rec) : m_rec(rec)
        {
            IArgument<Target>::registerArg(rec);
        }
        ~Registrar()
        {
            IArgument<Target>::unregisterArg(m_rec);
        }
        Registrar(const Registrar &original) = delete;

      private:
        ArgRecord m_rec;
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
