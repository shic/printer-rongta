# 180330问题：

## 1 打印机连接重复

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



