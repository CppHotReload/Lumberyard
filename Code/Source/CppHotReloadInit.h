//
//  Copyright (c) since 2014 - 2020 C++ Hot Reload SL. All rights reserved
//

//
// C/C++ Hot Reload
//
#include "CppHotReload/HotReloadAPI.h"
//
// C/C++
//
#include <string>
#include <vector>

namespace CppHotReload
{
	void InitHotReload();
    void Init(
		CPP_HOT_RELOAD_CALLBACK_PARSING_FILE_PROGRESS fnCallBackParsingFileProgress,
		CPP_HOT_RELOAD_CALLBACK_BEGIN_RELOAD		  fnCallBackBeginReload,
		CPP_HOT_RELOAD_CALLBACK_END_RELOAD			  fnCallBackEndReload,
		CPP_HOT_RELOAD_CALLBACK_FILTER_FILE_TO_BUILD  fnCallBackFilterFileToBuild,
		CPP_HOT_RELOAD_CALLBACK_WILL_COMPILE_FILE     fnCallBackWillCompileFile,
		CPP_HOT_RELOAD_CALLBACK_DID_COMPILE_FILE      fnCallBackDidCompileFile,
		CPP_HOT_RELOAD_CALLBACK_WILL_LINK_FILE        fnCallBackWillLinkFile,
		CPP_HOT_RELOAD_CALLBACK_DID_LINK_FILE         fnCallBackDidLinkFile,
		CPP_HOT_RELOAD_CALLBACK_DID_ADD_TYPE          fnCallBackDidAddType,
		CPP_HOT_RELOAD_CALLBACK_DID_REMOVE_TYPE       fnCallBackDidRemoveType,
		CPP_HOT_RELOAD_CALLBACK_DID_INSTANCE_RELOAD   fnCallBackDidInstanceReload,
		CPP_HOT_RELOAD_CALLBACK_DID_VARMANAGER_RELOAD fnCallBackDidVarManagerReload,
		CPP_HOT_RELOAD_CALLBACK_FATAL_ERROR           fnCallBackFatalError,
	    CPP_HOT_RELOAD_CALLBACK_LOG_FATAL 			  fnLogFatal,
	    CPP_HOT_RELOAD_CALLBACK_LOG_CRITICAL 		  fnLogCritical,
	    CPP_HOT_RELOAD_CALLBACK_LOG_ERROR 			  fnLogError,
	    CPP_HOT_RELOAD_CALLBACK_LOG_WARNING 		  fnLogWarning,
	    CPP_HOT_RELOAD_CALLBACK_LOG_SYSTEM 			  fnLogSystem);
}
