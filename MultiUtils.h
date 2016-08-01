#pragma once

#include "WacomMultiTouch.h"
#include "WacomMultiTouchTypes.h"
#include "ofApp.h"
#include <windows.h>
#include <WintabUtils.h>
#include <bbtabwin.h>
#include <string>
#include <map>
#include <memory>
#include <iostream>
#include <vector>
//#include <mutex>


//#include <PKTDEF.H>





class Init {
public:
	static WacomMTError Callback(HWND hWnd, std::vector<int>& dev, std::map<int, WacomMTCapability> CapDevice);
	static int checkPenInput(HCTX hCtx, WPARAM wParam, POINT &ptNew, POINT &ptOld, float &pressure, int &type);
	static int getCursorName(int type);
	static HCTX InitWintabAPI(HWND hwnd_I);

private:
	static int deviceCount;
};


