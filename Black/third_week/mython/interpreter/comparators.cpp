#include "object.h"
#include "object_holder.h"
#include <sstream>


namespace Runtime {

    bool Equal(ObjectHolder lhs, ObjectHolder rhs) {
        if ((lhs.TryAs<Class>()) && (rhs.TryAs<Class>())) {
            return lhs.TryAs<Class>()->GetName() ==
            rhs.TryAs<Class>()->GetName();
        }

        if ((lhs.TryAs<ClassInstance>()) && (rhs.TryAs<ClassInstance>())) {
            if (lhs.TryAs<ClassInstance>()->class_.GetName() ==
                    rhs.TryAs<ClassInstance>()->class_.GetName()) {
                std::ostringstream one, two;
                lhs->Print(one);
                rhs->Print(two);

                return one.str() == two.str();
            }
            return false;
        }

        std::ostringstream one, two;
        lhs->Print(one);
        rhs->Print(two);
        return one.str() == two.str();
    }

    bool Less(ObjectHolder lhs, ObjectHolder rhs) {
        if ((lhs.TryAs<Number>()) && (rhs.TryAs<Number>())) {
            return lhs.TryAs<Number>()->GetValue() <
                    rhs.TryAs<Number>()->GetValue();
        }

        if ((lhs.TryAs<String>()) && (rhs.TryAs<String>())) {
            return lhs.TryAs<String>()->GetValue() <
                    rhs.TryAs<String>()->GetValue();
        }

        throw Runtime::RuntimeError("Bad Comparison!");
    }

} /* namespace Runtime */