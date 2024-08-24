package com.atom.clond.demo.common;


import com.atom.clond.demo.utils.LogUtils;

import java.io.PrintWriter;
import java.io.StringWriter;

/**
 * 全局异常处理类
 *
 * @author cbh
 */
public class CrashHandler implements Thread.UncaughtExceptionHandler {

    private Thread.UncaughtExceptionHandler mDefaultHandler;

    public void init() {
        mDefaultHandler = Thread.getDefaultUncaughtExceptionHandler();
        Thread.setDefaultUncaughtExceptionHandler(this);
    }

    @Override
    public void uncaughtException(Thread t, Throwable e) {
        LogUtils.e(readExceptionInfo(e));
        mDefaultHandler.uncaughtException(t, e);
    }

    private String readExceptionInfo(Throwable e) {
        try {
            StringWriter writer = new StringWriter();
            PrintWriter printWriter = new PrintWriter(writer);
            e.printStackTrace(printWriter);
            Throwable cause = e.getCause();
            while (cause != null) {
                cause.printStackTrace(printWriter);
                cause = cause.getCause();
            }
            printWriter.flush();
            printWriter.close();
            return writer.toString();
        } catch (Exception ex) {
            return "";
        }
    }


}
