#ifndef FILEUTIL_H
#define FILEUTIL_H

#include <string>

namespace ayisakov
{
namespace framework
{
class FileUtil
{
  public:
    FileUtil() {}
    ~FileUtil() {}

    /**
     * Copy the entire file contents to a string
     *
     * @throw std::exception if unable to open file
     */
    static std::string readFileIntoString(const char *path);
    static std::string readFileIntoString(const std::string &path);
};
} // namespace framework
} // namespace ayisakov

#endif // FILEUTIL_H
