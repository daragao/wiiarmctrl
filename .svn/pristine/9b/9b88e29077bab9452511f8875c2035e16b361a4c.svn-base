//
//  WiimoteDeviceConnection.h
//  wiiuse
//
//  Created by Benoît Pin on 28/09/10.
//  Copyright 2010 Ecole des mines. All rights reserved.
//

#import <IOBluetooth/IOBluetooth.h>
#import "Queue.h"
#include "wiiuse.h"

@interface WiimoteDeviceConnection : NSObject {
	IOBluetoothDevice * _device;
	struct wiimote_t * _wm;
	IOBluetoothL2CAPChannel * _cchannel; // control interface
	IOBluetoothL2CAPChannel * _ichannel; // interrupt interface
	Queue * eventsQueue;
	wiiuse_read_cb _read_callback;
}

-(WiimoteDeviceConnection*)initWithDeviceRef:
		(IOBluetoothDeviceRef*)deviceRef:
		(struct wiimote_t*)wm;

-(WiimoteDeviceConnection*)initWithAddress:
		(const BluetoothDeviceAddress *) address:
		(struct wiimote_t*) wm;

-(IOReturn)openChannelsWithDelegate:(id)delegate;
-(IOBluetoothL2CAPChannel*)getCChannel;
-(IOBluetoothL2CAPChannel*)getIChannel;
-(struct wiimote_t*)getWMT;
-(void)setEventCallback: (wiiuse_read_cb)rcb;
-(void)queueAdd:(id)ed;
-(int)read;

@end

@interface EventData : NSObject
{	
	@public
	byte * data;
	unsigned short length;
}

@end

