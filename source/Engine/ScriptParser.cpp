#include "ScriptParser.h"
#include <fstream>
#include <iostream>
#include <regex>
#include <vector>
#include <any>

ScriptParser& ScriptParser::GetInstance() {
    static ScriptParser instance;
    return instance;
}

void ScriptParser::RegisterNativeClass(const std::string& className) {
    m_nativeClasses.insert(className);
}

bool ScriptParser::IsNativeClass(const std::string& className) const {
    return m_nativeClasses.find(className) != m_nativeClasses.end();
}

bool ScriptParser::ParseFile(const std::string& content) {
    std::string cleanContent = RemoveComments(content);
    //std::cout << "Parsing content:\n" << cleanContent << std::endl;
    
    if (cleanContent.find("class") == std::string::npos) {
        std::cout << "No class found in content" << std::endl;
        return true;
    }

    std::regex classRegex(R"(class\s+(\w+)(?:\s+extends\s+(\w+))?\s*\{([\s\S]+)\})", std::regex::ECMAScript);
    std::sregex_iterator it(cleanContent.begin(), cleanContent.end(), classRegex);
    std::sregex_iterator end;

    bool foundClass = false;
    while (it != end) {
        foundClass = true;
        std::string className = (*it)[1];
        std::string baseClass = ((*it)[2].matched) ? (*it)[2].str() : "";
        std::string classContent = (*it)[3];
        
        if (!baseClass.empty() && !IsNativeClass(baseClass)) {
            std::cerr << "Base class " << baseClass << " is not registered!" << std::endl;
            return false;
        }
        
        std::cout << "Found class: " << className;
        if (!baseClass.empty()) {
            std::cout << " extends " << baseClass;
        }
        std::cout << "\nContent: " << classContent << std::endl;
        
        ParseClass(className, baseClass, classContent);
        ++it;
    }

    return foundClass;
}

void ScriptParser::ParseClass(const std::string& className, const std::string& baseClass, const std::string& content) {
    std::regex functionRegex(R"(function\s+(\w+)\s*\(\)\s*\{([^}]*)\})", std::regex::ECMAScript);
    std::string::const_iterator searchStart(content.cbegin());
    std::smatch match;

    while (std::regex_search(searchStart, content.cend(), match, functionRegex)) {
        std::string functionName = match[1];
        std::string functionContent = match[2];
        
        std::regex traceRegex(R"(trace\((.*?)\);)");
        functionContent = std::regex_replace(functionContent, traceRegex, "printf($1);");
        
        std::cout << "Found function: " << className << "::" << functionName << "\nContent: " << functionContent << std::endl;
        
        if (m_classes.find(className) == m_classes.end()) {
            m_classes[className] = std::map<std::string, std::string>();
        }
        m_classes[className][functionName] = functionContent;
        
        searchStart = match.suffix().first;
    }
}

void ScriptParser::ExecuteFunction(const std::string& className, const std::string& functionName) {
    if (m_classes.find(className) == m_classes.end() ||
        m_classes[className].find(functionName) == m_classes[className].end()) {
        std::cerr << "Function not found: " << className << "::" << functionName << std::endl;
        return;
    }

    std::string code = m_classes[className][functionName];
    
    std::regex printfRegex("printf\\(\"([^\"]*)\"\\);");
    std::smatch match;
    while (std::regex_search(code, match, printfRegex)) {
        std::string content = match[1];
        size_t pos = 0;
        while ((pos = content.find("\\n", pos)) != std::string::npos) {
            content.replace(pos, 2, "\n");
            pos += 1;
        }
        std::cout << content;
        code = match.suffix();
    }

    std::regex assignmentRegex(R"((\w+)\s*=\s*(\w+)\.(\w+)\((.*?)\);)");
    std::regex methodCallRegex(R"((\w+)\.(\w+)\((.*?)\);)");
    
    while (std::regex_search(code, match, assignmentRegex)) {
        std::string className = match[2];
        std::string funcName = match[3];
        std::string args = match[4];

        if (m_scriptClasses.find(className) != m_scriptClasses.end()) {
            auto& scriptClass = m_scriptClasses[className];
            if (auto func = scriptClass.GetStaticFunction(funcName)) {
                (*func)(args);
            }
        }
        code = match.suffix();
    }

    while (std::regex_search(code, match, methodCallRegex)) {
        std::string className = "Sprite";
        std::string funcName = match[2];
        std::string args = match[3];

        if (m_scriptClasses.find(className) != m_scriptClasses.end()) {
            auto& scriptClass = m_scriptClasses[className];
            if (auto func = scriptClass.GetFunction(funcName)) {
                (*func)(args);
            }
        }
        code = match.suffix();
    }
}

std::string ScriptParser::RemoveComments(const std::string& content) {
    std::string result;
    result.reserve(content.length());
    
    enum class State { Code, SingleLineComment, MultiLineComment };
    State state = State::Code;
    
    for (size_t i = 0; i < content.length(); ++i) {
        switch (state) {
            case State::Code:
                if (i < content.length() - 1) {
                    if (content[i] == '/' && content[i + 1] == '/') {
                        state = State::SingleLineComment;
                        ++i;
                        continue;
                    }
                    if (content[i] == '/' && content[i + 1] == '*') {
                        state = State::MultiLineComment;
                        ++i;
                        continue;
                    }
                }
                result += content[i];
                break;
                
            case State::SingleLineComment:
                if (content[i] == '\n') {
                    state = State::Code;
                    result += '\n';  
                }
                break;
                
            case State::MultiLineComment:
                if (i < content.length() - 1 && content[i] == '*' && content[i + 1] == '/') {
                    state = State::Code;
                    ++i;
                }
                break;
        }
    }
    
    return result;
}

ScriptClass& ScriptParser::RegisterClass(const std::string& className) {
    RegisterNativeClass(className);
    return m_scriptClasses[className];
} 