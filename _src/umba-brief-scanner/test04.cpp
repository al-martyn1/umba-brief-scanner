/*! \file
    \brief Тестируем notes
*/

#include "umba/umba.h"
#include "umba/app_main.h"
//

#include <string>
#include <iostream>

#include "notes.h"

//

#include <iostream>


std::unordered_map<std::string, NoteConfig> makeNoteConfigs()
{
    std::unordered_map<std::string, NoteConfig>  notesCfg;

    notesCfg["todo"   ] = { {}, {"todo"}};
    notesCfg["att"    ] = { {}, {"attention", "att", "atte", "!!!"}};
    notesCfg["note"   ] = { {}, {"note"}};
    notesCfg["undone" ] = { {}, {"undone"}};
    notesCfg["warning"] = { {}, {"warning", "warn"}};

    return notesCfg;
}

const std::unordered_map<std::string, NoteConfig>& getNoteConfigs()
{
    static auto m = makeNoteConfigs();
    return m;
}

std::unordered_set<std::string> makeNoteStrips()
{
    std::unordered_set<std::string> noteStrips = {"!", "!!!", ":"};
    return noteStrips;
}

const std::unordered_set<std::string>& getNoteStrips()
{
    static auto s = makeNoteStrips();
    return s;
}

using std::cout;

inline
void testParseNote(std::string noteText)
{
    //cout << "\n";
    cout << "NoteText: " << noteText << "\n";

    std::string foundNoteType  ;
    std::string foundNoteMarker;

    if (!noteTextTestForMarkersAndStrip(noteText, getNoteConfigs(), getNoteStrips(), &foundNoteType, &foundNoteMarker))
    {
        cout << "Not a note\n\n";
        return;
    }

    cout << "Found note, type: " << foundNoteType << ", marker: " << foundNoteMarker << "\n";

    bool checked = false;
    if (noteTextHasCheck(noteText, &checked))
    {
        cout << "Note hase chesk, state: " << (checked ? "X" : "_") << "\n";
    }

    cout << "Final text: [" << noteText << "]\n";
    cout << "\n";
}


UMBA_APP_MAIN()
{
    testParseNote(" Simple comment");
    testParseNote(" TODO: simple todo");
    testParseNote(" TODO: [X] simple todo and checked check mark");
    testParseNote(" TODO: [ ] simple todo and unchecked check mark");
    testParseNote(" TODO: !!! todo with tripple '!'");
    testParseNote(" TODO: !!! [X] todo with tripple '!' and checked check mark");
    testParseNote(" TODO: !!! [ ] todo with tripple '!' and unchecked check mark");
    testParseNote("!!! attention !!!");
    testParseNote("ATTE attention");
    testParseNote("UNDONE uuuu");

    return 0;
}


