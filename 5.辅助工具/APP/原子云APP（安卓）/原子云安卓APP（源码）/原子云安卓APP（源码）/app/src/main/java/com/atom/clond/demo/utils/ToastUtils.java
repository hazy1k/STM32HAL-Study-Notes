package com.atom.clond.demo.utils;

import android.widget.Toast;

/**
 * Toast封装
 *
 * @author cbh
 */
public class ToastUtils {

    private static Toast mToast = null;

    /**
     * 取消Toast显示
     */
    public static void cancelToast() {
        if (mToast != null) {
            mToast.cancel();
            mToast = null;
        }
    }


    /**
     * 显示短时间Toast
     */
    public static void showShort(CharSequence message) {
        if (mToast != null) {
            cancelToast();
        }
        mToast = Toast.makeText(UiUtils.getContext(), message, Toast.LENGTH_SHORT);
        mToast.show();
    }

    /**
     * 显示长时间Toast
     */
    public static void showLong(CharSequence message) {
        if (mToast != null) {
            mToast.cancel();
        }
        mToast = Toast.makeText(UiUtils.getContext(), message, Toast.LENGTH_LONG);
        mToast.show();
    }


}
