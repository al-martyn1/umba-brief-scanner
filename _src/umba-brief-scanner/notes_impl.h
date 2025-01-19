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
#include "umba/filesys.h"
#include "umba/format_message.h"
#include "umba/text_utils.h"
#include "umba/cli_tool_helpers.h"
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
        m_map[folderNameForCmp] = fnc;
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
std::string NotesConfig::getNoteOutputType(const std::string &noteType) const
{
    return !singleOutput.empty() ? singleOutput : noteType;
}

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

    if (!singleOutput.empty())
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
std::string NotesConfig::formatNoteSrcInfo(const NoteInfo &note, bool formatMd) const
{
    // umba::macros::MacroTextFromMap<std::string> mtfm;
    // mtfm.m["FILE"] = note.file;
    // mtfm.m["LINE"] = std::to_string(note.line);
    //
    // return umba::macros::substMacros(srcInfoFormat, mtfm, umba::macros::smf_KeepUnknownVars | umba::macros::smf_uppercaseNames);

    if (!addSourceInfo)
        return std::string();

    auto file = !outputPath.empty()
              ? ( formatMd
                ? umba::filename::makeRelPath( note.file, note.rootSearchFolder, '/')
                : note.file
                )
              : umba::filename::getFileName(note.file)
              ;

    return umba::FormatMessage(srcInfoFormat)
          .arg("File", file)
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
    // bool hasCheck = !firstLine.empty();

    if (!singleOutput.empty()) // Если выводим все заметки в один файл, надо добавлять тип заметки
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

    text = umba::text_utils::textAddIndent(text, std::string(2, ' '));

    if (!text.empty())
    {
        if (firstLine.empty())
        {
            text[0] = '-';
        }
        else
        {
            // if (hasCheck)
            //     text = std::string("- ") + firstLine + "\n" + text;
            // else
            //     text = std::string("- ") + firstLine + "\n" + text;

            text = std::string("- ") + firstLine + "\n" + text;
        }
    }

    return text;
}

