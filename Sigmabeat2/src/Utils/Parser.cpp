#include "Parser.hpp"

namespace Utils {
    String RemoveComments(const String& s) {
        String ret = s;
        for (const auto& match : UR"(/\*[\s\S]*?\*/|//.*)"_re.findAll(s)) {
            ret.remove(String(match[0]));
        }
        return ret;
    }
}
