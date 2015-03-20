//
//  WiimoteDeviveInquiry.h
//  wiiuse
//
//  Created by Benoît Pin on 27/09/10.
//  Copyright 2010 Ecole des mines. All rights reserved.
//

#import <IOBluetooth/objc/IOBluetoothDeviceInquiry.h>

@class WiimoteDeviceInquiry;

@interface WiimoteDeviceInquiry : NSObject {
	IOBluetoothDeviceInquiry *		_inquiry;
	//CFRunLoopRef*					_inquiryLoop;
	//BOOL							_busy;
	NSMutableArray*					_foundDevices;


}

-(IOReturn)startInquiry:(int)timeout;
-(BOOL)addDeviceToList:(IOBluetoothDevice*)inDevice;
-(int)getDeviceCount;
-(NSMutableArray*)getDevices;
//-(IOReturn)stopInquiry;


@end
