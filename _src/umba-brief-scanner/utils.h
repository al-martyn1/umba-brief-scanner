/*! \file
    \brief Разные утилитки
 */

#pragma once


#include "cpp-tokenizer.h"
#include "log.h"
//

#include "umba/filename.h"
#include "umba/filesys.h"
#include "umba/string_plus.h"
#include "umba/transliteration.h"
//
#include "marty_cpp/marty_cpp.h"
#include "marty_cpp/src_normalization.h"


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

//----------------------------------------------------------------------------
template<typename StringType>
StringType getTokenNameString(umba::tokenizer::payload_type p)
{
    switch(p)
    {
        case UMBA_TOKENIZER_TOKEN_UNEXPECTED                                  : return umba::string_plus::make_string<StringType>("unxp");
        case UMBA_TOKENIZER_TOKEN_LINEFEED                                    : return umba::string_plus::make_string<StringType>("LINEFEED");
        case UMBA_TOKENIZER_TOKEN_SPACE                                       : return umba::string_plus::make_string<StringType>("SPACE");
        case UMBA_TOKENIZER_TOKEN_TAB                                         : return umba::string_plus::make_string<StringType>("TAB");
        case UMBA_TOKENIZER_TOKEN_IDENTIFIER                                  : return umba::string_plus::make_string<StringType>("ident");
        case UMBA_TOKENIZER_TOKEN_SEMIALPHA                                   : return umba::string_plus::make_string<StringType>("");
        // https://en.wikipedia.org/wiki/Bracket
        case UMBA_TOKENIZER_TOKEN_CURLY_BRACKET_OPEN                          : return umba::string_plus::make_string<StringType>("curly");
        case UMBA_TOKENIZER_TOKEN_CURLY_BRACKET_CLOSE                         : return umba::string_plus::make_string<StringType>("curly");
        case UMBA_TOKENIZER_TOKEN_ROUND_BRACKET_OPEN                          : return umba::string_plus::make_string<StringType>("round");
        case UMBA_TOKENIZER_TOKEN_ROUND_BRACKET_CLOSE                         : return umba::string_plus::make_string<StringType>("round");
        case UMBA_TOKENIZER_TOKEN_ANGLE_BRACKET_OPEN                          : return umba::string_plus::make_string<StringType>("angle");
        case UMBA_TOKENIZER_TOKEN_ANGLE_BRACKET_CLOSE                         : return umba::string_plus::make_string<StringType>("angle");
        case UMBA_TOKENIZER_TOKEN_SQUARE_BRACKET_OPEN                         : return umba::string_plus::make_string<StringType>("square");
        case UMBA_TOKENIZER_TOKEN_SQUARE_BRACKET_CLOSE                        : return umba::string_plus::make_string<StringType>("square");
        case UMBA_TOKENIZER_TOKEN_DBLSQUARE_BRACKET_OPEN                      : return umba::string_plus::make_string<StringType>("dblsquare");
        case UMBA_TOKENIZER_TOKEN_DBLSQUARE_BRACKET_CLOSE                     : return umba::string_plus::make_string<StringType>("dblsquare");
        case UMBA_TOKENIZER_TOKEN_OPERATOR_MULTI_LINE_COMMENT_START           : return umba::string_plus::make_string<StringType>("cmnt");
        case UMBA_TOKENIZER_TOKEN_OPERATOR_MULTI_LINE_COMMENT_END             : return umba::string_plus::make_string<StringType>("cmnt");
        case UMBA_TOKENIZER_TOKEN_CTRL_CC_PP_START                            : return umba::string_plus::make_string<StringType>("pp");
        case UMBA_TOKENIZER_TOKEN_CTRL_CC_PP_END                              : return umba::string_plus::make_string<StringType>("pp");

        case UMBA_TOKENIZER_TOKEN_RAW_DATA                                    : return umba::string_plus::make_string<StringType>("raw");


        // case : return umba::string_plus::make_string<StringType>("");
        // case : return umba::string_plus::make_string<StringType>("");
        // case : return umba::string_plus::make_string<StringType>("");
        // case : return umba::string_plus::make_string<StringType>("");
        // case : return umba::string_plus::make_string<StringType>("");
        // case : return umba::string_plus::make_string<StringType>("");




        //case : return umba::string_plus::make_string<StringType>("");
        default:

            if (p>=UMBA_TOKENIZER_TOKEN_NUMBER_LITERAL_FIRST && p<=UMBA_TOKENIZER_TOKEN_NUMBER_LITERAL_LAST)
            {
                if (p&UMBA_TOKENIZER_TOKEN_FLOAT_FLAG)
                    return umba::string_plus::make_string<StringType>("num");
                else
                    return umba::string_plus::make_string<StringType>("num");
            }

            if (p>=UMBA_TOKENIZER_TOKEN_OPERATOR_SINGLE_LINE_COMMENT_FIRST && p<=UMBA_TOKENIZER_TOKEN_OPERATOR_SINGLE_LINE_COMMENT_LAST)
                return umba::string_plus::make_string<StringType>("cmnt");

            //if (p>=UMBA_TOKENIZER_TOKEN_NUMBER_USER_LITERAL_FIRST && p<=UMBA_TOKENIZER_TOKEN_NUMBER_USER_LITERAL_LAST)
            if (p>=UMBA_TOKENIZER_TOKEN_NUMBER_LITERAL_FIRST && p<=UMBA_TOKENIZER_TOKEN_NUMBER_LITERAL_LAST)
                return umba::string_plus::make_string<StringType>("num");

            if (p>=UMBA_TOKENIZER_TOKEN_OPERATOR_FIRST && p<=UMBA_TOKENIZER_TOKEN_OPERATOR_LAST)
                return umba::string_plus::make_string<StringType>("op");

            if (p>=UMBA_TOKENIZER_TOKEN_STRING_LITERAL_FIRST && p<=UMBA_TOKENIZER_TOKEN_STRING_LITERAL_LAST)
                return umba::string_plus::make_string<StringType>("str");

            if (p>=UMBA_TOKENIZER_TOKEN_KEYWORD_SET1_FIRST && p<=UMBA_TOKENIZER_TOKEN_KEYWORD_SET1_LAST)
                return umba::string_plus::make_string<StringType>("kwd1");

            if (p>=UMBA_TOKENIZER_TOKEN_KEYWORD_SET2_FIRST && p<=UMBA_TOKENIZER_TOKEN_KEYWORD_SET2_LAST)
                return umba::string_plus::make_string<StringType>("kwd2");

            if (p>=UMBA_TOKENIZER_TOKEN_KEYWORD_SET3_FIRST && p<=UMBA_TOKENIZER_TOKEN_KEYWORD_SET3_LAST)
                return umba::string_plus::make_string<StringType>("kwd3");

            if (p>=UMBA_TOKENIZER_TOKEN_KEYWORD_SET4_FIRST && p<=UMBA_TOKENIZER_TOKEN_KEYWORD_SET4_LAST)
                return umba::string_plus::make_string<StringType>("kwd4");

            if (p>=UMBA_TOKENIZER_TOKEN_KEYWORD_SET5_FIRST && p<=UMBA_TOKENIZER_TOKEN_KEYWORD_SET5_LAST)
                return umba::string_plus::make_string<StringType>("kwd5");

            if (p>=UMBA_TOKENIZER_TOKEN_KEYWORD_SET6_FIRST && p<=UMBA_TOKENIZER_TOKEN_KEYWORD_SET6_LAST)
                return umba::string_plus::make_string<StringType>("kwd6");

            if (p>=UMBA_TOKENIZER_TOKEN_KEYWORD_SET7_FIRST && p<=UMBA_TOKENIZER_TOKEN_KEYWORD_SET7_LAST)
                return umba::string_plus::make_string<StringType>("kwd7");

            if (p>=UMBA_TOKENIZER_TOKEN_KEYWORD_SET8_FIRST && p<=UMBA_TOKENIZER_TOKEN_KEYWORD_SET8_LAST)
                return umba::string_plus::make_string<StringType>("kwd8");

            return umba::string_plus::make_string<StringType>("");
    }
}

