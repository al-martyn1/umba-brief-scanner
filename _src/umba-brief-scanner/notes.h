/*! \file
    \brief Заметки (TODO, NOTE, и тп)
 */

#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>


//----------------------------------------------------------------------------
struct NoteInfo
{
    std::string    noteType;
    std::string    noteText;
    std::string    file;
    std::size_t    line;
    bool           hasCheck  = false;
    bool           isChecked = false;
};
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
struct NoteConfig
{
    std::string                        fileName; // --todo-filename
    std::unordered_set<std::string>    markers ; // в нижнем регистре  --todo-marker

    // Text must be ltrimmed
    std::string::size_type findMarker(std::string text, std::string *pFoundMarker=0) const
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

};
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
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
bool noteTextHasCheck( std::string &text
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


