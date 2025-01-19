/*! \file
    \brief Класс, задающий параметры работы приложения umba-brief-scanner. Настраивается опциями командной строки.
 */

#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

//
#include "umba/program_location.h"
#include "umba/enum_helpers.h"
#include "umba/flag_helpers.h"

#include "umba/regex_helpers.h"

#include "enums_manual.h"

#include "signature.h"

//
#include "notes.h"
#include "utils.h"

//----------------------------------------------------------------------------





//----------------------------------------------------------------------------
enum class VerbosityLevel
{
     invalid      = -1,
     begin        = 0,

     quet         = 0,   // quet
     normal       = 1,   // normal - print common details
     config       = 2,   // print common details and app config
     detailed     = 3,   // print common details, app config and all declarations, found in user files
     //extra        = 4,    // print common details, app config and all found declarations

     end          = detailed

};

UMBA_ENUM_CLASS_IMPLEMENT_RELATION_OPERATORS(VerbosityLevel)

//----------------------------------------------------------------------------







//----------------------------------------------------------------------------
struct AppConfig
{

    //------------------------------
    static const unsigned                    ofEmptyOptionFlags      = 0x0000;
    static const unsigned                    ofNoOutput              = 0x0010; // Do not actually write output files
    static const unsigned                    ofMain                  = 0x0020; // Print only main files (whish contains main or other entry point)
    static const unsigned                    ofSkipUndocumented      = 0x0040; // Skip undocumented files
    static const unsigned                    ofRemovePath            = 0x0080; // Remove path from output names

    static const unsigned                    ofSplitGroups           = 0x0200; // Remove path from output names

    static const unsigned                    ofHtml                  = 0x1000; // Print output in html format
    static const unsigned                    ofMd                    = 0x2000; // Print output in md format, overrides html option

    static const unsigned                    ofTodo                  = 0x4000; // Scan for todo


    //------------------------------
    // !!! Не забывать копировать и/или подготавливать поля класса в функции getAdjustedConfig
    //------------------------------
    umba::macros::StringStringMap<std::string>  macros; // не используем

    // std::map< std::string,std::set<std::string> >  entryNames; // не используем
    std::vector<TextSignature>                  entrySignatures;


    #if defined(WIN32) || defined(_WIN32)
        marty_cpp::ELinefeedType outputLinefeed = marty_cpp::ELinefeedType::crlf;
    #else
        marty_cpp::ELinefeedType outputLinefeed = marty_cpp::ELinefeedType::lf;
    #endif

    bool                                     bOverwrite = false;

    //------------------------------
    std::vector<std::string>                 includeFilesMaskList;
    std::vector<std::string>                 excludeFilesMaskList;
    std::vector<std::string>                 excludeDirs;

    std::vector<std::string>                 scanPaths;
    //std::string                              outputPath;
    std::string                              outputName;
    bool                                     updateMode;
    std::string                              updateFromFile;

    //std::size_t                              splitGroups      = 0;
    //bool                                     splitGroups      = false;

    unsigned                                 filenameWidth    = 0;
    unsigned                                 descriptionWidth = 0;

    unsigned                                 optionFlags = 0; // ofNormalizeFilenames; // ofEmptyOptionFlags;

    VerbosityLevel                           verbosityLevel = VerbosityLevel::normal;

    DoxificationMode                         doxificationMode = DoxificationMode::noDoxyfication;

    NotesConfig                              notesConfig;

    //------------------------------


    //------------------------------
    bool setNotesAddCheck(std::string opt)
    {
        umba::string::tolower(opt);
        auto typeList = splitAndTrimAndSkipEmpty(opt,',');
        if (typeList.empty())
            return false;

        auto setAll = [&](bool bVal)
        {
            for(auto &kv : notesConfig.typeConfigs)
                kv.second.addCheck = bVal;
        };

        for(auto &&t : typeList)
        {
            if (t=="reset" || t=="clr" || t=="clear" || t=="-")
                setAll(false);
            else if (t=="all" || t=="set" || t=="+")
                setAll(true);
            else
                notesConfig.typeConfigs[t].addCheck = true;
        }

        return true;
    }

