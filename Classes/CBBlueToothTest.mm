//
//  CBBlueToothTest.mm
//  BlueToothTestProject-mobile
//
//  Created by miaolin on 2018/6/19.
//

#import <Foundation/Foundation.h>
#import <CoreBluetooth/CoreBluetooth.h>
#import <BLESettingHeader.h>

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

#pragma C++ PART

CBBlueTooth::CBBlueTooth() {
    _blueToothImpl = [[CBBlueToothTest alloc] init];
    
    [(CBBlueToothTest *)_blueToothImpl initCBBlueToothTest];
}

CBBlueTooth::~CBBlueTooth() {
    
}

CBBlueTooth *CBBlueTooth::create() {
    CBBlueTooth *ret = new CBBlueTooth();
    if (ret && ret->init())
        ret->autorelease();
    else
        CC_SAFE_DELETE(ret);
    return ret;
}

bool CBBlueTooth::init() {
    return true;
}

std::pair<std::string, int> CBBlueTooth::getPeripheralByIndex(ssize_t idx)
{
    NSMutableArray *dataSource = ((CBBlueToothTest *)_blueToothImpl).dataSource;
    CBPeripheral *peripheral = dataSource[idx];
    
    std::string str = [peripheral.name length] ? std::string([peripheral.name UTF8String]) : "No name";
    
    return std::make_pair(str, [peripheral.myRSSI intValue]);
}

#pragma Objective-C PART

@implementation CBBlueToothTest

- (void) initCBBlueToothTest {
    _centralManager = [[CBCentralManager alloc] initWithDelegate:self queue:nil];
    _dataSource = [[NSMutableArray arrayWithCapacity:0] retain];
    _data = [[[NSMutableData alloc] init] retain];
}

- (void) scanTimeout:(NSTimer*)timer {
    if (_centralManager!=NULL){
        [_centralManager stopScan];
    }else{
        NSLog(@"_centralManager is Null!");
    }
    NSLog(@"scanTimeout");
}

#pragma mark - CBCentralManagerDelegate

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
            //掃描指定device uuid, 帶入nil會找所有device
            //有找到會調用 didDiscoverPeripheral
            CBUUID *deviceInfo_uuid = [CBUUID UUIDWithString:DEVICE_UUID];
            NSArray<CBUUID *> *uuidArray = @[deviceInfo_uuid];
            
            [_centralManager scanForPeripheralsWithServices:uuidArray options:nil];
            //[_centralManager scanForPeripheralsWithServices:nil options:nil];
            //可設定搜尋幾秒 但如果是指定device uuid的話就可以不用限制
            [NSTimer scheduledTimerWithTimeInterval:20.0f target:self selector:@selector(scanTimeout:) userInfo:nil repeats:NO];
            break;
    }
}

//找到設備
- (void)centralManager:(CBCentralManager *)central didDiscoverPeripheral:(CBPeripheral *)peripheral advertisementData:(NSDictionary<NSString *, id> *)advertisementData RSSI:(NSNumber *)RSSI {
    NSLog(@"找到設備: %@",peripheral.name);
    if ([_dataSource containsObject:peripheral]) {
        [_dataSource removeObject:peripheral];
        [_dataSource insertObject:peripheral atIndex:0];
    }
    else {
        [_dataSource addObject:peripheral];
    }
    peripheral.myRSSI = RSSI;
    
    Value eventValue((int)_dataSource.count);
    
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("NotifyRefreshDataList", &eventValue);
}

//連接成功
- (void)centralManager:(CBCentralManager *)central didConnectPeripheral:(CBPeripheral *)peripheral {
    NSLog(@"成功連接到 %@",peripheral.name);
    
    [central stopScan];
    NSLog(@"停止掃描");
    
    [self.data setLength:0];
    //設 delegate 以便callback
    [peripheral setDelegate:self];
    
    //尋找主service跟電池的service
    NSArray<CBUUID *> *uuidArray = @[[CBUUID UUIDWithString:MAIN_SERVICE_UUID],
                                     [CBUUID UUIDWithString:BATTERY_UUID]];
    [peripheral discoverServices:uuidArray];
}

//連接失敗
- (void)centralManager:(CBCentralManager *)central didFailToConnectPeripheral:(CBPeripheral *)peripheral error:(nullable NSError *)error {
    NSLog(@"連接失敗:%@",error);
    [self cleanup];
}

//連接中斷
- (void)centralManager:(CBCentralManager *)central didDisconnectPeripheral:(CBPeripheral *)peripheral error:(nullable NSError *)error {
    NSLog(@"連接中斷%@",error);
    self.myPeripheral  = nil;
    //重啟掃描
    CBUUID *deviceInfo_uuid = [CBUUID UUIDWithString:DEVICE_UUID];
    NSArray<CBUUID *> *uuidArray = @[deviceInfo_uuid];
    [central scanForPeripheralsWithServices:uuidArray options:nil];
}

#pragma mark - CBPeripheralDelegate

