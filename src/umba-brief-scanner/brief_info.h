#pragma once

#include "umba/regex_helpers.h"

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

inline
bool findBriefInfo( std::vector<char> fileText, const std::map< std::string,std::set<std::string> > &entryNames, BriefInfo &info)
{
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
}
