/*! \file
    \brief Поиск сигнатур, в т.ч. многострочных. Вытащил из MD PP, там они маркерами сниппетов работают

 */
#pragma once

#include "marty_cpp/marty_cpp.h"
//

#include <deque>
#include <string>



//----------------------------------------------------------------------------
#include "umba/warnings/push_disable_spectre_mitigation.h"
inline
std::string normalizeSignatureLine(const std::string &str)
{
    std::string res; res.reserve();

    for(auto ch : str)
    {
        std::uint8_t uch = (std::uint8_t)ch;
        if (uch>' ' && uch!=0x7Fu)
            res.append(1, ch);
    }

    return res;
}
#include "umba/warnings/pop.h"

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct TextSignature
{
    // using options_type  = umba::container::small_vector_options< umba::container::growth_factor<umba::container::growth_factor_50>, umba::container::inplace_alignment<16> >::type;
    // //using signature_lines_vector_type = umba::container::small_vector<std::string, 4, void, umba::container::small_vector_option_inplace_alignment_16_t, umba::container::small_vector_option_growth_50_t >;
    // using signature_lines_vector_type = umba::container::small_vector<std::string, 4, void, options_type >;
    //
    //
    // signature_lines_vector_type    signatureLinesVector; // normalized or original? Чтобы при отладке знать, какой был оригинал, и заодно имеем тут число строк в искомой сигнатуре
    std::string                    normalizedSignature ;

    TextSignature() = default;
    TextSignature(const TextSignature &) = default;
    TextSignature& operator=(const TextSignature &) = default;
    TextSignature(TextSignature &&) = default;
    TextSignature& operator=(TextSignature &&) = default;

    explicit TextSignature(const std::string &signature)
        // : signatureLinesVector()
        // , normalizedSignature()
        : normalizedSignature(normalizeSignatureLine(marty_cpp::cUnescapeString(signature)))
    {
        // auto unescaped      = marty_cpp::cUnescapeString(signature);
        // normalizedSignature = normalizeSignatureLine(unescaped);
        // umba::string_plus::simple_string_split(std::back_inserter(signatureLinesVector), unescaped, std::string("\n") /* , nSplits = -1 */ );
    }

    template<typename IteratorType>
    explicit TextSignature(IteratorType b, IteratorType e)
        // : signatureLinesVector()
        // , normalizedSignature()
        : normalizedSignature(normalizeSignatureLine(marty_cpp::cUnescapeString(std::string(b,e))))
    {
        // auto unescaped      = marty_cpp::cUnescapeString(std::string(b,e));
        // normalizedSignature = normalizeSignatureLine(unescaped);
        // umba::string_plus::simple_string_split(std::back_inserter(signatureLinesVector), unescaped, std::string("\n") /* , nSplits = -1 */ );
    }

    void clear()
    {
        // signatureLinesVector.clear();
        normalizedSignature .clear();
    }

}; // struct TextSignature

//----------------------------------------------------------------------------
//! Возвращает номер первой строки сигнатуры в тексте, или (std::size_t)-1
/*!
    Итак.
    1) Очередь пуста. Тупо кладём элемент
    2) Сигнатурная строка, сформированная из очереди, короче искомой - значит, она не может начинаться с искомой - тупо добавляем туда очередную строку (п.1 является частным случаем п.2)
    3) Сигнатурная строка (СС), сформированная из очереди, равна по длине, или длиннее
       Пока текущая СС больше искомой:
         проверяем, начинается ли она с искомой
           Если начинается, то количество элементов в очереди надо вернуть вместе с результатом.
           Если не начинается, то удаляем элемент с начала очереди


 */
// #define UMBA_MD_FIND_TEXT_SIGNATURE_IN_LINES_OLD_VERSION


//----------------------------------------------------------------------------
#include "umba/warnings/push_disable_spectre_mitigation.h"
inline
bool findTextSignatureInLines(const std::vector<std::string> &lines, const TextSignature &ts )
                                    // , std::size_t &foundSignatureNumLines, std::size_t startLine /* =(std::size_t)-1 */ )
{
    std::size_t startLine = 0;

    if (startLine==(std::size_t)-1)
        startLine = 0;

    if (startLine>=lines.size())
        return false;
        //return (std::size_t)-1;

    if (ts.normalizedSignature.empty()) // Пустые сигнатуры не ищем
        return false;
        //return (std::size_t)-1;

    std::size_t curLineIdx = startLine;

    std::deque<std::string>  curTestLinesQue;
    auto makeSingleTestLineFromDeque = [&]()
    {
        std::string res;
        for(const auto &l: curTestLinesQue)
            res.append(l);
        return res;
    };

    // curLineIdx указывает на строку, следующую за нашей последней сигнатурной
    for(; curLineIdx!=lines.size(); ++curLineIdx)
    {
        std::string curSignature = makeSingleTestLineFromDeque();
        // if (curSignature.size()<ts.normalizedSignature.size())
        // {
        //     curTestLinesQue.emplace_back(normalizeSignatureLine(lines[curLineIdx]));
        //     continue;
        // }

        while(curSignature.size()>=ts.normalizedSignature.size())
        {
            if (umba::string_plus::starts_with(curSignature, ts.normalizedSignature))
            {
                //foundSignatureNumLines = curTestLinesQue.size();
                return true; // curLineIdx - curTestLinesQue.size();
            }

            curTestLinesQue.pop_front();
            curSignature = makeSingleTestLineFromDeque();
        }

        curTestLinesQue.emplace_back(normalizeSignatureLine(lines[curLineIdx]));
    }

    std::string curSignature = makeSingleTestLineFromDeque();
    if (umba::string_plus::starts_with(curSignature, ts.normalizedSignature))
    {
        // foundSignatureNumLines = curTestLinesQue.size();
        return true; // curLineIdx - curTestLinesQue.size();
    }

    return false; // (std::size_t)-1;
}
#include "umba/warnings/pop.h"

//----------------------------------------------------------------------------
inline
bool findTextSignatureInLines( const std::vector<std::string> &lines, const std::vector<TextSignature> &tsv )
                              // , std::size_t &foundSignatureNumLines, std::size_t startLine /* =(std::size_t)-1 */ )
{
    for(const auto &ts : tsv)
    {
        if (findTextSignatureInLines(lines, ts))
            return true;
    }

    return false;
}

//----------------------------------------------------------------------------



