#pragma once

#include "regex_helpers.h"

#include "umba/string_plus.h"


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

inline 
bool findEntryPoint( const std::vector<char> &fileText)
{
    

    //std::string regexStrMain = ".*(void|int).*";
    std::string regexStrMain = ".*(void|int) \\s*(main|wmain|tmain__tmain)\\s*\\(.*";

    // https://en.cppreference.com/w/cpp/regex/syntax_option_type
    std::regex re(regexStrMain, std::regex::ECMAScript  /* | std::regex::multiline */ ); // multiline not supported in MSVC2019

    return umba::regex_helpers::regexMatch(fileText, re);

    // std::string testText = "int";
    // return umba::regex_helpers::regexMatch(testText, re);

}

inline
bool findBriefInfo( std::vector<char> fileText, BriefInfo &info )
{
    makeSingleLineText(fileText.begin(), fileText.end());
    info.entryPoint = findEntryPoint(fileText);
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
    std::string regexStr = R"!!!(/\*(?:!|\*)[\s\S]*?\\file[\s\S]*?\\brief\s([\s\S]*?)\*/)!!!";

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
}
