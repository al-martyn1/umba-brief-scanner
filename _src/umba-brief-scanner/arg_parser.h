/*! \file
    \brief Разбор аргументов командной строки
 */

#pragma once

#include <stack>

#include "app_config.h"
#include "umba/cmd_line.h"


#if defined(WIN32) || defined(_WIN32)
    #include <shellapi.h>
#endif

AppConfig    appConfig;


struct ArgParser
{

std::stack<std::string> optFiles;


std::string makeAbsPath( std::string p )
{
    std::string basePath;

    if (optFiles.empty())
        basePath = umba::filesys::getCurrentDirectory();
    else
        basePath = umba::filename::getPath(optFiles.top());


    return umba::filename::makeAbsPath( p, basePath );

}



// 0 - ok, 1 normal stop, -1 - error
template<typename ArgsParser>
int operator()( const std::string                               &a           //!< строка - текущий аргумент
              , umba::command_line::CommandLineOption           &opt         //!< Объект-опция, содержит разобранный аргумент и умеет отвечать на некоторые вопросы
              , ArgsParser                                      &argsParser  //!< Класс, который нас вызывает, содержит некоторый контекст
              , umba::command_line::ICommandLineOptionCollector *pCol        //!< Коллектор опций - собирает инфу по всем опциям и готов вывести справку
              , bool fBuiltin
              , bool ignoreInfos
              )
{
    //using namespace marty::clang::helpers;

    UMBA_USED(fBuiltin);

    std::string dppof = "Don't parse predefined options from ";

    if (opt.isOption())
    {
        std::string errMsg;
        int intVal;
        bool boolVal;
        std::string strVal;

        if (opt.name.empty())
        {
            LOG_ERR_OPT<<"invalid (empty) option name\n";
            return -1;
        }

       if (opt.isOption("quet") || opt.isOption('q') || opt.setDescription("Operate quetly. Short alias for '--verbose=quet'"))
        {
            argsParser.quet = true;
            appConfig.setOptQuet(true);
        }

        #if defined(WIN32) || defined(_WIN32)
        else if (opt.isOption("home") || opt.setDescription("Open homepage"))
        {
            if (argsParser.hasHelpOption) return 0;
            ShellExecuteA( 0, "open", appHomeUrl, 0, 0, SW_SHOW );
            return 1;
        }
        #endif

        else if (opt.setParam("LEVEL", 1, "0/quet/no/q|"
                                          "1/normal/n|"
                                          "2/config/c|"
                                          "3/detailed/detail/d|"
                                          "4/extra/high/e"
                             )
              || opt.setInitial(1) || opt.isOption("verbose") || opt.isOption('V')
              || opt.setDescription("Set verbosity level. LEVEL parameter can be one of the next values:\n"
                                    "quet - maximum quet mode (same as --quet).\n"
                                    "normal - print common details.\n"
                                    "config - print common details and app config.\n"
                                    "detailed - print common details, app config and all declarations, which are found in user files.\n"
                                    "extra - print common details, app config and all found declarations (from all files)." // "\n"
                                   )
              )
        {
            if (argsParser.hasHelpOption) return 0;

            auto mapper = [](int i) -> VerbosityLevel
                          {
                              //return AppConfig::VerbosityLevel_fromStdString((VerbosityLevel)i);
                              switch(i)
                              {
                                  case  0: case  1: case  2: case  3: case  4: return (VerbosityLevel)i;
                                  default: return VerbosityLevel::begin;
                              }
                          };

            VerbosityLevel lvl;
            if (!opt.getParamValue( lvl, errMsg, mapper ) )
            {
                LOG_ERR_OPT<<errMsg<<"\n";
                return -1;
            }

            appConfig.setVerbosityLevel(lvl);
            if (lvl==VerbosityLevel::quet)
                argsParser.quet = true;
        }

        else if ( opt.isBuiltinsDisableOptionMain  ()
               || opt.setDescription( dppof + "main distribution options file '" + argsParser.getBuiltinsOptFileName(umba::program_location::BuiltinOptionsLocationFlag::appGlobal   ) + "'"))
        { } // simple skip - обработка уже сделана

        else if ( opt.isBuiltinsDisableOptionCustom()
               || opt.setDescription( dppof + "custom global options file '"     + argsParser.getBuiltinsOptFileName(umba::program_location::BuiltinOptionsLocationFlag::customGlobal) + "'"))
        { } // simple skip - обработка уже сделана

        else if ( opt.isBuiltinsDisableOptionUser  ()
               || opt.setDescription( dppof + "user local options file '"        + argsParser.getBuiltinsOptFileName(umba::program_location::BuiltinOptionsLocationFlag::userLocal   ) + "'"))
        { } // simple skip - обработка уже сделана

        else if (opt.isOption("version") || opt.isOption('v') || opt.setDescription("Show version info"))
        {
            if (argsParser.hasHelpOption) return 0;

            if (!ignoreInfos)
            {
                umba::cli_tool_helpers::printOnlyVersion(umbaLogStreamMsg);
                return 1;
            }

            return 0;
        }

        else if (opt.isOption("where") || opt.setDescription("Show where the executable file is"))
        {
            if (argsParser.hasHelpOption) return 0;

            LOG_MSG_OPT << argsParser.programLocationInfo.exeFullName << "\n";
            return 0;
        }

        else if (opt.setParam("CLR", 0, "no/none/file|"
                                        "ansi/term|"
                                        #if defined(WIN32) || defined(_WIN32)
                                        "win32/win/windows/cmd/console"
                                        #endif
                             )
              || opt.setInitial(-1) || opt.isOption("color")
              || opt.setDescription("Force set console output coloring")
              /* ", can be:\nno, none, file - disable coloring\nansi, term - set ansi terminal coloring\nwin32, win, windows, cmd, console - windows console specific coloring method" */
              )
        {
            if (argsParser.hasHelpOption) return 0;

            umba::term::ConsoleType res;
            auto mapper = [](int i) -> umba::term::ConsoleType
                          {
                              switch(i)
                              {
                                  case 0 : return umba::term::ConsoleType::file;
                                  case 1 : return umba::term::ConsoleType::ansi_terminal;
                                  case 2 : return umba::term::ConsoleType::windows_console;
                                  default: return umba::term::ConsoleType::file;
                              };
                          };
            if (!opt.getParamValue( res, errMsg, mapper ) )
            {
                LOG_ERR_OPT<<errMsg<<"\n";
                return -1;
            }

            coutWriter.forceSetConsoleType(res);
            cerrWriter.forceSetConsoleType(res);

            return 0;
        }

        else if ( opt.setParam("?MODE",true)
               || opt.isOption("overwrite") || opt.isOption('Y')
               // || opt.setParam("VAL",true)
               || opt.setDescription("Allow overwrite existing file."))
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(boolVal,errMsg))
            {
                LOG_ERR_OPT<<errMsg<<"\n";
                return -1;
            }

            appConfig.bOverwrite = boolVal;
            return 0;
        }

