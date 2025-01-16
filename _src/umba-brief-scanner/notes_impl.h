/*! \file
    \brief Заметки (TODO, NOTE, и тп)
 */

#pragma once

#include "app_config.h"
#include "log.h"
//

#include "umba/macro_helpers.h"
#include "umba/macros.h"
#include "umba/filename.h"
#include "umba/format_message.h"
#include "umba/text_utils.h"
#include "marty_utf/utf.h"
//

#include <algorithm>
#include <map>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <sstream>


//----------------------------------------------------------------------------
struct NotesConfig;

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
// Text must be ltrimmed
inline
std::string::size_type NoteConfig::findMarker(std::string text, std::string *pFoundMarker) const
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
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
inline
bool NoteInfo::isSpaceChar(char ch)
{
    return ch==' ' || ch=='\t' || ch=='\r' || ch=='\n';
}

//----------------------------------------------------------------------------
inline
std::string NoteInfo::concatTexts(const std::string &s1, const std::string &s2)
{
    if (s1.empty())
        return s2;
    if (s2.empty())
        return s1;

    if (!isSpaceChar(s1.back()) && !isSpaceChar(s2.front()))
        return s1 + " " + s2;

    return s1 + s2;
}

//----------------------------------------------------------------------------
inline
void NoteInfo::append(const std::string &s)
{
    noteText = concatTexts(noteText, s);
}

//----------------------------------------------------------------------------
inline
bool NoteInfo::empty() const
{
    return noteText.empty() && noteType.empty();
}

//----------------------------------------------------------------------------
inline
bool NoteInfo::needAddCheckToTarget(const NoteConfig &noteCfg, bool *pVal) const
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

//----------------------------------------------------------------------------
inline
bool NoteInfo::needAddCheckToTarget(const NoteConfig &noteCfg, char &ch) const
{
    bool bChecked = false;
    if (!needAddCheckToTarget(noteCfg, &bChecked))
        return false;

    ch = bChecked ? 'X' : ' ';

    return true;
}

