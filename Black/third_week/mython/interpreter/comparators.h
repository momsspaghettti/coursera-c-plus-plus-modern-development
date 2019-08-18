#pragma once
#include "object_holder.h"

namespace Runtime {

    bool Equal(ObjectHolder lhs, ObjectHolder rhs);

    bool Less(ObjectHolder lhs, ObjectHolder rhs);

    inline bool NotEqual(ObjectHolder lhs, ObjectHolder rhs) {
        return !Equal(lhs, rhs);
    }

    inline bool Greater(ObjectHolder lhs, ObjectHolder rhs) {
        return !Less(lhs, rhs) && !Equal(lhs, rhs);
    }

    inline bool LessOrEqual(ObjectHolder lhs, ObjectHolder rhs) {
        return !Greater(lhs, rhs);
    }

    inline bool GreaterOrEqual(ObjectHolder lhs, ObjectHolder rhs) {
        return !Less(lhs, rhs);
    }

} /* namespace Runtime */