    bool setNotesKeepCheck(std::string opt)
    {
        umba::string::tolower(opt);
        auto typeList = splitAndTrimAndSkipEmpty(opt,',');
        if (typeList.empty())
            return false;

        auto setAll = [&](bool bVal)
        {
            for(auto &kv : notesConfig.typeConfigs)
                kv.second.keepCheck = bVal;
        };

        for(auto &&t : typeList)
        {
            if (t=="reset" || t=="clr" || t=="clear" || t=="-")
                setAll(false);
            else if (t=="all" || t=="set" || t=="+")
                setAll(true);
            else
                notesConfig.typeConfigs[t].keepCheck = true;
        }

        return true;
    }

    bool setNotesTitleFormat(const std::string &titleFormat)
    {
        notesConfig.titleFormat = titleFormat;
        return true;
    }

    bool setNotesSrcInfoFormat(const std::string &srcInfoFormat)
    {
        notesConfig.srcInfoFormat = srcInfoFormat;
        return true;
    }

    bool setNotesSingleOutputType(const std::string &singleOutputType)
    {
        notesConfig.singleOutput = singleOutputType;
        return true;
    }

    bool setNoteTitle(std::string noteType, const std::string &noteTitle)
    {
        if (noteType.empty())
            return false;

        if (noteTitle.empty())
            return false;

        umba::string::tolower(noteType);
        notesConfig.typeConfigs[noteType].title = noteTitle;

        return true;
    }

    bool setNoteTitle(const std::string &noteTypeTitlePair)
    {
        std::string f;
        std::string s;
        if (!umba::string_plus::split_to_pair(noteTypeTitlePair, f, s, ':'))
            return false;

        return setNoteTitle(f, s);
    }

    bool addNoteMarker(std::string noteType, std::string noteMarker)
    {
        if (noteType.empty())
            return false;

        if (noteMarker.empty())
            return false;

        umba::string::tolower(noteType);
        umba::string::tolower(noteMarker);
        notesConfig.typeConfigs[noteType].markers.insert(noteMarker);

        return true;
    }

    bool addNoteMarker(const std::string &noteTypeMarkerPair)
    {
        std::string f;
        std::string s;
        if (!umba::string_plus::split_to_pair(noteTypeMarkerPair, f, s, ':'))
            return false;

        return addNoteMarker(f, s);
    }

    bool addNoteFilename(std::string noteType, std::string noteFilename)
    {
        if (noteType.empty())
            return false;

        if (noteFilename.empty())
            return false;

        umba::string::tolower(noteType);
        notesConfig.typeConfigs[noteType].fileName = noteFilename;

        return true;
    }

    bool addNoteFilename(const std::string &noteTypeFilenamePair)
    {
        std::string f;
        std::string s;
        if (!umba::string_plus::split_to_pair(noteTypeFilenamePair, f, s, ':'))
            return false;

        return addNoteFilename(f, s);
    }

    bool addNoteStripPrefix(std::string noteStrip)
    {
        if (noteStrip.empty())
            return false;

        umba::string::tolower(noteStrip);
        notesConfig.stripSet.insert(noteStrip);
        return true;
    }

    bool setNotesOutputPath(const std::string &p)
    {
        notesConfig.outputPath = p;
        return true;
    }

    bool setNotesFileExt(const std::string &e)
    {
        notesConfig.targetExt = e;
        return true;
    }


    //------------------------------

    typedef std::string StdString;
    UMBA_ENUM_CLASS_IMPLEMENT_STRING_CONVERTERS_MEMBER( StdString , VerbosityLevel, "quet", "normal", "config", "verbose" /* , "extra" */  )

    //------------------------------



