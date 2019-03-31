# 180330问题：

## 1 一个安卓设备只连一台打印机的时候没有任何问题，但是一旦连接多台打印机，有些时候会出现某台打印机断连的情况

以下是连接代码，您能看一下吗？或者说您那边有没有一台安卓设备连多台打印机的情况？

```java
for (int i = 0; i < kitchenPrinterList.size(); i++) {

            final KitchenPrinter kitchenPrinter = kitchenPrinterList.get(i);

            final String ip = kitchenPrinter.ip;
            final int port = kitchenPrinter.port;

            // 初始化为针打printer
            PrinterFactory printerFactory = new UniversalPrinterFactory();
            final RTPrinter rtPrinter = printerFactory.create();

            //添加连接状态监听
            PrinterObserverManager.getInstance().add(new PrinterObserver() {
                @Override
                public void printerObserverCallback(final PrinterInterface printerInterface, final int state) {
                    Log.d(LOG_TAG, "printerObserverCallback " + state);
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            switch (state) {
                                case CommonEnum.CONNECT_STATE_SUCCESS:
                                    Log.e(LOG_TAG, printerInterface.getConfigObject().toString() + " CONNECT_STATE_SUCCESS at " + new Date(System.currentTimeMillis()));
                                    showToast(printerInterface.getConfigObject().toString() + getString(R.string.connected));
                                    //rtPrinter.setPrinterInterface(printerInterface);
                                    mPrinterDataDeviceMap.put(ip, rtPrinter);
                                    break;
                                case CommonEnum.CONNECT_STATE_INTERRUPTED:
                                    if (printerInterface != null && printerInterface.getConfigObject() != null) {
                                        Log.e(LOG_TAG, printerInterface.getConfigObject().toString() + " CONNECT_STATE_INTERRUPTED at " + new Date(System.currentTimeMillis()));
                                        showToast(printerInterface.getConfigObject().toString() + getString(R.string.disconnected));
                                    } else {
                                        Log.e(LOG_TAG, "CONNECT_STATE_INTERRUPTED ");
                                        showToast(getString(R.string.disconnected));
                                    }
                                    break;
                                default:
                                    break;
                            }
                        }
                    });
                }

                @Override
                public void printerReadMsgCallback(PrinterInterface printerInterface, byte[] bytes) {
                    Log.d(LOG_TAG, "printerReadMsgCallback ");
                }
            });

            // Connect
            connectRTPrinter(ip, port, rtPrinter);
}

```





```java
    private void connectRTPrinter(String ip, int port, RTPrinter rtPrinter) {

        WiFiConfigBean wiFiConfigBean = new WiFiConfigBean(ip, port);

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
            Log.d(LOG_TAG, "connectRTPrinter" + ip + " Connected");
        }

    }

```



## 2 打印机连接重复

在连接打印机之后，在 call back `public void printerObserverCallback(final PrinterInterface printerInterface, final int state)` 里会出现同一台打印机在同一时间两次返回callback的情况

```
2019-03-30 21:45:31.918 17007-17007/com.ristoo E/HomeActivity: 192.168.1.87:9100 CONNECT_STATE_SUCCESS
2019-03-30 21:45:31.936 17007-17007/com.ristoo E/HomeActivity: 192.168.1.90:9100 CONNECT_STATE_SUCCESS
2019-03-30 21:45:31.948 17007-17007/com.ristoo E/HomeActivity: 192.168.1.87:9100 CONNECT_STATE_SUCCESS
2019-03-30 21:45:31.962 17007-17007/com.ristoo E/HomeActivity: 192.168.1.90:9100 CONNECT_STATE_SUCCESS
```



通过debug也可以看到打印机只进行了一次连接：

```java
    for (int i = 0; i < kitchenPrinterList.size(); i++) {

            final KitchenPrinter kitchenPrinter = kitchenPrinterList.get(i);
            // 初始化为针打printer
            PrinterFactory printerFactory = new UniversalPrinterFactory();
            final RTPrinter rtPrinter = printerFactory.create();

            //添加连接状态监听
            PrinterObserverManager.getInstance().add(new PrinterObserver() {
                @Override
                public void printerObserverCallback(final PrinterInterface printerInterface, final int state) {
                    Log.d(LOG_TAG, "printerObserverCallback " + state);
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            switch (state) {
                                case CommonEnum.CONNECT_STATE_SUCCESS:
                                    Log.e(LOG_TAG, printerInterface.getConfigObject().toString() + " CONNECT_STATE_SUCCESS at "+new Date(System.currentTimeMillis()));
                                    showToast(printerInterface.getConfigObject().toString() + getString(R.string.connected));
                                    //rtPrinter.setPrinterInterface(printerInterface);
                                    mPrinterDataDeviceMap.put(kitchenPrinter.ip, rtPrinter);
                                    break;
                                case CommonEnum.CONNECT_STATE_INTERRUPTED:
                                    if (printerInterface != null && printerInterface.getConfigObject() != null) {
                                        Log.e(LOG_TAG, printerInterface.getConfigObject().toString() + " CONNECT_STATE_INTERRUPTED at "+new Date(System.currentTimeMillis()));
                                        showToast(printerInterface.getConfigObject().toString() + getString(R.string.disconnected));
                                    } else {
                                        Log.e(LOG_TAG, "CONNECT_STATE_INTERRUPTED ");
                                        showToast(getString(R.string.disconnected));
                                    }
                                    break;
                                default:
                                    break;
                            }
                        }
                    });
                }

                @Override
                public void printerReadMsgCallback(PrinterInterface printerInterface, byte[] bytes) {
                    Log.d(LOG_TAG, "printerReadMsgCallback ");
                }
            });

            // Connect
            connectRTPrinter(kitchenPrinter, rtPrinter);
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

## 3 打印机断连以后如何重连（因为我们不知道在什么情况下会断连，所以想跟你讨论下重连的问题）

```
PrinterObserverManager.getInstance().add(new PrinterObserver() {
                @Override
                public void printerObserverCallback(final PrinterInterface printerInterface, final int state) {
                    //打印机断连
                }
```

在断连后肯定在这里会收到一个信号，收到后怎么进行重连呢？ 





## 4 有没有一个基本的打印的小票的模板？

我们这样一点一点调出来的模板很难看，你们客户或者你们开发过程中有没有一个基本的小票的打印？ 这样我只要换一下内容就行。

```

            escCmd.append(escCmd.getLFCRCmd());
            escCmd.append(escCmd.getLFCRCmd());
            escCmd.append(escCmd.getTextCmd(textSetting, dishName));
            escCmd.append(escCmd.getLFCRCmd());
            escCmd.append(escCmd.getLFCRCmd());
            escCmd.append(escCmd.getLFCRCmd());
            escCmd.append(escCmd.getTextCmd(textSetting, dateTimeStr));
            escCmd.append(escCmd.getLFCRCmd());
            escCmd.append(escCmd.getLFCRCmd());
            escCmd.append(escCmd.getLFCRCmd());
            escCmd.append(escCmd.getTextCmd(textSetting, tableNumber));

            escCmd.append(escCmd.getLFCRCmd());
            escCmd.append(escCmd.getLFCRCmd());
            escCmd.append(escCmd.getLFCRCmd());
            escCmd.append(escCmd.getLFCRCmd());
            escCmd.append(escCmd.getLFCRCmd());
            escCmd.append(escCmd.getLFCRCmd());
            //escCmd.append(escCmd.getHeaderCmd());//初始化, Initial
            escCmd.append(escCmd.getLFCRCmd());

            escCmd.append(escCmd.getAllCutCmd());
            
```

## 