/*! \file
    \brief Заметки (TODO, NOTE, и тп)
 */

#pragma once

#include "umba/macro_helpers.h"
#include "umba/macros.h"
#include "umba/filename.h"
#include "umba/format_message.h"
#include "umba/text_utils.h"
#include "marty_utf/utf.h"

//

#include <map>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <sstream>


//----------------------------------------------------------------------------
struct AppConfig;
struct NotesConfig;

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct NoteConfig
{
    std::string                        fileName; // --todo-filename
    std::unordered_set<std::string>    markers ; // в нижнем регистре  --todo-marker
    std::string                        title   = "Unknown"; // Заголовок, используется при выводе всех заметок в один файл --notes-single-output-title

    bool                               keepCheck = false; // Если в сорцах был маркер, то переносим его
    bool                               addCheck  = false; // Если в сорцах не было маркера, то добавляем его как unchecked


    std::string::size_type findMarker(std::string text, std::string *pFoundMarker=0) const;

}; // struct NoteConfig

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct NoteInfo
{
    std::string    noteType;
    std::string    noteText;

    std::string    file; // full name
    std::size_t    line;

    std::string    rootSearchFolder; // Каталог поиска, в котором был найден файл


    bool           hasCheck  = false; // В исходниках заметки был найден маркер
    bool           isChecked = false; // Найденный маркер был checked


    void append(const std::string &s);
    bool empty() const;
    bool needAddCheckToTarget(const NoteConfig &noteCfg, bool *pVal=0) const;
    bool needAddCheckToTarget(const NoteConfig &noteCfg, char &ch) const;
    std::string getTargetCheckStr(const NoteConfig &noteCfg) const;


protected:

    static bool isSpaceChar(char ch);
    static std::string concatTexts(const std::string &s1, const std::string &s2);


}; // struct NoteInfo
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
//! Храним тут информацию по каталогу со всеми заметками из всех файлов каталога
struct FolderNotesCollection
{
    std::string                folderName      ; // Каноническое имя
    std::string                rootSearchFolder; // Каталог поиска, в котором был найден данный каталог, каноническое имя

    std::vector<NoteInfo>      notes;

    std::ostream& getNotesFormatStream(const NotesConfig &cfg, std::string noteType) const;
    std::string   getNotesFormattedText(std::string noteType) const;
    void          clearNotesFormattedText() const;
    std::vector<std::string> getFormattedNoteTypesList() const;

    // Сортируем по типу, чтобы при выводе в один файл заметки были сгруппированы по типу. Сортируем с сохранением относительного порядка между заметками одного типа
    void sortNotes();

protected:

    mutable std::unordered_map<std::string, std::shared_ptr<std::ostringstream> >  m_noteStreams;

}; // struct FolderNotesCollection

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct NotesCollection
{
    using iterator       = typename std::map<std::string, FolderNotesCollection>::iterator;
    using const_iterator = typename std::map<std::string, FolderNotesCollection>::const_iterator;

    void addNote(const NoteInfo &note);
    void addNotes(const std::vector<NoteInfo> &notes);
    void sortNotes();

          iterator begin ()       { return m_map.begin (); }
    const_iterator begin () const { return m_map.begin (); }
    const_iterator cbegin() const { return m_map.cbegin(); }

          iterator end   ()       { return m_map.end   (); }
    const_iterator end   () const { return m_map.end   (); }
    const_iterator cend  () const { return m_map.cend  (); }

    bool serializeToFiles(const AppConfig &appCfg, std::vector<std::string> &writtenFiles) const;



protected:

    std::map<std::string, FolderNotesCollection>    m_map;

}; // struct NotesCollection

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct NotesConfig
{
    std::unordered_map<std::string, NoteConfig>  typeConfigs; // --todo-filename, --todo-marker
    std::unordered_set<std::string>              stripSet   ; // --todo-strip

    std::string                                  targetExt  ; // = "md"; // --todo-ext
    std::string                                  outputPath ; // --todo-output-path - пишем в одно место, а не распихиваем по каталогам

    std::string                                  singleOutput ; // --notes-single-output  - выводим все заметки в файл данного типа
    std::string                                  titleFormat  ; // --notes-single-output-title-format
    std::string                                  srcInfoFormat; // --notes-source-info-format
    bool                                         addSourceInfo = true; // TODO: Надо добавить опцию ком. строки для задания данного параметра --notes-source-info
    bool                                         textNotesFullPath = false; // TODO: --text-notes-full-path - используется при сохранении в простой текст
    // std::size_t                                  notesTextWidth; // Ширина форматирования текста, будем пока использовать descriptionWidth из AppConfig --notes-text-width

    /*
         Замечания по форматированию заметок.

         В текстовом виде заметки форматируются в двух вариантах:
         1) Заметки разных типов в одном файле:
              путь/к/файлу/имя_файла.расш:NN:NoteType
              Со следующей строки без пропуска идёт текст
              заметки
         2) Заметки разных типов в разных файлах, соответственно, в файле только заметки одного типа.
            В этом случае тип заметки не указывается после имени файла и номера строки.

         Между заметками добавляется пустая строка.

         При генерации заметок в текстовом виде мы можем указывать полное абсолютное имя файла - для того,
         чтобы можно в IDE было переходить к этим файлам из списка заметок - это локальная генерация,
         которая никогда не попадает в репо. Или же мы можем указывать имя файла относительно базового
         каталога поиска. Такой вид заметок можно гитовать, он не зависит от локального расположения клона репы.

         --text-notes-full-path

         Также в текстовом режиме не используется никакое форматирование - нет маркеров списка, нет маркеров-checkBox'ов,
         только текст заметки, выровненный на заданную ширину.

         Используем formatTextParas.

         Вопрос. Может, сделать вставку разделительной линии между заметками? Тогда можно разбивать на параграфы двойным переводом строки.




         Форматирование в MD реализовано ниже в методе formatMarkdownNote, который использует функцию formatTextParas.

         Разбивку на параграфы (двойной перевод строки) надо заменить на "<BR>"

     */


    std::string getNoteOutputType(const std::string &noteType) const;
    NoteConfig  findNoteConfig(const std::string &noteType) const;
    std::string formatNoteTitle(const NoteInfo &note, const NoteConfig &noteCfg) const;
    std::string formatNoteTitle(const NoteInfo &note) const;
    std::string formatNoteSrcInfo(const NoteInfo &note, bool formatMd=true) const;

    static void appendNoteText(std::string &text, const std::string &strAppend);

    // Делаем текст
    std::string formatNoteToMarkdown( /* const AppConfig &appCfg, */ const NoteInfo &note, std::size_t textWidth=94) const;
    std::string formatNoteToText    ( /* const AppConfig &appCfg, */ const NoteInfo &note, std::size_t textWidth=94) const;

}; // struct NotesConfig

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------

#include "notes_impl.h"

//----------------------------------------------------------------------------



