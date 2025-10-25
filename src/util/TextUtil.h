#ifndef TEXTUTIL_H
#define TEXTUTIL_H
#include <cctype>

inline bool containsIgnoreCase(const char* text, const char* substring) {
    if (!text || !substring) {
        return false;
    }

    const char* pText = text;
    const char* pSubstring = substring;

    while (*pText) {
        const char* pTextIter = pText;
        pSubstring = substring;

        while (*pTextIter && *pSubstring &&
               (std::tolower(*pTextIter) == tolower(*pSubstring))) {
            pTextIter++;
            pSubstring++;
        }

        if (!*pSubstring) {
            return true;
        }

        pText++;
    }

    return false;
}

#endif //TEXTUTIL_H
