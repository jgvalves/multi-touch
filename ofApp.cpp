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
	hWnd = WindowFromDC(wglGetCurrentDC());
	WacomMTError res;
	res = Init::Callback(hWnd, dev, CapDevice);
	callbackType = true;		//Initializes with callback from HWND
}

//--------------------------------------------------------------
void ofApp::update() {
	if (getCallbackType()) {
		LPMSG msg;
		GetMessage(msg, hWnd, 0, 0);
		WPARAM wp = msg->wParam;
		LPARAM lp = msg->lParam;

		switch (msg->message) {
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
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	if (count > 0 && fingerino && version > 0) {
		for (int i = 0; i < count; i++) {
			ofDrawCircle(fingerino[i].X, fingerino[i].Y, 50);
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

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
		setCallbackInfo(*fingerData);
	}
	return 0;
}

WacomMTFingerCollection Callbacks::getCallbackInfo() {
	return callbackInfo;
}

void Callbacks::setCallbackInfo(WacomMTFingerCollection fingerData) {
	callbackInfo = fingerData;
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