//----------------------------------------------------------------------------
template<typename StreamType, typename MsgType>
void printError(StreamType &ss, const std::string &inputFilename, umba::tokenizer::payload_type tokenType, umba::iterator::TextPositionCountingIterator<char> it, umba::iterator::TextPositionCountingIterator<char> itEnd, MsgType msg)
{
     UMBA_USED(tokenType);

     ss << "!!! ";

     if (it==itEnd)
     {
         ss << inputFilename << ": Unexpected end of file\n";
         ss << "TokenType: " << tokenType << "\n";
         return;
     }

     if (msg.empty())
     {
         msg = "Unexpected symbol";
     }

     auto errPos = it.getPosition(true); // с поиском конца строки (а вообще не надо пока, но пусть)
     std::string erroneousLineText = umba::iterator::makeString(it.getLineStartIterator(), it.getLineEndIterator());
     ss << inputFilename << ":" << errPos.toString<std::string>() << ": " << msg << "\n";
     ss << "Line: " << erroneousLineText << "\n";
     auto errMarkerStr = std::string(erroneousLineText.size(), ' ');
     if (errPos.symbolOffset>=errMarkerStr.size())
         errMarkerStr.append(1,'^');
     else
         errMarkerStr[errPos.symbolOffset] = '^';
     ss << "     |" << errMarkerStr << "|\n";

     if (it!=itEnd)
     {
         // char ch = *it;
         // ss << "ch: " << umba::escapeStringC(std::string(1,ch)) << "\n";
     }
}

