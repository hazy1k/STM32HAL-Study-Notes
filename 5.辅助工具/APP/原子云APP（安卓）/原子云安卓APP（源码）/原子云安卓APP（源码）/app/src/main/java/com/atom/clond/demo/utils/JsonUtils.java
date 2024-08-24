package com.atom.clond.demo.utils;

import com.alibaba.fastjson.JSON;

import java.util.List;

/**
 * json解析工具类
 *
 * @author bohan.chen
 */
public class JsonUtils {

    /**
     * 解析json
     *
     * @param json  json数据
     * @param clazz 目标对象
     */
    public static <T> T parseJson(String json, Class<T> clazz) {
        return JSON.parseObject(json, clazz);
    }

    /**
     * 解析数组对象
     */
    public static <T> List<T> parseArray(String json, Class<T> clazz) {
        return JSON.parseArray(json, clazz);
    }


    /**
     * 转成json字符串
     */
    public static String toJson(Object obj) {
        return JSON.toJSONString(obj);
    }

}
