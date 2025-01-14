/*! \file
    \brief Заметки (TODO, NOTE, и тп)
 */

#pragma once

#include "umba/macro_helpers.h"
#include "umba/macros.h"
#include "umba/format_message.h"
#include "umba/text_utils.h"
#include "marty_utf/utf.h"

//

#include <string>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>


//----------------------------------------------------------------------------
struct NoteConfig
{
    std::string                        fileName; // --todo-filename
    std::unordered_set<std::string>    markers ; // в нижнем регистре  --todo-marker
    std::string                        title   = "Unknown"; // Заголовок, используется при выводе всех заметок в один файл --notes-single-output-title

    bool                               keepCheck = false; // Если в сорцах был маркер, то переносим его
    bool                               addCheck  = false; // Если в сорцах не было маркера, то добавляем его как unchecked


    // Text must be ltrimmed
    std::string::size_type findMarker(std::string text, std::string *pFoundMarker=0) const
    {
        umba::string::tolower(text);

        for(auto m: markers)
        {
            umba::string::tolower(m);
            if (m.size()<text.size() && umba::string::starts_with(text, m) && !umba::string::is_alpha(text[m.size()]))
            {
                if (pFoundMarker)
                    *pFoundMarker = m;
                return m.size();
            }
        }
    
        return 0;
    }


};

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct NoteInfo
{
    std::string    noteType;
    std::string    noteText;

    std::string    file;
    std::size_t    line;

    bool           hasCheck  = false; // В исходниках заметки был найден маркер
    bool           isChecked = false; // Найденный маркер был checked


    bool empty() const
    {
        return noteText.empty() && noteType.empty();
    }

    bool needAddCheckToTarget(const NoteConfig &noteCfg, bool *pVal=0) const
    {
        if (noteCfg.keepCheck && hasCheck)
        {
            if (pVal)
               *pVal = isChecked; 

            return true;
        }

        if (noteCfg.addCheck)
        {
            if (pVal)
               *pVal = false; 

            return true;
        }

        return false;
    }

    bool needAddCheckToTarget(const NoteConfig &noteCfg, char &ch) const
    {
        bool bChecked = false;
        if (!needAddCheckToTarget(noteCfg, &bChecked))
            return false;

        ch = bChecked ? 'X' : ' ';

        return true;
    }

    std::string getTargetCheckStr(const NoteConfig &noteCfg) const
    {
        bool bChecked = false;
        if (!needAddCheckToTarget(noteCfg, &bChecked))
            return std::string();

        return std::string(bChecked ? "[X]" : "[ ]");
    }


}; // struct NoteInfo
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct NotesConfig
{
    std::unordered_map<std::string, NoteConfig>  typeConfigs; // --todo-filename, --todo-marker
    std::unordered_set<std::string>              stripSet   ; // --todo-strip
    std::string                                  targetExt  ; // = "md"; // --todo-ext
    std::string                                  outputPath ; // --todo-output-path

    std::string                                  singleOutput ; // --notes-single-output
    std::string                                  titleFormat  ; // --notes-single-output-title-format
    std::string                                  srcInfoFormat; // --notes-source-info-format
    bool                                         addSourceInfo = true; // TODO: Надо добавить опцию ком. строки для задания данного параметра


    NoteConfig findNoteConfig(const std::string &noteType) const
    {
        std::unordered_map<std::string, NoteConfig>::const_iterator noteCfgIt = typeConfigs.find(umba::string::tolower_copy(noteType));
        if (noteCfgIt==typeConfigs.end())
            return NoteConfig();
        return noteCfgIt->second;
    }

    std::string formatNoteTitle(const NoteInfo &note, const NoteConfig &noteCfg) const
    {
        // umba::macros::MacroTextFromMap<std::string> mtfm;
        // mtfm.m["TITLE"] = noteCfg.title;
        // return umba::macros::substMacros(titleFormat, mtfm, umba::macros::smf_KeepUnknownVars | umba::macros::smf_uppercaseNames);

        UMBA_USED(note);

        if (singleOutput.empty())
            return std::string(); // Когда у нас раздельный output - не выводим тип заметок

        return umba::FormatMessage(titleFormat)
              .arg("Title", noteCfg.title)
              .toString();
    }

    std::string formatNoteTitle(const NoteInfo &note) const
    {
        return formatNoteTitle(note, findNoteConfig(note.noteType));
    }

    std::string formatNoteSrcInfo(const NoteInfo &note) const
    {
        // umba::macros::MacroTextFromMap<std::string> mtfm;
        // mtfm.m["FILE"] = note.file;
        // mtfm.m["LINE"] = std::to_string(note.line);
        //  
        // return umba::macros::substMacros(srcInfoFormat, mtfm, umba::macros::smf_KeepUnknownVars | umba::macros::smf_uppercaseNames);

        if (!addSourceInfo)
            return std::string();

        return umba::FormatMessage(titleFormat)
              .arg("File", note.file)
              .arg("Line", note.line)
              .toString();
    }

    static
    void appendNoteText(std::string &text, const std::string &strAppend)
    {
        if (!text.empty() && !strAppend.empty())
            text.append(1, ' ');
        text.append(strAppend);
    }

    // Делаем текст 
    std::string formatNote(const NoteInfo &note) const
    {
        NoteConfig noteCfg = findNoteConfig(note.noteType);

        std::string firstLine = note.getTargetCheckStr(noteCfg);
        bool hasCheck = !firstLine.empty();

        appendNoteText(firstLine, formatNoteTitle(note));
        appendNoteText(firstLine, formatNoteSrcInfo(note));

        // Пока в заметках не поддерживается никакого форматирования

        std::string text = note.noteText;
        for(auto &ch : text)
        {
            if (ch=='\n' || ch=='\r' || ch=='\t')
                ch = ' ';
        }

        text = umba::text_utils::formatTextParas( text
                                                , 96 // line len
                                                , umba::text_utils::TextAlignment::left
                                                , marty_utf::SymbolLenCalculatorEncodingUtf8()
                                                );

        text = umba::text_utils::textAddIndent(text, std::string(6, ' '));

        if (!text.empty())
        {
            if (firstLine.empty())
            {
                text[0] = '-';
            }
            else
            {
                if (hasCheck)
                    text = std::string("- ")     + firstLine + "\n" + text;
                else
                    text = std::string("-     ") + firstLine + "\n" + text;
            }
        }

        return text;
    }


// TODO: В TODO-файле может использоваться своя семантика, какие-то идентификаторы тикетов, дата задания тикета, 
// ожидаемое время реализации, фактическое время реализации, ссылка на story, ссылка на артефакты.
// Эта семантика пока не проработана, надо посмотреть на тулзу `AbstractSpoon` `ToDoList` (расширение `.tdl`).
// Посмотреть `work/carno/tasks` для примера.


// std::string textAddIndent(const std::string &text, const std::string &indent)

// umba::text_utils::
// enum class TextAlignment
// {
//     width  = 0,
//     left      ,
//     center    ,
//     right
//  
// };

// std::string formatTextParas( std::string text, std::string::size_type paraWidth
//                            , TextAlignment textAlignment // = TextAlignment::width
//                            , const SymbolLenCalculator &symbolLenCalculator // = SymbolLenCalculatorEncodingSingleByte()
//                            )



// struct NoteConfig
// {
//     std::string                        fileName; // --todo-filename
//     std::unordered_set<std::string>    markers ; // в нижнем регистре  --todo-marker
//     std::string                        title   ; // Заголовок, используется при выводе всех заметок в один файл --notes-single-output-title
//  
//     bool                               keepCheck = false; // Если в сорцах был маркер, то переносим его
//     bool                               addCheck  = false; // Если в сорцах не было маркера, то добавляем его как unchecked


//    umba::macros::StringStringMap<std::string>
    // smf_uppercaseNames

// substMacros( const ::std::basic_string<CharType, Traits, Allocator> &str
//            , const MacroTextGetter                                  &getMacroText
//            , int                                                     flags = smf_KeepUnknownVars // smf_ArgsAllowed|smf_ConditionAllowed
//            )


// template<typename StringType>
// struct MacroTextFromMap : public IMacroTextGetter<StringType>
// {
//     StringStringMap<StringType> m;



// struct NoteInfo
// {
//     std::string    noteType;
//     std::string    noteText;
//  
//     std::string    file;
//     std::size_t    line;


}; // struct NotesConfig

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
//! Проверяем текст на предмет обрезаемых префиксов, и обрезаем их все
inline
bool noteTextTestForStripsAndStrip( std::string                             &text
                                  , const std::unordered_set<std::string>   &strips
                                  , std::string                             *pFoundStrip
                                  , bool                                    stripAll
                                  )
{
    umba::string_plus::ltrim(text);

    bool anyStripsFound = false;
    bool stripsFound = true;
    while(stripsFound)
    {
        stripsFound = false;
        for(auto &&s: strips)
        {
            if (umba::string::starts_with(text, s))
            {
                if (pFoundStrip)
                   *pFoundStrip = s;
                text.erase(0, s.size());
                umba::string_plus::ltrim(text);
                if (!stripAll)
                     return true;
                stripsFound    = true;
                anyStripsFound = true;
            }

        } // for(auto &&s: strips)

    } // while(stripsFound)

    return anyStripsFound;
}

