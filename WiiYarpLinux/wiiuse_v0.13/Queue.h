//
//  Queue.h
//  wiiuse
//
//  Created by Benoît Pin on 07/01/11.
//  Copyright 2011 Ecole des mines. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface Queue : NSObject {
	NSMutableArray * objects;
}

-(void)pushObject:(id)object;
-(id)popObject;
-(NSUInteger)count;

@end
