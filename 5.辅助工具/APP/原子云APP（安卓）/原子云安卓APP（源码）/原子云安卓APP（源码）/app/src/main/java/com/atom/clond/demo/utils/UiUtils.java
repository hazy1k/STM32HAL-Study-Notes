package com.atom.clond.demo.utils;

import android.app.Activity;
import android.content.Context;
import android.content.res.Resources;
import android.graphics.drawable.Drawable;
import android.support.v4.content.ContextCompat;
import android.util.DisplayMetrics;
import android.view.WindowManager;

import com.atom.clond.demo.common.MyApplication;

/**
 * 界面资源相关工具类
 *
 * @author cbh
 */
public class UiUtils {

    /**
     * 带有占位符的字符串
     */
    public static String formatStringRes(int resId, Object... obj) {
        return String.format(getString(resId), obj);
    }

    /**
     * 获取上下文
     */
    public static Context getContext() {
        return MyApplication.getAppContext();
    }

    /**
     * 获取string资源
     */
    public static String getString(int resId) {
        return getContext().getString(resId);
    }

    /**
     * 获取数组
     */
    public static String[] getStringArray(int resId) {
        return getContext().getResources().getStringArray(resId);
    }

    /**
     * 获取颜色值
     */
    public static int getColor(int resId) {
        return ContextCompat.getColor(getContext(), resId);
    }

    /**
     * 获取字体大小资源
     */
    public static float getDimen(int resId) {
        return getContext().getResources().getDimension(resId);
    }

    /**
     * 获取资源文件图片
     */
    public static Drawable getDrawable(int resId) {
        return ContextCompat.getDrawable(getContext(), resId);
    }

    /**
     * dip转换成px
     */
    public static int dp2px(float dipValue) {
        float scale = Resources.getSystem().getDisplayMetrics().density;
        return (int) (dipValue * scale + 0.5f);
    }

    /**
     * px转dip
     */
    public static int px2dp(float pxValue) {
        float scale = Resources.getSystem().getDisplayMetrics().density;
        return (int) (pxValue / scale + 0.5f);
    }

    /**
     * sp转换成px
     */
    public static int sp2px(float spValue) {
        float scale = Resources.getSystem().getDisplayMetrics().scaledDensity;
        return (int) (spValue * scale + 0.5f);
    }

    /**
     * px转换成sp
     */
    public static int px2sp(float pxValue) {
        float scale = Resources.getSystem().getDisplayMetrics().scaledDensity;
        return (int) (pxValue / scale + 0.5f);
    }

    /**
     * 获取屏幕宽度
     */
    public static int getScreenWidth(Activity activity) {
        WindowManager wm = activity.getWindowManager();
        DisplayMetrics metrics = new DisplayMetrics();
        wm.getDefaultDisplay().getMetrics(metrics);
        return metrics.widthPixels;
    }

    /**
     * 获取屏幕高度
     */
    public static int getScreenHeight(Activity activity) {
        WindowManager wm = activity.getWindowManager();
        DisplayMetrics metrics = new DisplayMetrics();
        wm.getDefaultDisplay().getMetrics(metrics);
        return metrics.heightPixels;
    }

}