//----------------------------------------------------------------------------
//! Проверяем текст на предмет 
inline
bool noteTextTestForMarkersAndStrip( std::string &text
                                   , const std::unordered_map<std::string, NoteConfig> &noteConfigs
                                   , const std::unordered_set<std::string>             &notesStrip
                                   , std::string *pFoundNoteType=0
                                   , std::string *pFoundMarker=0
                                   )
{
    umba::string_plus::ltrim(text);

    std::unordered_map<std::string, NoteConfig>::const_iterator cfgIt = noteConfigs.begin();
    for(; cfgIt!=noteConfigs.end(); ++cfgIt)
    {
        auto sz = cfgIt->second.findMarker(text, pFoundMarker);
        if (sz!=0)
        {
            text.erase(0, sz);
            if (pFoundNoteType)
               *pFoundNoteType = cfgIt->first;
            break;
        }
    }

    if (cfgIt==noteConfigs.end())
        return false;

    // umba::string_plus::ltrim(text);

    noteTextTestForStripsAndStrip(text, notesStrip, 0 /* pFoundStrip */, true /* stripAll */);

    return true;

}

//----------------------------------------------------------------------------
inline
std::unordered_set<std::string> makeNoteChecksSet()
{
    return std::unordered_set<std::string>{ "[]", "[ ]", "[x]", "[X]" };
}

