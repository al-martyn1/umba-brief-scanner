/*! \file
    \brief Токенизер для плюсов
 */

#pragma once

//
#include "umba/tokenizer.h"
//
#include "umba/tokenizer/lang/cpp.h"


inline
const auto& getTokenizerBuilder()
{
    static auto tokenizerBuilder = umba::tokenizer::makeTokenizerBuilderCpp<char>();
    return tokenizerBuilder;
}


struct CppToeknHandler
{



}; // struct CppToeknHandler

