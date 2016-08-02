#pragma once
//#pragma comment (lib, "bbtablet_vc100d")

#include "ofMain.h"
#include "WacomMultiTouch.h"
#include "WacomMultiTouchTypes.h"
#include "MyTabletObserver.h"
#include <string>
#include <vector>
#include <memory>
#include <windows.h>

//#define HAVE_STRUCT_TIMESPEC
//#include <pthread.h>
#include <thread>
////#define PACKETDATA	(PK_X | PK_Y | PK_BUTTONS | PK_NORMAL_PRESSURE)
////#define PACKETMODE	PK_BUTTONS
////#include "pktdef.h"

typedef std::unique_ptr<WacomMTHitRect> WacomMTHitRectPtr;


class ofApp : public ofBaseApp{
	
	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);	
		static HWND getHWND();
		static bool getCallbackType(); // True: by HWND
		static void changeCallbackType();
		//static void ofDrawManual(int setCount, int deviceID, WacomMTFingerCollection* fingers);

		static void setFingerino(WacomMTFinger* fingers);
		static void setCount(int fingerCount);
		static void setDevice(int deviceID);
		static void setVersion(int DeviceVersion);


		//Variaveis publicas
		static int DeviceCount;

	private:
		
		static bool callbackType;
		static HWND hWnd;
		char str[100];
		//pthread_t getFingerCollection;

		
		std::vector<int> dev;

		ofTrueTypeFont font;
		ofTrueTypeFont infoCallback;
		
		//WACOM
		//WacomMTFingerState finger;
		//WacomMTFinger* finger;
		WacomMTFingerCollection* FingerCollection;
		WacomMTFingerCollection* fingerData;
		std::map<int,WacomMTCapability> CapDevice;
		
		
		

		//Variaveis drawFinger
		static int count;					//Number of fingers on the screen
		static int device;					//By defaut, 0. device ID
		static WacomMTFinger* fingerino;	//A list of fingers	
		static int version;					//Quite irrelevant. If bigger than 0, input is trustworthy

		//Variaveis Caneta
		struct _pen {
			int x;
			int y;
			float pressure;
			int type;
		} pen;

		static POINT ptOld;
		static POINT ptNew;

		std::thread t;
};

class Callbacks : public ofApp {
public:
	static void AttachCallback(WacomMTCapability deviceInfo, void *userRef);
	static WacomMTError RegisterCallback(int deviceID);
	static void DetachCallback(int deviceID, void *userRef);
	static WacomMTError UnregisterCallback(int deviceID, HWND hWnd);
	static int FingerCallback(WacomMTFingerCollection *fingerPoint, void *UserRef);
	static WacomMTFingerCollection getCallbackInfo();
	static void setFingerVariables(WacomMTFingerCollection fingerData);

	static WacomMTFingerCollection callbackInfo;

private:
	
	

};

