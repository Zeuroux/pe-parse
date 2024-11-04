/*
The MIT License (MIT)

Copyright (c) 2019 Trail of Bits, Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <codecvt>
#include <locale>
#include <pe-parse/to_string.h>
#include <cstdint>

namespace peparse {
// See
// https://stackoverflow.com/questions/38688417/utf-conversion-functions-in-c11
std::string from_utf16(const UCharString &u) {
    std::string result;
    result.reserve(u.size());

    for (size_t i = 0; i < u.size(); ++i) {
        uint16_t code_unit = u[i];
        if (code_unit >= 0xD800 && code_unit <= 0xDBFF && i + 1 < u.size()) {
            uint16_t low_surrogate = u[i + 1];
            if (low_surrogate >= 0xDC00 && low_surrogate <= 0xDFFF) {
                int code_point = ((code_unit - 0xD800) << 10) + (low_surrogate - 0xDC00) + 0x10000;
                i++;
                result += static_cast<char>(0xF0 | ((code_point >> 18) & 0x07));
                result += static_cast<char>(0x80 | ((code_point >> 12) & 0x3F));
                result += static_cast<char>(0x80 | ((code_point >> 6) & 0x3F));
                result += static_cast<char>(0x80 | (code_point & 0x3F));
            }
        } else {
            if (code_unit <= 0x7F) {
                result += static_cast<char>(code_unit);
            } else if (code_unit <= 0x7FF) {
                result += static_cast<char>(0xC0 | ((code_unit >> 6) & 0x1F));
                result += static_cast<char>(0x80 | (code_unit & 0x3F));
            } else {
                result += static_cast<char>(0xE0 | ((code_unit >> 12) & 0x0F));
                result += static_cast<char>(0x80 | ((code_unit >> 6) & 0x3F));
                result += static_cast<char>(0x80 | (code_unit & 0x3F));
            }
        }
    }

    return result;
}

} // namespace peparse
