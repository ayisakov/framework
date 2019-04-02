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

std::string ayif::JSON::valueAsString(const std::list<std::string> &path)
{
    std::list<std::string>::const_iterator lit = path.begin();
    json *j = &m_json;
    // json::const_iterator jit = j.begin();
    //return j["file_info"]["name"].get<std::string>();
    std::string path1(*lit);
    std::string path2("name");
    //    return j[*lit][path2].get<std::string>();
    //    return jit->dump();
    while(lit != path.end()) {
        // jit = j.find(*lit);
        // if(jit == j.end()) {
        //    throw not_found("Value with key \"" + *lit + "\" not found");
        //}
        // j = *jit;
        j = &(*j)[*lit];
        ++lit;
    }
    // TODO: the line below does not implicitly represent JSON numeric
    // values as strings, instead throwing an exception when it encounters
    // a numeric value. Find a way around this.
    return j->get<std::string>();
}
