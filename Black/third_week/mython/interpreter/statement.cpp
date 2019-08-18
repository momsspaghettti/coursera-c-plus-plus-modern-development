#include "statement.h"
#include "object.h"
#include <iostream>
#include <sstream>


namespace Ast {

    using Runtime::Closure;

    ObjectHolder Assignment::Execute(Closure &closure) {
        const auto& new_value = right_value->Execute(closure);
        closure[var_name] = new_value;
        return new_value;
    }

    Assignment::Assignment(std::string var, std::unique_ptr<Statement> rv) :
    var_name(std::move(var)),
    right_value(std::move(rv)) {}

    VariableValue::VariableValue(std::string var_name) {
        dotted_ids.push_back(std::move(var_name));
    }

    VariableValue::VariableValue(std::vector<std::string> dotted_ids) {
        this->dotted_ids = std::move(dotted_ids);
    }

    ObjectHolder VariableValue::Execute(Closure &closure) {
        return executeChain(0, closure);
    }

    ObjectHolder VariableValue::executeChain(size_t index, Runtime::Closure &closure) {
        if (closure.find(dotted_ids[index]) == closure.end())
            throw Runtime::RuntimeError("No such variable!");

        if (index + 1 == dotted_ids.size()) {
            return closure[dotted_ids[index]];
        } else {
            return executeChain(
                    index + 1,
                    closure[dotted_ids[index]].TryAs<Runtime::ClassInstance>()->Fields()
                    );
        }
    }

    std::unique_ptr<Print> Print::Variable(std::string var) {
        return std::make_unique<Print>(
                std::make_unique<VariableValue>(std::move(var))
                        );
    }

    Print::Print(std::unique_ptr<Statement> argument) {
        args_.push_back(std::move(argument));
    }

    Print::Print(std::vector<std::unique_ptr<Statement>> args) : args_(std::move(args)) {}

    ObjectHolder Print::Execute(Closure &closure) {
        if (output) {
            bool first_ = true;
            for (auto& statement : args_) {
                if (first_) {
                    ObjectHolder object = statement->Execute(closure);
                    if (object) {
                        object->Print(*output);
                    } else {
                        *output << "None";
                    }
                    first_ = false;
                } else {
                    *output << ' ';
                    ObjectHolder object = statement->Execute(closure);
                    if (object) {
                        object->Print(*output);
                    } else {
                        *output << "None";
                    }
                }
            }
            *output << '\n';
        }

        return Runtime::ObjectHolder::None();
    }

    std::ostream *Print::output = &std::cout;

    void Print::SetOutputStream(std::ostream &output_stream) {
        output = &output_stream;
    }

    MethodCall::MethodCall(
            std::unique_ptr<Statement> object, std::string method, std::vector<std::unique_ptr<Statement>> args
    ) :
    object(std::move(object)),
    method(std::move(method)),
    args(std::move(args)) {}

    ObjectHolder MethodCall::Execute(Closure &closure) {
        std::vector<ObjectHolder> actual_args;
        for (const auto& arg : args) {
            actual_args.push_back(arg->Execute(closure));
        }

        auto* class_instance = object->Execute(closure).TryAs<Runtime::ClassInstance>();

        if (class_instance) {
            if (class_instance->HasMethod(method, actual_args.size())) {
                return class_instance->Call(method, actual_args);
            }
        }

        throw Runtime::RuntimeError("Bad Method call: " + method);
    }

    ObjectHolder Stringify::Execute(Closure &closure) {
        std::ostringstream out;
        argument->Execute(closure)->Print(out);
        return Runtime::ObjectHolder::Own(Runtime::String(out.str()));
    }

    std::optional<ObjectHolder> BinaryOperation::executeBinaryOperationOnClasses(
            Runtime::ClassInstance *lhs,
            const Runtime::ObjectHolder& rhs,
            const std::string &operation) {
        if (lhs->HasMethod(operation, 1)) {
            return lhs->Call(operation, {rhs});
        }

        return std::nullopt;
    }

