/*! \file
    \brief Токенизер для плюсов
 */

#pragma once

//
#include "umba/tokenizer.h"
//
#include "umba/tokenizer/lexers/cpp.h"


inline
const auto& getTokenizerBuilder()
{
    static auto tokenizerBuilder = umba::tokenizer::cpp::makeTokenizerBuilder<char>();
    return tokenizerBuilder;
}


struct CppToeknHandler
{



}; // struct CppToeknHandler

