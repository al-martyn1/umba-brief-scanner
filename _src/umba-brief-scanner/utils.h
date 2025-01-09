/*! \file
    \brief Разные утилитки
 */

#pragma once


#include "umba/filename.h"
#include "umba/filesys.h"
#include "umba/string_plus.h"
#include "umba/transliteration.h"
//
#include "log.h"


//
#include <string>
#include <algorithm>
#include <iterator>


//----------------------------------------------------------------------------
template<typename StringType> inline
std::vector<StringType> splitAndTrimAndSkipEmpty(const StringType &str, typename StringType::value_type ch)
{
    // на замену marty_cpp::splitToLinesSimple(paths, false, ',')

    // return marty_cpp::splitToLinesSimple(str, false, ch);

    using CharType = typename StringType::value_type;

    auto vec = marty_cpp::splitToLinesSimple(str, false, ch);

    for(auto &v : vec)
    {
        v = marty_cpp::simple_trim(v, [](CharType ch) { return ch==' '; });
    }

    std::vector<StringType> res; res.reserve(vec.size());
    for(auto && v : vec)
    {
        if (!v.empty())
            res.emplace_back(v);
    }

    return res;
}


