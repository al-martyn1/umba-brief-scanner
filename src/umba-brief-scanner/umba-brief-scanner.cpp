/*! \file
    \brief Утилита umba-brief-scanner - сканирование исходных текстов на C++ и получение списка описаний
 */

#include "umba/umba.h"
#include "umba/simple_formatter.h"
#include "umba/char_writers.h"

#include "umba/debug_helpers.h"

#include <iostream>
#include <iomanip>
#include <string>
// #include <cstdio>
#include <filesystem>

#include "umba/debug_helpers.h"
#include "umba/string_plus.h"
#include "umba/program_location.h"
#include "umba/scope_exec.h"
#include "umba/macro_helpers.h"
#include "umba/macros.h"
#include "umba/info_log.h"
#include "umba/scanners.h"

#include "umba/time_service.h"
#include "umba/text_utils.h"
#include "marty_cpp/src_normalization.h"


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


int main(int argc, char* argv[])
{
    umba::time_service::init();
    umba::time_service::start();

    umba::time_service::TimeTick startTick = umba::time_service::getCurTimeMs();


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

    // Force set CLI arguments while running under debugger
    if (umba::isDebuggerPresent())
    {
        argsParser.args.clear();
        argsParser.args.push_back("@..\\umba-brief-scanner.rsp");
        // argsParser.args.clear();
        // argsParser.args.push_back("@..\\tests\\data\\test01.rsp");
        //argsParser.args.push_back("@..\\make_sources_brief.rsp");
        // argsParser.args.push_back(umba::string_plus::make_string(""));
        // argsParser.args.push_back(umba::string_plus::make_string(""));
        // argsParser.args.push_back(umba::string_plus::make_string(""));
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
        LOG_ERR_OPT << "output name not taken" << endl;
        return 1;
    }


    std::map<std::string, std::string>  originalFileInfo;
    if (appConfig.updateMode)
    {
        if (appConfig.updateFromFile.empty())
        {
            using namespace umba::filename;
            std::string ext = getFileExtention(appConfig.outputName);
            if (ext=="txt" || ext==".txt" || ext=="TXT" || ext==".TXT")
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
            std::string updateFromText = marty_cpp::normalizeCrLfToLf(std::string(fileData.begin(), fileData.end()), false);
            std::vector<std::string> updateFromLines = marty_cpp::splitToLinesSimple(updateFromText, false);

            std::string curName;
            std::string curText;

            auto addFileInfo = [&]()
            {
                if (!curName.empty() && !curText.empty())
                {
                    originalFileInfo[curName] = curText;
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

    } // if (appConfig.updateMode)



    std::vector<std::string> foundFiles, excludedFiles;
    std::set<std::string>    foundExtentions;
    umba::scanners::scanFolders(appConfig, logMsg, foundFiles, excludedFiles, foundExtentions);


    if (appConfig.testVerbosity(VerbosityLevel::detailed))
    {
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

    std::map<std::string, BriefInfo>  briefInfo;


    for(const auto & filename : foundFiles)
    {
        // logMsg << name << endl;
        std::vector<char> filedata;
        if (!umba::filesys::readFile( filename, filedata ))
        {
            LOG_WARN_OPT("open-file-failed") << "failed to open file '" << filename << "'\n";
            continue;
        }

        BriefInfo  info;
        bool bFound = findBriefInfo( filedata, appConfig.entryNames, info );
        briefInfo[filename] = info;

        if (appConfig.testVerbosity(VerbosityLevel::detailed))
        {
            logMsg << (info.briefFound ? '+' : '-')
                   << (info.entryPoint ? 'E' : ' ')
                   << "    " << filename
                   << "\n";
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

        infoStream.open( appConfig.outputName, std::ios_base::out | std::ios_base::trunc );
        if (!infoStream)
        {
            LOG_WARN_OPT("create-file-failed") << "failed to create output file: " << appConfig.outputName << endl;
            return 1;
        }
    }


    std::string titleStr = "Brief Description for Project Sources";
    std::string sepLine  = "-------------------------------------";

    if (!appConfig.getOptHtml())
    {
        infoStream << titleStr << "\n" << sepLine << "\n\n";
    }
    else
    {
        infoStream << "<!DOCTYPE html>\n<html>\n";
        infoStream << "<head>\n<title>" << titleStr << "</title>\n</head>\n";
        infoStream << "<body>\n";
    }


    auto printInfo = [&]( bool bMain )
    {
        umba::StdStreamCharWriter infoWriter(infoStream);
        umba::SimpleFormatter uinfoStream(&infoWriter);

        if (appConfig.getOptHtml())
        {
            uinfoStream << "<table><tbody>\n";
        }

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

            if (appConfig.getOptRemovePath())
                relName = umba::filename::getFileName( relName );

            std::string infoText = info.infoText;

            std::map<std::string, std::string>::const_iterator uit = originalFileInfo.find(relName);
            if (uit!=originalFileInfo.end())
            {
                if (/* !info.briefFound || */ infoText.empty())
                {
                    infoText = uit->second;
                }
            }

            if (!appConfig.getOptHtml())
            {
                int fnw = (int)appConfig.filenameWidth;

                if (appConfig.descriptionWidth==0)
                {
                    uinfoStream << width(fnw) << left << relName << " - " << infoText << "\n";
                }
                else
                {
                    auto formattedParas = umba::text_utils::formatTextParas( infoText, appConfig.descriptionWidth, umba::text_utils::TextAlignment::left );

                    // Не будем париться - первая строка может свисать справа, ну и фик с ним

                    //TODO: !!! Не корректно определяется длина строк в многобайтной кодировке
                    //          Мой поток вывода (umba::SimpleFormatter) - также некорректно работает с многобайтной кодировкой,
                    //          но это обычно не проблема, если имена файлов  исходников используют только английский алфавит.
                    //          При форматировании же текста кодировка зависит от кодировки исходника - может быть как однобайтной,
                    //          так и многобайтной (UTF-8, например, и в линуксе это обычно уже стандарт)
                    //          Поэтому, если где-то используются русскоязычные описания и используется форматирование, то лучше
                    //          сделать его побольше - не 80, а 120 символов шириной, например.

                    auto textWithIndent = umba::text_utils::textAddIndent( formattedParas, std::string(fnw+3, ' '), std::string() );

                    uinfoStream << width(fnw) << left << relName << " - " 
                                << textWithIndent
                                << "\n";
                }

                // descriptionWidth
                // umba::text_utils::  
                // std::string textAddIndent(const std::string &text, const std::string &indent)
                // std::string formatTextParas( std::string text, std::string::size_type paraWidth )
                // std::string textAddIndent(const std::string &text, const std::string &indent, const std::string &firstIndent)
                //infoStream << relName << " - " << info.infoText << "\n";
            }
            else // Html
            {
                //TODO: !!! Add HTML output here
                uinfoStream << "<tr><td>" << htmlEscape(relName) << "</td><td>" << htmlEscape(info.infoText) << "</td></tr>\n";
            }
        
        } // for


        if (appConfig.getOptHtml())
        {
            uinfoStream << "</tbody></table>\n";
        }


    };


    printInfo(true);
        
    if (!appConfig.getOptMain())
    {
        // print all

        if (!appConfig.getOptHtml())
        {
            infoStream << "\n";
        }
        else
        {
            //TODO: !!! Add HTML line break here
        }

        printInfo(false);

    }


    if (appConfig.getOptHtml())
    {
        infoStream << "</body>\n";
        infoStream << "</html>\n";
    }


    if (appConfig.testVerbosity(VerbosityLevel::normal))
        logMsg << "Done";


    return 0;
}


