/*! \file
    \brief Извлечение brief info
 */

#pragma once

#include "signature.h"
#include "commentUtils.h"
//
// #include "umba/regex_helpers.h"
#include "umba/string_plus.h"
//
#include "umba/tokenizer.h"
//
#include "umba/tokenizer/lang/cpp.h"



struct BriefInfo
{
    std::string    infoText;
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

#if 0
#ifdef LOG_REGEX_MATCH

inline
bool findEntryPoint( std::vector<char> fileText, const std::map< std::string,std::set<std::string> > &entryNames )
{
    makeSingleLineText(fileText.begin(), fileText.end());

    #if defined(__GNUC__)

        //TODO: !!! Если не обрезать, то регулярки в GCC 7.3 падают
        // Переехало в umba. Или нет?
        if (fileText.size()>7000u)
        {
            fileText.erase(fileText.begin()+7000u, fileText.end());
        }

    #endif

#else

inline
bool findEntryPoint( const std::vector<char> &fileText, const std::map< std::string,std::set<std::string> > &entryNames )
{

#endif


    for(const auto& [name, args] : entryNames)
    {
        auto stringifier = [](const std::string &s) { return s; };
        auto argsAlters = umba::string_plus::merge< std::string, decltype(args.begin()), decltype(stringifier) >( args.begin(), args.end(), '|', stringifier );
        // if (argsAlters.empty())
        //     argsAlters = "int";

        if (!argsAlters.empty())
            argsAlters = std::string("(") + argsAlters + std::string(")") + std::string("[\\s]+?");

        std::string regexStr = std::string(".*")
                             + argsAlters
                             + name
                             + std::string("\\s*\\(.*")
                             ;

        #ifdef LOG_REGEX_MATCH

            std::string text = std::string(fileText.begin(), fileText.end());

            //std::string testRegex = ".*(void|int) \\s*(main|wmain|tmain__tmain)\\s*\\(.*";
            //std::string testRegex = "(void|int) \\s*(main|wmain|tmain__tmain)\\s*\\(";
            std::string testRegex = ".*main.*";

            std::cout << "test : [" << testRegex << "]\n";
            std::cout << "regex: [" << regexStr  << "]\n";

        #endif

        try
        {
            #ifdef LOG_REGEX_MATCH

                bool testRes = umba::regex_helpers::regexMatch(fileText, std::regex(testRegex, std::regex::ECMAScript));
                std::cout << "test res: " << testRes << "\n";

            #endif

            //std::regex re(regexStr, std::regex::ECMAScript  /* | std::regex::multiline */ ); // multiline not supported in MSVC2019
            if (umba::regex_helpers::regexMatch(fileText, regexStr))
                return true;
        }
        catch(...)
        {
            // ignore regex errors
        }
    }

    return false;


    //   std::string regexStrMain = "(void|int) \\s*(main|wmain|tmain__tmain)\\s*\\(";
    // //std::string regexStrMain = ".*(void|int) \\s*(main|wmain|tmain__tmain)\\s*\\(.*";
    //
    // // https://en.cppreference.com/w/cpp/regex/syntax_option_type
    // std::regex re(regexStrMain, std::regex::ECMAScript  /* | std::regex::multiline */ ); // multiline not supported in MSVC2019
    //
    // return umba::regex_helpers::regexMatch(fileText, re);

    // std::string testText = "int";
    // return umba::regex_helpers::regexMatch(testText, re);

}
#endif


inline
const auto& getTokenizerBuilder()
{
    static auto tokenizerBuilder = umba::tokenizer::makeTokenizerBuilderCpp<char>();
    return tokenizerBuilder;
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

    // if (tokenType==UMBA_TOKENIZER_TOKEN_OPERATOR_MULTI_LINE_COMMENT_START)
    // {
    //     return marty_cpp::normalizeCrLfToLf(umba::iterator::makeString(b, e));
    // }
    //
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
    using token_parsed_data    = typename tokenizer_type::token_parsed_data;
    using payload_type         = umba::tokenizer::payload_type;

    //bool inPreprocessor = false;
    std::string fileTextNoComments; // Потом будет использоваться для поиска Entry Point

    //makeTokenText(tokenType, b, e);

    //info.briefFound = false;

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

    //std::string
    fileTextNoComments = marty_cpp::normalizeCrLfToLf(fileTextNoComments, 0);
    std::vector<std::string> fileTextLines = marty_cpp::splitToLinesSimple(fileTextNoComments, false);

    info.entryPoint = findTextSignatureInLines(fileTextLines, entrySignatures);

    // fileTextNoComments

    //info.entryPoint = findEntryPoint(fileText, entryNames);

    return info.briefFound;


#if 0
    makeSingleLineText(fileText.begin(), fileText.end());

    #if defined(__GNUC__)

        // Переехало в umba. Или нет?
        if (fileText.size()>7000u)
        {
            fileText.erase(fileText.begin()+7000u, fileText.end());
        }

    #endif


    info.entryPoint = findEntryPoint(fileText, entryNames);
    info.briefFound = false;

    // https://en.cppreference.com/w/cpp/regex/ecmascript
    // https://developer.mozilla.org/ru/docs/Web/JavaScript/Guide/Regular_Expressions
    //std::string regexStr = ".*/\\*.*";
    //std::string regexStr = ".*/\\*.*\\\\file.*";
    //std::string regexStr = ".*/\\*.*\\\\file.*\\\\brief\\s.*";
    //std::string regexStr = R"!!!(.*/\*.*\\file.*\\brief\s.*)!!!";
    //std::string regexStr = R"!!!(.*/\*(!|\*).*\\file.*\\brief\s(.*?)\*/.*)!!!";
    //std::string regexStr = R"!!!(.*/\*(!|\*).*\\file.*\\brief\s(.*?)(?=\*/).*)!!!";
    //std::string regexStr = R"!!!(.*/\*(!|\*).*\\file.*\\brief\s(.*)(?=\*/).*)!!!";
    //std::string regexStr = R"!!!(.*/\*(!|\*).*\\file.*\\brief\s(.*)(?=\*/))!!!";
    //std::string regexStr = R"!!!(/\*(!|\*).*\\file.*\\brief\s(.*)\*/)!!!";
    //std::string regexStr = R"!!!(/\*(?:!|\*).*\\file.*\\brief\s(.*)(?=\*/))!!!";
    // std::string regexStr = R"!!!(/\*(?:!|\*).*\\file.*\\brief\s(.*)\*/(.*))!!!";
    //std::string regexStr = R"!!!(/\*(?:!|\*).*\\file.*\\brief\s(.*)\*/)!!!";

    //std::string regexStr = R"!!!(/\*(?:!|\*)[\s\S]*?\\file[\s\S]*?\\brief\s([\s\S]*?)\*/)!!!";
    std::string regexStr = R"!!!(/\*(?:!|\*)[\s\S]*?(?:\\|@)file[\s\S]*?(?:\\|@)brief\s([\s\S]*?)\*/)!!!";

    // [\s\S]*?\*\/

    std::regex re(regexStr, std::regex::ECMAScript  /* | std::regex::multiline */ ); // multiline not supported in MSVC2019
    //info.briefFound = umba::regex_helpers::regexMatch(fileText, re);

    std::string text = std::string(fileText.begin(), fileText.end());
    std::smatch m;

    try
    {
        std::regex_search(text, m, re);
    }
    catch(const std::exception &e)
    {
        std::cerr << "regex_search exception: " << e.what() << "\n";
    }

    #ifdef LOG_REGEX_MATCH
        if (!m.empty())
        {
            std::cout << "regex=[" << regexStr << "]\n"
                         "prefix=[" << m.prefix() << "]\nsmatch:\n";
            for(std::size_t n = 0; n < m.size(); ++n)
                std::cout << "\nm[" << n << "]=[" << m[n] << "] ";

            std::cout << "\n\nsuffix=[" << m.suffix() << "]\n";

        }
        else
        {
            std::cout << "regex=[" << regexStr << "]: NO MATCH\n";
        }
    #endif

    if (m.size()>1)
    {
        info.briefFound = true;
        info.infoText   = m[1]; // m[0] - содержит всю строку, соответствующую выражению, m[1] - первому захвату, и тп

        // std::cout << "Matches found: " << m.size() << "\n";
        // for(std::size_t mI=0; mI!=m.size(); ++mI)
        // {
        //     std::cout << "  match[" << mI << "]: {" << m[mI] << "}\n";
        // }

        // Костыль - regexp получился кривоватый, не силён я в них, и он захватывает что-то лишнее.
        // Обычно это что-то из продолжения описания, которое отделено двумя (или даже больше) переводами строки.
        // Так как я заменяю \r\n на пробелы, то это 4 пробела (на линупсе по идее будет два пробела),
        // Но все результаты с багами отделены как минимум 8ю пробелами по факту - на линупсе будет 4.
        // Так что ищем 4 пробела

        auto pos = info.infoText.find("    ");
        if (pos!=info.infoText.npos)
            info.infoText.erase(pos);

        umba::string_plus::trim(info.infoText);
    }

    return info.briefFound;
#endif

}
