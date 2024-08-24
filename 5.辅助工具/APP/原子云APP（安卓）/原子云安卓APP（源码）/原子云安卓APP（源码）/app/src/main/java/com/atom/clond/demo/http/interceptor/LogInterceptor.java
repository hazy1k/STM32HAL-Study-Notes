package com.atom.clond.demo.http.interceptor;

import android.support.annotation.NonNull;

import com.atom.clond.demo.utils.LogUtils;

import java.io.IOException;

import okhttp3.Interceptor;
import okhttp3.Request;
import okhttp3.Response;
import okhttp3.ResponseBody;

/**
 * 日志拦截器
 *
 * @author bohan.chen
 */
public class LogInterceptor implements Interceptor {

    @Override
    public Response intercept(@NonNull Chain chain) throws IOException {
        //请求发起的时间
        long t1 = System.nanoTime();
        Request request = chain.request();
        LogUtils.i("发送${" + request.method() + "}请求" + request.url());
        Response response = chain.proceed(request);
        //收到响应的时间
        long t2 = System.nanoTime();
        ResponseBody peekBody = response.peekBody(1024 * 1024);
        LogUtils.i("接收响应:" + peekBody.string() + "\n响应时间:$" + (t1 - t2));
        return response;
    }


}
