#include "./Class.hpp"
#include "./Instance.hpp"
#include "./Function.hpp"
#include <memory>
#include <unordered_map>


CallableClass::CallableClass(
        const std::string& name,
        const std::unordered_map<std::string, std::shared_ptr<CallableFunction>>& methods)
        : name(name), methods(methods) {}


int CallableClass::arity(void) const {

    return 0;
}


super::object CallableClass::call(Interpreter&, std::vector<super::object>&) {

    std::shared_ptr<Instance> instance = std::make_shared<Instance>(*this);

    return super::object(instance);
}


std::string CallableClass::to_string(void) const {

    return "<class " + this->name + ">";
}


std::shared_ptr<CallableFunction> CallableClass::findMethod(const std::string& name) const {

    const auto& method = this->methods.find(name);

    if (method != this->methods.end())
        return method->second;

    return nullptr;
}





