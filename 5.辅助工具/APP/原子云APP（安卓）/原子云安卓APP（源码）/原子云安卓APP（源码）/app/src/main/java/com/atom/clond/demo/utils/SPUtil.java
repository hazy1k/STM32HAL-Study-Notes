package com.atom.clond.demo.utils;

import android.content.Context;
import android.content.SharedPreferences;

import java.util.Set;

/**
 * sp工具类
 *
 * @author cbh
 */
public class SPUtil {

    /**
     * 默认sp文件名称
     */
    private static String SP_NAME = "SP";
    private static SharedPreferences sp;

    /**
     * 设置SP文件夹名称
     */
    public static void setSpName(String name) {
        SPUtil.SP_NAME = name;
    }

    private static SharedPreferences getSp() {
        if (sp == null) {
            synchronized (SPUtil.class) {
                if (sp == null) {
                    sp = UiUtils.getContext().getSharedPreferences(SP_NAME, Context.MODE_PRIVATE);
                }
            }
        }
        return sp;
    }

    /**
     * 存储
     */
    public static void put(String key, Object value) {
        SharedPreferences.Editor edit = getSp().edit();
        if (value instanceof String) {
            edit.putString(key, (String) value);
        } else if (value instanceof Boolean) {
            edit.putBoolean(key, (Boolean) value);
        } else if (value instanceof Float) {
            edit.putBoolean(key, (Boolean) value);
        } else if (value instanceof Integer) {
            edit.putInt(key, (Integer) value);
        } else if (value instanceof Long) {
            edit.putLong(key, (Long) value);
        } else if (value instanceof Set) {
            edit.putStringSet(key, (Set<String>) value);
        }
        edit.apply();
    }

    /**
     * 获取
     */
    public static Object get(String key, Object defaultValue) {
        SharedPreferences sp = getSp();
        if (defaultValue instanceof String) {
            return sp.getString(key, (String) defaultValue);
        } else if (defaultValue instanceof Integer) {
            return sp.getInt(key, (Integer) defaultValue);
        } else if (defaultValue instanceof Boolean) {
            return sp.getBoolean(key, (Boolean) defaultValue);
        } else if (defaultValue instanceof Float) {
            return sp.getFloat(key, (Float) defaultValue);
        } else if (defaultValue instanceof Long) {
            return sp.getLong(key, (Long) defaultValue);
        } else if (defaultValue instanceof Set) {
            return sp.getStringSet(key, (Set<String>) defaultValue);
        } else {
            return "-999";//输入的数据类型有误
        }
    }

    /**
     * 移除
     */
    public static void remove(String key) {
        getSp().edit().remove(key).apply();
    }

    /**
     * 清除
     */
    public static void clear() {
        getSp().edit().clear().apply();
    }


}