//----------------------------------------------------------------------------
inline
std::string NoteInfo::getTargetCheckStr(const NoteConfig &noteCfg) const
{
    bool bChecked = false;
    if (!needAddCheckToTarget(noteCfg, &bChecked))
        return std::string();

    return std::string(bChecked ? "[X]" : "[ ]");
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
inline
void NotesCollection::addNote(const NoteInfo &note)
{
    using namespace umba::filename;

    auto folderName       = makeCanonical(getPath(note.file));

    auto folderNameForCmp = makeCanonicalForCompare(getPath(note.file));

    std::map<std::string, FolderNotesCollection>::iterator mit = m_map.find(folderNameForCmp);
    if (mit==m_map.end())
    {
        FolderNotesCollection fnc;
        fnc.folderName       = makeCanonical(getPath(note.file));
        fnc.rootSearchFolder = makeCanonical(note.rootSearchFolder);
        fnc.notes.emplace_back(note);
    }
    else
    {
        mit->second.notes.emplace_back(note);
    }

}

//----------------------------------------------------------------------------
inline
void NotesCollection::addNotes(const std::vector<NoteInfo> &notes)
{
    for(auto && note : notes)
    {
        addNote(note);
    }
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
inline
NoteConfig NotesConfig::findNoteConfig(const std::string &noteType) const
{
    std::unordered_map<std::string, NoteConfig>::const_iterator noteCfgIt = typeConfigs.find(umba::string::tolower_copy(noteType));
    if (noteCfgIt==typeConfigs.end())
        return NoteConfig();
    return noteCfgIt->second;
}

//----------------------------------------------------------------------------
inline
std::string NotesConfig::formatNoteTitle(const NoteInfo &note, const NoteConfig &noteCfg) const
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

//----------------------------------------------------------------------------
inline
std::string NotesConfig::formatNoteTitle(const NoteInfo &note) const
{
    return formatNoteTitle(note, findNoteConfig(note.noteType));
}

//----------------------------------------------------------------------------
inline
std::string NotesConfig::formatNoteSrcInfo(const NoteInfo &note) const
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

//----------------------------------------------------------------------------
inline
void NotesConfig::appendNoteText(std::string &text, const std::string &strAppend)
{
    if (!text.empty() && !strAppend.empty())
        text.append(1, ' ');
    text.append(strAppend);
}

//----------------------------------------------------------------------------
// Делаем текст 
inline
std::string NotesConfig::formatNoteToMarkdown( /* const AppConfig &appCfg, */ const NoteInfo &note, std::size_t textWidth) const
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
        // Переводы строки потом заменим на <BR>
        if ( /* ch=='\n' || ch=='\r' || */  ch=='\t')
            ch = ' ';
    }

    text = umba::text_utils::formatTextParas( text
                                            , textWidth // line len
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

//----------------------------------------------------------------------------
inline
std::string NotesConfig::formatNoteToText( /* const AppConfig &appCfg, */ const NoteInfo &note, std::size_t textWidth=94) const
{
    NoteConfig noteCfg = findNoteConfig(note.noteType);

    // std::ostringstream oss;

    std::string text = textNotesFullPath 
                     ? umba::filesystem::makeCanonical(note.file) // Либо полный путь в локальной ФС, с разделителями, принятыми в данной ОС
                     : umba::filesystem::makeRelPath( note.file, note.rootSearchFolder, '/') // либо относительно каталога, в котором файл найден, с универсальными разделителями '/'
                     ;

    text.append(":");
    text.append(std::to_string(note.line));
    text.append(":");

    if (!singleOutput.empty()) // Если выводим все заметки в один файл, надо добавлять тип заметки
    {
        text.append(note.noteType);
        text.append(":");
    }
    text.append('\n');
    text = umba::text_utils::formatTextParas( text
                                            , textWidth // line len
                                            , umba::text_utils::TextAlignment::left
                                            , marty_utf::SymbolLenCalculatorEncodingUtf8()
                                            );
    text.append('\n');

    return text;
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
inline
std::ostream& FolderNotesCollection::getNotesFormatStream(const NotesConfig &cfg, const std::string &noteType) const
{
    if (!cfg.singleOutput.empty())
        return m_noteStreams[cfg.singleOutput];

    return m_noteStreams[noteType];
}

//----------------------------------------------------------------------------
inline
std::string   FolderNotesCollection::getNotesFormattedText(const std::string &noteType) const
{
    std::unordered_map<std::string, std::ostringstream>::const_iterator mit = m_noteStreams.find(noteType);
    if (mit==m_noteStreams.end())
        return std::string();

    return mit->second.str();
}

//----------------------------------------------------------------------------
inline
void FolderNotesCollection::clearNotesFormattedText() const
{
   m_noteStreams.clear();
}

//----------------------------------------------------------------------------
inline
std::vector<std::string> FolderNotesCollection::getFormattedNoteTypesList() const
{
    std::vector<std::string> resVec; resVec.reserve(m_noteStreams.size());

    std::unordered_map<std::string, std::ostringstream>::const_iterator mit = m_noteStreams.begin();
    for(; mit!=m_noteStreams.end(); ++mit)
    {
        if (mit->second.str().empty())
            continue;
        resVec.emplace_back(mit->first);
    }

    return resVec;
}

//----------------------------------------------------------------------------
inline
void FolderNotesCollection::sortNotes()
{
    std::stable_sort( notes.begin(), notes.end()
                    , [](const NoteInfo &n1, const NoteInfo &n2)
                      {
                          return n1.noteType < n2.noteType;
                      }
                    );
}

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

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
inline
void NotesCollection::sortNotes()
{
    //std::map<std::string, FolderNotesCollection>    m_map;
    for(auto &kv : m_map)
    {
        kv.second.sortNotes();
    }
}

//----------------------------------------------------------------------------
inline
bool NotesCollection::serializeToFiles(const AppConfig &appCfg) const
{
    const NotesConfig &notesCfg = appCfg.notesConfig;

    // Сначала мы делаем доки для отдельных каталогов в любом случае

    // Для каждого каталога мы можем писать все заметкиа в один файл, или же писать в разные, в зависимости от типа

    bool singleOutputMode = !notesCfg.singleOutput.empty();
    bool bFormatMd = (appCfg.optionFlags&AppConfig::ofMd)!=0;

    std::string noteTypeSectionTitleMarker = std::string(singleOutputMode ? 2 : 1, '#');


    // Форматируем заметки по каталогам, в один или разные потоки

    for(const auto &kv : m_map)
    {
        // kv.second.sortNotes();

        std::string prevNoteType;

        for(const auto &note : kv.second.notes)
        {
            std::ostream& oss = getNotesFormatStream(notesCfg, note.noteType);

            if (prevNoteType!=note.noteType)
            {
                prevNoteType = note.noteType;

                // Тип заметки изменился
                if (bFormatMd)
                {
                    // TODO: !!! Нужно выводить не тип заметки, а какой-то заголовок
    
                    oss << "\n" << noteTypeSectionTitleMarker << " " << note.noteType << "\n\n";
    
                    // NoteConfig noteCfg = findNoteConfig(note.noteType);
                    //  
                    // std::string firstLine = note.getTargetCheckStr(noteCfg);
                    // bool hasCheck = !firstLine.empty();
                    //  
                    // appendNoteText(firstLine, formatNoteTitle(note));
                }

            } // if (prevNoteType!=note.noteType)

            if (bFormatMd)
            {
                oss << notesCfg.formatNoteToMarkdown(note, appCfg.descriptionWidth) << "\n\n";
            }
            else
            {
                oss << notesCfg.formatNoteToText(note, appCfg.descriptionWidth) << "\n\n";
            }

        } // for(const auto &note : kv.second.notes)

    }

    // Теперь надо пробежаться опять по всем каталогам, и:
    // если мы хотим сохранять заметки прямо в каталогах - сохраняем их там, 
    // если мы хотим сохранять заметки в одном каталоге, то при пробежке просто
    // собираем тексты для единых файлов, и тогда нам надо добавлять перед каждой секцией заголовок, 
    // в который будем помещать путь к каталогу



    // std::string formatNoteToMarkdown( /* const AppConfig &appCfg, */ const NoteInfo &note, std::size_t textWidth=94) const;
    // std::string formatNoteToText    ( /* const AppConfig &appCfg, */ const NoteInfo &note, std::size_t textWidth=94) const;

    // appCfg.notesConfig
    // appCfg.optionFlags ofHtml ofMd
    // appCfg.bOverwrite

    return appCfg.bOverwrite;
}


// struct NotesConfig
// {
//     std::unordered_map<std::string, NoteConfig>  typeConfigs; // --todo-filename, --todo-marker
//     std::unordered_set<std::string>              stripSet   ; // --todo-strip
//  
//     std::string                                  targetExt  ; // = "md"; // --todo-ext
//     std::string                                  outputPath ; // --todo-output-path - пишем в одно место, а не распихиваем по каталогам
//  
//     std::string                                  singleOutput ; // --notes-single-output  - выводим все заметки в файл данного типа
//     std::string                                  titleFormat  ; // --notes-single-output-title-format
//     std::string                                  srcInfoFormat; // --notes-source-info-format
//     bool                                         addSourceInfo = true; // TODO: Надо добавить опцию ком. строки для задания данного параметра --notes-source-info
//     bool                                         textNotesFullPath = false; // --text-notes-full-path - используется при сохранении в простой текст


// struct FolderNotesCollection
// {
//     std::string                folderName      ; // Каноническое имя
//     std::string                rootSearchFolder; // Каталог поиска, в котором был найден данный каталог, каноническое имя
//  
//     std::vector<NoteInfo>      notes;
//  
//     std::ostream& getNotesFormatStream(const NotesConfig &cfg, const std::string &noteType) const;
//     std::string   getNotesFormattedText(const std::string &noteType) const;
//     void          clearNotesFormattedText() const;
//     std::vector<std::string> getFormattedNoteTypesList() const;
//  
//     // Сортируем по типу, чтобы при выводе в один файл заметки были сгруппированы по типу. Сортируем с сохранением относительного порядка между заметками одного типа
//     void sortNotes(); 
//  
// protected:
//  
//     mutable std::unordered_map<std::string, std::ostringstream>  m_noteStreams;
//  
// }; // struct FolderNotesCollection



// struct NoteInfo
// {
//     std::string    noteType;
//     std::string    noteText;
//  
//     std::string    file; // full name
//     std::size_t    line;
//  
//     std::string    rootSearchFolder; // Каталог поиска, в котором был найден файл
//  
//  
//     bool           hasCheck  = false; // В исходниках заметки был найден маркер
//     bool           isChecked = false; // Найденный маркер был checked
//  
//  
//     void append(const std::string &s);
//     bool empty() const;
//     bool needAddCheckToTarget(const NoteConfig &noteCfg, bool *pVal=0) const;
//     bool needAddCheckToTarget(const NoteConfig &noteCfg, char &ch) const;
//     std::string getTargetCheckStr(const NoteConfig &noteCfg) const;





