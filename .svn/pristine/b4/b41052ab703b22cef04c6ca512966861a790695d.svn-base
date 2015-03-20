//
//  io_osx.m
//  wiiuse
//
//  Created by Benoît Pin on 27/09/10.
//  Copyright 2010 Ecole des mines. All rights reserved.
//

#import <Foundation/Foundation.h>



#include "definitions.h"
#include "wiiuse_internal.h"
#include "io.h"
#include "WiimoteDeviceInquiry.h"
#include "WiimoteDeviceConnection.h"
#import "WiimoteEventsDispatcher.h"
#include <IOBluetooth/IOBluetoothUtilities.h>
#include <string.h>

#if 0
#pragma mark -
#pragma mark === wiiuse IO API ===
#endif

//static int wiiuse_connect_single(struct wiimote_t* wm, char* address);
//static NSMutableDictionary *WIIMOTES_CONNECTIONS;
static NSAutoreleasePool *GLOBAL_NSAP;
static WiimoteEventsDispatcher * global_dispatcher;

void init_os_x(int wiimotes) {
	GLOBAL_NSAP = [[NSAutoreleasePool alloc] init];
	global_dispatcher = [[WiimoteEventsDispatcher alloc] initWithNumWiimotes:wiimotes];
}

void cleanup_os_x(int wiimotes) {
	[GLOBAL_NSAP drain];
}


/**
 *	@brief Find a wiimote or wiimotes.
 *
 *	@param wm			An array of wiimote_t structures.
 *	@param max_wiimotes	The number of wiimote structures in \a wm.
 *	@param timeout		The number of seconds before the search times out.
 *
 *	@return The number of wiimotes found.
 *
 *	@see wiimote_connect()
 *
 *	This function will only look for wiimote devices.						\n
 *	When a device is found the address in the structures will be set.		\n
 *	You can then call wiimote_connect() to connect to the found				\n
 *	devices.
 */
int wiiuse_find(struct wiimote_t** wm, int max_wiimotes, int timeout)
{
	WiimoteDeviceInquiry* wi; 
	wi = [[WiimoteDeviceInquiry alloc] init];
	[wi startInquiry: timeout];
	
	NSMutableArray* devices = [wi getDevices];
	int count = (int)[devices count];
	
	NSEnumerator* enumerator;
	IOBluetoothDevice* tmpDevice;
	
	enumerator = [devices objectEnumerator];
	//wm = wiiuse_init(count);
	int i = 0;
	
	while( (tmpDevice = [enumerator nextObject]) )
	{
		struct wiimote_t* wt;
		wt = wm[i];
		wt->unid = i;
		wt->device = (IOBluetoothDeviceRef*)[tmpDevice getDeviceRef];
		memcpy (wt->bdaddr_str, [[tmpDevice getAddressString] UTF8String], 18);
		i++;
	}
	
	return count;
}


/**
 *	@brief Search the first available wiimote.
 *
 *	@param timeout		The number of seconds before the search times out.
 *
 *	@return The bluetooth address.
 *
 *	@see wiimote_connect_single()
 *
 *	This function will only look for the first wiimote device.				\n
 *	When a device its address is returned.									\n
 *	You can then call wiimote_connect_single() to connect it				\n
 */


const char * wiiuse_find_single(int timeout) {
	//NSAutoreleasePool *p = [[NSAutoreleasePool alloc] init];
	WiimoteDeviceInquiry* wi; 
	wi = [[WiimoteDeviceInquiry alloc] init];
	[wi startInquiry: timeout];
	
	NSMutableArray* devices = [wi getDevices];
	NSEnumerator* enumerator = [devices objectEnumerator];
	IOBluetoothDevice* firstDevice = [enumerator nextObject];
	
	const char * adr = [[firstDevice getAddressString] UTF8String];
	//[p drain];
	if (adr) {
		return adr;
	}
	else {
		return NULL;
	}

}


/**
 *	@brief Connect to a wiimote or wiimotes once an address is known.
 *
 *	@param wm			An array of wiimote_t structures.
 *	@param wiimotes		The number of wiimote structures in \a wm.
 *
 *	@return The number of wiimotes that successfully connected.
 *
 *	@see wiiuse_find()
 *	@see wiiuse_connect_single()
 *	@see wiiuse_disconnect()
 *
 *	Connect to a number of wiimotes when the address is already set
 *	in the wiimote_t structures.  These addresses are normally set
 *	by the wiiuse_find() function, but can also be set manually.
 */
