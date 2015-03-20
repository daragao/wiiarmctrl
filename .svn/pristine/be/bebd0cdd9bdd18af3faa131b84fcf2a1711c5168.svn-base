//
//  WiimoteEventsDispatcher.m
//  wiiuse
//
//  Created by Benoît Pin on 10/01/11.
//  Copyright 2011 Ecole des mines. All rights reserved.
//

#import "WiimoteEventsDispatcher.h"
#import "WiimoteDeviceConnection.h"
#include <IOBluetooth/IOBluetoothUtilities.h>
#include "wiiuse.h"
#include "definitions.h"


@implementation WiimoteEventsDispatcher
-(WiimoteEventsDispatcher*)initWithNumWiimotes:(int)wiimotes; {
	self = [super init];
	wiimotesDevicesConnections = [NSMutableDictionary dictionaryWithCapacity:(NSInteger)wiimotes];
	return self;
}

-(WiimoteDeviceConnection*)createWiimoteDeviceConnection: (wiimote*) wm {
	IOBluetoothDeviceRef* wtr;
	WiimoteDeviceConnection * wdc;
	NSString * adrKey;
	
	wtr = (IOBluetoothDeviceRef*)wm->device;
	wdc = [[WiimoteDeviceConnection alloc] initWithDeviceRef:wtr :wm];
	adrKey = [[NSString alloc] initWithUTF8String:wm->bdaddr_str];
	[wiimotesDevicesConnections setValue:wdc forKey:adrKey];
	
	return wdc;
}

-(WiimoteDeviceConnection*)createWiimoteDeviceConnectionWithAddress:(wiimote *)wm :(const char *)address {
	NSString * ns_adr = [NSString alloc];
	if (address) {
		ns_adr = [ns_adr initWithCString:address];
		memcpy (wm->bdaddr_str, [ns_adr UTF8String], 18);
	}
	
	else {
		ns_adr = [ns_adr initWithUTF8String: wm->bdaddr_str];
	}
	
	BluetoothDeviceAddress bt_adr;
	
	if (IOBluetoothNSStringToDeviceAddress(ns_adr, &bt_adr))
		return NULL;
	
	WiimoteDeviceConnection * wdc;
	wdc = [[WiimoteDeviceConnection alloc] initWithAddress:&bt_adr :wm];
	[wiimotesDevicesConnections setValue:wdc forKey:ns_adr];
	return wdc;
	
}

-(WiimoteDeviceConnection*)getWiimoteDeviceConnection:(char*)adr {
	NSString * adrKey = [[NSString alloc] initWithCString:adr];
	return (WiimoteDeviceConnection*)[wiimotesDevicesConnections valueForKey:adrKey];
}

-(void)startRunLoop {
	[[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate distantFuture]];
}

-(int)poll {
	[self startRunLoop];
	NSEnumerator * enumerator = [wiimotesDevicesConnections objectEnumerator];
	WiimoteDeviceConnection * wdc;
	wiimote * wm;
	int wiimotesThatAnEventHasOccured = 0;

	while (wdc = (WiimoteDeviceConnection*)[enumerator nextObject]) {
		wm = [wdc getWMT];
		if ([wdc read])
			wiimotesThatAnEventHasOccured++;
	}
	return wiimotesThatAnEventHasOccured;
}


- (void)l2capChannelData:(IOBluetoothL2CAPChannel*)l2capChannel data:(void *)dataPointer length:(size_t)dataLength {
	IOBluetoothDevice * device = [l2capChannel getDevice];
	unsigned short length = (unsigned short)dataLength;
	byte * data = (byte *)dataPointer;

	if (length > MAX_PAYLOAD) {
		WIIUSE_WARNING("Dropping wiimote packet - too large");
		return;
	}
	
	EventData * ed = [[EventData alloc] init];
	ed->data = data;
	ed->length = length;

	WiimoteDeviceConnection * wdc = [wiimotesDevicesConnections valueForKey:[device getAddressString]];
	[wdc queueAdd:ed];
	
}
- (void)l2capChannelOpenComplete:(IOBluetoothL2CAPChannel*)l2capChannel status:(IOReturn)error {
	
}
- (void)l2capChannelClosed:(IOBluetoothL2CAPChannel*)l2capChannel {
	IOBluetoothDevice * device = [l2capChannel getDevice];
	WiimoteDeviceConnection * wdc = [wiimotesDevicesConnections valueForKey:[device getAddressString]];
	wiiuse_disconnected([wdc getWMT]);
}
- (void)l2capChannelReconfigured:(IOBluetoothL2CAPChannel*)l2capChannel {
	
}
- (void)l2capChannelWriteComplete:(IOBluetoothL2CAPChannel*)l2capChannel refcon:(void*)refcon status:(IOReturn)error {
	
}
- (void)l2capChannelQueueSpaceAvailable:(IOBluetoothL2CAPChannel*)l2capChannel {
	
}


@end