//----------------------------------------------------------------------------
inline
const std::unordered_set<std::string>& getNoteChecksSet()
{
    static auto m = makeNoteChecksSet();
    return m;
}

//----------------------------------------------------------------------------
inline
bool noteTextTestForCheckAndStrip( std::string &text
                     , bool *pChecked
                     )
{
    std::string stripFound;
    if (!noteTextTestForStripsAndStrip(text, getNoteChecksSet(), &stripFound, false /* stripAll */))
        return false;

    if (pChecked)
    {
        *pChecked = false;

        if (stripFound.size()>1 && (stripFound[1]=='x' || stripFound[1]=='X'))
            *pChecked = true;
    }

    return true;

}

//----------------------------------------------------------------------------
inline
bool parseTextNote(std::string text, const NotesConfig &cfg, NoteInfo &note)
{
    if (!noteTextTestForMarkersAndStrip(text, cfg.typeConfigs, cfg.stripSet, &note.noteType))
        return false; // marker not found

    umba::string_plus::ltrim(text);

    note.hasCheck = noteTextTestForCheckAndStrip(text, &note.isChecked);

    umba::string_plus::trim(text);

    note.noteText = text;

    return true;
}


// bool noteTextTestForMarkersAndStrip( std::string &text
//                                    , const std::unordered_map<std::string, NoteConfig> &noteConfigs
//                                    , const std::unordered_set<std::string>             &notesStrip
//                                    , std::string *pFoundNoteType=0
//                                    , std::string *pFoundMarker=0
//                                    )

// struct NotesConfig
// {
//     std::unordered_map<std::string, NoteConfig>  typeConfigs; // --todo-filename, --todo-marker
//     std::unordered_set<std::string>              stripSet   ; // --todo-strip


// struct NoteInfo
// {
//     std::string    noteType;
//     std::string    noteText;
//  
//     std::string    file;
//     std::size_t    line;
//  
//     bool           hasCheck  = false; // В исходниках заметки был найден маркер
//     bool           isChecked = false; // Найденный маркер был checked


