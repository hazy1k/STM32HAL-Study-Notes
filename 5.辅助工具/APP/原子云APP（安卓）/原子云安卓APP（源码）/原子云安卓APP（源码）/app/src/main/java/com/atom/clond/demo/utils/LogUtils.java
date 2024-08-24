package com.atom.clond.demo.utils;

import android.util.Log;

/**
 * 日志相关工具类
 *
 * @author bohan.chen
 */
public class LogUtils {

    /**
     * 日志长度
     */
    private final static int LOG_LENGTH = 2000;
    /**
     * 日志打印
     */
    private static String DEFAULT_TAG = "MY_LOG";

    private static boolean isShowLog = false;

    public static boolean isShowLog() {
        return isShowLog;
    }

    public static void init(boolean isShow) {
        isShowLog = isShow;
    }

    public static void init(boolean isShowLog, String logTag) {
        LogUtils.isShowLog = isShowLog;
        LogUtils.DEFAULT_TAG = logTag;
    }

    /**
     * i级别Log
     */
    public static void i(Object object) {
        if (isShowLog && object != null) {
            String log = object.toString();
            int length = log.length();
            for (int i = 0; i < length; i += LOG_LENGTH) {
                if (i + LOG_LENGTH < length) {
                    Log.i(DEFAULT_TAG, log.substring(i, i + LOG_LENGTH));
                } else {
                    Log.i(DEFAULT_TAG, log.substring(i, length));
                }
            }
        }
    }

    /**
     * e级别log
     */
    public static void e(Object object) {
        if (object != null) {
            String log = object.toString();
            int length = log.length();
            for (int i = 0; i < length; i += LOG_LENGTH) {
                if (i + LOG_LENGTH < length) {
                    Log.e(DEFAULT_TAG, log.substring(i, i + LOG_LENGTH));
                } else {
                    Log.e(DEFAULT_TAG, log.substring(i, length));
                }
            }
        }
    }
}