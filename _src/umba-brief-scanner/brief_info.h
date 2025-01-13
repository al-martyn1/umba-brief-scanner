/*! \file
    \brief Реализация извлечения brief info из файлов сорцов
 */

#pragma once

#include "signature.h"
#include "commentUtils.h"
//
#include "umba/string_plus.h"
//
#include "umba/tokenizer.h"
//
#include "umba/tokenizer/lang/cpp.h"

#include "cpp-tokenizer.h"


struct BriefInfo
{
    std::string    infoText;
    std::string    fileName;
    std::string    encodingName;
    bool           entryPoint   = false;

    bool           briefFound   = false;
};



template<typename IterType>
void makeSingleLineText( IterType b, IterType e )
{
    for(; b!=e; ++b)
    {
        if (*b=='\r' || *b=='\n')
            *b = ' ';
    }
}



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

    return umba::iterator::makeString(b, e);
}



inline
bool findBriefInfo( std::string fileText, const std::vector<TextSignature> &entrySignatures, BriefInfo &info)
{
    auto tokenizerBuilder = getTokenizerBuilder();

    using TokenizerBuilderType = decltype(tokenizerBuilder);

    //using tokenizer_type      = std::decay<decltype(tokenizer)>;
    using tokenizer_type       = typename TokenizerBuilderType::tokenizer_type;
    using InputIteratorType    = typename tokenizer_type::iterator_type;
    using tokenizer_char_type  = typename tokenizer_type::value_type;
    using messages_string_type = typename tokenizer_type::messages_string_type;
    using token_parsed_data    = typename tokenizer_type::token_parsed_data_type;
    using payload_type         = umba::tokenizer::payload_type;

    std::string fileTextNoComments; // Потом будет использоваться для поиска Entry Point


    auto tokenHandler =     [&]( auto &tokenizer
                               , bool bLineStart, payload_type tokenType
                               , InputIteratorType b, InputIteratorType e
                               , token_parsed_data parsedData // std::basic_string_view<tokenizer_char_type> parsedData
                               , messages_string_type &errMsg
                               ) -> bool
                               {

                                UMBA_USED(tokenizer);
                                UMBA_USED(bLineStart);
                                UMBA_USED(errMsg);

                                // UMBA_TOKENIZER_TOKEN_CTRL_FIN, UMBA_TOKENIZER_TOKEN_CTRL_CC_PP_*
                                if (tokenType&UMBA_TOKENIZER_TOKEN_CTRL_FLAG)
                                {
                                    return true; // Управляющий токен, не надо выводить, никакой нагрузки при нём нет
                                }

                                if (tokenType>=UMBA_TOKENIZER_TOKEN_OPERATOR_SINGLE_LINE_COMMENT_FIRST && tokenType<=UMBA_TOKENIZER_TOKEN_OPERATOR_SINGLE_LINE_COMMENT_LAST)
                                {
                                    // fileTextNoComments.append(1, '\n'); // Вместо коментария выводим окончание строки
                                    // Или однострочный коментарий не включает в себя перевод строки, и его можно просто игнорировать?
                                    return true;
                                }
                                else if (tokenType==UMBA_TOKENIZER_TOKEN_OPERATOR_MULTI_LINE_COMMENT)
                                {
                                    auto commentData = std::get<typename tokenizer_type::CommentData>(parsedData);
                                    auto commentStr = std::string(commentData.data);

                                    if (commentStr.empty())
                                        return true; // Просто пропускаем пустые коментарии

                                    // https://www.doxygen.nl/manual/docblocks.html#cppblock

                                    if ( commentStr[0]!='*' // JavaDoc comment style
                                      && commentStr[0]!='!' // Qt comment style
                                        )
                                    {
                                        return true; // Просто пропускаем обычные коментарии
                                    }

                                    commentStr.erase(0, 1);
                                    commentStr = prepareMultilineCommentBannerText(commentStr);

                                    auto fileDirectivePos = commentStr.find("@file", 0);
                                    if (fileDirectivePos==commentStr.npos)
                                        fileDirectivePos = commentStr.find("\\file", 0);
                                    if (fileDirectivePos==commentStr.npos)
                                        return true; // Просто пропускаем - директива @file не найдена

                                    fileDirectivePos += 5;
                                    auto fileDirectiveEndPos = commentStr.find("\n", fileDirectivePos);
                                    if (fileDirectiveEndPos==commentStr.npos)
                                        return true; // Просто пропускаем - директива @file найдена, но конца её нет, и brief'а тут не будет

                                    std::string fileName = marty_cpp::simple_trim(std::string(commentStr, fileDirectivePos, fileDirectiveEndPos-fileDirectivePos), [&](char ch) { return ch==' '; } );
                                    ++fileDirectiveEndPos;

                                    auto briefDirectivePos = commentStr.find("@brief", fileDirectiveEndPos);
                                    if (briefDirectivePos==commentStr.npos)
                                        briefDirectivePos = commentStr.find("\\brief", fileDirectiveEndPos);
                                    if (briefDirectivePos==commentStr.npos)
                                        return true; // Просто пропускаем - директива @brief не найдена

                                    briefDirectivePos += 6;
                                    std::string briefText;
                                    auto briefDirectiveEndPos = commentStr.find("\n\n", briefDirectivePos);
                                    if (briefDirectiveEndPos==commentStr.npos)
                                        briefText = marty_cpp::simple_trim(std::string(commentStr, briefDirectivePos, commentStr.size()-briefDirectivePos), [&](char ch) { return ch==' '; } );
                                    else
                                        briefText = marty_cpp::simple_trim(std::string(commentStr, briefDirectivePos, briefDirectiveEndPos-briefDirectivePos), [&](char ch) { return ch==' '; } );

                                    info.briefFound = true;
                                    info.infoText   = makeBriefSingleString(briefText);

                                    return true;
                                }

                                fileTextNoComments += makeTokenText(tokenType, b, e);
                                return true;
                            };


    auto tokenizer = umba::tokenizer::makeTokenizerCpp( tokenizerBuilder
                                                      , tokenHandler
                                                      );

    bool bOk = true;
    auto itBegin = InputIteratorType(&fileText[0], fileText.size());
    auto itEnd   = InputIteratorType();
    tokenizer.tokenizeInit();
    InputIteratorType it = itBegin;
    for(; it!=itEnd && bOk; ++it)
    {
        if (!tokenizer.tokenize(it, itEnd))
        {
            bOk = false;
        }
    }

    if (bOk)
    {
        bOk = tokenizer.tokenizeFinalize(itEnd);
    }

    fileTextNoComments = marty_cpp::normalizeCrLfToLf(fileTextNoComments, 0);
    std::vector<std::string> fileTextLines = marty_cpp::splitToLinesSimple(fileTextNoComments, false);

    info.entryPoint = findTextSignatureInLines(fileTextLines, entrySignatures);

    return info.briefFound;

}

