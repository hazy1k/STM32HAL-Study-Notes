package com.atom.clond.demo.common;

/**
 * 全局常量配置
 *
 * @author bohan.chen
 */
public class Constants {

    /**
     * webSocket服务器地址
     */
    public static final String WS_SERVER_ADDRESS = "ws://cloud.alientek.com/connection/";
    /**
     * 服务器地址
     */
    public static final String SERVER_ADDRESS = "https://cloud.alientek.com/api/";

    /**
     * 原子云平台的token，替换为自己的token
     */
    public static String HTTP_TOKEN = "1523e79d746e7ba9699e596b7a1562ea";

    /**
     * 是否调试状态
     */
    public static final boolean IS_DEBUG = true;


    /**
     * DTU设备
     */
    public static final String DEVICE_DTU = "01";
    /**
     * WIFI设备
     */
    public static final String DEVICE_WIFI = "02";
    /**
     * NBIOT设备
     */
    public static final String DEVICE_NBIOT = "03";

    /**
     * 断开
     */
    public static final String DEVICE_STATUS_DISCONNECTED = "disconnected";
    /**
     * 连接
     */
    public static final String DEVICE_STATUS_CONNECTED = "connected";

    /**
     * 已绑定
     */
    public static final String DEVICE_STATUS_BIND = "binded";

    /**
     * 未绑定
     */
    public static final String DEVICE_STATUS_UN_BIND = "notbind";

    /**
     * sp保存TOKEN信息的key
     */
    public static final String SP_TOKEN = "SP_TOKEN";

}