    ObjectHolder Add::Execute(Closure &closure) {
        ObjectHolder lhs_h = lhs->Execute(closure);
        ObjectHolder rhs_h = rhs->Execute(closure);

        if ((lhs_h.TryAs<Runtime::Number>()) && (rhs_h.TryAs<Runtime::Number>())) {
            return Runtime::ObjectHolder::Own(Runtime::Number(
                    lhs_h.TryAs<Runtime::Number>()->GetValue() +
                    rhs_h.TryAs<Runtime::Number>()->GetValue()
                    ));
        }

        if ((lhs_h.TryAs<Runtime::String>()) && (rhs_h.TryAs<Runtime::String>())) {
            return Runtime::ObjectHolder::Own(Runtime::String(
                    lhs_h.TryAs<Runtime::String>()->GetValue() +
                    rhs_h.TryAs<Runtime::String>()->GetValue()
            ));
        }

        if (lhs_h.TryAs<Runtime::ClassInstance>()) {
            const auto result = executeBinaryOperationOnClasses(
                    lhs_h.TryAs<Runtime::ClassInstance>(),
                    rhs_h,
                    "__add__"
                    );
            if (result) {
                return *result;
            }
        }

        throw Runtime::RuntimeError("Bad Addition!");
    }

    ObjectHolder Sub::Execute(Closure &closure) {
        ObjectHolder lhs_h = lhs->Execute(closure);
        ObjectHolder rhs_h = rhs->Execute(closure);

        if ((lhs_h.TryAs<Runtime::Number>()) && (rhs_h.TryAs<Runtime::Number>())) {
            return Runtime::ObjectHolder::Own(Runtime::Number(
                    lhs_h.TryAs<Runtime::Number>()->GetValue() -
                    rhs_h.TryAs<Runtime::Number>()->GetValue()
            ));
        }

        if (lhs_h.TryAs<Runtime::ClassInstance>()) {
            const auto result = executeBinaryOperationOnClasses(
                    lhs_h.TryAs<Runtime::ClassInstance>(),
                    rhs_h,
                    "__sub__"
            );
            if (result) {
                return *result;
            }
        }

        throw Runtime::RuntimeError("Bad Subtraction!");
    }

    ObjectHolder Mult::Execute(Runtime::Closure &closure) {
        ObjectHolder lhs_h = lhs->Execute(closure);
        ObjectHolder rhs_h = rhs->Execute(closure);

        if ((lhs_h.TryAs<Runtime::Number>()) && (rhs_h.TryAs<Runtime::Number>())) {
            return Runtime::ObjectHolder::Own(Runtime::Number(
                    lhs_h.TryAs<Runtime::Number>()->GetValue() *
                    rhs_h.TryAs<Runtime::Number>()->GetValue()
            ));
        }

        if (lhs_h.TryAs<Runtime::ClassInstance>()) {
            const auto result = executeBinaryOperationOnClasses(
                    lhs_h.TryAs<Runtime::ClassInstance>(),
                    rhs_h,
                    "__mul__"
            );
            if (result) {
                return *result;
            }
        }

        throw Runtime::RuntimeError("Bad Multiplication!");
    }

    ObjectHolder Div::Execute(Runtime::Closure &closure) {
        ObjectHolder lhs_h = lhs->Execute(closure);
        ObjectHolder rhs_h = rhs->Execute(closure);

        if ((lhs_h.TryAs<Runtime::Number>()) && (rhs_h.TryAs<Runtime::Number>())) {
            if (rhs_h.TryAs<Runtime::Number>()->GetValue() == 0) {
                throw Runtime::RuntimeError("Zero Division!");
            }

            return Runtime::ObjectHolder::Own(Runtime::Number(
                    lhs_h.TryAs<Runtime::Number>()->GetValue() /
                    rhs_h.TryAs<Runtime::Number>()->GetValue()
            ));
        }

        if (lhs_h.TryAs<Runtime::ClassInstance>()) {
            const auto result = executeBinaryOperationOnClasses(
                    lhs_h.TryAs<Runtime::ClassInstance>(),
                    rhs_h,
                    "__div__"
            );
            if (result) {
                return *result;
            }
        }

        throw Runtime::RuntimeError("Bad Division!");
    }

    ObjectHolder Compound::Execute(Closure &closure) {
        for (auto& statement : statements_) {
            if (dynamic_cast<Return*>(statement.get()))
                return statement->Execute(closure);

            if (
                    dynamic_cast<IfElse*>(statement.get()) ||
                    dynamic_cast<MethodCall*>(statement.get())
                    ) {
                ObjectHolder result = statement->Execute(closure);
                if (result) {
                    return result;
                }
            } else {
                statement->Execute(closure);
            }
        }

        return Runtime::ObjectHolder::None();
    }

    ObjectHolder Return::Execute(Closure &closure) {
        return statement->Execute(closure);
    }

