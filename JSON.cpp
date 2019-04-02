#include "JSON.h"

namespace ayif = ayisakov::framework;
using json = nlohmann::json;

static const int json_indent_spaces = 4;

ayif::JSON::JSON(const std::string &jsonStr)
: m_json(json::parse(jsonStr))
{
    // TODO: check to see what kind of exception the json constructor can throw
}

ayif::JSON::~JSON() {}

void ayif::JSON::fromJSON(const std::string &str)
{
    m_json = json::parse(str);
}

std::string ayif::JSON::toJSON() const
{
    return m_json.dump(json_indent_spaces);
}

json *ayif::JSON::getDeepestObject(const std::list<std::string> &path)
{
    std::list<std::string>::const_iterator lit = path.begin();
    json *j = &m_json;
    while(lit != path.end()) {
        j = &(*j).at(*lit);
        if (!j) {
            throw not_found("JSON object with key " + *lit + " not found.");
        }
        ++lit;
    }
    return j;
}

std::string ayif::JSON::valueAsString(const std::list<std::string> &path)
{
    json *j = getDeepestObject(path);
    return j->get<std::string>();
}

int ayif::JSON::valueAsInt(const std::list<std::string> &path)
{
    json *j = getDeepestObject(path);
    return j->get<int>();
}

double ayif::JSON::valueAsDouble(const std::list<std::string> &path)
{
    json *j = getDeepestObject(path);
    return j->get<double>();
}