        else if ( opt.setParam("LINEFEED",umba::command_line::OptionType::optString)
               || opt.isOption("linefeed") || opt.isOption("LF") || opt.isOption('L')
               // || opt.setParam("VAL",true)
               || opt.setDescription("Output linefeed. LINEFEED is one of: CR/LF/CRLF/LFCR/DETECT. "
                                     #if defined(WIN32) || defined(_WIN32)
                                     "Default is CRLF."
                                     #else
                                     "Default is LF."
                                     #endif
                                    )
                )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue(strVal,errMsg))
            {
                LOG_ERR_OPT<<errMsg<<"\n";
                return -1;
            }

            marty_cpp::ELinefeedType tmp = marty_cpp::enum_deserialize( strVal, marty_cpp::ELinefeedType::invalid );
            if (tmp== marty_cpp::ELinefeedType::invalid)
            {
                LOG_ERR_OPT<<"Invalid linefeed option value: "<<strVal<<"\n";
                return -1;
            }

            appConfig.outputLinefeed = tmp;

            return 0;
        }

        //------------

        // else if ( opt.isOption("output-path") || opt.isOption("output-root") ||  /* opt.isOption("output") ||  */ opt.isOption('O') || opt.setParam("PATH")
        //        || opt.setDescription("Set output root path")
        //         )
        // {
        //     if (argsParser.hasHelpOption) return 0;
        //
        //     if (!opt.hasArg())
        //     {
        //         LOG_ERR_OPT<<"output path not taken (--output-path)\n";
        //         return -1;
        //     }
        //
        //     auto optArg = umba::macros::substMacros(opt.optArg,umba::macros::MacroTextFromMapOrEnv<std::string>(appConfig.macros),umba::macros::keepUnknownVars);
        //     appConfig.outputPath = makeAbsPath(optArg);
        //     return 0;
        // }
        //

        else if ( opt.setParam("?FILENAME")
               || opt.isOption("update") || opt.isOption('T')
               || opt.setDescription(""
                                    )
                )
        {
            if (argsParser.hasHelpOption) return 0;

            appConfig.updateMode = true;

            if (!opt.hasArg())
            {
                return 0;
            }

            appConfig.updateFromFile = makeAbsPath(opt.optArg);

            return 0;
        }

        else if ( opt.isOption("include-files") || opt.isOption('I') || opt.setParam("MASK,...")
               || opt.setDescription("Include C/C++ names for output. Only files which file name matched any of taken masks, will be added to output.\n"
                                     "Note: exclude masks also performed on included names\n"
                                     "For details about 'MASK' parameter see '--exclude-files' option description."
                                    )
                )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.hasArg())
            {
                LOG_ERR_OPT<<"include files mask not taken (--exclude-files)\n";
                return -1;
            }

            std::vector< std::string > lst = umba::string_plus::split(opt.optArg, ',');
            appConfig.includeFilesMaskList.insert(appConfig.includeFilesMaskList.end(), lst.begin(), lst.end());

            return 0;
        }

        else if ( opt.setParam("MASK,...")
               || opt.isOption("exclude-files") || opt.isOption('X')
               || opt.setDescription("Exclude files from parsing. The 'MASK' parameter is a simple file mask, where '*' "
                                     "means any number of any chars, and '?' means exact one of any char. In addition, "
                                     "symbol '^' in front and/or back of the mask means that the mask will be bound to beginning/ending "
                                     "of the tested file name.\n"
                                     "Also, regular expresion syntax allowed in form '" +
                                     umba::regex_helpers::getRawEcmaRegexPrefix<std::string>() + "YOURREGEX'. The regular expresions supports\n"
                                     "See also: C++ Modified ECMA Script regular expression grammar - https://en.cppreference.com/w/cpp/regex/ecmascript"
                                    )
                )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.hasArg())
            {
                LOG_ERR_OPT<<"exclude files mask not taken (--exclude-files)\n";
                return -1;
            }

            std::vector< std::string > lst = umba::string_plus::split(opt.optArg, ',');
            appConfig.excludeFilesMaskList.insert(appConfig.excludeFilesMaskList.end(), lst.begin(), lst.end());

            return 0;
        }

        else if ( opt.isOption("exclude-dir") || opt.isOption("exclude-dirs") || opt.setParam("DIRNAME,...")
               || opt.setDescription("Exclude dirs from scaning. The 'DIRNAME' parameter is a simple directory name, not a mask"
                                    )
                )
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.hasArg())
            {
                LOG_ERR_OPT<<"exclude dirs not taken (--exclude-dir)\n";
                return -1;
            }

            // TODO: (111) Хорошо бы переделать, чтобы минус мог быть элементом списка, очищая его.
            // (222) заодно надо бы переделать и --exclude-files/--include-files
            //
            // (333) Заодно проверяем, как работает заметка в несколько параграфов
            // if (opt.optArg=="-")
            // {
            //     appConfig.excludeDirs.clear();
            //     // std::vector< std::string > lst = umba::string_plus::split(opt.optArg, ',');
            //     // //appConfig.excludeDirs.clear();
            //     // for(const auto li: lst)
            //     // {
            //     //     auto pos = std::find()
            //     //     erase( iterator pos );
            //     // }
            // }
            // else
            {
                std::vector< std::string > lst = umba::string_plus::split(opt.optArg, ',');
                for(auto li: lst)
                {
                    if (li=="-")
                    {
                        appConfig.excludeDirs.clear();
                    }
                    else if (!li.empty())
                    {
                        bool bRemove = false;
                        if (li[0]=='-')
                        {
                            li.erase(0, 1);
                            bRemove = true;
                        }
                        else if (li[0]=='+')
                        {
                            li.erase(0, 1);
                        }

                        if (bRemove)
                        {
                            auto it = std::find(appConfig.excludeDirs.begin(), appConfig.excludeDirs.end(), li);
                            if (it!=appConfig.excludeDirs.end())
                                appConfig.excludeDirs.erase(it);
                        }
                        else
                        {
                            appConfig.excludeDirs.emplace_back(li);
                        }
                    }
                }
                //appConfig.excludeDirs.insert(appConfig.excludeDirs.end(), lst.begin(), lst.end());
            }

            return 0;
        }



        else if (opt.isOption("no-output") || opt.isOption("dry-run") || opt.setDescription("Do not actually write output files. Simulation mode. Behave normally, but do not copy/creater/update any files."))
            {
                if (argsParser.hasHelpOption) return 0;
                appConfig.setOptNoOutput(true);
                return 0;
            }

        else if (opt.isOption("main-only") || opt.isOption("main") || opt.setDescription("Print only main files (whish contains main or other entry point)."))
            {
                if (argsParser.hasHelpOption) return 0;
                appConfig.setOptMain(true);
                return 0;
            }

        // else if (opt.isOption("split-main") || opt.setDescription("Put entry points to separate groups."))
        //     {
        //         if (argsParser.hasHelpOption) return 0;
        //         appConfig.setOptMain(true);
        //         return 0;
        //     }

        else if (opt.isOption("remove-path") || opt.setDescription("Remove path from file names while ghenerating output."))
            {
                if (argsParser.hasHelpOption) return 0;
                appConfig.setOptRemovePath(true);
                return 0;
            }

        else if (opt.isOption("html") || opt.setDescription("Print output in html format."))
            {
                if (argsParser.hasHelpOption) return 0;
                appConfig.setOptHtml(true);
                return 0;
            }

        else if (opt.isOption("md") || opt.setDescription("Print output in md format."))
            {
                if (argsParser.hasHelpOption) return 0;
                appConfig.setOptMd(true);
                return 0;
            }

        else if (opt.isOption("split-group") || opt.isOption("group") || opt.isOption("split") || opt.setDescription("Split files to separate groups (by single empty line) by its path."))
            {
                if (argsParser.hasHelpOption) return 0;
                appConfig.setOptSplitGroups(true);
                return 0;
            }

        else if (opt.isOption("skip-undocumented") || opt.isOption('U') || opt.setDescription("Skip undocumented files. By default, all files will be added to report, also with empty description."))
            {
                if (argsParser.hasHelpOption) return 0;
                appConfig.setOptSkipUndocumented(true);
                return 0;
            }

        else if (opt.isOption("scan-notes") || opt.isOption("scan-todo") || opt.setDescription("Scan source files for todo/notes."))
            {
                if (argsParser.hasHelpOption) return 0;
                appConfig.setOptTodo(true);
                return 0;
            }

        else if ( opt.setParam("PATH")
               || opt.isOption("path") || opt.isOption("scan") || opt.isOption('P')
               || opt.setDescription("Add path to scan path list"))
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.hasArg())
            {
                LOG_ERR_OPT<<"Adding path to scan path list requires argument (--path)\n";
                return -1;
            }

            auto optArg = umba::macros::substMacros(opt.optArg,umba::macros::MacroTextFromMapOrEnv<std::string>(appConfig.macros),umba::macros::keepUnknownVars);
            appConfig.scanPaths.push_back(makeAbsPath(optArg));

            return 0;
        }

        else if ( opt.setParam("PATH", umba::command_line::OptionType::optString)
               || opt.isOption("notes-output-path") || opt.isOption("todo-output-path")
               || opt.setDescription("Set output path to write notes/todo-list files."))
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue( strVal, errMsg ) )
            {
                LOG_ERR_OPT<<errMsg<<" (--notes-output-path)\n";
                return -1;
            }

            if (!appConfig.setNotesOutputPath(strVal))
            {
                LOG_ERR_OPT<<"Setting output path for writting notes/todo-list files failed (--notes-output-path)\n";
                return -1;
            }

            return 0;
        }

        else if ( opt.setParam("EXT", umba::command_line::OptionType::optString)
               || opt.isOption("notes-filename-ext") || opt.isOption("todo-filename-ext")
               || opt.setDescription("Set extention for notes/todo-list output files."))
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue( strVal, errMsg ) )
            {
                LOG_ERR_OPT<<errMsg<<" (--notes-filename-ext)\n";
                return -1;
            }

            if (!appConfig.setNotesFileExt(strVal))
            {
                LOG_ERR_OPT<<"Setting extention for notes/todo-list files failed (--notes-output-ext)\n";
                return -1;
            }

            return 0;
        }

        else if ( opt.setParam("TYPE:NAME", umba::command_line::OptionType::optString)
               || opt.isOption("notes-filename") || opt.isOption("todo-filename")
               || opt.setDescription("Set file name for notes/todo-list output files for specified note type."))
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue( strVal, errMsg ) )
            {
                LOG_ERR_OPT<<errMsg<<" (--notes-filename)\n";
                return -1;
            }

            if (!appConfig.addNoteFilename(strVal))
            {
                LOG_ERR_OPT<<"Setting file name for notes/todo-list output files for specified type failed (--notes-filename)\n";
                return -1;
            }

            return 0;
        }

        else if ( opt.setParam("TYPE:MARKER", umba::command_line::OptionType::optString)
               || opt.isOption("notes-marker") || opt.isOption("todo-marker")
               || opt.setDescription("Set note marker for specified note type."))
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue( strVal, errMsg ) )
            {
                LOG_ERR_OPT<<errMsg<<" (--notes-marker)\n";
                return -1;
            }

            if (!appConfig.addNoteMarker(strVal))
            {
                LOG_ERR_OPT<<"Setting note marker failed (--notes-marker)\n";
                return -1;
            }

            return 0;
        }

        else if ( opt.setParam("STRIP", umba::command_line::OptionType::optString)
               || opt.isOption("notes-strip") || opt.isOption("todo-strip")
               || opt.setDescription("Add note strip prefix (for all note types)."))
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue( strVal, errMsg ) )
            {
                LOG_ERR_OPT<<errMsg<<" (--notes-strip)\n";
                return -1;
            }

            if (!appConfig.addNoteStripPrefix(strVal))
            {
                LOG_ERR_OPT<<"Adding note strip prefix failed (--notes-strip)\n";
                return -1;
            }

            return 0;
        }

        else if ( opt.setParam("TYPE1[,TYPE2...]", umba::command_line::OptionType::optString)
               || opt.isOption("notes-add-check") || opt.isOption("todo-add-check")
               || opt.setDescription("Force set check marks in output for notes of TYPE1, TYPE2 etc. "
                                     "`reset`, `clr`, `clear` - set option OFF for all types. "
                                     "`all`, `set` - set option ON for all types."))
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue( strVal, errMsg ) )
            {
                LOG_ERR_OPT<<errMsg<<" (--notes-add-check)\n";
                return -1;
            }

            if (!appConfig.setNotesAddCheck(strVal))
            {
                LOG_ERR_OPT<<"Setting notes add-check option failed (--notes-add-check)\n";
                return -1;
            }

            return 0;
        }

        else if ( opt.setParam("TYPE1[,TYPE2...]", umba::command_line::OptionType::optString)
               || opt.isOption("notes-keep-check") || opt.isOption("todo-keep-check")
               || opt.setDescription("Keep extracted check marks in output for notes of TYPE1, TYPE2 etc. "
                                     "`reset`, `clr`, `clear` - set option OFF for all types. "
                                     "`all`, `set` - set option ON for all types."))
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue( strVal, errMsg ) )
            {
                LOG_ERR_OPT<<errMsg<<" (--notes-keep-check)\n";
                return -1;
            }

            if (!appConfig.setNotesKeepCheck(strVal))
            {
                LOG_ERR_OPT<<"Setting notes keep-check option failed (--notes-keep-check)\n";
                return -1;
            }

            return 0;
        }

        else if ( opt.setParam("TYPE:TITLE", umba::command_line::OptionType::optString)
               || opt.isOption("notes-single-output-title") || opt.isOption("todo-single-output-title")
               || opt.setDescription("Set note title for specified note type when writting all notes to single output."))
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue( strVal, errMsg ) )
            {
                LOG_ERR_OPT<<errMsg<<" (--notes-single-output-title)\n";
                return -1;
            }

            if (!appConfig.setNoteTitle(strVal))
            {
                LOG_ERR_OPT<<"Setting note title failed (--notes-single-output-title)\n";
                return -1;
            }

            return 0;
        }

        else if ( opt.setParam("FMT", umba::command_line::OptionType::optString)
               || opt.isOption("notes-single-output-title-format") || opt.isOption("todo-single-output-title-format")
               || opt.setDescription("Set note title format."))
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue( strVal, errMsg ) )
            {
                LOG_ERR_OPT<<errMsg<<" (--notes-single-output-title-format)\n";
                return -1;
            }

            if (!appConfig.setNotesTitleFormat(strVal))
            {
                LOG_ERR_OPT<<"Setting note title format failed (--notes-single-output-title-format)\n";
                return -1;
            }

            return 0;
        }

        else if ( opt.setParam("FMT", umba::command_line::OptionType::optString)
               || opt.isOption("notes-source-info-format") || opt.isOption("todo-source-info-format")
               || opt.setDescription("Set note source info format."))
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue( strVal, errMsg ) )
            {
                LOG_ERR_OPT<<errMsg<<" (--notes-source-info-format)\n";
                return -1;
            }

            if (!appConfig.setNotesSrcInfoFormat(strVal))
            {
                LOG_ERR_OPT<<"Setting note source info format failed (--notes-source-info-format)\n";
                return -1;
            }

            return 0;
        }

        else if ( opt.setParam("TYPE", umba::command_line::OptionType::optString)
               || opt.isOption("notes-single-output") || opt.isOption("todo-single-output")
               || opt.setDescription("Set notes single output type."))
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue( strVal, errMsg ) )
            {
                LOG_ERR_OPT<<errMsg<<" (--notes-single-output)\n";
                return -1;
            }

            if (!appConfig.setNotesSingleOutputType(strVal))
            {
                LOG_ERR_OPT<<"Setting notes single output failed (--notes-single-output)\n";
                return -1;
            }

            return 0;
        }

        else if (opt.isOption("text-notes-full-path") || opt.setDescription("Add full path to text notes instead of relative."))
            {
                if (argsParser.hasHelpOption) return 0;
                appConfig.notesConfig.textNotesFullPath = true;
                return 0;
            }
        // text-notes-full-path



        // else if ( opt.setParam("NAME")
        //        || opt.isOption("entry-name") || opt.isOption('E')
        //        || opt.setDescription("Add name to lookup as entry point"))
        // {
        //     if (argsParser.hasHelpOption) return 0;
        //
        //     if (!opt.hasArg())
        //     {
        //         LOG_ERR_OPT<<"Adding entry name requres argument (--entry-name)\n";
        //         return -1;
        //     }
        //
        //     std::vector< std::string > lst = umba::string_plus::split(opt.optArg, ',');
        //     // if (lst.size()<2)
        //     //     lst.push_back("int"); // default return type of entry point is int
        //
        //     std::vector< std::string >::const_iterator lit = lst.begin();
        //     std::string entryName = *lit; ++lit;
        //
        //     // appConfig.entryNames[entryName].insert(lit, lst.end()); // лень разбираться
        //     auto &retTypes = appConfig.entryNames[entryName];
        //     for(; lit!=lst.end(); ++lit) retTypes.insert(*lit);
        //
        //     return 0;
        // }

        else if ( opt.setParam("NAME")
               || opt.isOption("entry-point") || opt.isOption('E')
               || opt.setDescription("Add signature to lookup as entry point"))
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.hasArg())
            {
                LOG_ERR_OPT<<"Adding entry point signature requres argument (--entry-point)\n";
                return -1;
            }

            appConfig.entrySignatures.emplace_back(TextSignature(opt.optArg));

            return 0;
        }

        else if ( opt.setParam("WIDTH", -1, 8, 64)
               || opt.isOption("filename-width") || opt.isOption('W')
               || opt.setDescription("Set width for filename report column"))
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue( intVal, errMsg ) )
            {
                LOG_ERR_OPT<<errMsg<<"\n";
                return -1;
            }

            appConfig.filenameWidth = (unsigned)intVal;

            return 0;
        }

        else if ( opt.setParam("WIDTH", -1, 64, 160)
               || opt.isOption("text-width") || opt.isOption("description-width") || opt.isOption("width") || opt.isOption('D')
               || opt.setDescription("Set width for description text report column"))
        {
            if (argsParser.hasHelpOption) return 0;

            if (!opt.getParamValue( intVal, errMsg ) )
            {
                LOG_ERR_OPT<<errMsg<<"\n";
                return -1;
            }

            appConfig.descriptionWidth = (unsigned)intVal;

            return 0;
        }

        else if (opt.setParam("MODE", 0, "0/no/none|1/doxify||2/always")
              || opt.setInitial(-1) || opt.isOption("doxyfication")
              || opt.setDescription("Generate .dox files for existing C/C++ sources")
              /* ", can be:\nno, none, file - disable coloring\nansi, term - set ansi terminal coloring\nwin32, win, windows, cmd, console - windows console specific coloring method" */
              )
        {
            if (argsParser.hasHelpOption) return 0;

            DoxificationMode res;
            auto mapper = [](int i) -> DoxificationMode
                          {
                              switch(i)
                              {
                                  case 0 : return DoxificationMode::noDoxyfication;
                                  case 1 : return DoxificationMode::doxyfy;
                                  case 2 : return DoxificationMode::doxyfyAlways;
                                  default: return DoxificationMode::invalid;
                              };
                          };
            if (!opt.getParamValue( res, errMsg, mapper ) )
            {
                LOG_ERR_OPT<<errMsg<<"\n";
                return -1;
            }

            if (res==DoxificationMode::invalid)
            {
                LOG_ERR_OPT<<"Invalid value (--doxyfication)\n";
            }

            appConfig.doxificationMode = res;

            return 0;
        }

        //------------

        else if ( opt.isOption("autocomplete-install")
               || opt.setDescription("Install autocompletion to bash"
                                     #if defined(WIN32) || defined(_WIN32)
                                         "/clink(cmd)"
                                     #endif
                                    )
               )
        {
            if (argsParser.hasHelpOption) return 0;

            //return autocomplete(opt, true);
            return umba::command_line::autocompletionInstaller( pCol, opt, pCol->getPrintHelpStyle(), true, [&]( bool bErr ) -> decltype(auto) { return bErr ? LOG_ERR_OPT : LOG_MSG_OPT; } );
        }
        else if ( opt.isOption("autocomplete-uninstall")
               || opt.setDescription("Remove autocompletion from bash"
                                     #if defined(WIN32) || defined(_WIN32)
                                         "/clink(cmd)"
                                     #endif
                                    )
                )
        {
            if (argsParser.hasHelpOption) return 0;

            //return autocomplete(opt, false);
            return umba::command_line::autocompletionInstaller( pCol, opt, pCol->getPrintHelpStyle(), false, [&]( bool bErr ) -> decltype(auto) { return bErr ? LOG_ERR_OPT : LOG_MSG_OPT; } );
        }

        else if (opt.isHelpStyleOption())
        {
            // Job is done in isHelpStyleOption
        }
        else if (opt.isHelpOption()) // if (opt.infoIgnore() || opt.isOption("help") || opt.isOption('h') || opt.isOption('?') || opt.setDescription(""))
        {
            if (!ignoreInfos)
            {
                if (pCol && !pCol->isNormalPrintHelpStyle())
                    argsParser.quet = true;
                //printNameVersion();
                if (!argsParser.quet)
                {
                    // umba::cli_tool_helpers::printBuildDateTime(umbaLogStreamMsg);
                    // umba::cli_tool_helpers::printCommitHash(umbaLogStreamMsg);
                    std::cout<<"\n";
                //printHelp();
                }

                if (pCol && pCol->isNormalPrintHelpStyle() && argsParser.argsNeedHelp.empty())
                {
                    auto helpText = opt.getHelpOptionsString();
                    std::cout << "Usage: " << argsParser.programLocationInfo.exeName
                              //<< " [OPTIONS] input_file [output_file]\n\nOptions:\n\n"<<helpText;
                              << " [OPTIONS] output_file\n\nOptions:\n\n"<<helpText;
                }

                if (pCol) // argsNeedHelp
                    std::cout<<pCol->makeText( 78, &argsParser.argsNeedHelp );

                return 1;

            }

            return 0; // simple skip then parse builtins
        }
        else
        {
            LOG_ERR_OPT<<"unknown option: "<<opt.argOrg<<"\n";
            return -1;
        }

        return 0;

    } // if (opt.isOption())
    else if (opt.isResponseFile())
    {
        std::string optFileName = makeAbsPath(opt.name);

        optFiles.push(optFileName);

        auto parseRes = argsParser.parseOptionsFile( optFileName );

        optFiles.pop();

        if (!parseRes)
            return -1;

        if (argsParser.mustExit)
            return 1;

        /*
        std::ifstream optFile(opt.name.c_str());
        if (!optFile)
        {
            LOG_ERR_OPT<<"failed to read response file '"<<opt.name<<"'\n";
            return -1;
        }
        else
        {
            std::string optLine;
            while( std::getline( optFile, optLine) )
            {
                umba::string_plus::trim(optLine);
                if (optLine.empty())
                    continue;

                if (umba::command_line::isComment( optLine ))
                    continue;

                int paRes = parseArg( optLine, pCol, false, true );
                if (paRes)
                {
                   return paRes;
                }
            }
        }
        */

        return 0;

    }

    //appConfig.clangCompileFlagsTxtFilename.push_back(makeAbsPath(a));

    appConfig.outputName = makeAbsPath(a);

/*
    if (inputFilename.empty())
        inputFilename = a;
    else
        outputFilename = a;
*/

    return 0;

}

}; // struct ArgParser



class CommandLineOptionCollector : public umba::command_line::CommandLineOptionCollectorImplBase
{
protected:
    virtual void onOptionDup( const std::string &opt ) override
    {
        LOG_ERR_OPT<<"Duplicated option key - '"<<opt<<"'\n";
        throw std::runtime_error("Duplicated option key");
    }

};



