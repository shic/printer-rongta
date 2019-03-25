# 描述：

我们现在需要一个开发的文档，也就是怎么做连接，连接过期时间等具体的技术问题的文档



我们无法通过一个SDK和示例代码就来开发一个这么重要的功能（这个是系统最重要的功能）



我们只是看到了javadoc，也就是每个函数是做什么的。但是具体用法和细节是没有的。比如链接过期时间；如果需要连接应该先调用哪个方法来连接打印机  等具体的技术问题的讲解。



我们看到了demo。但是demo里也没写每次连接过期时间等内容啊， 还有打印返回值的判断



我们这边用的所有sdk都是有使用文档的。 我们的开发也习惯于这种方式，而不是照着源代码一点一点对着做。这样一定会有bug。



 所以才要跟一个开发人员沟通



一下一些我们现在就遇到的问题的例子：



1 打印机连接是要每次打印都做一次连接么？



2 每次链接的过期时间（timeout）是多长？



3USB 连接怎么用？只需要一条线，一端连接打印机一端连接安卓设备么？线有什么要求，安卓设备怎么设置？



4USB 连接一般来说是不是比wifi连接要稳定？



5发送打印指令给打印机，如果打印有问题（打印纸用完），打印机有没有队列系统来保存未打印的内容。



# 具体问题

## 具体介绍下事例代码的结构

## 需要连接应该先调用哪个方法来连接打印机

1 打印机连接是要每次打印都做一次连接么？

2 每次链接的过期时间（timeout）是多长？

## USB 连接

USB 连接怎么用？只需要一条线，一端连接打印机一端连接安卓设备么？线有什么要求，安卓设备怎么设置？

USB 连接一般来说是不是比wifi连接要稳定？

## 打印返回值

连接失败

发送打印指令给打印机，如果打印有问题（打印纸用完），打印机有没有队列系统来保存未打印的内容。

## 字符串的意义都没有说明

比如 `  public static final int CMD_ESC = 1, CMD_TSC = 2, CMD_CPCL = 3, CMD_ZPL = 4, CMD_PIN = 5;`

```
package com.printer.example.utils;

import android.support.annotation.IntDef;

import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;

/**
 * Created by Tony on 2017/12/3.
 */

public class BaseEnum {

    public static final int NONE = -1;
    public static final int CMD_ESC = 1, CMD_TSC = 2, CMD_CPCL = 3, CMD_ZPL = 4, CMD_PIN = 5;
    public static final int CON_BLUETOOTH = 1, CON_BLUETOOTH_BLE = 2, CON_WIFI = 3, CON_USB = 4, CON_COM = 5;
    public static final int NO_DEVICE = -1, HAS_DEVICE = 1;

    @IntDef({CMD_ESC, CMD_TSC, CMD_CPCL, CMD_ZPL, CMD_PIN})
    @Retention(RetentionPolicy.SOURCE)
    public @interface CmdType {
    }

    @IntDef({CON_BLUETOOTH, CON_WIFI, CON_USB, CON_COM, NONE})
    @Retention(RetentionPolicy.SOURCE)
    public @interface ConnectType {
    }


    @IntDef({NO_DEVICE, HAS_DEVICE})
    @Retention(RetentionPolicy.SOURCE)
    public @interface ChooseDevice {
    }



}

```




