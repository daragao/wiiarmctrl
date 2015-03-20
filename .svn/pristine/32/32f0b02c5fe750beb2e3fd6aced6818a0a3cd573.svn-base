//
//  WiimoteDeviveInquiry.m
//  wiiuse
//
//  Created by Benoît Pin on 27/09/10.
//  Copyright 2010 Ecole des mines. All rights reserved.
//

#import "WiimoteDeviceInquiry.h"
#import <IOBluetooth/IOBluetooth.h>


@implementation WiimoteDeviceInquiry

-(IOReturn)startInquiry:(int)timeout
{
	IOReturn	status;
	
	_inquiry = [IOBluetoothDeviceInquiry	inquiryWithDelegate:self];
	[_inquiry setSearchCriteria: kBluetoothServiceClassMajorAny
			   majorDeviceClass: kBluetoothDeviceClassMajorPeripheral
			   minorDeviceClass: kBluetoothDeviceClassMinorPeripheral2Joystick];
	
	[_inquiry setInquiryLength: timeout];
	
	status = [_inquiry	start];
	CFRunLoopRun();
	if( status == kIOReturnSuccess )
	{
		[_inquiry	retain];
	}
	else
	{
		printf("Search Failed");
	}
	
	return( status );

}

- (void)	deviceInquiryStarted:(IOBluetoothDeviceInquiry*)sender
{
	printf("Searching for Devices...\n");
}

- (void)	deviceInquiryDeviceFound:(IOBluetoothDeviceInquiry*)sender	device:(IOBluetoothDevice*)device
{
	printf("found: %s @ %s\n",
		   [[device getNameOrAddress] UTF8String],
		   [[device getAddressString] UTF8String]);
	[self addDeviceToList:device];
	//[_messageText setObjectValue:[NSString stringWithFormat:@"Found %d devices...", [[sender foundDevices] count]]];
}

- (void)	deviceInquiryComplete:(IOBluetoothDeviceInquiry*)sender	error:(IOReturn)error	aborted:(BOOL)aborted
{
	if( aborted )
	{
		printf("inquiry stopped.\n");
	}
	else
	{
		printf("Idle (inquiry complete).\n");
	}
	
	[_inquiry stop];
	[_inquiry release];
	_inquiry = nil;
	CFRunLoopStop(CFRunLoopGetCurrent());		
}

-(BOOL)addDeviceToList:(IOBluetoothDevice*)inDevice
{
	NSEnumerator*					enumerator;
	IOBluetoothDevice*				tmpDevice;
	const BluetoothDeviceAddress* 	newDeviceAddress = [inDevice getAddress];
	
	if( !_foundDevices )
	{
		_foundDevices = [[NSMutableArray alloc] initWithCapacity:1];
		if( !_foundDevices ) return( FALSE );
		[_foundDevices retain];
	}
	
	// walk the devices in the array.
	
	enumerator = [_foundDevices objectEnumerator];
	if( enumerator )
	{
		const BluetoothDeviceAddress* tempAddress = NULL;
		
		while( (tmpDevice = [enumerator nextObject]) )
		{
			tempAddress = [tmpDevice getAddress];
			
			if( memcmp( newDeviceAddress, tempAddress, sizeof( BluetoothDeviceAddress ) ) == 0 )
			{
				// Already have it.
				return( FALSE );
			}
		}
	}
	
	[_foundDevices addObject:inDevice];
	
	// Return that we haven't seen it.
	
	return( TRUE );
}

-(int)getDeviceCount
{
	return (int)[_foundDevices count];
}

-(NSMutableArray*)getDevices
{
	return _foundDevices;
}

@end
