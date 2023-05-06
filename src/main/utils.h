#pragma once

#include "umba/umba.h"
#include "umba/simple_formatter.h"
#include "umba/string_plus.h"

#include <string>



//----------------------------------------------------------------------------
/*
bool isCppSpecialName( const std::string &s )
{
    return s.find_first_of( "<=>?!~[]&|+-/*%.(){}" )!=s.npos;
}
*/
//----------------------------------------------------------------------------
std::string cppNameToFileName( std::string name )
{
    umba::string_plus::ltrim( name, umba::string_plus::is_one_of<char>(":") );

    std::string fileName;
    fileName.reserve(name.size());

    for(auto ch : name)
    {
        if (ch==':')
        {
            if (!fileName.empty() && fileName.back()=='/')
                continue;

            fileName.push_back('/');
            continue;
        }

        fileName.push_back(ch);
    }

    return fileName;
}


// 1) Фильтры выходных имён
// 3) Отфильтровать всё по типу: --class, --typedef, --enum, --function, --define, --type(class/typedef/enum), --all (default) - KindOfKind
// 4) Опция, как генерить инклуды - с кавычками или угловыми скобками: --quoted-include
// 5) Преобразовать имена типов в имена файлов

