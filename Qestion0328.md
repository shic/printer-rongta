# 问题：

## 1 Socket timeout

我们完全根据文档写的连接打印机步骤 (每次打印都进行连接)

```java
    private void connectRTPrinter(String ip, int port) {
        WiFiConfigBean configObj = new WiFiConfigBean(ip, port);

        WiFiConfigBean wiFiConfigBean = (WiFiConfigBean) configObj;

        PIFactory piFactory = new WiFiFactory();
        PrinterInterface printerInterface = piFactory.create();
        printerInterface.setConfigObject(wiFiConfigBean);
        rtPrinter.setPrinterInterface(printerInterface);
        try {
            rtPrinter.connect(wiFiConfigBean);
        } catch (Exception e) {
            e.printStackTrace();
            Log.e(LOG_TAG, e.toString());
        } finally {
            Log.d(LOG_TAG, "connectRTPrinter Connected");
        }

    }

```

这个方法每次都返回连接成功。



但是用以下方法打印的时候

```java
 private void escPrint(String dishName, String tableNumber, String dateTimeStr) throws UnsupportedEncodingException {

        Log.d(LOG_TAG, "printer received: "+dishName+tableNumber);
        String mChartsetName = "UTF-8";
        TextSetting textSetting;
        textSetting = new TextSetting();
        textSetting.setEscFontType(ESCFontTypeEnum.FONT_B_9x24);

        /*

            TextSetting textSetting = new TextSetting();
            textSetting.setDoubleWidth(SettingEnum.Enable);
            textSetting.setDoubleHeight(SettingEnum.Enable);
            textSetting.setBold(SettingEnum.Enable);

        */

        if (rtPrinter != null) {
            CmdFactory escFac = new EscFactory();
            Cmd escCmd = escFac.create();
            escCmd.append(escCmd.getHeaderCmd());//初始化, Initial

            escCmd.setChartsetName(mChartsetName);

            CommonSetting commonSetting = new CommonSetting();
            //commonSetting.setEscLineSpacing(getInputLineSpacing());
            escCmd.append(escCmd.getCommonSettingCmd(commonSetting));

            escCmd.append(escCmd.getTextCmd(textSetting, dishName));

            escCmd.append(escCmd.getLFCRCmd());
            escCmd.append(escCmd.getLFCRCmd());
            escCmd.append(escCmd.getLFCRCmd());
            escCmd.append(escCmd.getLFCRCmd());
            escCmd.append(escCmd.getLFCRCmd());
            escCmd.append(escCmd.getHeaderCmd());//初始化, Initial
            escCmd.append(escCmd.getLFCRCmd());

            rtPrinter.writeMsgAsync(escCmd.getAppendCmds());
        }
    }

```



前几分钟正常打印，但是过几分钟会返回这个错误：

```java

2019-03-28 21:16:58.756 18126-18587/com.ristoo W/System.err: java.net.SocketTimeoutException: failed to connect to /192.168.1.87 (port 9100) from /192.168.1.8 (port 53718) after 3000ms
2019-03-28 21:16:58.757 18126-18587/com.ristoo W/System.err:     at libcore.io.IoBridge.connectErrno(IoBridge.java:185)
2019-03-28 21:16:58.758 18126-18587/com.ristoo W/System.err:     at libcore.io.IoBridge.connect(IoBridge.java:130)
2019-03-28 21:16:58.758 18126-18587/com.ristoo W/System.err:     at java.net.PlainSocketImpl.socketConnect(PlainSocketImpl.java:129)
2019-03-28 21:16:58.759 18126-18587/com.ristoo W/System.err:     at java.net.AbstractPlainSocketImpl.doConnect(AbstractPlainSocketImpl.java:356)
2019-03-28 21:16:58.759 18126-18587/com.ristoo W/System.err:     at java.net.AbstractPlainSocketImpl.connectToAddress(AbstractPlainSocketImpl.java:200)
2019-03-28 21:16:58.759 18126-18587/com.ristoo W/System.err:     at java.net.AbstractPlainSocketImpl.connect(AbstractPlainSocketImpl.java:182)
2019-03-28 21:16:58.760 18126-18587/com.ristoo W/System.err:     at java.net.SocksSocketImpl.connect(SocksSocketImpl.java:357)
2019-03-28 21:16:58.760 18126-18587/com.ristoo W/System.err:     at java.net.Socket.connect(Socket.java:616)
2019-03-28 21:16:58.761 18126-18587/com.ristoo W/System.err:     at com.rt.printerlibrary.driver.wifi.WifiDriver.connect(Unknown Source:33)
2019-03-28 21:16:58.761 18126-18587/com.ristoo W/System.err:     at com.rt.printerlibrary.driver.wifi.WifiDriver.run(Unknown Source:7)
2019-03-28 21:17:03.109 18126-18126/com.ristoo D/HomeActivity:: dishDownloaderRunnable
```

但是明明每次打印前都调用了connet方法，会什么还会出现SocketTimeoutException？



## 2 不明等待

在logcat里每秒会打印很多次以下信息（应该是SDK打印的），请问您觉得是什么原因？

```
2019-03-28 20:09:21.316 15379-15821/com.ristoo I/System.out: waitting for instream
2019-03-28 20:09:21.411 15379-15801/com.ristoo I/System.out: waitting for instream
2019-03-28 20:09:21.412 15379-15809/com.ristoo I/System.out: waitting for instream
2019-03-28 20:09:21.412 15379-15842/com.ristoo I/System.out: waitting for instream
2019-03-28 20:09:21.412 15379-15833/com.ristoo I/System.out: waitting for instream
2019-03-28 20:09:21.461 15379-15788/com.ristoo I/System.out: waitting for instream
2019-03-28 20:09:21.461 15379-15741/com.ristoo I/System.out: waitting for instream
2019-03-28 20:09:21.461 15379-15827/com.ristoo I/System.out: waitting for instream
2019-03-28 20:09:21.516 15379-15848/com.ristoo I/System.out: waitting for instream
2019-03-28 20:09:21.516 15379-15821/com.ristoo I/System.out: waitting for instream
```



