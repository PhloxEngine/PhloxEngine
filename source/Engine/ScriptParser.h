#pragma once
#include <string>
#include <vector>
#include <map>
#include <set>

class ScriptParser {
public:
    static ScriptParser& GetInstance();
    bool ParseFile(const std::string& content);
    void ExecuteFunction(const std::string& className, const std::string& functionName);
    void RegisterNativeClass(const std::string& className);
    bool IsNativeClass(const std::string& className) const;

private:
    ScriptParser() = default;
    std::map<std::string, std::map<std::string, std::string>> m_classes;
    std::set<std::string> m_nativeClasses;
    
    void ParseClass(const std::string& className, const std::string& baseClass, const std::string& content);
    void ParseFunction(const std::string& className, const std::string& content);
    std::string ConvertToNativeCode(const std::string& code);
    std::string RemoveComments(const std::string& content);
}; 