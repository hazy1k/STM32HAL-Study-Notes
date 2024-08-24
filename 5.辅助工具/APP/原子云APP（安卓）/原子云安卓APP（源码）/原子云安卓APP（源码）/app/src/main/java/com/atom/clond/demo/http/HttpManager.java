package com.atom.clond.demo.http;

import android.os.Handler;
import android.os.Looper;

import com.atom.clond.demo.http.interceptor.HeaderInterceptor;
import com.atom.clond.demo.http.interceptor.LogInterceptor;
import com.atom.clond.demo.utils.JsonUtils;

import java.io.IOException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.TimeUnit;

import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.MediaType;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.RequestBody;
import okhttp3.Response;

/**
 * http
 *
 * @author bohan.chen
 */
public class HttpManager {

    /**
     * 请求头token的key
     */
    private static final String HEADER_TOKEN = "token";

    private static final MediaType MEDIA_TYPE_JSON = MediaType.parse("application/json; charset=utf-8");

    private HttpManager() {
        mHeaderMap = new HashMap<>(4);
    }

    private static HttpManager instance;

    public static HttpManager getInstance() {
        if (instance == null) {
            synchronized (HttpManager.class) {
                if (instance == null) {
                    instance = new HttpManager();
                }
            }
        }
        return instance;
    }

    /**
     * 请求头
     */
    private HashMap<String, Object> mHeaderMap;
    /**
     * 服务器地址
     */
    private String mServerUrl;
    private Handler mMainHandler;

    private OkHttpClient mHttpClient;

    /**
     * 初始化操作
     *
     * @param isDebug       是否打印日志
     * @param serverAddress 服务器地址
     */
    public void initHttp(boolean isDebug, String serverAddress) {
        mServerUrl = serverAddress;
        OkHttpClient.Builder builder = new OkHttpClient.Builder()
                //添加请求头
                .addInterceptor(new HeaderInterceptor(mHeaderMap))
                //全局超时配置
                .readTimeout(20, TimeUnit.SECONDS)
                //全局超时配置
                .writeTimeout(20, TimeUnit.SECONDS)
                //全局超时配置
                .connectTimeout(20, TimeUnit.SECONDS);
        if (isDebug) {
            builder.addInterceptor(new LogInterceptor());
        }
        mMainHandler = new Handler(Looper.getMainLooper());
        mHttpClient = builder.build();
    }

    /**
     * token请求头拦截器
     */
    public void addToken2Header(String token) {
        mHeaderMap.put(HEADER_TOKEN, token);
    }

    /**
     * get请求
     *
     * @param url   url
     * @param cb    回调
     * @param clazz 解析的对象
     */
    public <T> void get(String url, HttpCallback<T> cb, Class<T> clazz) {
        Request request = new Request.Builder()
                .get()
                .url(mServerUrl + url)
                .build();
        doRequest(request, clazz, cb);
    }

    /**
     * get请求，返回列表
     *
     * @param url      url
     * @param cb       回调
     * @param clazz    列表解析的对象
     * @param queryMap 查询条件
     */
    public <T> void get(String url, HttpCallback<T> cb, Class<T> clazz, Map<String, Object> queryMap) {
        Request request = new Request.Builder()
                .get()
                .url(buildQueryMap(mServerUrl + url, queryMap))
                .build();
        doRequest(request, clazz, cb);
    }

    /**
     * get请求，返回列表
     *
     * @param url   url
     * @param cb    回调
     * @param clazz 列表解析的对象
     */
    public <T> void getArray(String url, HttpCallback<List<T>> cb, Class<T> clazz) {
        Request request = new Request.Builder()
                .get()
                .url(mServerUrl + url)
                .build();
        doRequestArray(request, clazz, cb);
    }

    /**
     * get请求，返回列表
     *
     * @param url      url
     * @param cb       回调
     * @param clazz    列表解析的对象
     * @param queryMap 查询条件
     */
    public <T> void getArray(String url, HttpCallback<List<T>> cb, Class<T> clazz, Map<String, Object> queryMap) {
        Request request = new Request.Builder()
                .get()
                .url(buildQueryMap(mServerUrl + url, queryMap))
                .build();
        doRequestArray(request, clazz, cb);
    }