    //------------------------------
    void setVerbosityLevel(VerbosityLevel lvl) { verbosityLevel = lvl; }

    //! Проверяет уровень lvl на предмет допустимости детализации выхлопа в лог для данного уровня.
    /*! Уровень детализации lvl должен быть меньше или равен заданному в конфиге.
     */
    bool testVerbosity(VerbosityLevel lvl) const
    {
        return (verbosityLevel==VerbosityLevel::invalid)
             ? false
             : lvl<=verbosityLevel
             ;
    }

    std::string testVerbosityStringRes(VerbosityLevel lvl) const
    {
        return testVerbosity(lvl) ? "true" : "false";
    }

    //------------------------------



    //------------------------------
    void ofSet  ( unsigned ofFlags )       { optionFlags |=  ofFlags; }
    void ofReset( unsigned ofFlags )       { optionFlags &= ~ofFlags; }
    bool ofGet  ( unsigned ofFlags ) const { return (optionFlags&ofFlags)==ofFlags; }
    void ofSet  ( unsigned ofFlags , bool setState )
    {
        if (setState) ofSet  (ofFlags);
        else          ofReset(ofFlags);
    }

    static std::string getOptValAsString(unsigned opt)    { return opt ? "Yes" : "No"; }

    static std::string getOptNameString(unsigned ofFlag)
    {
        switch(ofFlag)
        {
            case ofNoOutput              : return "Disable writting outputs";
            case ofMain                  : return "Print only main files";
            case ofHtml                  : return "Print output in html format";
            case ofMd                    : return "Print output in md format (overrides html option)";
            case ofSkipUndocumented      : return "Skip undocumented";
            case ofRemovePath            : return "Remove path from file names in output";
            case ofSplitGroups           : return "Split to groups";
            case ofTodo                  : return "Scan for TODOs";

            default                      : return "Multiple flags taken!!!";
        }
    }

    #define UMBA_PRETTY_HEADERS_APPC_CONFIG_DECLARE_SET_GET_OPT( opt ) \
                void setOpt##opt( bool q ) { ofSet(of##opt,q);      }  \
                bool getOpt##opt( )  const { return ofGet(of##opt); }

    UMBA_PRETTY_HEADERS_APPC_CONFIG_DECLARE_SET_GET_OPT(NoOutput)
    UMBA_PRETTY_HEADERS_APPC_CONFIG_DECLARE_SET_GET_OPT(Main)
    UMBA_PRETTY_HEADERS_APPC_CONFIG_DECLARE_SET_GET_OPT(Html)
    UMBA_PRETTY_HEADERS_APPC_CONFIG_DECLARE_SET_GET_OPT(Md)
    UMBA_PRETTY_HEADERS_APPC_CONFIG_DECLARE_SET_GET_OPT(SkipUndocumented)
    UMBA_PRETTY_HEADERS_APPC_CONFIG_DECLARE_SET_GET_OPT(RemovePath)
    UMBA_PRETTY_HEADERS_APPC_CONFIG_DECLARE_SET_GET_OPT(SplitGroups)
    UMBA_PRETTY_HEADERS_APPC_CONFIG_DECLARE_SET_GET_OPT(Todo)


    void setOptQuet(bool q) { UMBA_USED(q); setVerbosityLevel(VerbosityLevel::quet); }
    //bool getOptQuet( )  const { return testVerbosity(VerbosityLevel::quet); }

    bool getOptShowConfig( )  const { return testVerbosity(VerbosityLevel::config); }

    //------------------------------



    //------------------------------
    std::string getScanRelativeName( std::string name ) const
    {
        for(const auto &path : scanPaths)
        {
            if (umba::filename::isSubPathName(path, name, &name, '/'))
                break;
        }

        return name;
    }

    #if 0
    std::string getOutputRelativePath( std::string path ) const
    {
        // if (umba::filename::isSubPathName(outputPath, path, &path))
        //     return path;
        return std::string();
    }

