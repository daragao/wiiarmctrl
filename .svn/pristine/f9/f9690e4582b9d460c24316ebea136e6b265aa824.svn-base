//
//  WiimoteEventsDispatcher.h
//  wiiuse
//
//  Created by Benoît Pin on 10/01/11.
//  Copyright 2011 Ecole des mines. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <IOBluetooth/IOBluetooth.h>
#import	"WiimoteDeviceConnection.h"
#import "wiiuse.h"

@interface WiimoteEventsDispatcher : NSObject <IOBluetoothL2CAPChannelDelegate>{
	NSMutableDictionary * wiimotesDevicesConnections;
}


-(WiimoteEventsDispatcher*)initWithNumWiimotes:(int)wiimotes;
-(WiimoteDeviceConnection*)createWiimoteDeviceConnection: (wiimote*) wm;
-(WiimoteDeviceConnection*)createWiimoteDeviceConnectionWithAddress: (wiimote*) wm: (const char*) address;
-(WiimoteDeviceConnection*)getWiimoteDeviceConnection:(char*)adr;
-(void)startRunLoop;
-(int)poll;

@end
