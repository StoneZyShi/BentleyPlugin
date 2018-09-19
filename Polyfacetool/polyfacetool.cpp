/*--------------------------------------------------------------------------------------+
|
|     $Source: MstnExamples/Elements/Polyfacetool/polyfacetool.cpp $
|
|  $Copyright: (c) 2015 Bentley Systems, Incorporated. All rights reserved. $
|
+--------------------------------------------------------------------------------------*/
#include "polyfacetool.h"


//extern void startPolyfaceCreateTool(WCharCP);
//extern void startPolyfaceConvertTool(WCharCP);
//extern void newTest(WCharCP);
extern void ServerCreatData(WCharCP);
extern void ClientCreatData(WCharCP);
/*---------------------------------------------------------------------------------**//**
* @description  Map key-in to function
* Commands
+---------------+---------------+---------------+---------------+---------------+------*/
static MdlCommandNumber s_commandNumbers[] =
    {
	//{ startPolyfaceCreateTool, CMD_POLYFACE_CREATE },
	//{ startPolyfaceConvertTool, CMD_POLYFACE_CONVERT },
	//{ newTest, CMD_POLYFACE_NEWTEST },
	{ ServerCreatData, CMD_POLYFACE_SERVERCREATDATA },
	{ ClientCreatData, CMD_POLYFACE_CLIENTCREATDATA },
    
    // end of list
    0
    };

/*---------------------------------------------------------------------------------**//**
* @description  MdlMain
* @param        argc      The number of command line parameters sent to the application.
* @param        argv[]    The array of strings sent to the application on the command line.
* @bsimethod                                                              Bentley Systems
+---------------+---------------+---------------+---------------+---------------+------*/
extern "C" void MdlMain (int argc, WCharCP argv[])
    {
    RscFileHandle rfHandle;

    mdlResource_openFile (&rfHandle, NULL, RSC_READONLY);
    mdlState_registerStringIds (STRINGLISTID_Commands, STRINGLISTID_Prompts);
    mdlSystem_registerCommandNumbers (s_commandNumbers);
    mdlParse_loadCommandTable (NULL);
    }