    std::string getOutputPath( std::string path ) const
    {
        //return umba::filename::makeCanonical( umba::filename::appendPath(outputPath, path) );
        return std::string();
    }
    #endif
    //------------------------------



    //------------------------------
    template<typename StreamType>
    StreamType& printVerbosity( StreamType &s ) const
    {
        s << "Verbosity         : " << VerbosityLevel_toStdString(verbosityLevel) << "\n";
        return s;
    }

    template<typename StreamType>
    StreamType& print( StreamType &s ) const
    {
        s << "\n";
        printVerbosity(s) << "\n";

        //------------------------------

        s << "Output Name       : " << outputName << "\n"; // endl;
        s << "\n";
        s << "Update            : " << (updateMode?"true":"false") << "\n"; // endl;
        s << "Update from       : " << updateFromFile << "\n"; // endl;
        s << "\n";

        s << "Filename width    : ";
        if (filenameWidth)
           s << filenameWidth;
        else
           s << "default";
        s << "\n"; // endl;

        s << "Description width : ";
        if (descriptionWidth)
           s << "format to " << descriptionWidth << " columns";
        else
           s << "format to single line";
        s << "\n"; // endl;

        s << "\n";

        s << "Option Flags      :\n";
        s << "    " << getOptNameString(ofNoOutput)            << ": " << getOptValAsString(optionFlags&ofNoOutput) << "\n";
        s << "    " << getOptNameString(ofMain)                << ": " << getOptValAsString(optionFlags&ofMain) << "\n";
        s << "    " << getOptNameString(ofHtml)                << ": " << getOptValAsString(optionFlags&ofHtml) << "\n";
        s << "    " << getOptNameString(ofMd)                  << ": " << getOptValAsString(optionFlags&ofMd) << "\n";
        s << "    " << getOptNameString(ofSkipUndocumented)    << ": " << getOptValAsString(optionFlags&ofSkipUndocumented) << "\n";
        s << "    " << getOptNameString(ofRemovePath)          << ": " << getOptValAsString(optionFlags&ofRemovePath) << "\n";
        s << "    " << getOptNameString(ofSplitGroups)         << ": " << getOptValAsString(optionFlags&ofSplitGroups) << "\n";
        s << "    " << getOptNameString(ofTodo)                << ": " << getOptValAsString(optionFlags&ofTodo) << "\n";

        s << "\n";

        //------------------------------

        s << "\n";
        s << "Scan Paths:\n";
        for(auto scanPath : scanPaths)
        {
            s << "    " << scanPath << " (" << umba::filename::makeCanonical(scanPath) << ")\n";
        }

        s << "\n";

        //------------------------------

        /*
        if (macros.empty())
            s << "Macros : <EMPTY>";
        else
        {
            s << "Macros:\n";
            for(auto [key,val] : macros)
            {
                s << "    '" << key << "' : '" << val << "'\n";
            }
        }

        s << "\n";
        */

        //------------------------------

        s << "Include File Masks:\n";
        for(auto includeFileMask : includeFilesMaskList)
        {
            //auto regexStr = expandSimpleMaskToEcmaRegex(includeFileMask);
            auto regexStr = umba::regex_helpers::expandSimpleMaskToEcmaRegex(includeFileMask, true /* useAnchoring */, true /* allowRawRegexes */);
            s << "    '" << includeFileMask;

            bool isRaw = false;
            if (umba::string_plus::starts_with<std::string>(includeFileMask,umba::regex_helpers::getRawEcmaRegexPrefix<std::string>()))
                isRaw = true;

            if (regexStr==includeFileMask || isRaw)
                s << "'\n";
            else
            {
                s << "', corresponding mECMA regexp: '"
                  << regexStr
                  << "'\n";
            }
        }

        s << "\n";

        //------------------------------

        s << "Exclude File Masks:\n";
        for(auto excludeFileMask : excludeFilesMaskList)
        {
            // auto regexStr = expandSimpleMaskToEcmaRegex(excludeFileMask);
            auto regexStr = umba::regex_helpers::expandSimpleMaskToEcmaRegex( excludeFileMask, true /* useAnchoring */, true /* allowRawRegexes */ );

            s << "    '" << excludeFileMask;

            bool isRaw = false;
            if (umba::string_plus::starts_with<std::string>(excludeFileMask,umba::regex_helpers::getRawEcmaRegexPrefix<std::string>()))
                isRaw = true;

            if (regexStr==excludeFileMask || isRaw)
                s << "'\n";
            else
            {
                s << "', corresponding mECMA regexp: '"
                  << regexStr
                  << "'\n";
            }
        }

        s << "\n";

        //------------------------------



        return s;
    }


