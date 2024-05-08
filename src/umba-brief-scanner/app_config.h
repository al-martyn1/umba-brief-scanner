#pragma once

#include <string>
#include <vector>
#include <map>

#include "umba/program_location.h"
#include "umba/enum_helpers.h"
#include "umba/flag_helpers.h"

#include "umba/regex_helpers.h"

#include "enums_manual.h"


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
    static const unsigned                    ofHtml                  = 0x0040; // Print output in html format
    static const unsigned                    ofSkipUndocumented      = 0x0080; // Skip undocumented files
    static const unsigned                    ofRemovePath            = 0x0100; // Remove path from output names

    static const unsigned                    ofSplitGroups           = 0x0200; // Remove path from output names

    //------------------------------
    // !!! Не забывать копировать и/или подготавливать поля класса в функции getAdjustedConfig
    //------------------------------
    umba::macros::StringStringMap<std::string>  macros; // не используем

    std::map< std::string,std::set<std::string> >  entryNames; // не используем



    //------------------------------
    std::vector<std::string>                 includeFilesMaskList;

    std::vector<std::string>                 excludeFilesMaskList;

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
            case ofMain                  : return "Print only main filess";
            case ofHtml                  : return "Print output in html format";
            case ofSkipUndocumented      : return "Skip undocumented";
            case ofRemovePath            : return "Remove path from file names in output";
            case ofSplitGroups           : return "Split to groups";

            default                      : return "Multiple flags taken!!!";
        }
    }

    #define UMBA_PRETTY_HEADERS_APPC_CONFIG_DECLARE_SET_GET_OPT( opt ) \
                void setOpt##opt( bool q ) { ofSet(of##opt,q);      }  \
                bool getOpt##opt( )  const { return ofGet(of##opt); }

    UMBA_PRETTY_HEADERS_APPC_CONFIG_DECLARE_SET_GET_OPT(NoOutput)
    UMBA_PRETTY_HEADERS_APPC_CONFIG_DECLARE_SET_GET_OPT(Main)
    UMBA_PRETTY_HEADERS_APPC_CONFIG_DECLARE_SET_GET_OPT(Html)
    UMBA_PRETTY_HEADERS_APPC_CONFIG_DECLARE_SET_GET_OPT(SkipUndocumented)
    UMBA_PRETTY_HEADERS_APPC_CONFIG_DECLARE_SET_GET_OPT(RemovePath)
    UMBA_PRETTY_HEADERS_APPC_CONFIG_DECLARE_SET_GET_OPT(SplitGroups)
    

    void setOptQuet( bool q ) { setVerbosityLevel(VerbosityLevel::quet);  }
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
        s << "Verbosity      : " << VerbosityLevel_toStdString(verbosityLevel) << "\n";
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
        s << "    " << getOptNameString(ofSkipUndocumented)    << ": " << getOptValAsString(optionFlags&ofSkipUndocumented) << "\n";
        s << "    " << getOptNameString(ofRemovePath)          << ": " << getOptValAsString(optionFlags&ofRemovePath) << "\n";
        s << "    " << getOptNameString(ofSplitGroups)         << ": " << getOptValAsString(optionFlags&ofSplitGroups) << "\n";
        

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
        AppConfig appConfig;

        //appConfig.macros             = macros;
        //appConfig.keepGeneratedFiles = keepGeneratedFiles;
        appConfig.scanPaths          = scanPaths;
        appConfig.outputName         = outputName;
        appConfig.updateMode         = updateMode    ;
        appConfig.updateFromFile     = updateFromFile;
        appConfig.optionFlags        = optionFlags;
        appConfig.verbosityLevel     = verbosityLevel;

        appConfig.filenameWidth      = filenameWidth   ;
        appConfig.descriptionWidth   = descriptionWidth;

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


        appConfig.entryNames         = entryNames;
        if (appConfig.entryNames.empty())
        {
            appConfig.entryNames["main"].insert("int");
            appConfig.entryNames["main"].insert("void");
        }


        if (appConfig.scanPaths.empty())
            appConfig.scanPaths.push_back(umba::filesys::getCurrentDirectory<std::string>());

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



