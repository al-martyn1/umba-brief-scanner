/*! Copyright info goes here

 */

/*! \file
    \brief Простой тест регулярок по поиску точки входа и brief'а
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

#include "umba/time_service.h"

// #include "utils.h"

#define LOG_REGEX_MATCH

#include "brief_info.h"


umba::StdStreamCharWriter coutWriter(std::cout);
umba::StdStreamCharWriter cerrWriter(std::cerr);
umba::NulCharWriter       nulWriter;

umba::SimpleFormatter logMsg(&coutWriter);
umba::SimpleFormatter logErr(&cerrWriter);
umba::SimpleFormatter logNul(&nulWriter);

bool logWarnType   = true;
bool logGccFormat  = false;
bool logSourceInfo = false;


#include "log.h"
// #include "utils.h"
#include "umba/scan_folders.h"

//#include "scan_sources.h"

umba::program_location::ProgramLocation<std::string>   programLocationInfo;


#include "umba/cmd_line.h"


#include "app_ver_config.h"
#include "print_ver.h"

#include "arg_parser.h"

/* One more comment */

int main(int argc, char* argv[])
{

    std::string filename = "..\\src\\main\\main.cpp";
    //std::string filename = "..\\src\\main\\test02.cpp";
    //std::string filename = "..\\src\\main\\brief_info.h";

    std::vector<char> fileData;
    if (!umba::filesys::readFile( filename, fileData ))
    {
        LOG_WARN_OPT("open-file-failed") << "failed to open file '" << filename << "'\n";
        return 1;
    }

    //std::map< std::string,std::set<std::string> > entryNames;
    appConfig.entryNames["main"].insert("int");
    appConfig.entryNames["main"].insert("void");

    std::string fileText = std::string(fileData.begin(), fileData.end());

    bool matchRes = false;
    try
    {
        BriefInfo info;
        //matchRes = findBriefInfo( fileData, appConfig.entryNames, info );
        matchRes = findEntryPoint( fileData, appConfig.entryNames );
        //logMsg << "Text:\n" << fileText << "\n";
        logMsg << "Match res: " << matchRes << "\n";
    }
    catch(const std::exception &e)
    {
        LOG_ERR_OPT << "exception catched: " << e.what() << "\n";
        return 1;
    }


    return 0;
}