    AppConfig getAdjustedConfig( const umba::program_location::ProgramLocation<std::string> &programLocation ) const
    {
        UMBA_USED(programLocation);

        AppConfig appConfig;

        //appConfig.macros             = macros;
        //appConfig.keepGeneratedFiles = keepGeneratedFiles;
        appConfig.scanPaths          = scanPaths;
        appConfig.excludeDirs        = excludeDirs;
        appConfig.outputName         = outputName;
        appConfig.updateMode         = updateMode    ;
        appConfig.updateFromFile     = updateFromFile;
        appConfig.optionFlags        = optionFlags;
        appConfig.verbosityLevel     = verbosityLevel;
        appConfig.doxificationMode   = doxificationMode;
        appConfig.outputLinefeed     = outputLinefeed;
        appConfig.bOverwrite         = bOverwrite;

        appConfig.filenameWidth      = filenameWidth   ;
        appConfig.descriptionWidth   = descriptionWidth;

        appConfig.notesConfig        = notesConfig    ;


        if (appConfig.filenameWidth==0)
        {
            if (optionFlags&ofRemovePath)
                appConfig.filenameWidth = 16;
            else
                appConfig.filenameWidth = 32;
        }

        if (appConfig.filenameWidth > 64)
            appConfig.filenameWidth = 64;

        if (appConfig.descriptionWidth > 160)
            appConfig.descriptionWidth = 160;

        if (appConfig.descriptionWidth < 1)
            appConfig.descriptionWidth = 64;


        appConfig.entrySignatures = entrySignatures;
        if (appConfig.entrySignatures.empty())
        {
            appConfig.entrySignatures.emplace_back(TextSignature("void main("));
            appConfig.entrySignatures.emplace_back(TextSignature("int main("));
            // appConfig.entryNames["main"].insert("int");
            // appConfig.entryNames["main"].insert("void");
        }


        if (appConfig.scanPaths.empty())
            appConfig.scanPaths.push_back(umba::filesys::getCurrentDirectory());

        for(auto includeFileMask: includeFilesMaskList)
        {
            if (umba::string_plus::starts_with(includeFileMask,umba::regex_helpers::getRawEcmaRegexPrefix<std::string>()))
                appConfig.includeFilesMaskList.push_back(includeFileMask); // keep regex as is
            else
                appConfig.includeFilesMaskList.push_back( umba::filename::normalizePathSeparators(includeFileMask,'/') );
        }

        for(auto excludeFileMask: excludeFilesMaskList)
        {
            if (umba::string_plus::starts_with(excludeFileMask,umba::regex_helpers::getRawEcmaRegexPrefix<std::string>()))
                appConfig.excludeFilesMaskList.push_back(excludeFileMask); // keep regex as is
            else
                appConfig.excludeFilesMaskList.push_back( umba::filename::normalizePathSeparators(excludeFileMask,'/') );
        }

        return appConfig;
    }

}; // struct AppConfig


template<typename StreamType> inline
StreamType& operator<<(StreamType &s, const AppConfig &cfg)
{
    return cfg.print( s );
}



