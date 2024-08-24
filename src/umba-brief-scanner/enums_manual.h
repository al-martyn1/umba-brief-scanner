/*! \file
    \brief "Ручные" перечисления
 */

#pragma once


enum class DoxificationMode : std::uint32_t
{
    invalid          = (std::uint32_t)-1,
    unknown          = (std::uint32_t)-1,
    noDoxyfication   = 0,
    doxyfy           = 1, // doxyfy only if no @file doxygen tag found at beginning of the file
    doxyfyAlways     = 2  // doxyfy always, regardles of existence @file tag
};