//----------------------------------------------------------------------------
template<typename StreamType>
void printError(StreamType &ss, const std::string &inputFilename, umba::tokenizer::payload_type tokenType, umba::iterator::TextPositionCountingIterator<char> it, umba::iterator::TextPositionCountingIterator<char> itEnd)
{
    printError(ss, inputFilename, tokenType, it, itEnd, std::string());
}

//----------------------------------------------------------------------------
template<typename StreamType, typename MsgType>
void printError(StreamType &ss, const std::string &inputFilename, umba::tokenizer::payload_type tokenType, umba::iterator::TextPositionCountingIterator<char> it, umba::iterator::TextPositionCountingIterator<char> itEnd, const char* srcFile, int srcLine, const MsgType &msg)
{
    printError(ss, inputFilename, tokenType, it, itEnd, msg);
    ss << "At " << srcFile << ":" << srcLine << "\n";
}

//----------------------------------------------------------------------------
template<typename StreamType>
void printError(StreamType &ss, const std::string &inputFilename, umba::tokenizer::payload_type tokenType, umba::iterator::TextPositionCountingIterator<char> it, umba::iterator::TextPositionCountingIterator<char> itEnd, const char* srcFile, int srcLine)
{
    printError(ss, inputFilename, tokenType, it, itEnd);
    ss << "At " << srcFile << ":" << srcLine << "\n";
}

//----------------------------------------------------------------------------
inline
auto makeTokenText(umba::tokenizer::payload_type tokenType, umba::iterator::TextPositionCountingIterator<char> b, umba::iterator::TextPositionCountingIterator<char> e)
{
    if (tokenType==UMBA_TOKENIZER_TOKEN_LINEFEED)
    {
        return std::string("\n");
    }

    if (tokenType==UMBA_TOKENIZER_TOKEN_LINE_CONTINUATION)
    {
        return std::string("\\\n");
    }

    if (tokenType==UMBA_TOKENIZER_TOKEN_OPERATOR_MULTI_LINE_COMMENT_START)
    {
        return marty_cpp::normalizeCrLfToLf(umba::iterator::makeString(b, e));
    }

    return umba::iterator::makeString(b, e);
}

//----------------------------------------------------------------------------
template<typename StreamType, typename InputIteratorType>
StreamType& printTokenImpl(StreamType &ss, umba::tokenizer::payload_type tokenType, InputIteratorType b, InputIteratorType e)
{
    auto kindStr   = getTokenNameString<std::string>(tokenType);
    auto tokenText = makeTokenText(tokenType, b, e);

    ss << tokenText;
    #if defined(DUPLICATE_TO_STD_OUT)
    std::cout << tokenText;
    #endif

    if (!kindStr.empty())
    {
        ss << " - " << kindStr;
        #if defined(DUPLICATE_TO_STD_OUT)
        std::cout << " - " << kindStr;
        #endif
    }

    return ss;
}

//----------------------------------------------------------------------------
template<typename StreamType, typename InputIteratorType>
StreamType& printToken(StreamType &ss, umba::tokenizer::payload_type tokenType, InputIteratorType b, InputIteratorType e)
{
    #if defined(DUPLICATE_TO_STD_OUT)
    std::cout << "\n";
    #endif
    return printTokenImpl(ss, tokenType, b, e) << "\n";
}

//----------------------------------------------------------------------------
template<typename StreamType, typename InputIteratorType>
StreamType& printToken(StreamType &ss, umba::tokenizer::payload_type tokenType, InputIteratorType b, InputIteratorType e, bool lineStart)
{
    printTokenImpl(ss, tokenType, b, e);
    if (tokenType==UMBA_TOKENIZER_TOKEN_OPERATOR_MULTI_LINE_COMMENT)
    {
        ss << " - lineStart: " << (lineStart?"true":"false");
        #if defined(DUPLICATE_TO_STD_OUT)
        std::cout << " - lineStart: " << (lineStart?"true":"false");
        #endif
    }

    ss << "\n";
    #if defined(DUPLICATE_TO_STD_OUT)
    std::cout << "\n";
    #endif

    return ss;
}

//----------------------------------------------------------------------------









