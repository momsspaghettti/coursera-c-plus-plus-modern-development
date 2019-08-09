#include "json_printer.h"


void PrintJsonString(std::ostream& out, std::string_view str) {
    out << "\"";
    for (const char& ch : str) {
        if (ch == '"') {
            out << R"(\")";
        } else if (ch == '\\') {
            out << R"(\\)";
        } else {
            out << ch;
        }
    }
    out << "\"";
}


ArrayContext PrintJsonArray(std::ostream& out) {
    return ArrayContext(out, new EmptyContext());
}


ObjectContext PrintJsonObject(std::ostream& out) {
    return ObjectContext(out, new EmptyContext());
}