package com.atom.clond.demo.ws;

import com.atom.clond.demo.bean.DeviceBean;
import com.atom.clond.demo.bean.MessageBean;
import com.atom.clond.demo.utils.LogUtils;

import java.nio.charset.Charset;
import java.util.concurrent.TimeUnit;

import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;
import okhttp3.WebSocket;
import okhttp3.WebSocketListener;
import okio.ByteString;

/**
 * webSocket管理者
 *
 * @author bohan.chen
 */
public class WsManager {

    private WebSocket ws;
    private byte[] deviceNum;
    private DeviceBean mDeviceBean;
    private WsEventListener listener;

    public void init(String url, DeviceBean deviceBean, final WsEventListener listener) {
        mDeviceBean = deviceBean;
        this.listener = listener;
        this.deviceNum = deviceBean.getNumber().getBytes();
        Request request = new Request.Builder().url(url).build();
        OkHttpClient okHttpClient = new OkHttpClient.Builder()
                .retryOnConnectionFailure(true)
                .connectTimeout(8, TimeUnit.SECONDS)
                .readTimeout(5, TimeUnit.SECONDS)
                .writeTimeout(5, TimeUnit.SECONDS)
                .build();
        okHttpClient.newWebSocket(request, new WebSocketListener() {
            @Override
            public void onOpen(WebSocket webSocket, Response response) {
                ws = webSocket;
                if (listener != null) {
                    listener.onConnect();
                }
            }

            @Override
            public void onMessage(WebSocket webSocket, String text) {
            }

            @Override
            public void onMessage(WebSocket webSocket, final ByteString bytes) {
                if (listener != null) {
                    byte[] byteArray = bytes.toByteArray();
                    if (byteArray.length > 0) {
                        switch (byteArray[0]) {
                            case 0x04:
                                listener.onConnect();
                                break;
                            case 0x05:
                                listener.onDisconnect();
                                break;
                            case 0x06:
                            case 0x08:
                                showMessage(bytes);
                                break;
                        }
                    }
                }
            }

            @Override
            public void onClosing(WebSocket webSocket, int code, String reason) {
                super.onClosing(webSocket, code, reason);
            }

            @Override
            public void onClosed(WebSocket webSocket, int code, String reason) {
                if (listener != null) {
                    listener.onDisconnect();
                }
            }

            @Override
            public void onFailure(WebSocket webSocket, final Throwable t, Response response) {
                if (listener != null) {
                    listener.onFailed(t.getMessage());
                }
            }
        });
    }

    private synchronized void showMessage(ByteString bytes) {
        try {
            MessageBean bean = new MessageBean();
            String hex = bytes.hex().substring(42);
            bean.setName("RX");
            bean.setHexData(hex);
            byte[] byteArray = bytes.toByteArray();
            byte[] newByteArr = new byte[byteArray.length - 21];
            System.arraycopy(byteArray, 21, newByteArr, 0, newByteArr.length);
            bean.setNormalData(new String(newByteArr, Charset.forName("ASCII")));
            if (listener != null) {
                listener.onRecMessage(bean);
            }
        } catch (Exception e) {
            LogUtils.e(e);
        }
    }

    /**
     * 向设备发送消息
     *
     * @param msg
     * @return
     */
    public boolean sendMsg(byte[] msg) {
//        if (mDeviceBean.getDevice_type().equals(Constants.DEVICE_DTU)) {
        return send2DTU(msg);
//        } else if (mDeviceBean.getDevice_type().equals(Constants.DEVICE_NBIOT)) {
//            return send2NBIOT(msg);
//        }
//        return false;
    }

    public void close() {
        if (ws != null) {
            ws.cancel();
        }
    }

    /**
     * 订阅设备
     */
    public boolean subDevice() {
        byte[] head = {0x01};
        byte[] dataByte = concat(head, deviceNum);
        return ws.send(ByteString.of(dataByte));
    }

    /**
     * 取消订阅
     */
    public boolean unSubDevice() {
        byte[] head = {0x02};
        byte[] dataByte = concat(head, deviceNum);
        return ws.send(ByteString.of(dataByte));
    }

    /**
     * 发送消息到dtu
     */
    public boolean send2DTU(byte[] msg) {
        byte[] head = concat(new byte[]{0x03}, deviceNum);
        return ws.send(ByteString.of(concat(head, msg)));
    }

    /**
     * 发送消息到nbiot
     */
    public boolean send2NBIOT(byte[] msg) {
        byte[] head = concat(new byte[]{0x07}, deviceNum);
        return ws.send(ByteString.of(concat(head, msg)));
    }


    /**
     * 合并数组
     */
    static byte[] concat(byte[] a, byte[] b) {
        byte[] c = new byte[a.length + b.length];
        System.arraycopy(a, 0, c, 0, a.length);
        System.arraycopy(b, 0, c, a.length, b.length);
        return c;
    }

    /**
     * 打印数组
     *
     * @param array
     */
    private static String printArray(byte[] array) {
        StringBuilder sb = new StringBuilder();
        for (byte b : array) {
            sb.append(Integer.toHexString(b)).append(" ");
        }
        return sb.toString();
    }

}