- (void)peripheral:(CBPeripheral *)peripheral didDiscoverServices:(nullable NSError *)error {
    //連接device後 啟動 discoverServices 找到service的callback
    if (error) {
        NSLog(@"Error discover services : %@",error.localizedDescription);
        [self cleanup];
        return;
    }
    
    NSLog(@"服务: %@",peripheral.services);
    
    //掃描每个service的Characteristics
    for (CBService *service in peripheral.services) {
        NSLog(@"%@",service.UUID);
        
        [peripheral discoverCharacteristics:nil forService:service];
    }
}

- (void)peripheral:(CBPeripheral *)peripheral didDiscoverCharacteristicsForService:(CBService *)service error:(NSError *)error {
    if (error) {
        NSLog(@"Error Discover caracteristics : %@",error.localizedDescription);
        [self cleanup];
        return;
    }
    //這裏有可能對不同的 characteristic 進行不同的處理
    for (CBCharacteristic *characteristic  in service.characteristics) {
        //打開Characteristic的notify, 當有notify回傳時會call didUpdateValueForCharacteristic
        [peripheral setNotifyValue:YES forCharacteristic:characteristic];
    }
    
    //    //获取 Characteristic,读取数据
    //    //会调用 didUpdateValueForCharacteristic 方法
    //    for (CBCharacteristic *characteristic  in service.characteristics) {
    //        [peripheral readValueForCharacteristic:characteristic];
    //    }
    //
    //    //搜索Characteristic的Descriptors，读到数据
    //    //会调用 didDiscoverDescriptorsForCharacteristic 方法
    //    for (CBCharacteristic *characteristic  in service.characteristics) {
    //        [peripheral discoverDescriptorsForCharacteristic:characteristic];
    //    }
}

//notify相關
- (void)peripheral:(CBPeripheral *)peripheral didUpdateNotificationStateForCharacteristic:(CBCharacteristic *)characteristic error:(NSError *)error {
    //檢查此characteristic有沒有notify的功能 沒有就會被關掉
    if (error) {
        NSLog(@"Error notificaiton state: %@",error.localizedDescription);
    }
    if (characteristic.isNotifying)
        //通知開啟
        NSLog(@"開啟 %@ 通知",characteristic);
    else
        //通知關閉
        NSLog(@"關閉 %@ 通知",characteristic);
}

//获取characteristic的值
- (void)peripheral:(CBPeripheral *)peripheral didUpdateValueForCharacteristic:(nonnull CBCharacteristic *)characteristic error:(nullable NSError *)error {
    if (error) {
        NSLog(@"Error characteristic : %@",error.localizedDescription);
        return;
    }
    
    //讀取數據 這邊需要判斷現在傳資料過來的 characteristic 是哪一個 分別處理
    NSString *stringFromData = [[NSString alloc]initWithData:characteristic.value encoding:NSUTF8StringEncoding];
    
    if ([stringFromData isEqualToString:@"EOM"]) {
        
        //UIImage *image = [UIImage imageWithData:self.data];
        //self.imagaView.image = image;
        
        //取消訂閱
        [peripheral setNotifyValue:NO forCharacteristic:characteristic];
        
        //[peripheral w
        
        //中斷連接
        //[_centralManager cancelPeripheralConnection:peripheral];
    }
    
    //[self.data appendData:characteristic.value];
    
    NSLog(@"收到值: %@",stringFromData);
}

//搜索characteristic的 Descriptors
- (void)peripheral:(CBPeripheral *)peripheral didDiscoverDescriptorsForCharacteristic:(CBCharacteristic *)characteristic error:(NSError *)error {
    if (!error) {
        for (CBDescriptor *descroptor in characteristic.descriptors) {
            NSLog(@"Descriptor uuid: %@",descroptor.UUID);
        }
    }
}

//訊號強度
- (void)peripheral:(CBPeripheral *)peripheral didReadRSSI:(NSNumber *)RSSI error:(NSError *)error {
    if (!error) {
        peripheral.myRSSI = RSSI;
    }
}

//接收 Descriptors值
- (void)peripheral:(CBPeripheral *)peripheral didUpdateValueForDescriptor:(CBDescriptor *)descriptor error:(NSError *)error {
    if (!error) {
        NSLog(@"descriptor uuid: %@ value: %@",descriptor.UUID,descriptor.value);
    }
}

#pragma mark - private

- (void)cleanup {
    if (self.myPeripheral.state != CBPeripheralStateConnected) {
        return;
    }
    
    //檢查peripheral 是否有正在發送notify的characteristic
    if (self.myPeripheral.services != nil) {
        
        for (CBService *service in self.myPeripheral.services) {
            if (service.characteristics != nil) {
                for (CBCharacteristic *characteristic in service.characteristics) {
                    if (characteristic.isNotifying) {
                        [self.myPeripheral setNotifyValue:NO forCharacteristic:characteristic];
                        return;
                    }
                }
            }
        }
    }
    
    //有連接但沒訂閱 就斷開連接
    [_centralManager cancelPeripheralConnection:self.myPeripheral];
}

@end
