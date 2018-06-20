//
//  CBBlueToothTest.mm
//  BlueToothTestProject-mobile
//
//  Created by miaolin on 2018/6/19.
//

#import <Foundation/Foundation.h>
#import <CoreBluetooth/CoreBluetooth.h>

#include "CBBlueTooth.h"

@interface CBPeripheral (RSSI)

@property (nonatomic, strong) NSNumber *myRSSI;

@end

@implementation CBPeripheral (RSSI)

static char *my_rssi = "my_ssi";

- (NSNumber *)myRSSI {
    return objc_getAssociatedObject(self, my_rssi);
}

- (void)setMyRSSI:(NSNumber *)myRSSI {
    objc_setAssociatedObject(self, my_rssi, myRSSI, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
}

@end


@interface CBBlueToothTest : NSObject<CBCentralManagerDelegate,CBPeripheralDelegate>
    @property (nonatomic, strong) CBCentralManager *centralManager;
    @property (nonatomic, strong) CBPeripheral *myPeripheral;
    @property (nonatomic, strong) NSMutableArray *dataSource;
    @property (nonatomic, copy) NSMutableData *data;

    - (void) initCBBlueToothTest;
    - (void) scanTimeout:(NSTimer*)timer;

@end

CBBlueTooth::CBBlueTooth()
{
    _blueToothImpl = [[CBBlueToothTest alloc] init];
    
    [(CBBlueToothTest *)_blueToothImpl initCBBlueToothTest];
}

CBBlueTooth::~CBBlueTooth()
{
    
}

@implementation CBBlueToothTest

- (void) initCBBlueToothTest {
    _centralManager = [[CBCentralManager alloc] initWithDelegate:self queue:nil];
    _dataSource = [[NSMutableArray arrayWithCapacity:0] retain];
    _data = [[[NSMutableData alloc] init] retain];
}

- (void) scanTimeout:(NSTimer*)timer
{
    if (_centralManager!=NULL){
        [_centralManager stopScan];
    }else{
        NSLog(@"_centralManager is Null!");
    }
    NSLog(@"scanTimeout");
}

- (void)centralManagerDidUpdateState:(nonnull CBCentralManager *)central {
    switch (central.state) {
        case CBManagerStateUnknown:
            NSLog(@"State unknown, update imminent");
            break;
        case CBManagerStateResetting:
            NSLog(@"The connection with the system service was momentarily lost, update imminent.");
            break;
        case CBManagerStateUnsupported:
            NSLog(@"The platform doesn't support the Bluetooth Low Energy Central/Client role.");
            break;
        case CBManagerStateUnauthorized:
            NSLog(@"The application is not authorized to use the Bluetooth Low Energy role.");
            break;
        case CBManagerStatePoweredOff:
            NSLog(@"Bluetooth is currently powered off.");
            break;
        case CBManagerStatePoweredOn:
            NSLog(@"Bluetooth is currently powered on and available to use.");
            //开始扫描外设
            //第一个参数为 nil 则会扫描所有可连接设备.
            //也可以指定一个 CBUUID对象，则只扫描注册用指定服务的设备
            //扫描到外设后，会调用发现外设委托方法
            [_centralManager scanForPeripheralsWithServices:nil options:nil];
            [NSTimer scheduledTimerWithTimeInterval:20.0f target:self selector:@selector(scanTimeout:) userInfo:nil repeats:NO];
            break;
    }
}

//发现外设委托
- (void)centralManager:(CBCentralManager *)central didDiscoverPeripheral:(CBPeripheral *)peripheral advertisementData:(NSDictionary<NSString *, id> *)advertisementData RSSI:(NSNumber *)RSSI {
    NSLog(@"外设: %@",peripheral);
    if ([_dataSource containsObject:peripheral]) {
        [_dataSource removeObject:peripheral];
        [_dataSource insertObject:peripheral atIndex:0];
    }
    else {
        [_dataSource addObject:peripheral];
    }
    peripheral.myRSSI = RSSI;
    //[_tableView reloadData];
}

@end
