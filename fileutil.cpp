#include <fstream>
#include <sstream>
#include "fileutil.h"

namespace ayif = ayisakov::framework;

std::string ayif::FileUtil::readFileIntoString(const std::string &path)
{
    return readFileIntoString(path.c_str());
}

std::string ayif::FileUtil::readFileIntoString(const char *path)
{
    std::ifstream ifs;
    ifs.open(path, std::ios::in);
    std::ostringstream ss;
    ss << ifs.rdbuf();
    return ss.str();
}
