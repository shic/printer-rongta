//
//  Cmd.h
//  RTPrinterSDK
//
//  Created by King 22/11/2017.
//  Copyright © 2017 Rongta. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "TextSetting.h"
#import "CommonSetting.h"
#import <UIKit/UIKit.h>
#import "BitmapSetting.h"
#import "BarcodeSetting.h"
#import "EnumTypeDef.h"


#define BUFFER_MAX_LEN (1024*5)
#define BARCODECMD_MAX_LEN (200)

/*!
 所有命令类(Esc,TSC,cpcl,针打)的基类
 The base class for all command classes (Esc, TSC, cpcl, and pin)
 
 */
@interface Cmd :NSObject
/*!编码格式 Encoding Type*/
@property  (nonatomic) EncodingType encodingType;
/*!条形码结束后，要在打印几个结束符0x00, 默认为1
 After the end of the bar code, to print a few end characters 0x00, the default is 1
 */
@property (nonatomic) NSInteger barcodeBufferSize;
@property (nonatomic, copy,readonly) NSString  * _Nullable companyid;


/*!
 *获取所有通过调用Append方法生成的指令
 *Get all the instructions generated by calling the Append method
 @return NSData
 */
-(NSData *_Nullable) GetCmd;
/*!
 *@brief 添加指令，到缓存中，以便最后调用GetCmd时，使用
 *   Add instructions to the cache for use in the last call to GetCmd
 @param bytes 要添加的指令 Instructions to be added
 @param len 指令长度 Instruction length
 */
-(void)Append:(Byte*)bytes len:(NSInteger)len;
/*!
 * 添加指令，到缓存中，以便最后调用GetCmd时，使用
 Add instructions to the cache for use in the last call to GetCmd
 @param data 指令数据
 */
-(void)Append:(NSData *)data;

/*!
 把source的值赋值给dest，dest从index开始赋值，每赋一个字节index会加1
 The value of the source assigned to dest, dest from the index assignment, each assigned a byte index will increase by one
 @param source 源数据 source data
 @param dest 目的数据 dest data
 @param index 当前索引 Current index
 */
-(void)MoveData:(NSData *)source dest:(Byte *)dest index:(NSInteger *)index;

/*!
 把source的值(从ibegin开始到,长度为len)赋值到dest，dest从index开始赋值，每赋一个字节index会加1
 The source value (starting from ibegin to length len) assigned to dest, dest from the index assignment, each assigned a byte index will increase by 1
 @param source 源数据 source data
 @param ibegin 开始索引 start index
 @param dest 目的数据 dest data
 @param dindex 当前索引  Current index
 */
-(void)MoveData:(Byte *)source ibegin:(NSInteger)ibegin len:(NSInteger) len   dest:(Byte *)dest dindex:(NSInteger *)dindex;

/*!
 生成指定encodingType编码的数据
 @discussion Generate data of the specified encodingType encoding
 @param data 要生成的数据 Data to be generated
 @return 返回指定encodingType编码的数据 Returns data of the specified encodingType encoding
 */
-(NSData *) ImportData:(nonnull  NSString*) data;


/*!
 清除缓存数据
 Clear the cache data
 */
-(void)Clear;

/*!获取初始化命令头
 Get initialization command header
 */
-(NSData *) GetHeaderCmd;

/*!获取初始化命令头,并根据ComSetting设置相关参数
 Get initialization command header, and set parameters according to ComSetting
 */
-(NSData *) GetHeaderCmd:(CommonSetting *_Nonnull)ComSetting;

/*!
 根据textSetting 得到对应的文本指令
 By textSetting get the corresponding text instruction
 @param textSetting 文本设置 Text setting
 @param text 输入的文本 Enter the text
 @return 返回命令数据 Return command data
 */
-(NSData *) GetTextCmd:(TextSetting *)textSetting text:(NSString *)text;

/*!
 获取图片打印命令
 Get the picture print order
 @param bitmapSetting 图片设置 image setting
 @param image image
 @return NSData
 */
-(NSData *) GetBitMapCmd:(BitmapSetting *) bitmapSetting image:(UIImage *) image;

/*!
 获取图片打印命令 (用于Esc命令，打印长图片时，并且用wifi,usb连接) 命令不分段
 Get the picture print order(Used for the Esc command, when printing long images, and connected with wifi, usb)
 @param bitmapSetting 图片设置 image setting
 @param image image
 @return NSData
 */
-(NSData *) GetBitMapCmdExt:(BitmapSetting *) bitmapSetting image:(UIImage *) image;