int wiiuse_connect(struct wiimote_t** wm, int wiimotes) {
	//WIIMOTES_CONNECTIONS = [NSMutableDictionary dictionaryWithCapacity:(NSInteger)wiimotes];

	int i, connected = 0;
	WiimoteDeviceConnection* wdc;
	//IOBluetoothDeviceRef* wtr;
	IOReturn status;
	struct wiimote_t* wmt;
	
	for(i=0 ; i < wiimotes ; i++)
	{
		wmt = wm[i];
		if (WIIMOTE_IS_CONNECTED(wmt))
			continue;
		printf("try to connect wiimote: %d\n", wmt->unid);
		wdc = [global_dispatcher createWiimoteDeviceConnection:wmt];

		status = [wdc openChannelsWithDelegate:global_dispatcher];
		if (status == kIOReturnSuccess)
		{
			//wiiuse_handshake(wmt, NULL, 0);
			wiiuse_set_report_type(wmt);
			wiiuse_set_leds(wmt, WIIMOTE_LED_2 | WIIMOTE_LED_3);
			WIIMOTE_ENABLE_STATE(wmt, WIIMOTE_STATE_CONNECTED);
			
			printf("wiimote %d connected\n", wmt->unid);
			connected++;
		}
	}
	
	for(i=0 ; i < wiimotes ; i++) {
		wmt = wm[i];
		wiiuse_handshake(wmt, NULL, 0);
	}
	
	return connected;
}

/**
 *	@brief Connect to a wiimote with a known address.
 *
 *	@param wm		Pointer to a wiimote_t structure.
 *	@param address	The address of the device to connect to.
 *					If NULL, use the address in the struct set by wiiuse_find().
 *
 *	@return 1 on success, 0 on failure
 */
int wiiuse_connect_single(struct wiimote_t* wm, const char * address) {
	if (!address || strlen(address) != 17) {
		return 0;
	}
	WiimoteDeviceConnection* wdc;
	IOReturn status;

	wdc =  [global_dispatcher createWiimoteDeviceConnectionWithAddress:wm :address];
	status = [wdc openChannelsWithDelegate:global_dispatcher];

	if (status == kIOReturnSuccess) {
		WIIMOTE_ENABLE_STATE(wm, WIIMOTE_STATE_CONNECTED);
		wiiuse_handshake(wm, NULL, 0);
		wiiuse_set_report_type(wm);
		wiiuse_set_leds(wm, WIIMOTE_LED_2 | WIIMOTE_LED_3);
		return 1;
	}
	return 0;
}

/**
 *	@brief Disconnect a wiimote.
 *
 *	@param wm		Pointer to a wiimote_t structure.
 *
 *	@see wiiuse_connect()
 *
 *	Note that this will not free the wiimote structure.
 */
void wiiuse_disconnect(struct wiimote_t* wm) {
	WIIMOTE_DISABLE_STATE(wm, WIIMOTE_STATE_CONNECTED);
	WIIMOTE_DISABLE_STATE(wm, WIIMOTE_STATE_HANDSHAKE);
}

int wiiuse_io_read(struct wiimote_t* wm) {
	WiimoteDeviceConnection *wdc;
	wdc = [global_dispatcher getWiimoteDeviceConnection:wm->bdaddr_str];
	return [wdc read];
}

int wiiuse_io_write(struct wiimote_t* wm, byte* buf, int len) {
	WiimoteDeviceConnection *wdc;
	wdc = [global_dispatcher getWiimoteDeviceConnection:wm->bdaddr_str];
	IOBluetoothL2CAPChannel *chan = [wdc getCChannel];
	if ([chan writeSync:buf length:len] == kIOReturnSuccess) {
		return len;
	}
	return 0;
}

int wiiuse_poll_os_x() {
	return [global_dispatcher poll];
}

/*
void io_runloop(wiiuse_read_cb rcb)
{
	NSAutoreleasePool *p = [[NSAutoreleasePool alloc] init];
	NSEnumerator *enumerator = [WIIMOTES_CONNECTIONS objectEnumerator];
	WiimoteDeviceConnection *w;
	while (w = [enumerator nextObject]) {
		[w setEventCallback:rcb];
	}
	CFRunLoopRun();
	[p drain];
}
*/