/*! \file
    \brief Утилита umba-brief-scanner - сканирование исходных текстов на C/C++ и получение списка кратких описаний файлов исходных текстов
 */

#include "umba/umba.h"
//
#include "umba/simple_formatter.h"
#include "umba/char_writers.h"

#include "umba/debug_helpers.h"
#include "umba/shellapi.h"
#include "umba/app_main.h"

#include <iostream>
#include <iomanip>
#include <string>
// #include <cstdio>
// #include <filesystem>

#include "umba/debug_helpers.h"
#include "umba/string_plus.h"
#include "umba/program_location.h"
#include "umba/scope_exec.h"
#include "umba/macro_helpers.h"
#include "umba/macros.h"
#include "umba/info_log.h"
#include "umba/filesys_scanners.h"

#include "umba/time_service.h"
#include "umba/text_utils.h"
#include "marty_cpp/marty_cpp.h"
#include "marty_cpp/marty_enum.h"
#include "marty_cpp/marty_flags.h"
#include "marty_cpp/src_normalization.h"

#include "encoding/encoding.h"

#include "marty_utf/utf.h"

#include "brief_info.h"


umba::StdStreamCharWriter coutWriter(std::cout);
umba::StdStreamCharWriter cerrWriter(std::cerr);
umba::NulCharWriter       nulWriter;

umba::SimpleFormatter umbaLogStreamMsg(&coutWriter);
umba::SimpleFormatter umbaLogStreamErr(&cerrWriter);
umba::SimpleFormatter umbaLogStreamNul(&nulWriter);

auto &logMsg = umbaLogStreamMsg;

bool logWarnType   = true;
bool umbaLogGccFormat  = false;
bool umbaLogSourceInfo = false;


#include "log.h"
// #include "scan_folders.h"

//#include "scan_sources.h"

//umba::program_location::ProgramLocation<std::string>   programLocationInfo;


#include "umba/cmd_line.h"


#include "app_ver_config.h"
#include "umba/cli_tool_helpers.h"

#include "arg_parser.h"


std::string htmlEscape(const std::string &s)
{
    std::string resStr; resStr.reserve(s.size());

    for(auto ch: s)
    {
        switch(ch)
        {
            case '<' : resStr.append("&lt;"  ); break;
            case '>' : resStr.append("&gt;"  ); break;
            case '\'': resStr.append("&apos;"); break;
            case '\"': resStr.append("&quot;"); break;
            case '&' : resStr.append("&amp;" ); break;
            // case '': resStr.append(""); break;
            default: resStr.append(1, ch);
        }
    }

    return resStr;
}


// int main(int argc, char* argv[])
int unsafeMain(int argc, char* argv[]);

UMBA_APP_MAIN()
{
    try
    {
        auto res = unsafeMain(argc, argv);
        return res;
    }
    catch(const std::exception &e)
    {
        LOG_ERR_OPT << "Exception: " << e.what() << "\n";
    }
    catch(...)
    {
        LOG_ERR_OPT << "Exception: " << "unknown exception" << "\n";
    }

    return 66;
}

