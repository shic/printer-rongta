//
//  CodepagelistTVController.h
//  PrinterExample
//
//  Created by 朱正锋 on 12/03/2018.
//  Copyright © 2018 Printer. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "PrinterManager.h"

@interface CodepagelistTVController : UITableViewController
@property (strong ,nonatomic) id<SelectDelegate> delegate;
@end
