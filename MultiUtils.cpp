#include "MultiUtils.h"


#include "WacomMultiTouch.h"
#include "WacomMultiTouchTypes.h"
#include <windows.h>
#include "WintabUtils.h"
#include <string>
#include <memory>
#include <iostream>
#include <vector>
#include <map>


#define PACKETDATA	(PK_X | PK_Y | PK_BUTTONS | PK_NORMAL_PRESSURE | PK_CURSOR)
#define PACKETMODE	PK_BUTTONS
#include "pktdef.h"

int Init::deviceCount;



HCTX Init::InitWintabAPI(HWND hwnd_I)
{
	if (!LoadWintab())
	{
		ShowError("Wintab not available");
		return 0;
	}

	char TabletName[50] = "";
	gpWTInfoA(WTI_DEVICES, DVC_NAME, TabletName);
	gpWTInfoA(WTI_INTERFACE, IFC_WINTABID, TabletName);

	/* check if WinTab available. */
	if (!gpWTInfoA(0, 0, NULL))
	{
		ShowError("WinTab Services Not Available.");
		return FALSE;
	}

	/* get default region */
	LOGCONTEXTA logContext = { 0 };
	gpWTInfoA(WTI_DEFSYSCTX, 0, &logContext);

	// No need to specify lcInOrg* and lcInExt* as they are the entire
	// physical tablet area by default.

	// Get messages
	logContext.lcOptions |= CXO_MESSAGES;

	// Move the system cursor.
	logContext.lcOptions |= CXO_SYSTEM;

	logContext.lcPktData = PACKETDATA;
	logContext.lcPktMode = PACKETMODE;
	logContext.lcMoveMask = PACKETDATA;
	logContext.lcBtnUpMask = logContext.lcBtnDnMask;

	//// Guarantee the output coordinate space to be in screen coordinates.
	logContext.lcOutOrgX = GetSystemMetrics(SM_XVIRTUALSCREEN);
	logContext.lcOutOrgY = GetSystemMetrics(SM_YVIRTUALSCREEN);
	logContext.lcOutExtX = GetSystemMetrics(SM_CXVIRTUALSCREEN); //SM_CXSCREEN );

																 // In Wintab, the tablet origin is lower left.  Move origin to upper left
																 // so that it coincides with screen origin.
	logContext.lcOutExtY = -GetSystemMetrics(SM_CYVIRTUALSCREEN);	//SM_CYSCREEN );

																	/* open the region */
	return gpWTOpenA(hwnd_I, (LPLOGCONTEXT)&logContext, TRUE);
}

WacomMTError Init::Callback(HWND hWnd, std::vector<int>& dev, std::map<int, WacomMTCapability> CapDevice) {

	//////////////////////////////////////////////////////////////////////////////////////////////
	/////---------------------------------INICIA API MT--------------------------------------/////
	//////////////////////////////////////////////////////////////////////////////////////////////
	WacomMTError res = WacomMTInitialize(WACOM_MULTI_TOUCH_API_VERSION);
	if (res != WMTErrorSuccess) {
		ShowError("Multi Touch API failed to load");
		abort();
	}
	

	

	//////////////////////////////////////////////////////////////////////////////////////////////
	/////---------------------------------INICIA WINTAB--------------------------------------/////
	//////////////////////////////////////////////////////////////////////////////////////////////
	//InitWintabAPI(hWnd);
	if (!InitWintabAPI(hWnd)) { ShowError("Wintab Failed to Load"); abort(); }
	

	//////////////////////////////////////////////////////////////////////////////////////////////
	/////---------------------------------CONECTA DEVICE-------------------------------------/////
	//////////////////////////////////////////////////////////////////////////////////////////////
	
	deviceCount = (WacomMTGetAttachedDeviceIDs(NULL, 0));
	if(deviceCount > 0) {
		dev.resize(deviceCount, 0);
		WacomMTGetAttachedDeviceIDs(&dev[0], deviceCount * sizeof(int));


		int loopCount = (int)dev.size();
		for (int idx = 0; idx < loopCount; idx++)
		{
			WacomMTCapability cap = { 0 };
			res = WacomMTGetDeviceCapabilities(dev[idx], &cap);
			CapDevice[dev[idx]] = cap;
		}

		res = WacomMTRegisterAttachCallback(Callbacks::AttachCallback, NULL);
		if (res == WMTErrorSuccess) {
			return res;
		}
		else { ShowError("Fatal: Failed to Attach Callback"); abort();}
	}
	else { ShowError("No Device detected"); abort();}

}


//void AttachCallback(WacomMTCapability deviceInfo, void *userRef) {
//	WacomMTHitRectPtr wtHitRect = WacomMTHitRectPtr();
//
//	if (DeviceCount == 0) {
//		DeviceCount++;
//		//WacomMTError res = WacomMTRegisterFingerReadHWND(deviceInfo.DeviceID, WMTProcessingModeNone, hWnd, 5);
//		WacomMTError res = WacomMTRegisterFingerReadCallback(deviceInfo.DeviceID, wtHitRect.get(), WMTProcessingModeNone, FingerCallback, NULL);
//		if (res == WMTErrorSuccess) {
//			return;
//		}
//		else {
//			ShowError("FATAL ERROR: Callback - Register For Data");
//			abort();
//		}
//	}
//}
//
//int FingerCallback(WacomMTFingerCollection *finger, void *UserRef) {
//	if (finger) {
//		helping = *finger;
//	}
//	return 0;
//}

/*WacomMTHitRectPtr GetAppHitRect(HWND hWnd)
{
	if (true)
	{
		WINDOWINFO appWindowInfo = { 0 };
		appWindowInfo.cbSize = sizeof(appWindowInfo);
		GetWindowInfo(hWnd, &appWindowInfo);

		WacomMTHitRect hitRect = {
			(float)appWindowInfo.rcClient.left,
			(float)appWindowInfo.rcClient.top,
			(float)(appWindowInfo.rcClient.right - appWindowInfo.rcClient.left),
			(float)(appWindowInfo.rcClient.bottom - appWindowInfo.rcClient.top)
		};

		return WacomMTHitRectPtr(new WacomMTHitRect(hitRect));
	}
	return WacomMTHitRectPtr();
}*/

int Init::checkPenInput(HCTX hCtx, WPARAM wParam, POINT &ptNew, POINT &ptOld, float &pressure, int &type) {
	
	PACKET pkt;
	
	if (gpWTPacket(hCtx, wParam, &pkt)) {
		ptNew.x = pkt.pkX;
		ptNew.y = pkt.pkY;
		pressure = (pkt.pkNormalPressure)/100;
		type = pkt.pkCursor;
		


		
		return 1;
		
	}
	else { return 0; }


}

int Init::getCursorName(int type) {
	int CursorName;
	if (type == 1) {
		CursorName = 1;
	}
	else if (type == 2) {
		CursorName = 0;
		
	}
	return CursorName;
}