    /**
     * 构建请求体
     */
    private String buildQueryMap(String reqUrl, Map<String, Object> queryMap) {
        if (queryMap != null) {
            StringBuilder sb = null;
            for (Map.Entry<String, Object> entry : queryMap.entrySet()) {
                if (sb == null) {
                    sb = new StringBuilder();
                    sb.append(reqUrl).append("?");
                } else {
                    sb.append("&");
                }
                sb.append(entry.getKey())
                        .append("=")
                        .append(entry.getValue().toString());
            }
            if (sb != null) {
                return sb.toString();
            }
        }
        return reqUrl;
    }

    /**
     * post请求
     *
     * @param url   url
     * @param cb    回调
     * @param clazz 解析的对象
     */
    public <T> void post(String url, Object body, HttpCallback<T> cb, Class<T> clazz) {
        RequestBody requestBody = RequestBody.create(MEDIA_TYPE_JSON, JsonUtils.toJson(body));
        Request request = new Request.Builder()
                .post(requestBody)
                .url(mServerUrl + url)
                .build();
        doRequest(request, clazz, cb);
    }

    /**
     * post请求，返回列表
     *
     * @param url   url
     * @param cb    回调
     * @param clazz 列表解析的对象
     */
    public <T> void postArray(String url, Object body, HttpCallback<List<T>> cb, Class<T> clazz) {
        RequestBody requestBody = RequestBody.create(MEDIA_TYPE_JSON, JsonUtils.toJson(body));
        Request request = new Request.Builder()
                .post(requestBody)
                .url(mServerUrl + url)
                .build();
        doRequestArray(request, clazz, cb);
    }

    /**
     * 执行请求
     */
    private <T> void doRequestArray(final Request req, final Class<T> clazz, final HttpCallback<List<T>> cb) {
        mHttpClient.newCall(req).enqueue(new Callback() {
            @Override
            public void onFailure(Call call, final IOException e) {
                if (cb != null) {
                    mMainHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            cb.onError(0, e.getMessage());
                            cb.onFinish();
                        }
                    });
                }
            }

            @Override
            public void onResponse(Call call, final Response response) throws IOException {
                if (cb != null) {
                    mMainHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            try {
                                String json = response.body().string();
                                ResponseBean responseBean = JsonUtils.parseJson(json, ResponseBean.class);
                                switch (responseBean.getCode()) {
                                    case CODE_SUCCESS:
                                        cb.onSuccess(JsonUtils.parseArray(responseBean.getData(), clazz));
                                        break;
                                    case CODE_ERROR:
                                    case CODE_SERVER_ERROR:
                                    case CODE_PARAMS_ERROR:
                                    case CODE_TOKEN_VERIFY_ERROR:
                                        cb.onError(responseBean.getCode(), responseBean.getMessage());
                                        break;
                                }
                            } catch (Exception e) {
                                e.printStackTrace();
                                cb.onError(0, e.getMessage());
                            } finally {
                                cb.onFinish();
                            }
                        }
                    });
                }
            }
        });
    }

    /**
     * 执行请求
     */
    private <T> void doRequest(final Request req, final Class<T> clazz, final HttpCallback<T> cb) {
        mHttpClient.newCall(req).enqueue(new Callback() {
            @Override
            public void onFailure(Call call, final IOException e) {
                if (cb != null) {
                    mMainHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            cb.onError(0, e.getMessage());
                            cb.onFinish();
                        }
                    });
                }
            }

            @Override
            public void onResponse(Call call, final Response response) throws IOException {
                if (cb != null) {
                    mMainHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            try {
                                String json = response.body().string();
                                ResponseBean responseBean = JsonUtils.parseJson(json, ResponseBean.class);
                                switch (responseBean.getCode()) {
                                    case CODE_SUCCESS:
                                        if (clazz == String.class) {
                                            cb.onSuccess((T) responseBean.getData());
                                        } else {
                                            cb.onSuccess(JsonUtils.parseJson(responseBean.getData(), clazz));
                                        }
                                        break;
                                    case CODE_ERROR:
                                    case CODE_SERVER_ERROR:
                                    case CODE_PARAMS_ERROR:
                                    case CODE_TOKEN_VERIFY_ERROR:
                                        cb.onError(responseBean.getCode(), responseBean.getMessage());
                                        break;
                                }
                            } catch (Exception e) {
                                e.printStackTrace();
                                cb.onError(0, e.getMessage());
                            } finally {
                                cb.onFinish();
                            }
                        }
                    });
                }
            }
        });
    }

    private static final int CODE_SUCCESS = 200;
    private static final int CODE_ERROR = 201;
    private static final int CODE_TOKEN_VERIFY_ERROR = 300;
    private static final int CODE_PARAMS_ERROR = 400;
    private static final int CODE_SERVER_ERROR = 500;


}
