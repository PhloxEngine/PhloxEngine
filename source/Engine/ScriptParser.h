#pragma once
#include <string>
#include <vector>
#include <map>
#include <set>
#include <functional>

class ScriptClass {
public:
    using NativeFunction = std::function<void(const std::string&)>;
    
    virtual ~ScriptClass() = default;
    
    template<typename Class>
    ScriptClass& AddFunction(const std::string& name, void (Class::*func)(const std::string&)) {
        Class* instance = dynamic_cast<Class*>(this);
        if (!instance) return *this;
        
        m_functions[name] = [instance, func](const std::string& args) {
            (instance->*func)(args);
        };
        return *this;
    }

    ScriptClass& AddFunction(const std::string& name, void (*func)(const std::string&)) {
        m_functions[name] = func;
        return *this;
    }

    ScriptClass& AddStaticFunction(const std::string& name, void (*func)(const std::string&)) {
        m_staticFunctions[name] = func;
        return *this;
    }

    NativeFunction* GetFunction(const std::string& name) {
        auto it = m_functions.find(name);
        return (it != m_functions.end()) ? &it->second : nullptr;
    }

    NativeFunction* GetStaticFunction(const std::string& name) {
        auto it = m_staticFunctions.find(name);
        return (it != m_staticFunctions.end()) ? &it->second : nullptr;
    }

private:
    std::map<std::string, NativeFunction> m_functions;
    std::map<std::string, NativeFunction> m_staticFunctions;
};

class ScriptParser {
public:
    static ScriptParser& GetInstance();
    bool ParseFile(const std::string& content);
    void ExecuteFunction(const std::string& className, const std::string& functionName);
    void RegisterNativeClass(const std::string& className);
    bool IsNativeClass(const std::string& className) const;
    ScriptClass& RegisterClass(const std::string& className);

private:
    ScriptParser() = default;
    std::map<std::string, std::map<std::string, std::string>> m_classes;
    std::set<std::string> m_nativeClasses;
    std::map<std::string, ScriptClass> m_scriptClasses;
    
    void ParseClass(const std::string& className, const std::string& baseClass, const std::string& content);
    void ParseFunction(const std::string& className, const std::string& content);
    std::string ConvertToNativeCode(const std::string& code);
    std::string RemoveComments(const std::string& content);
}; 