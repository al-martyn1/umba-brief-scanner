/*! \file
    \brief Утилитки для разбора коментариев
 */

#include "marty_cpp/marty_cpp.h"
//
#include "marty_cpp/src_normalization.h"

//
#include <utility>


inline
std::string makeBriefSingleString(const std::string &str)
{
    std::vector<std::string> briefLines = marty_cpp::splitToLinesSimple(str, false /* addEmptyLineAfterLastLf */);

    std::string res; res.reserve(str.size());
    for(auto line: briefLines)
    {
        line = marty_cpp::simple_trim(line, [&](char ch) { return ch==' '; } );
        if (line.empty())
            continue;

        if (!res.empty())
            res.append(1, ' ');

        res.append(line);
    }

    return res;
}

inline
bool isSingleCharLine(const std::string &line, std::size_t minChars = 3)
{
    for(auto ch : line)
    {
        if (ch!=line[0])
            return false;
    }

    return line.size()>= minChars;
}

/**
 * A brief history of JavaDoc-style (C-style) comments.
 *
 * This is the typical JavaDoc-style C-style comment. It starts with two
 * asterisks.
 *
 * @brief Функция производит подготовку текста многострочного коментария для последующей обработки, разбаннеривает баннеры
 *
 */

inline
std::string prepareMultilineCommentBannerText(std::string commentText)
{
    //commentText = marty_cpp::normalizeCrLfToLf(commentText.begin(), commentText.end());
    commentText = marty_cpp::normalizeCrLfToLf(commentText);

    std::vector<std::string> commentLines = marty_cpp::splitToLinesSimple(commentText, false /* addEmptyLineAfterLastLf */);

    {
        std::vector<std::string> tmp; tmp.reserve(commentLines.size());
        for(const auto &line: commentLines)
        {
            auto lineTrimmed = marty_cpp::simple_trim(line, [&](char ch) { return ch==' '; } );

            if (tmp.empty() && lineTrimmed.empty())
                continue; // Пропускаем пустые строки в начале

            if (isSingleCharLine(lineTrimmed))
                continue;
            tmp.emplace_back(line);
        }

        tmp.swap(commentLines);
    }


    char firstCh = 0;
    char lastCh  = 0;
    bool sameFirstCh = true;
    bool sameLastCh  = true;

    for(auto line : commentLines)
    {
        line = marty_cpp::simple_trim(line, [&](char ch) { return ch==' '; } );
        if (line.empty())
        {
            sameFirstCh = false;
            sameLastCh  = false;
            //return marty_cpp::mergeLines(commentLines, marty_cpp::ELinefeedType::lf);
        }
        else if (firstCh==0)
        {
            firstCh = line.front();
            lastCh  = line.back();
        }
        else
        {
            if (firstCh!=line.front())
                sameFirstCh = false;

            if (line.size()<2)
                sameLastCh = false;
            else if (lastCh!=line.back())
                sameLastCh = false;
        }

        if (!sameFirstCh && !sameLastCh) // и первые символы строк, и последние - различаются, это не баннер
        {
            break;
        }
    }

    // Тут может быть что-то ещё
    if (!sameFirstCh && !sameLastCh) // и первые символы строк, и последние - различаются, это не баннер
    {
        return marty_cpp::mergeLines(commentLines, marty_cpp::ELinefeedType::lf);
    }

    for(auto &line : commentLines)
    {
        if (sameFirstCh)
        {
            line = marty_cpp::simple_ltrim(line, [&](char ch) { return ch==' '; } );
            if (!line.empty())
            {
                line.erase(0, 1);
            }
        }

        if (sameLastCh)
        {
            line = marty_cpp::simple_rtrim(line, [&](char ch) { return ch==' '; } );
            if (!line.empty())
            {
                line.erase(line.size()-1, 1);
            }
        }

    }

    return marty_cpp::mergeLines(commentLines, marty_cpp::ELinefeedType::lf);
}


