#include <stdio.h>
#include <string.h>

#include "monitor.h"
#include "photoAnalog.h"
#include "mSecExe.h"
#include "UttecUtil.h"
//monitor_t monitor::m_monitor = {0,};

eMonitor_t monitor::m_monitorResult = eMNoProblem;
bool monitor::m_lampOk = true;

static Timeout monitorT;
static Timeout trafficT;

static bool bAckTimeoutFlag = true;
static uint32_t ulTrafficCount = 0;
static bool bChangeCountFlag = false;

static void timeoutM(){
	printf("\r\ntimeoutAck\n\r");
	bAckTimeoutFlag = false;
}
static void timeoutTraffic(){
	printf("\r\n------------ timeoutTraffic\n\r");
	ulTrafficCount++;
	bChangeCountFlag = true;
}

monitor::monitor(){
}

void monitor::clearTrafficCountFlag(){
	bChangeCountFlag = false;
}
bool monitor::getTrafficCountFlag(){
	return bChangeCountFlag;
}

uint32_t monitor::getTraffic(){
	return ulTrafficCount;
}

void monitor::setTrafficFlag(){
		trafficT.attach(&timeoutTraffic, 1);
}

bool monitor::isLampOk(){
	m_lampOk = true;
	mSecExe myMsec;
	photoAnalog myPhoto;
	UttecUtil myUtil;
	myUtil.setWdt(5);
	float photo;
	monitorT.attach(&timeoutM,1);
	bAckTimeoutFlag = true;
	myMsec.setDirectDim(0);
	while(bAckTimeoutFlag){
		myUtil.setWdtReload();
		wait(0.1);
	}
	photo = myPhoto.getPhotoAnalog();
	printf("myPhoto = %f\r\n",photo);
	wait(0.2);
	monitorT.attach(&timeoutM,1);
	bAckTimeoutFlag = true;
	myMsec.setDirectDim(1);
	while(bAckTimeoutFlag){
		myUtil.setWdtReload();
		wait(0.1);
	}
	photo -= myPhoto.getPhotoAnalog();
	printf("difference = %f\r\n",photo);	
	if(photo > 0.2){
		printf("Ok Lamp\r\n");
	}
	else{
		printf("Lamp Error \r\n");
		m_lampOk = false;
	}
	return m_lampOk;
}

eMonitor_t monitor::reportMonitorResult(){
	if(m_lampOk) return eMNoProblem;
	else return eMLedOut;
}


