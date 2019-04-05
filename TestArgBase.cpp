#include "TestArgBase.h"

namespace ayif = ayisakov::framework;

// The map must be instantiated by the base non-template
// IArgument-derived class.
template<>
ayif::TestArgBase::ArgStore ayif::TestArgBase::args {};
