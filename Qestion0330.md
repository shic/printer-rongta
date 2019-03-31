# 180330问题：

## 1 有没有一个基本的打印的小票的模板？

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

## 2 打印机断连以后如何重连（因为我们不知道在什么情况下会断连，所以想跟你讨论下重连的问题）

```
PrinterObserverManager.getInstance().add(new PrinterObserver() {
                @Override
                public void printerObserverCallback(final PrinterInterface printerInterface, final int state) {
                    //打印机断连
                }
```

在断连后肯定在这里会收到一个信号，收到后怎么进行重连呢？ 



## 3 这些调试中的打印每秒钟都打很多次，而且有时候有有时候没有。能不能问一下是什么时候才会出现？能不能去掉SDK中的调试中的打印 （很影响看log中的其他更重要的信息）

```
2019-03-28 20:09:21.316 15379-15821/com.ristoo I/System.out: waitting for instream
2019-03-28 20:09:21.411 15379-15801/com.ristoo I/System.out: waitting for instream
2019-03-28 20:09:21.412 15379-15809/com.ristoo I/System.out: waitting for instream
2019-03-28 20:09:21.412 15379-15842/com.ristoo I/System.out: waitting for instream
2019-03-28 20:09:21.412 15379-15833/com.ristoo I/System.out: waitting for instream
```
