//
//  WiimoteDeviceConnection.m
//  wiiuse
//
//  Created by Benoît Pin on 28/09/10.
//  Copyright 2010 Ecole des mines. All rights reserved.
//

#import "wiiuse_internal.h"
#import "WiimoteDeviceConnection.h"
#import <IOBluetooth/IOBluetooth.h>
#import <IOBluetooth/BluetoothAssignedNumbers.h>
#import "definitions.h"
#import "events.h"


@implementation WiimoteDeviceConnection

-(WiimoteDeviceConnection*)initWithDeviceRef:
		(IOBluetoothDeviceRef*)deviceRef:
		(struct wiimote_t*) wm
{
	self = [super init];
	if (self) {
		_device = [IOBluetoothDevice withDeviceRef:deviceRef] ;
		_wm = wm;
	}
	eventsQueue = [[Queue alloc] init];
	return self;
}

-(WiimoteDeviceConnection*)initWithAddress:
	(const BluetoothDeviceAddress *) address:
	(struct wiimote_t*) wm
{
	self = [super init];
	if (self) {
		_device = [IOBluetoothDevice withAddress:address];
		_wm = wm;
	}
	eventsQueue = [[Queue alloc] init];
	return self;
}


-(IOReturn)openChannelsWithDelegate:(id)delegate
{
	IOReturn ret;
	ret = [_device openL2CAPChannelSync:&_cchannel
				   withPSM:(BluetoothL2CAPPSM)kBluetoothL2CAPPSMHIDControl
				   delegate:delegate];
	
	if (ret == kIOReturnSuccess)
	{
		ret = [_device openL2CAPChannelSync:&_ichannel
					   withPSM:(BluetoothL2CAPPSM)kBluetoothL2CAPPSMHIDInterrupt
					   delegate:delegate];
	}
	
	return ret;
}

-(void)setEventCallback: (wiiuse_read_cb)rcb
{
	_read_callback = rcb;
}

-(IOBluetoothL2CAPChannel*)getCChannel {
	return _cchannel;
}

-(IOBluetoothL2CAPChannel*)getIChannel {
	return _ichannel;
}

-(struct wiimote_t*)getWMT {
	return _wm;
}

-(void)queueAdd:(EventData*) ed {
	[eventsQueue pushObject:ed];
}

-(int)read {
	int evnt = 0;
	_wm->event = WIIUSE_NONE;
	if ([eventsQueue count]) {
		while ([eventsQueue count]) {
			EventData * ed = (EventData*)[eventsQueue popObject];
			if (ed->length) {
				memcpy((void*)_wm->event_buf, ed->data, ed->length);
				propagate_event(_wm, _wm->event_buf[1], _wm->event_buf+2);
				evnt += (_wm->event != WIIUSE_NONE);
				memset(_wm->event_buf, 0, sizeof(_wm->event_buf));
			}
			[ed release];
		}
	}
	else {
		idle_cycle(_wm);
	}

	return evnt;
}


/*
 * IOBluetoothL2CAPChannelDelegate protocol
 */
- (void) l2capChannelData: (IOBluetoothL2CAPChannel *) l2capChannel
					 data: (byte *) data
				   length: (unsigned short) length
{
	printf("data received (%d)\n", length);
	if (length > MAX_PAYLOAD) {
		WIIUSE_WARNING("Dropping wiimote packet - too large");
		return;
	}
	
	EventData * ed = [[EventData alloc] init];
	ed->data = data;
	ed->length = length;
	[eventsQueue pushObject:ed];
	

	//memcpy((void*)_wm->event_buf, data, length);
	
	//propagate_event(_wm, _wm->event_buf[1], _wm->event_buf+2);
	//(*_read_callback)(_wm, data, length);
	//memset((void*)_wm->event_buf, 0, sizeof(_wm->event_buf));

	
}
- (void)l2capChannelOpenComplete:(IOBluetoothL2CAPChannel*)l2capChannel status:(IOReturn)error
{

}
- (void)l2capChannelClosed:(IOBluetoothL2CAPChannel*)l2capChannel
{
	printf("disconnect.\n");
	wiiuse_disconnect(_wm);
	[l2capChannel release];
}
- (void)l2capChannelReconfigured:(IOBluetoothL2CAPChannel*)l2capChannel;
{

}
- (void)l2capChannelWriteComplete:(IOBluetoothL2CAPChannel*)l2capChannel refcon:(void*)refcon status:(IOReturn)error
{

}
- (void)l2capChannelQueueSpaceAvailable:(IOBluetoothL2CAPChannel*)l2capChannel
{

}

@end

@implementation EventData

@end