    ClassDefinition::ClassDefinition(ObjectHolder class_) :
            cls(std::move(class_)),
            class_name(cls.TryAs<Runtime::Class>()->GetName()) {}

    ObjectHolder ClassDefinition::Execute(Runtime::Closure &closure) {
        closure[class_name] = cls;
        return Runtime::ObjectHolder::None();
    }

    FieldAssignment::FieldAssignment(
            VariableValue object, std::string field_name, std::unique_ptr<Statement> rv
    )
            : object(std::move(object)),
            field_name(std::move(field_name)),
            right_value(std::move(rv)) {}

    ObjectHolder FieldAssignment::Execute(Runtime::Closure &closure) {
        ObjectHolder& field = object.Execute(closure).TryAs<Runtime::ClassInstance>()->Fields()[field_name];
        field = right_value->Execute(closure);
        return field;
    }

    IfElse::IfElse(
            std::unique_ptr<Statement> condition,
            std::unique_ptr<Statement> if_body,
            std::unique_ptr<Statement> else_body
    ) :
    condition_(std::move(condition)),
    if_body_(std::move(if_body)),
    else_body_(std::move(else_body))
    {}

    ObjectHolder IfElse::Execute(Runtime::Closure &closure) {
        ObjectHolder condition = condition_->Execute(closure);

        if (condition) {
            if (condition.Get()->IsTrue()) {
                if (if_body_)
                    return if_body_->Execute(closure);
            } else {
                if (else_body_)
                    return else_body_->Execute(closure);
            }
        } else {
            if (else_body_)
                return else_body_->Execute(closure);
        }
        return ObjectHolder::None();
    }

    ObjectHolder Or::Execute(Runtime::Closure &closure) {
        ObjectHolder lhs_h = lhs->Execute(closure);
        ObjectHolder rhs_h = rhs->Execute(closure);
        if (!lhs_h) {
            lhs_h = ObjectHolder::Own(Runtime::Bool(false));
        }
        if (!rhs_h) {
            rhs_h = ObjectHolder::Own(Runtime::Bool(false));
        }
        return Runtime::ObjectHolder::Own(
                Runtime::Bool(
                        lhs_h->IsTrue() ||
                        rhs_h->IsTrue()
                        )
                );
    }

    ObjectHolder And::Execute(Runtime::Closure &closure) {
        ObjectHolder lhs_h = lhs->Execute(closure);
        ObjectHolder rhs_h = rhs->Execute(closure);
        if (!lhs_h) {
            lhs_h = ObjectHolder::Own(Runtime::Bool(false));
        }
        if (!rhs_h) {
            rhs_h = ObjectHolder::Own(Runtime::Bool(false));
        }
        return Runtime::ObjectHolder::Own(
                Runtime::Bool(
                        lhs_h->IsTrue() &&
                        rhs_h->IsTrue()
                )
        );
    }

    ObjectHolder Not::Execute(Runtime::Closure &closure) {
        ObjectHolder argument_h = argument->Execute(closure);
        if (!argument_h) {
            argument_h = ObjectHolder::Own(Runtime::Bool(false));
        }
        return Runtime::ObjectHolder::Own(
                Runtime::Bool(
                        !argument_h->IsTrue()
                )
        );
    }

    Comparison::Comparison(
            Comparator cmp, std::unique_ptr <Statement> lhs, std::unique_ptr <Statement> rhs
    ) :
    comparator(std::move(cmp)),
    left(std::move(lhs)),
    right(std::move(rhs))
    {}

    ObjectHolder Comparison::Execute(Runtime::Closure &closure) {
        return Runtime::ObjectHolder::Own(
                Runtime::Bool(
                        comparator(
                                left->Execute(closure),
                                right->Execute(closure)
                                )
                        )
                );
    }

    NewInstance::NewInstance(
            const Runtime::Class &class_, std::vector<std::unique_ptr<Statement>> args
    ) : class_(class_),
    args(std::move(args)) {}

    NewInstance::NewInstance(const Runtime::Class &class_) : NewInstance(class_, {}) {}

    ObjectHolder NewInstance::Execute(Runtime::Closure &closure) {
        auto* new_instance = new Runtime::ClassInstance(class_);
        if (new_instance->HasMethod("__init__", args.size())) {
            std::vector<ObjectHolder> actual_args;
            for (const auto& statement : args) {
                actual_args.push_back(statement->Execute(closure));
            }
            new_instance->Call("__init__", actual_args);
        }

        return ObjectHolder::Share(*new_instance);
    }


} /* namespace Ast */