//图片如果小于limitWidth，会进行放大，limitmaxzooms:允许放大的最大倍数 0:不限制
-(NSData *) GetBitMapCmdZoom:(BitmapSetting *) bitmapSetting image:(UIImage *) image limitmaxzooms:(NSInteger)limitmaxzooms;


/*!
 获取条码指令
 Get barcode instructions
 @param barcodeset 条码设置 Barcode settings
 @param codeType 条码类型 Barcode type
 @param scode 条码内容 Barcode content
 @param codeError 错误代码 error code
 @return NSData
 */
-(NSData *_Nullable) GetBarCodeCmd:(BarcodeSetting *_Nonnull)barcodeset  codeType:(BarcodeType)codeType scode:(nonnull NSString * )scode  codeError:(PrinterCodeError *_Nonnull)codeError;

/*!
 获取打印自测页的指令
 Get instructions to print a self-test page
 @return NSData
 */
-(NSData *) GetSelftestCmd;
/*!
 获取打印回车的指令
 Get print carriage return instruction
 @return NSData
 */
-(NSData *) GetCRCmd;
/*!
 获取换行的指令
 Get line feed instruction
 @return NSData
 */
-(NSData *) GetLFCmd;
/*!
 获取回车换行的指令
 Get the carriage return line feed instruction
 @return NSData
 */
-(NSData *_Nonnull) GetLFCRCmd;
/*!
 获取打印结束的指令（针打是退纸用，TSC,CPCL是发送打印命令，ESC:回车换行指令）
 Get the end of the print order (Pincmd is used for rewinding, TSC, CPCL is to send print commands, ESC: carriage return line feed command)
 @return NSData
 */
-(NSData *_Nullable) GetPrintEndCmd;

/*!
 获取打印结束的指令（针打是退纸用，TSC,CPCL是发送打印命令，ESC:回车换行指令）
 Get the end of the print order (Pincmd is used for rewinding, TSC, CPCL is to send print commands,
 ESC: carriage return line feed command)
 @param copies 打印份数(适用于TSC，其他调用等同于GetPrintEndCmd)
 Print copies (for TSC, other calls equal to GetPrintEndCmd)
 @return NSData
 */
-(NSData *_Nullable) GetPrintEndCmd:(NSInteger)copies;
/*!
 获取空的NSData
 Get empty NSData
 @return NSData
 */
-(NSData *_Nullable) GetNullData;

/*!
 Expand memory
 @param buff Memory to expand
 @param Addsize 要扩展的尺寸 The size to be expanded
 */
-(BOOL)AddMemory:(Byte *)buff Addsize:(NSInteger)Addsize;


/*!打开钱箱 Open the cashbox
 @param DrawerNumber  0:钱箱引脚2 (Drawer pin 2)  1:钱箱引脚5 (Drawer pin 5)   default:0
 @param pulseStartTime  default:5
 @param PulseEndTime   default:0
 */
- (NSData *) GetOpenDrawerCmd:(Byte)DrawerNumber startTime:(Byte)pulseStartTime endTime:(Byte)PulseEndTime;

/*!
 发出beep声 Beep sound
 @param level 次数 frequency
 @param interval 单次时间(single time) interval*100 ms
 */
- (NSData *) GetBeepCmd:(Byte)level interval:(Byte)interval;

/*!
 切纸控制命令 Paper cutting control command
 @param cutterMode 切刀模式 Cutter mode
 */
- (NSData *) GetCutPaperCmd:(CutterMode) cutterMode;


/*!
 选择切纸模式并切纸  (for ESC)
 Select the cut mode and cut the paper
 @param isFeed  YES:部分切纸 Part cut paper
 NO:进纸, 并且进行部分切纸(保留一点不切) Feed, and cut some paper (keep a bit not cut)
 @param FeedDistance 进纸距离[n+0.125 毫米] Feed distance [n + 0.125 mm]
 
 */
- (NSData *) GetFeedAndCutPaperCmd:(BOOL)isFeed FeedDistance:(Byte)FeedDistance;

/*!for RP410 ZPL  Custom customer U.S.A  CMC
 RP806 TSC Custom  U.S.A MINI PACK
 */
- (NSData *) GetPrintStautsCmd:(PrintStautsCmd) printStautsCmd;
/*!设置codepage
 set the codepage for ESC
 */
-(NSData *)GetCodePageCmd:(NSString *)codepage;
/*!询问是否打印成功，成功能返回$80,失败不返回，适用于定制客户 Rpp80Use
 Ask if the print is successful, the function returns $80, failure does not return
 For custom customers Rpp80Use
 */
-(NSData *)GetAskPrintOkCmd;//



@end

