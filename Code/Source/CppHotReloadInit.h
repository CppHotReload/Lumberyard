//
//  Copyright (c) 2014 - 2019 C++ Hot Reload SL. All rights reserved
//
//  This file is subject to the terms and conditions defined in
//  file 'LICENSE.txt', which is part of this source code package.
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
		CPP_HOT_RELOAD_CALLBACK_BEGIN_RELOAD		  fnBeginReload,
		CPP_HOT_RELOAD_CALLBACK_END_RELOAD			  fnEndReload,
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
	    CPP_HOT_RELOAD_CALLBACK_LOG_FATAL 		fnLogFatal,
	    CPP_HOT_RELOAD_CALLBACK_LOG_CRITICAL 	fnLogCritical,
	    CPP_HOT_RELOAD_CALLBACK_LOG_ERROR 		fnLogError,
	    CPP_HOT_RELOAD_CALLBACK_LOG_WARNING 	fnLogWarning,
	    CPP_HOT_RELOAD_CALLBACK_LOG_SYSTEM 		fnLogSystem);
}