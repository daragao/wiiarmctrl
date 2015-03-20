//
//  Queue.m
//  wiiuse
//
//  Created by Benoît Pin on 07/01/11.
//  Copyright 2011 Ecole des mines. All rights reserved.
//

#import "Queue.h"


@implementation Queue

-(id) init {
	if (self = [super init]) {
		objects = [[NSMutableArray alloc] init];
	}
	return self;
}

-(void)pushObject:(id)object {
	[objects addObject:object];
}

-(id)popObject {
	id object = nil;
	if ([objects count] > 0) {
		object = [[[objects objectAtIndex:0] retain] autorelease];
		[objects removeObjectAtIndex:0];
	}
	return object;
}

-(NSUInteger)count {
	return [objects count];
}

@end