//----------------------------------------------------------------------------
inline
std::string NotesConfig::formatNoteToText( /* const AppConfig &appCfg, */ const NoteInfo &note, std::size_t textWidth) const
{
    NoteConfig noteCfg = findNoteConfig(note.noteType);

    // std::ostringstream oss;


    std::string text = textNotesFullPath
                     ? umba::filename::makeCanonical(note.file) // Либо полный путь в локальной ФС, с разделителями, принятыми в данной ОС
                     : ( !outputPath.empty()
                       ? umba::filename::makeRelPath( note.file, note.rootSearchFolder, '/') // либо относительно каталога, в котором файл найден, с универсальными разделителями '/'
                       : umba::filename::getFileName(note.file)
                       )
                     ;

    text.append(":");
    text.append(std::to_string(note.line));
    text.append(":");

    if (!singleOutput.empty()) // Если выводим все заметки в один файл, надо добавлять тип заметки
    {
        text.append(note.noteType);
        text.append(":");
    }
    text.append(1, '\n');
    text += umba::text_utils::formatTextParas( note.noteText
                                             , textWidth // line len
                                             , umba::text_utils::TextAlignment::left
                                             , marty_utf::SymbolLenCalculatorEncodingUtf8()
                                             );
    text.append(1, '\n');

    return text;
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
inline
std::ostream& FolderNotesCollection::getNotesFormatStream(const NotesConfig &cfg, std::string noteType) const
{
    noteType = cfg.getNoteOutputType(noteType);
    std::unordered_map<std::string, std::shared_ptr<std::ostringstream> >::const_iterator it = m_noteStreams.find(noteType);
    if (it!=m_noteStreams.end())
    {
        return *it->second;
    }

    auto oss = std::make_shared<std::ostringstream>();
    m_noteStreams[noteType] = oss;

    return *oss;
}

//----------------------------------------------------------------------------
inline
std::string   FolderNotesCollection::getNotesFormattedText(std::string noteType) const
{
    //noteType = cfg.getNoteOutputType(noteType);
    std::unordered_map<std::string, std::shared_ptr<std::ostringstream> >::const_iterator mit = m_noteStreams.find(noteType);
    if (mit==m_noteStreams.end())
        return std::string();

    return mit->second->str();
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

    std::unordered_map<std::string, std::shared_ptr<std::ostringstream> >::const_iterator mit = m_noteStreams.begin();
    for(; mit!=m_noteStreams.end(); ++mit)
    {
        if (mit->second->str().empty())
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
bool NotesCollection::serializeToFiles(const AppConfig &appCfg, std::vector<std::string> &writtenFiles) const
{
    using namespace umba::filename;

    const NotesConfig &notesCfg = appCfg.notesConfig;

    // Сначала мы делаем доки для отдельных каталогов в любом случае

    // Для каждого каталога мы можем писать все заметкиа в один файл, или же писать в разные, в зависимости от типа

    bool bSingleOutputPath = !notesCfg.outputPath.empty();
    bool bSingleOutputMode = !notesCfg.singleOutput.empty();
    bool bFormatMd = (appCfg.optionFlags&AppConfig::ofMd)!=0;

    std::string noteTypeSectionTitleMarker = std::string(bSingleOutputMode ? 2u : 1u, '#');


    // Форматируем заметки по каталогам, в один или разные потоки

    for(const auto &kv : m_map)
    {
        // kv.second.sortNotes();

        std::string prevNoteType;

        for(const auto &note : kv.second.notes)
        {
            std::ostream& oss = kv.second.getNotesFormatStream(notesCfg, note.noteType);

            if (prevNoteType!=note.noteType)
            {
                prevNoteType = note.noteType;

                // Тип заметки изменился
                if (bFormatMd)
                {
                    // TODO: !!! Нужно выводить не тип заметки, а какой-то заголовок
                    if (bSingleOutputMode)
                    {
                        oss << "\n" << noteTypeSectionTitleMarker << " " << note.noteType << "\n\n";
                    }
                }

            } // if (prevNoteType!=note.noteType)

            if (bFormatMd)
                oss << notesCfg.formatNoteToMarkdown(note, appCfg.descriptionWidth) << "\n\n";
            else
                oss << notesCfg.formatNoteToText(note, appCfg.descriptionWidth) << "\n\n";

        } // for(const auto &note : kv.second.notes)

    }

    // Теперь надо пробежаться опять по всем каталогам, и:
    // если мы хотим сохранять заметки прямо в каталогах - сохраняем их там,
    // если мы хотим сохранять заметки в одном каталоге, то при пробежке просто
    // собираем тексты для единых файлов, и тогда нам надо добавлять перед каждой секцией заголовок,
    // в который будем помещать путь к каталогу


    std::unordered_map<std::string, std::string> singlePathOutputs;

    auto targetExt = notesCfg.targetExt;
    if (targetExt.empty())
    {
        if (bFormatMd)
            targetExt = "md";
        else
            targetExt = "txt";
    }


    for(const auto &kv : m_map)
    {
        auto notesTypeList = kv.second.getFormattedNoteTypesList();
        for(auto noteType : notesTypeList)
        {
            noteType = notesCfg.getNoteOutputType(noteType);
            auto notesText = kv.second.getNotesFormattedText(noteType);

            if (bSingleOutputPath)
            {
                auto &allText = singlePathOutputs[noteType];
                if (bFormatMd)
                {
                    if (!allText.empty())
                        allText.append(2, '\n');

                    allText.append("# ");
                    allText.append(makeRelPath( kv.second.folderName, kv.second.rootSearchFolder, '/'));
                    allText.append(2, '\n');
                }
                else
                {
                    // allText.append(makeRelPath( kv.second.folderName, kv.second.rootSearchFolder, '/'));
                    // allText.append(2, '\n');
                }

                allText.append(notesText);

            }
            else // write notes to their folder
            {
                notesText = bFormatMd ? ("---\nGenerator: Umba-Brief-Scanner\n---\n\n" + notesText) : notesText;
                notesText = marty_cpp::converLfToOutputFormat(notesText, appCfg.outputLinefeed);

                NoteConfig noteTypeCfg = notesCfg.findNoteConfig(noteType);

                auto notesFolder = makeCanonical(kv.second.folderName);
                umba::filesys::createDirectoryEx( notesFolder, true /* forceCreatePath */ );

                auto notesFilename = makeCanonical(appendExt(appendPath(kv.second.folderName, noteTypeCfg.fileName), targetExt));
                umba::cli_tool_helpers::writeOutput( notesFilename, umba::cli_tool_helpers::IoFileType::regularFile // outputFileType
                                                   , encoding::ToUtf8(), encoding::FromUtf8()
                                                   , notesText, std::string() // bomData
                                                   , true /* fromFile */, true /* utfSource */ , appCfg.bOverwrite
                                                   );
                writtenFiles.emplace_back(notesFilename);
            }
        }
    }

    if (bSingleOutputPath)
    {
        for(const auto &kv : singlePathOutputs)
        {
            auto noteType = kv.first;
            NoteConfig noteTypeCfg = notesCfg.findNoteConfig(noteType);

            auto
            notesText = bFormatMd ? ("---\nGenerator: Umba-Brief-Scanner\n---\n\n" + kv.second) : kv.second;
            notesText = marty_cpp::converLfToOutputFormat(notesText, appCfg.outputLinefeed);

            auto notesFolder = makeCanonical(notesCfg.outputPath);
            umba::filesys::createDirectoryEx( notesFolder, true /* forceCreatePath */ );

            auto notesFilename = makeCanonical(appendExt(appendPath(notesCfg.outputPath, noteTypeCfg.fileName), targetExt));
            umba::cli_tool_helpers::writeOutput( notesFilename, umba::cli_tool_helpers::IoFileType::regularFile // outputFileType
                                               , encoding::ToUtf8(), encoding::FromUtf8()
                                               , notesText, std::string() // bomData
                                               , true /* fromFile */, true /* utfSource */ , appCfg.bOverwrite
                                               );
            writtenFiles.emplace_back(notesFilename);
        }
    }

    for(const auto &kv : m_map)
    {
        kv.second.clearNotesFormattedText();
    }

    return true;
}









