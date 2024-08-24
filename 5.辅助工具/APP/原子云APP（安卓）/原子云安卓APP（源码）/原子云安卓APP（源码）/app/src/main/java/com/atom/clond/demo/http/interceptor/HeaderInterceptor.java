package com.atom.clond.demo.http.interceptor;

import android.support.annotation.NonNull;

import java.io.IOException;
import java.util.Map;
import java.util.TreeMap;
import okhttp3.Interceptor;
import okhttp3.Request;
import okhttp3.Response;

/**
 * 请求拦截器，统一添加请求头
 *
 * @author bohan.chen
 */
public class HeaderInterceptor implements Interceptor {

    private Map<String, Object> headerMaps;

    public Map<String, Object> getHeaderMaps() {
        return headerMaps;
    }

    public HeaderInterceptor(Map<String, Object> headerMaps) {
        this.headerMaps = headerMaps;
    }

    @NonNull
    @Override
    public Response intercept(@NonNull Interceptor.Chain chain) throws IOException {
        Request.Builder request = chain.request().newBuilder();
        if (headerMaps != null && headerMaps.size() > 0) {
            for (Map.Entry<String, Object> entry : headerMaps.entrySet()) {
                request.addHeader(entry.getKey(), (String) entry.getValue());
            }
        }
        return chain.proceed(request.build());
    }
}
