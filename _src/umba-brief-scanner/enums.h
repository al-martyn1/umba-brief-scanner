#pragma once





//#!OutputFormat
enum  OutputFormat
{
    cmAboutCmd                  = 100 /*!<  */,
    cmPuzzleCmd                 = 101 /*!<  */,
    cmCalendarCmd               = 102 /*!<  */,
    cmAsciiCmd                  = 103 /*!<  */,
    cmCalcCmd                   = 104 /*!<  */,
    cmOpenCmd                   = 105 /*!<  */,
    cmChDirCmd                  = 106 /*!<  */,
    cmMouseCmd                  = 107 /*!<  */,
    cmColorCmd                  = 108 /*!<  */,
    cmSaveCmd                   = 109 /*!<  */,
    cmRestoreCmd                = 110 /*!<  */,
    cmEventViewCmd              = 111 /*!<  */,
    cmChBackground              = 112 /*!<  */,
    cmFndEventView              = 113 /*!<  */,
    cmGreetThemCmd              = 114 /*!<  */,
    cmPortSelectCmd             = 115 /*!<  */,
    cmCellsTestCmd              = 116 /*!<  */,
    cmRefreshCmd                = 117 /*!<  */,
    cmTestDlgCmd                = 118 /*!<  */,
    cmConnectCmd                = 119 /*!<  */,
    cmDisconnectCmd             = 120 /*!<  */,
    cmShowErrorCmd              = 121 /*!<  */,
    cmShowBattModules           = 122 /*!<  */,
    cmFindBattModules           = 123 /*!<  */,
    cmStartup                   = 124 /*!<  */,
    cmFindTest                  = 125 /*!<  */,
    cmFindTestTextInfo          = 126 /*!<  */,
    cmFindDeviceInfo            = 127 /*!<  */,
    cmFindPinConfigInfo         = 128 /*!<  */,
    cmTest                      = 129 /*!<  */,
    cmTestTextInfo              = 130 /*!<  */,
    cmDeviceInfo                = 131 /*!<  */,
    cmPinConfigInfo             = 132 /*!<  */,
    cmOpenIni                   = 133 /*!<  */,
    cmOpenIniAndQuit            = 134 /*!<  */,
    cmShowMinMax                = 135 /*!<  */,
    cmFindMinMax                = 136 /*!<  */,
    cmShowChargeDischargeInfo   = 137 /*!<  */,
    cmFindChargeDischargeInfo   = 138 /*!<  */,
    cmShowFanInfo               = 139 /*!<  */,
    cmFindFanInfo               = 140 /*!<  */,
    cmShowCellControl           = 141 /*!<  */,
    cmFindCellControl           = 142 /*!<  */,
    cmChargeForceOn             = 143 /*!<  */,
    cmChargeForceOff            = 144 /*!<  */,
    cmChargeAuto                = 145 /*!<  */,
    cmDischargeForceOn          = 146 /*!<  */,
    cmDischargeForceOff         = 147 /*!<  */,
    cmDischargeAuto             = 148 /*!<  */,
    cmIgnoreAramOn              = 149 /*!<  */,
    cmIgnoreAramOff             = 150 /*!<  */,
    cmFanForceOn1               = 151 /*!<  */,
    cmFanForceOff1              = 152 /*!<  */,
    cmFanAuto1                  = 153 /*!<  */,
    cmFanForceOn2               = 154 /*!<  */,
    cmFanForceOff2              = 155 /*!<  */,
    cmFanAuto2                  = 156 /*!<  */,
    cmFanForceOn3               = 157 /*!<  */,
    cmFanForceOff3              = 158 /*!<  */,
    cmFanAuto3                  = 159 /*!<  */,
    cmFanForceOn4               = 160 /*!<  */,
    cmFanForceOff4              = 161 /*!<  */,
    cmFanAuto4                  = 162 /*!<  */,
    cmVideoMode                 = 2002 /*!<  */,
    cmCellFirst                 = 2003 /*!<  */,
    cmCellEnd                   = cmCellFirst + 3*16*4 /*!<  */,
    cmCellAllFirst              = cmCellEnd /*!<  */,
    cmCellAllEnd                = cmCellAllFirst + 3*1*4 /*!<  */

}; // enum 
//#!

MARTY_CPP_MAKE_ENUM_IS_FLAGS_FOR_NON_FLAGS_ENUM(OutputFormat)


