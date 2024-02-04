#include "../httpRequestAndConfig/.hpp"
#include ".hpp"

namespace
{
std::string::size_type findScriptExtensionPos(const std::string &target, const std::string &cgiExtension)
{
    const std::string::size_type scriptExtensionPos = target.find(cgiExtension);
    if (scriptExtensionPos != std::string::npos && (scriptExtensionPos + cgiExtension.size() == target.size() ||
                                                    target.at(scriptExtensionPos + cgiExtension.size()) == '/'))
    {
        return scriptExtensionPos;
    }
    else
    {
        return cgiExtension.size() * -1;
    }
}
} // namespace

Uri segmentUri(const std::string &target, const std::string &cgiExtension)
{
    const std::string::size_type scriptExtensionPos = findScriptExtensionPos(target, cgiExtension);
    const std::string scriptPath = target.substr(0, scriptExtensionPos + cgiExtension.size());
    const std::string rest = target.substr(scriptExtensionPos + cgiExtension.size());
    const std::string::size_type questionPos = rest.find('?');
    const std::string extraPath = rest.substr(0, questionPos);
    const std::string queryString = questionPos <= rest.size() ? rest.substr(questionPos + 1) : "";
    return Uri(scriptPath, extraPath, queryString);
}
