#include "ofApp.h"
#include "MultiUtils.h"

#include <Windows.h>

HWND ofApp::hWnd;
bool ofApp::callbackType;
WacomMTFingerCollection Callbacks::callbackInfo;

int ofApp::count;
int ofApp::device;
int ofApp::version;
WacomMTFinger* ofApp::fingerino;

POINT ofApp::ptOld;
POINT ofApp::ptNew;


//--------------------------------------------------------------
void ofApp::setup() {
	callbackType = true;		//Initializes with callback from HWND
	hWnd = WindowFromDC(wglGetCurrentDC());
	WacomMTError res;
	res = Init::Callback(hWnd, dev, CapDevice);	
}

//--------------------------------------------------------------
void ofApp::update() {
	
		MSG msg;
		while(GetMessage(&msg, hWnd, 0, 0));
		WPARAM wp = msg.wParam;
		LPARAM lp = msg.lParam;

		switch (msg.message) {
			case WM_FINGERDATA:
				count = ((WacomMTFingerCollection*)lp)->FingerCount;
				device = ((WacomMTFingerCollection*)lp)->DeviceID;
				version = ((WacomMTFingerCollection*)lp)->Version;
				fingerino = ((WacomMTFingerCollection*)lp)->Fingers;
				break;
			case WM_KEYDOWN:
				switch (wp) {
					case 'C':
						changeCallbackType();
						Callbacks::UnregisterCallback(0, hWnd);
						Callbacks::RegisterCallback(0);
						break;
				}
				break;
			case WT_PACKET:
				if (Init::checkPenInput((HCTX)lp, wp, ptNew, ptOld, pen.pressure, pen.type)) {
					pen.x = ptNew.x;
					pen.y = ptNew.y;

					ptOld = ptNew;
				}
				break;
			default:
				break;
		}
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetHexColor(0x666666);
	sprintf(str, "Callback: %s", getCallbackType() ? "Window" : "Event");
	infoCallback.loadFont("Lato-Black.ttf", 10);
	infoCallback.drawString(str, 103 , 58);

	if (count > 0 && fingerino && version > 0) {		//Pen & Touch
		if (pen.x && pen.y) {
			ofSetColor(0x000000);
			ofDrawCircle(pen.x, pen.y, 10 * pen.pressure);

			//ofSetHexColor(0xD1D1D1);
			//sprintf(str, "%s", Init::getCursorName(pen.type) ? "Stylus" : "Eraser");
			//font[10].loadFont("Lato-Black.ttf", 10);
			//font[10].drawString(str, pen.x - ((font[10].stringWidth(str)) / 2), pen.y + ((font[10].stringHeight(str))) / 2);
		}
		for (int i = 0; i < count; i++) {
			ofSetColor(0x000000);
			ofDrawCircle(fingerino[i].X, fingerino[i].Y, 50);

			//ofSetHexColor(0xD1D1D1);
			//sprintf(str, "%d", fingerino[i].FingerID);
			//font[i].loadFont("Lato-Black.ttf", 10);
			//font[i].drawString(str, fingerino[i].X - ((font[i].stringWidth(str)) / 2), fingerino[i].Y + ((font[i].stringHeight(str))) / 2);
		}
	}
	else if (pen.x && pen.y) {							//Only Pen
		ofSetColor(0x000000);
		ofDrawCircle(pen.x, pen.y, 10*pen.pressure);

		//ofSetHexColor(0xD1D1D1);
		//sprintf(str, "%s", Init::getCursorName(pen.type) ? "Stylus" : "Eraser");
		//font[10].loadFont("Lato-Black.ttf", 10);
		//font[10].drawString(str, pen.x - ((font[10].stringWidth(str)) / 2), pen.y + ((font[10].stringHeight(str))) / 2);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key = 'c') {
		changeCallbackType();
		Callbacks::UnregisterCallback(dev[0], hWnd);
		Callbacks::RegisterCallback(dev[0]);
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

void Callbacks::AttachCallback(WacomMTCapability deviceInfo, void* userRef) {
	WacomMTError res;
	if (getCallbackType()) 
		res = WacomMTRegisterFingerReadHWND(deviceInfo.DeviceID, WMTProcessingModeNone, getHWND(), 5);
	else res = WacomMTRegisterFingerReadCallback(deviceInfo.DeviceID, WacomMTHitRectPtr().get(), WMTProcessingModeNone, FingerCallback, NULL);

	if (res != WMTErrorSuccess) {
		ShowError("Finger Read Failed to attach");
	}
	return;
}

void Callbacks::DetachCallback(int deviceID, void* userRef) {
	UnregisterCallback(0, getHWND());
}

WacomMTError Callbacks::RegisterCallback(int deviceID) {
	if (getCallbackType())
		return WacomMTRegisterFingerReadHWND(deviceID, WMTProcessingModeNone, ofApp::getHWND(), 5);
	return WacomMTRegisterFingerReadCallback(deviceID, WacomMTHitRectPtr().get(), WMTProcessingModeNone, FingerCallback, NULL);
	
}

WacomMTError Callbacks::UnregisterCallback(int deviceID, HWND hWnd) {
	if (getCallbackType())
		return WacomMTUnRegisterFingerReadCallback(deviceID, WacomMTHitRectPtr().get(), WMTProcessingModeNone, NULL);
	return WacomMTUnRegisterFingerReadHWND(hWnd);
}

int Callbacks::FingerCallback(WacomMTFingerCollection* fingerData, void* userRef) {
	if (fingerData) {
		setFingerino(fingerData->Fingers);
		setCount(fingerData->FingerCount);
		setDevice(fingerData->DeviceID);
		setVersion(fingerData->Version);
	}
	return 0;
}

WacomMTFingerCollection Callbacks::getCallbackInfo() {
	return callbackInfo;
}

void Callbacks::setFingerVariables(WacomMTFingerCollection fingerData) {
	 
}

HWND ofApp::getHWND() {
	return hWnd;
}

bool ofApp::getCallbackType() {
	return callbackType;
}

void ofApp::changeCallbackType() {
	callbackType = !callbackType;
}

void ofApp::setFingerino(WacomMTFinger* fingers) {
	fingerino = fingers;
}
void ofApp::setCount(int fingerCount) {
	count = fingerCount;
}
void ofApp::setDevice(int deviceID){
	device = deviceID;
}
void ofApp::setVersion(int DeviceVersion) {
	version = DeviceVersion;
}