int unsafeMain(int argc, char* argv[])
{
    umba::time_service::init();
    umba::time_service::start();

    umba::time_service::TimeTick startTick = umba::time_service::getCurTimeMs();
    UMBA_USED(startTick);


    using namespace umba::omanip;


    auto argsParser = umba::command_line::makeArgsParser( ArgParser()
                                                        , CommandLineOptionCollector()
                                                        , argc, argv
                                                        , umba::program_location::getProgramLocation
                                                            ( argc, argv
                                                            , false // useUserFolder = false
                                                            //, "" // overrideExeName
                                                            )
                                                        );

    if (umba::isDebuggerPresent())
    {
        std::string cwd;
        std::string rootPath = umba::shellapi::getDebugAppRootFolder(&cwd);
        std::cout << "App Root Path: " << rootPath << "\n";
        std::cout << "Working Dir  : " << cwd << "\n";

        #if 1
        #if (defined(WIN32) || defined(_WIN32))


            // if (winhelpers::isProcessHasParentOneOf({"devenv"}))
            // {
            //     // По умолчанию студия задаёт текущим каталогом На  уровень выше от того, где лежит бинарник
            //     rootPath = umba::filename::makeCanonical(umba::filename::appendPath<std::string>(cwd, "..\\..\\..\\"));
            //     //argsParser.args.push_back("--batch-output-root=D:/temp/mdpp-test");
            // }
            // else if (winhelpers::isProcessHasParentOneOf({"code"}))
            // {
            //     // По умолчанию VSCode задаёт текущим каталогом тот, где лежит бинарник
            //     rootPath = umba::filename::makeCanonical(umba::filename::appendPath<std::string>(cwd, "..\\..\\..\\..\\"));
            //     //argsParser.args.push_back("--batch-output-root=C:/work/temp/mdpp-test");
            //
            // }
            // else
            // {
            //     //rootPath = umba::filename::makeCanonical(umba::filename::appendPath<std::string>(cwd, "..\\..\\..\\"));
            // }
            //
            // //#endif
            //
            // if (!rootPath.empty())
            //     rootPath = umba::filename::appendPathSepCopy(rootPath);
            //
            // argsParser.args.clear();
            //
            // argsParser.args.push_back("--verbose=detailed");
            //
            // // argsParser.args.push_back("--batch-exclude-dir=_libs,libs,_lib,lib,tests,test,rc,_generators,_distr_conf,src,.msvc2019,boost,icons");
            // argsParser.args.push_back("@" + rootPath + "\\umba-brief-scanner.rsp");
            // argsParser.args.push_back("--overwrite");
            // //argsParser.args.push_back("--doxyfication=always");
            // argsParser.args.push_back("--scan=" + rootPath + "/src");
            // //argsParser.args.push_back("--scan=" + rootPath + "/_libs");
            // argsParser.args.push_back(rootPath + "/doc/_sources_brief.txt");
            // //argsParser.args.push_back("tests/doxy");


            // argsParser.args.clear();
            // argsParser.args.push_back("@..\\tests\\data\\test01.rsp");
            //argsParser.args.push_back("@..\\make_sources_brief.rsp");
            // argsParser.args.push_back(umba::string_plus::make_string(""));
            // argsParser.args.push_back(umba::string_plus::make_string(""));
            // argsParser.args.push_back(umba::string_plus::make_string(""));



            argsParser.args.clear();
            // argsParser.args.push_back("--help");

            argsParser.args.push_back("--overwrite");

            #if 0
            argsParser.args.push_back("@" + rootPath + "umba-brief-scanner.rsp");
            argsParser.args.push_back("--scan=" + rootPath + "/src");
            argsParser.args.push_back("--scan=" + rootPath + "/_src");
            argsParser.args.push_back("--scan=" + rootPath + "/examples");
            argsParser.args.push_back(rootPath + "/doc/_sources_brief.txt");
            #endif

            // F:\_github\mca\roboware
            // F:\_github\umba-tools\umba-brief-scanner

            //rootPath += "/../../mca/roboware";

            rootPath = umba::filename::makeCanonical(rootPath);
            argsParser.args.push_back("@" + rootPath + "/umba-brief-scanner.rsp");
            argsParser.args.push_back(rootPath + "/doc/_sources_brief.txt");


            // argsParser.args.push_back("--md");
            // argsParser.args.push_back("--scan=" + rootPath + "/_src");
            argsParser.args.push_back("--scan=F:\\_github\\mca\\roboware\\src");
            

            argsParser.args.push_back("--scan-notes");
            argsParser.args.push_back("--notes-output-path=" + rootPath + "/doc");

            //argsParser.args.push_back("--main");

// --scan=src
// --scan=_libs\unicont
// --scan=_libs\unicont_boost
// --scan=_libs\unicont_ubus



// @if exist "%~dp0..\umba-brief-scanner.rsp" @set RSP="@%~dp0..\umba-brief-scanner.rsp"
// @rem echo RSP="@%RSP%"
// umba-brief-scanner --where
// umba-brief-scanner --overwrite "%RSP%"        "--scan=%~dp0\../src" "--scan=%~dp0\../_src" "--scan=%~dp0\../examples" "%~dp0\../doc/_sources_brief.txt"
// umba-brief-scanner --overwrite "%RSP%" --html "--scan=%~dp0\../src" "--scan=%~dp0\../_src" "--scan=%~dp0\../examples" "%~dp0\../doc/_sources_brief.html"
// umba-brief-scanner --overwrite "%RSP%" --md   "--scan=%~dp0\../src" "--scan=%~dp0\../_src" "--scan=%~dp0\../examples" "%~dp0\../doc/_sources_brief.md_"


        #endif
        #endif

    }


    // programLocationInfo = argsParser.programLocationInfo;

    // Job completed - may be, --where option found
    if (argsParser.mustExit)
        return 0;

    if (!argsParser.quet)
    {
        umba::cli_tool_helpers::printNameVersion(umbaLogStreamMsg);
    }

    if (!argsParser.parseStdBuiltins())
        return 1;
    if (argsParser.mustExit)
        return 0;

    if (!argsParser.parse())
        return 1;
    if (argsParser.mustExit)
        return 0;


    appConfig = appConfig.getAdjustedConfig(argsParser.programLocationInfo);
    //pAppConfig = &appConfig;

    if (appConfig.getOptShowConfig())
    {
        umba::info_log::printSectionHeader(logMsg, "Actual Config");
        // logMsg << appConfig;
        appConfig.print(logMsg) << "\n";
    }

    if (appConfig.outputName.empty())
    {
        LOG_ERR_OPT << "output name (or path, while doxification mode is on) not taken" << endl;
        return 1;
    }

    std::unordered_map<std::string, std::string>  prevBriefFileInfo; // Прочитанное из файла c brief'ами

    bool doxyficationMode = !(appConfig.doxificationMode==DoxificationMode::noDoxyfication || appConfig.doxificationMode==DoxificationMode::invalid);


    // Нет доксификации или режим доксификации инвалидный
    if (appConfig.updateMode && !doxyficationMode)
    {
        if (appConfig.updateFromFile.empty())
        {
            using namespace umba::filename;
            std::string ext = getFileExtention(appConfig.outputName);
            //if (ext=="txt" || ext==".txt" || ext=="TXT" || ext==".TXT")
            if (marty_cpp::toLower(ext)=="txt")
            {
                appConfig.updateFromFile = appConfig.outputName;
            }
            else
            {
                appConfig.updateFromFile = appendExtention(appendPath(getPath(appConfig.outputName), getName(appConfig.outputName)), std::string("txt"));
            }
        }

        std::vector<char> fileData;
        if (umba::filesys::readFile(appConfig.updateFromFile,fileData))
        {
            std::string updateFromText = marty_cpp::normalizeCrLfToLf(std::string(fileData.begin(), fileData.end()), 0);
            std::vector<std::string> updateFromLines = marty_cpp::splitToLinesSimple(updateFromText, false);

            std::string curName;
            std::string curText;

            auto addFileInfo = [&]()
            {
                if (!curName.empty()  /* && !curText.empty() */ )
                {
                    prevBriefFileInfo[curName] = curText;
                }

                curName.clear();
                curText.clear();
            };

            bool skip = true;
            for(const auto &line : updateFromLines)
            {
                if (umba::string_plus::starts_with(line, "--------"))
                {
                    skip = false;
                    continue;
                }

                if (skip)
                {
                    continue;
                }

                if (umba::string_plus::trim_copy(line).empty())
                {
                    addFileInfo(); // если было что-то
                    continue;
                }

                if (line[0]=='#' || line[0]==';')
                {
                    // Это коммент или группа
                    addFileInfo(); // если было что-то
                    continue;
                }

                // строка не пустая

                if (line[0]==' ' || line[0]=='\t')
                {
                    // это продолжение описания
                    if (!curText.empty())
                    {
                        curText.append(1, ' ');
                    }

                    curText.append(umba::string_plus::trim_copy(line));
                }
                else
                {
                    // Новое описание
                    addFileInfo(); // если было что-то

                    std::vector<std::string> nameAndText = marty_cpp::simple_string_split(line, " - ", 1 /* nSplits */);
                    if (nameAndText.size()>0)
                    {
                        curName = umba::string_plus::trim_copy(nameAndText[0]);
                    }

                    if (nameAndText.size()>1)
                    {
                        curText = umba::string_plus::trim_copy(nameAndText[1]);
                    }
                }

            } // for

            addFileInfo(); // если было что-то

        } // if readFile

    } // if (appConfig.updateMode && !doxyficationMode)


    auto &infoLog = argsParser.quet ? umbaLogStreamNul : umbaLogStreamMsg;


    std::vector<std::string> foundFiles, foundFilesFolders, excludedFiles;
    std::set<std::string>    foundExtentions;
    //umba::scanners::scanFolders(appConfig, logMsg, foundFiles, excludedFiles, foundExtentions);
    umba::filesys::scanners::scanFolders( appConfig
                                        , infoLog // argsParser.quet ? umbaLogStreamNul : umbaLogStreamMsg
                                        , foundFiles
                                        , excludedFiles
                                        , foundExtentions
                                        , &foundFilesFolders // а пусть всегда, не жалко // doxyficationMode ? &foundFilesFolders : 0 // для доксификации нам нужны пути, где файлы были найдены
                                        , appConfig.excludeDirs // std::vector<std::string>() //  excludeFoldersExact
                                        , true // scanRecurse
                                        , true // logFoundHeader
                                        , false // addFolders to list of found names
                                        );
    if (foundFiles.size()!=foundFilesFolders.size())
    {
        LOG_ERR_OPT << "Number of found folders mismatch number of found files" << endl;
        return 1;
    }

    if (appConfig.testVerbosity(VerbosityLevel::detailed))
    {
        umba::info_log::printSectionHeader(logMsg, "Paths for scan:");

        for(const auto &p : appConfig.scanPaths)
        {
            logMsg << "    " << umba::filename::makeCanonical(p) << " (" << p << ")" << endl;
        }


        if (!foundFiles.empty())
            umba::info_log::printSectionHeader(logMsg, "Files for Processing");

        for(const auto & name : foundFiles)
        {
            logMsg << name << endl;
        }


        if (!excludedFiles.empty())
            umba::info_log::printSectionHeader(logMsg, "Files Excluded from Processing");

        for(const auto & name : excludedFiles)
        {
            logMsg << name << endl;
        }


        if (!foundExtentions.empty())
            umba::info_log::printSectionHeader(logMsg, "Found File Extentions");

        for(const auto & ext : foundExtentions)
        {
            if (ext.empty())
                logMsg << "<EMPTY>" << endl;
            else
                logMsg << "." << ext << endl;
        }
    }


    if (appConfig.testVerbosity(VerbosityLevel::detailed))
        umba::info_log::printSectionHeader(logMsg, "Processing");


    // Читаем брифы

    std::map<std::string, BriefInfo>  briefInfo;
    NotesCollection                   notesCollection;

    encoding::EncodingsApi* pEncApi = encoding::getEncodingsApi();

    // std::vector<std::string> foundFiles, foundFilesFolders
    std::vector<std::string>::const_iterator ffIt  = foundFiles.begin();
    std::vector<std::string>::const_iterator fffIt = foundFilesFolders.begin();
    //for(const auto & filename : foundFiles)
    for(; ffIt!=foundFiles.end() && fffIt!=foundFilesFolders.end(); ++ffIt, ++fffIt)
    {
        const std::string & filename   = *ffIt;
        const std::string & fileFolder = *fffIt;
        // logMsg << name << endl;
        //std::vector<char> filedata;
        std::string filedataStr;
        if (!umba::filesys::readFile( filename, filedataStr ))
        {
            LOG_WARN_OPT("open-file-failed") << "failed to open file '" << filename << "'\n";
            continue;
        }


        //auto filedataStr = std::string(filedata.begin(), filedata.end());

        size_t bomSize = 0;
        std::string detectRes = pEncApi->detect( filedataStr, bomSize );

        if (bomSize)
        {
            filedataStr.erase(0,bomSize);
        }

        auto cpId = pEncApi->getCodePageByName(detectRes);

        std::string filedataStrUtf8 = pEncApi->convert( filedataStr, cpId, encoding::EncodingsApi::cpid_UTF8 );
        //filedata = std::vector<char>(filedataStrUtf8.begin(), filedataStrUtf8.end());

        //filedataStr = marty_cpp::normalizeCrLfToLf(filedataStr);
        filedataStr = marty_cpp::normalizeCrLfToLf(filedataStrUtf8);

        bool bFound = false;
        UMBA_USED(bFound);
        BriefInfo  info;
        info.fileName       = umba::filename::makeCanonical(filename);
        info.encodingName   = detectRes;
        // std::string curFile = info.fileName;
        // int lineNo = 1;

#define UMBA_BRIEF_SCANNER_LOG_EXCEPTION(where, what, ma_briefInfo) \
        do{                                                         \
            std::string curFile = ma_briefInfo.fileName;            \
            std::size_t lineNo = 1u;                                \
            LOG_ERR << where << ": " << what << "\n";               \
            LOG_ERR << "File encoding: " << ma_briefInfo.encodingName << "\n"; \
        }                                                           \
        while(0)

        try
        {
            #if 0
            auto nameOnly = umba::filename::getFileName(filename);
            if (nameOnly=="main.cpp")
            {
                UMBA_DEBUGBREAK();
            }
            #endif

            std::vector<NoteInfo> notes;
            bFound = findBriefInfo( filedataStr, appConfig.entrySignatures, info, appConfig.notesConfig, notes, filename, fileFolder );
            notesCollection.addNotes(notes);
        }
        catch(const std::exception &e)
        {
            // UMBA_DEBUGBREAK();
            UMBA_BRIEF_SCANNER_LOG_EXCEPTION("Exception in findBriefInfo", e.what(), info );
            continue;
        }
        catch(...)
        {
            UMBA_BRIEF_SCANNER_LOG_EXCEPTION("Exception in findBriefInfo", "unknown error", info );
            continue;
        }

        briefInfo[info.fileName] = info;

        if (appConfig.testVerbosity(VerbosityLevel::detailed))
        {
            logMsg << (info.briefFound ? '+' : '-')
                   << (info.entryPoint ? 'E' : ' ')
                   << "    " << filename
                   << "\n";
        }

    }


    // Нет доксификации или режим доксификации инвалидный
    if (!doxyficationMode)
    {
        if (appConfig.getOptTodo())
        {
            try
            {
                //if (!foundFiles.empty())
                if (appConfig.testVerbosity(VerbosityLevel::detailed))
                    umba::info_log::printSectionHeader(logMsg, "Writing collected notes:");
                std::vector<std::string> writtenFiles;
                notesCollection.sortNotes();
                notesCollection.serializeToFiles(appConfig, writtenFiles);

                if (appConfig.testVerbosity(VerbosityLevel::detailed))
                {
                    if (writtenFiles.empty())
                        logMsg << "Note: there is no notes files written\n";
                    else
                    {
                        for(auto &&f: writtenFiles)
                            logMsg << "  " << f << "\n";
                    }
                }
            }
            catch(const std::runtime_error &e)
            {
                LOG_ERR_OPT << e.what() << "\n";
                return 1;
            }

        }

        std::ofstream infoStream;
        if (!appConfig.getOptNoOutput())
        {

            // if (!createDirectory(path))
            // {
            //     LOG_WARN_OPT("create-dir-failed") << "failed to create directory: " << path << endl;
            //     continue;
            // }

            if (umba::filesys::isPathExist(appConfig.outputName) && !appConfig.bOverwrite)
            {
                LOG_WARN_OPT("create-file-failed") << "failed to create output file: " << appConfig.outputName << " - file allready exist" << endl;
                 return 1;
                //bool isPathFile(const StringType &path)
            }


            infoStream.open( appConfig.outputName, std::ios_base::out | std::ios_base::trunc );
            if (!infoStream)
            {
                LOG_WARN_OPT("create-file-failed") << "failed to create output file: " << appConfig.outputName << endl;
                return 1;
            }
        }


        std::string titleStr = "Brief Description for Project Sources";
        std::string sepLine  = "-------------------------------------";

        if (appConfig.getOptMd())
        {
            infoStream << "---\n"
                       << "Title: " << titleStr << "\n"
                       << "Generator: " << MD_META_GENERATOR_NAME << "\n"
                       << "---\n";
        }
        else if (appConfig.getOptHtml())
        {
            infoStream << "<!DOCTYPE html>\n<html>\n";
            infoStream << "<head>\n<title>" << titleStr << "</title>\n</head>\n";
            infoStream << "<body>\n";
        }
        else
        {
            infoStream << titleStr << "\n" << sepLine << "\n\n";
        }


        /* Нужно прочитанное при чтении старых описаний проверять, есть ли такой файл в новом скане.
           Если в скане его нет, то файл пропал - изменилась стуктура каталогов
         */
        std::unordered_set<std::string> relNames; // Имена, такие же как и прежнем brief'е

        std::string prevFilePath;

        auto printInfo = [&]( bool bMain )
        {
            umba::StdStreamCharWriter infoWriter(infoStream);
            umba::SimpleFormatter uinfoStream(&infoWriter);


            if (appConfig.getOptMd())
            {
            }
            else if (appConfig.getOptHtml())
            {
                uinfoStream << "<table><tbody>\n";
            }
            else
            {
            }

            bool bFirstItem = true;

            //std::map<std::string, BriefInfo>
            for( const auto& [name,info] : briefInfo)
            {
                if (info.entryPoint!=bMain)
                    continue;

                if (appConfig.getOptSkipUndocumented())
                {
                    if (!info.briefFound)
                        continue;
                }

                // umba::StdStreamCharWriter infoWriter(infoStream);
                // umba::SimpleFormatter uinfoStream(&infoWriter);

                auto relName = appConfig.getScanRelativeName(name);

                auto relPath = umba::filename::getPath(relName);

                if (appConfig.getOptSplitGroups())
                {
                    // bFirstItem
                    bool prevFilePathEmpty    =  prevFilePath.empty();
                    bool prevFilePathNotEmpty = !prevFilePathEmpty;     UMBA_USED(prevFilePathNotEmpty);
                    bool prevPathNotSame      = prevFilePath!=relPath;
                    bool prevPathEmpty        = prevFilePath.empty();   UMBA_USED(prevPathEmpty);

                    bool partBreak = false;

                    if (bFirstItem)
                        partBreak = true;

                    if (prevPathNotSame)
                        partBreak = true;

                    //if (!bFirstItem && ((!prevFilePath.empty() && prevFilePath!=relPath) || prevFilePath.empty()))
                    if (partBreak)
                    {
                        if (appConfig.getOptMd())
                        {
                            if (relPath.empty())
                            {
                                if (bMain)
                                    uinfoStream << "\n# " << "Entry points" << "\n\n";
                                else
                                    uinfoStream << "\n";
                            }
                            else
                            {
                                uinfoStream << "\n# " << relPath << (bMain?" (Entry points)":"") << "\n\n";
                            }
                        }
                        else if (appConfig.getOptHtml())
                        {
                            // if (bFirstItem || relPath.empty())
                            uinfoStream << "<tr><td><br><b>" << htmlEscape(relPath) << (bMain?" (Entry points)":"") << "</b><br></td><td>" << "</td></tr>\n";
                        }
                        else // Text
                        {
                            if ( /* bFirstItem ||  */ relPath.empty())
                                uinfoStream << (bMain?"# Entry points":"") << "\n";
                            else
                                uinfoStream << "\n# " << relPath << (bMain?" (Entry points)":"") << "\n";
                        }
                    }
                }

                bFirstItem = false;

                prevFilePath = relPath;

                if (appConfig.getOptRemovePath())
                    relName = umba::filename::getFileName( relName );

                relNames.insert(relName);

                std::string infoText = info.infoText;

                std::unordered_map<std::string, std::string>::const_iterator uit = prevBriefFileInfo.find(relName);
                if (uit!=prevBriefFileInfo.end())
                {
                    if (/* !info.briefFound || */ infoText.empty())
                    {
                        infoText = uit->second;
                    }
                }


                if (appConfig.getOptMd())
                {
                    uinfoStream << " - **" << relName << "** - " << info.infoText << "\n";
                }
                else if (appConfig.getOptHtml())
                {
                    //TODO: !!! Add HTML output here
                    uinfoStream << "<tr><td>" << htmlEscape(relName) << "</td><td>" << htmlEscape(info.infoText) << "</td></tr>\n";
                }
                else // txt
                {
                    int fnw = (int)appConfig.filenameWidth;

                    if (appConfig.descriptionWidth==0)
                    {
                        uinfoStream << width(fnw) << left << relName << " - " << infoText << "\n";
                    }
                    else
                    {
                        std::string formattedParas;
                        std::string textWithIndent;

                        try
                        {
                            formattedParas = umba::text_utils::formatTextParas<marty_utf::SymbolLenCalculatorEncodingUtf8>( infoText, appConfig.descriptionWidth
                                                                                                                          , umba::text_utils::TextAlignment::left
                                                                                                                          , marty_utf::SymbolLenCalculatorEncodingUtf8()
                                                                                                                          );
                        }
                        catch(const std::exception &e)
                        {
                            UMBA_BRIEF_SCANNER_LOG_EXCEPTION("Exception in formatTextParas", e.what(), info );
                            continue;
                        }
                        catch(...)
                        {
                            UMBA_BRIEF_SCANNER_LOG_EXCEPTION("Exception in formatTextParas", "unknown error", info );
                            continue;
                        }


                        try
                        {
                            // Не будем париться - первая строка может свисать справа, ну и фик с ним

                            //TODO: !!! Не корректно определяется длина строк в многобайтной кодировке
                            //          Мой поток вывода (umba::SimpleFormatter) - также некорректно работает с многобайтной кодировкой,
                            //          но это обычно не проблема, если имена файлов  исходников используют только английский алфавит.
                            //          При форматировании же текста кодировка зависит от кодировки исходника - может быть как однобайтной,
                            //          так и многобайтной (UTF-8, например, и в линуксе это обычно уже стандарт)
                            //          Поэтому, если где-то используются русскоязычные описания и используется форматирование, то лучше
                            //          сделать его побольше - не 80, а 120 символов шириной, например.

                            textWithIndent = umba::text_utils::textAddIndent( formattedParas, std::string(fnw+3u, ' '), std::string() );
                        }
                        catch(const std::exception &e)
                        {
                            UMBA_BRIEF_SCANNER_LOG_EXCEPTION("Exception in textAddIndent", e.what(), info );
                            continue;
                        }
                        catch(...)
                        {
                            UMBA_BRIEF_SCANNER_LOG_EXCEPTION("Exception in textAddIndent", "unknown error", info );
                            continue;
                        }


                        try
                        {
                            uinfoStream << width(fnw) << left << relName << " - "
                                        << textWithIndent
                                        << "\n";
                        }
                        catch(const std::exception &e)
                        {
                            UMBA_BRIEF_SCANNER_LOG_EXCEPTION("Exception in uinfoStream", e.what(), info );
                            continue;
                        }
                        catch(...)
                        {
                            UMBA_BRIEF_SCANNER_LOG_EXCEPTION("Exception in uinfoStream", "unknown error", info );
                            continue;
                        }


                    }

                    // descriptionWidth
                    // umba::text_utils::
                    // std::string textAddIndent(const std::string &text, const std::string &indent)
                    // std::string formatTextParas( std::string text, std::string::size_type paraWidth )
                    // std::string textAddIndent(const std::string &text, const std::string &indent, const std::string &firstIndent)
                    //infoStream << relName << " - " << info.infoText << "\n";
                }

            } // for


            if (appConfig.getOptMd())
            {
            }
            else if (appConfig.getOptHtml())
            {
                uinfoStream << "</tbody></table>\n";
            }


        };


        //infoStream << "##### 1\n";

        printInfo(true);

        if (!appConfig.getOptMain())
        {
            // print all

            if (appConfig.getOptMd())
            {
                infoStream << "\n";
            }
            else if (appConfig.getOptHtml())
            {
                //TODO: !!! Add HTML line break here
            }
            else
            {
                infoStream << "\n";
            }

            //infoStream << "##### 2\n";
            printInfo(false);

        }


        if (appConfig.getOptMd())
        {
        }
        else if (appConfig.getOptHtml())
        {
            infoStream << "</body>\n";
            infoStream << "</html>\n";
        }


        if (appConfig.testVerbosity(VerbosityLevel::normal))
        {
            std::unordered_map<std::string, std::string>::const_iterator uit = prevBriefFileInfo.begin();
            for(; uit!=prevBriefFileInfo.end(); ++uit)
            {
                if (relNames.find(uit->first)==relNames.end())
                {
                    // В новом наборе старого файла нет
                    LOG_WARN_OPT("update-missing") << "previously scanned file not found in current source tree, file: '" << uit->first << "'\n";
                    LOG_WARN_OPT("file-description") << uit->second << "\n";
                }
            }
        }

        //!!! конец обработки брифов
    }
    else // А вот тут делаем доксификацию
    {
        // appConfig.outputName

        if (foundFiles.size()!=foundFilesFolders.size())
        {
            LOG_ERR_OPT << "something goes wrong: number of found files is not the same as number of it's folders" << endl;
            return 1;
        }

        auto checkCreatePath = [&](const std::string &p)
        {
            auto canonP = umba::filename::makeCanonical(p);

            if (umba::filesys::isPathExist(canonP))
            {
                if (!umba::filesys::isPathDirectory(canonP))
                {
                    LOG_WARN_OPT("create-dir-failed") << "path exist, but not a directory: " << p << endl;
                    LOG_ERR_OPT << "fatal error" << endl;
                    return false;
                }
            }
            else
            {
                if (!umba::filesys::createDirectoryEx(canonP, true /* forceCreatePath */ ))
                {
                    LOG_WARN_OPT("create-dir-failed") << "failed to create directory: " << p << endl;
                    LOG_ERR_OPT << "fatal error" << endl;
                    return false;
                }
            }

            return true;
        };

        //std::vector<std::string> foundFiles, foundFilesFolders

        if (!appConfig.getOptNoOutput())
        {
            std::vector<std::string>::const_iterator fNameIt = foundFiles.begin();
            std::vector<std::string>::const_iterator fldrIt  = foundFilesFolders.begin();

            if (!checkCreatePath(appConfig.outputName))
                return 1;


            // Вектора одного размера, мы ранее проверили, поэтому не паримся, и проверяем на окончание только один итератор
            for(; fNameIt!=foundFiles.end(); ++fNameIt, ++fldrIt)
            {
                auto fileName          = *fNameIt; // Полное имя найденного файла
                auto fileNameCanonical = umba::filename::makeCanonical(fileName);

                auto fileFolderOrg = *fldrIt ; // Корень, в котором нашли файл, исходный
                auto fileFolder    = umba::filename::makeCanonical(fileFolderOrg); // сделали канонично
                //auto relFileName   = fileName; // имя относительно корня, пока полное - просто чтобы с типами не возиться
                using FilenameStringType     = decltype(fileName);
                using FilenameStringCharType = typename FilenameStringType::value_type;
                FilenameStringType relFileName; // имя относительно корня, пока пустое - повозился с типом

                if (briefInfo.find(fileNameCanonical)!=briefInfo.end())
                {
                    if (appConfig.doxificationMode!=DoxificationMode::doxyfyAlways)
                    {
                        if (appConfig.testVerbosity(VerbosityLevel::detailed))
                        {
                            logMsg << "File " << fileName << " skiped due it iss documented" << endl << normal;
                        }

                        continue;
                    }
                }

                //briefInfo[canonicalName] = info;

                bool isSubPath = umba::filename::isSubPathName(fileFolder, fileName, &relFileName); //
                if (!isSubPath)
                {
                    LOG_WARN_OPT("no-sub-path") << "file path is not a sub-path of it's root: " << fileFolder << " / " << fileName << endl;
                    LOG_ERR_OPT << "fatal error" << endl;
                    return 1;
                }

                //logMsg << "Found " << relFileName << " in " << fileFolder << "\n";
                //logMsg << "Found " << fileName << " in " << fileFolder << "\n";

                FilenameStringType targetSubPath  = umba::filename::getPath(relFileName);
                FilenameStringType targetFilename = umba::filename::getFileName(relFileName);
                FilenameStringType sourceFilename = targetFilename;

                // logMsg << "Found " << targetFilename << " in " << targetSubPath << "\n";

                // Все исходные расширения становятся частью имени файла
                for(auto &ch : targetFilename)
                {
                    if (ch==(FilenameStringCharType)'.')
                        ch = (FilenameStringCharType)'_';
                }

                targetFilename = umba::filename::appendExtention(targetFilename, umba::string_plus::make_string<FilenameStringType>("dox"));

                FilenameStringType targetPath = umba::filename::makeCanonical(umba::filename::appendPath(appConfig.outputName, targetSubPath));

                if (!checkCreatePath(targetPath))
                    return 1;

                std::string text = "/*! \\file " + sourceFilename + "\n    \\brief \n */\n\n";

                text = marty_cpp::converLfToOutputFormat(text, appConfig.outputLinefeed);

                FilenameStringType targetPathFilename = umba::filename::appendPath(targetPath, targetFilename);

                //appCfg.outputLinefeed
                try
                {
                    umba::cli_tool_helpers::writeOutput( targetPathFilename, umba::cli_tool_helpers::IoFileType::regularFile
                                                       , encoding::ToUtf8(), encoding::FromUtf8()
                                                       , text, std::string() // bomData
                                                       , true /* fromFile */, true /* utfSource */ , appConfig.bOverwrite
                                                       );

                }
                catch(const std::exception &e)
                {
                    LOG_WARN_OPT("create-file-failed") << e.what() /* "failed to write file: " << targetPathFilename */  << endl;
                    LOG_ERR_OPT << "fatal error" << endl;
                    return 1;
                }

            } // for(; fNameIt!=foundFiles.end(); ++fNameIt, ++fldrIt)

        } // if (!appConfig.getOptNoOutput())

    } // else // А вот тут делаем доксификацию


    if (appConfig.testVerbosity(VerbosityLevel::normal))
        logMsg << "Done\n\n\n";


    return 0;
}


