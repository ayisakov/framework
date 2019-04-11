#include "TestArgTarget.h"
#include "TestArgument.h"
#include <stdlib.h>

namespace ayif = ayisakov::framework;


static const std::string key_short = "t";
static const std::string key_long = "-test";
static const char *help_string = "Sample help string";
static const char *usage_string = "Sample usage string";
const int values_required = 2;

ayif::TestArgBase::Registrar
reg_this(ayif::TestArgBase::ArgRecord({ key_short, key_long },
                                      &ayif::TestArgument::create, usage_string,
                                      help_string));

ayif::TestArgument::TestArgument() : m_valuesProvided(0) {}

ayif::TestArgument::~TestArgument() {}

ayif::TestArgBase *ayif::TestArgument::create()
{
    return new TestArgument();
}

bool ayif::TestArgument::isStandalone() const { return false; }

int ayif::TestArgument::addValue(const std::string &value)
{
    // first value is int, second value is string
    if(!m_valueInt) {
        m_valueInt = atoi(value.c_str());
        m_valuesProvided++;
    } else if(!m_valueString) {
        m_valueString = value;
        m_valuesProvided++;
    }
    return values_required - m_valuesProvided;
}

void ayif::TestArgument::apply(ayif::TestArgTarget *pTarget)
{
    if(!pTarget) {
        throw missing_target(key_long);
    }
    if(!(m_valueInt && m_valueString)) {
        throw missing_values(key_long, m_valuesProvided, values_required);
    }
    pTarget->setValues(*m_valueInt, *m_valueString);
}

const char *ayif::TestArgument::usage() const
{
    return usage_string;
}

const char *ayif::TestArgument::help() const
{
    return help_string;
}
