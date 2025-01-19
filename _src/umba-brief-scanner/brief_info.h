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

#include "utils.h"

#include "notes.h"


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
bool isSingleCharText(const std::string &str)
{
    if (str.empty())
        return false;

    auto first = str[0];
    for(auto ch: str)
    {
        if (ch!=first)
            return false;
    }

    return true;
}

inline
bool findBriefInfo( std::string fileText, const std::vector<TextSignature> &entrySignatures, BriefInfo &info
                  , const NotesConfig &notesConfig
                  , std::vector<NoteInfo> &notes
                  , const std::string & filename
                  , const std::string & fileFolder
                  )
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

    std::size_t lineFeedAfterSingleLineCommentCount = 0;

    // auto isLastNoteEmpty = [&]()
    // {
    //     return !notes.empty() && notes.back().empty();
    // };

    auto notePushOrReplaceLastEmpty = [&](const NoteInfo &note, bool appendEmptyNote)
    {
        // if (isLastNoteEmpty())
        if (!notes.empty() && notes.back().empty()) // вектор не пуст, но последний элемент - пуст
            notes.back() = note;
        else
            notes.emplace_back(note);

        if (appendEmptyNote)
            notes.emplace_back();
    };

    auto checkPushNoteFromMultilineComment = [&](const std::string &text, InputIteratorType b)
    {
        NoteInfo note;
        if (!parseTextNote(text, notesConfig, note))
            return;

        note.line = b.getPosition().lineNumber;
        note.file             = filename  ;
        note.rootSearchFolder = fileFolder;
        notePushOrReplaceLastEmpty(note, true);
    };

    // auto noteAppendOr = [&](const NoteInfo &note)
    // {
    //     if (isLastNoteEmpty())
    //         notes.back() = note;
    //     else
    //         notes.emplace_back(note);
    //
    //     notes.emplace_back(); // Всегда храним в конце вектора заметок одну пустую
    // };



    notes.emplace_back(); // Всегда храним в конце вектора заметок одну пустую


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

                                if (tokenType==UMBA_TOKENIZER_TOKEN_LINEFEED)
                                {
                                    ++lineFeedAfterSingleLineCommentCount;
                                    if (lineFeedAfterSingleLineCommentCount>1)
                                    {
                                        if (notes.empty() || !notes.back().empty())
                                        {
                                            // Если больше одного перевода строки, то если в хвосте что-то лежало, то пропихиваем его, чтобы нельзя было ничего добавить
                                            notes.emplace_back();
                                        }
                                    }
                                    return true;
                                }
                                else if (tokenType==UMBA_TOKENIZER_TOKEN_SPACE || tokenType==UMBA_TOKENIZER_TOKEN_TAB)
                                {
                                    return true;
                                }
                                else if (tokenType>=UMBA_TOKENIZER_TOKEN_OPERATOR_SINGLE_LINE_COMMENT_FIRST && tokenType<=UMBA_TOKENIZER_TOKEN_OPERATOR_SINGLE_LINE_COMMENT_LAST)
                                {
                                    auto commentData = std::get<typename tokenizer_type::CommentData>(parsedData);
                                    auto commentStr  = std::string(commentData.data);

                                    NoteInfo note;
                                    if (parseTextNote(commentStr, notesConfig, note))
                                    {
                                        note.line = b.getPosition().lineNumber;
                                        note.file             = filename  ;
                                        note.rootSearchFolder = fileFolder;
                                        notePushOrReplaceLastEmpty(note, false);
                                    }
                                    else
                                    {
                                        // У нас обычный текст

                                        if (isSingleCharText(commentStr))
                                        {
                                            if (!notes.empty() && !notes.back().empty()) // Есть последняя заметка и она не пустая
                                                notes.emplace_back();
                                            return true;
                                        }

                                        if (lineFeedAfterSingleLineCommentCount<2)
                                        {
                                            // Был только один перевод строки - это продолжение заметки, если она была
                                            if (!notes.empty() && !notes.back().empty()) // Есть последняя заметка и она не пустая, надо просто добавить текст
                                            {
                                                umba::string_plus::trim(commentStr);
                                                if (commentStr.empty())
                                                    commentStr = "\n\n"; // пустая строка коментария в тудушке - это отдельный параграф в ней
                                                notes.back().append(commentStr);
                                            }
                                        }
                                    }

                                    lineFeedAfterSingleLineCommentCount = 0;

                                    // fileTextNoComments.append(1, '\n'); // Вместо коментария выводим окончание строки
                                    // Или однострочный коментарий не включает в себя перевод строки, и его можно просто игнорировать?
                                    return true;
                                }
                                else if (tokenType==UMBA_TOKENIZER_TOKEN_OPERATOR_MULTI_LINE_COMMENT)
                                {
                                    auto commentData = std::get<typename tokenizer_type::CommentData>(parsedData);
                                    auto commentStr  = std::string(commentData.data);

                                    if (commentStr.empty())
                                        return checkPushNoteFromMultilineComment(commentStr, b), true; // Просто пропускаем пустые коментарии

                                    // https://www.doxygen.nl/manual/docblocks.html#cppblock

                                    if ( commentStr[0]!='*' // JavaDoc comment style
                                      && commentStr[0]!='!' // Qt comment style
                                        )
                                    {
                                        return checkPushNoteFromMultilineComment(commentStr, b), true; // Просто пропускаем обычные коментарии
                                    }

                                    commentStr.erase(0, 1);
                                    commentStr = prepareMultilineCommentBannerText(commentStr);

                                    auto fileDirectivePos = commentStr.find("@file", 0);
                                    if (fileDirectivePos==commentStr.npos)
                                        fileDirectivePos = commentStr.find("\\file", 0);
                                    if (fileDirectivePos==commentStr.npos)
                                        return checkPushNoteFromMultilineComment(commentStr, b), true; // Просто пропускаем - директива @file не найдена

                                    fileDirectivePos += 5;
                                    auto fileDirectiveEndPos = commentStr.find("\n", fileDirectivePos);
                                    if (fileDirectiveEndPos==commentStr.npos)
                                        return checkPushNoteFromMultilineComment(commentStr, b), true; // Просто пропускаем - директива @file найдена, но конца её нет, и brief'а тут не будет

                                    std::string fileName = marty_cpp::simple_trim(std::string(commentStr, fileDirectivePos, fileDirectiveEndPos-fileDirectivePos), [&](char ch) { return ch==' '; } );
                                    ++fileDirectiveEndPos;

                                    auto briefDirectivePos = commentStr.find("@brief", fileDirectiveEndPos);
                                    if (briefDirectivePos==commentStr.npos)
                                        briefDirectivePos = commentStr.find("\\brief", fileDirectiveEndPos);
                                    if (briefDirectivePos==commentStr.npos)
                                        return checkPushNoteFromMultilineComment(commentStr, b), true; // Просто пропускаем - директива @brief не найдена

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
                                else
                                {
                                    lineFeedAfterSingleLineCommentCount = 3;
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

    if (!notes.empty() && notes.back().empty())
    {
        notes.erase(notes.begin()+std::ptrdiff_t(notes.size()-1));
    }

    return info.briefFound;

}

