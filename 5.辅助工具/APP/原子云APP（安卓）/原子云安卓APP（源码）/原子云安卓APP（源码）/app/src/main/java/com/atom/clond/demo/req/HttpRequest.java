package com.atom.clond.demo.req;

import com.atom.clond.demo.bean.DeviceBean;
import com.atom.clond.demo.bean.DeviceGroupBean;
import com.atom.clond.demo.bean.MessageListBean;
import com.atom.clond.demo.bean.OrganizationBean;
import com.atom.clond.demo.http.HttpCallback;
import com.atom.clond.demo.http.HttpManager;

import java.util.HashMap;
import java.util.List;

/**
 * http请求
 *
 * @author bohan.chen
 */
public class HttpRequest {

    /**
     * 获取机构列表
     */
    public static void getOrganizationList(HttpCallback<List<OrganizationBean>> cb) {
        String url = "orgs";
        HttpManager.getInstance().getArray(url, cb, OrganizationBean.class);
    }

    /**
     * 获取设备分组列表
     */
    public static void getDeviceGroupList(int orgId, HttpCallback<List<DeviceGroupBean>> cb) {
        String url = "orgs/" + orgId + "/grouplist";
        HttpManager.getInstance().getArray(url, cb, DeviceGroupBean.class);
    }

    /**
     * 获取设备列表
     */
    public static void getDeviceList(int orgId, int groupId, HttpCallback<List<DeviceBean>> cb) {
        String url = "orgs/" + orgId + "/groups/" + groupId + "/devices";
        HttpManager.getInstance().getArray(url, cb, DeviceBean.class);
    }

    /**
     * 获取设备的连接状态
     */
    public static void getDeviceConnectStaatus(int orgId, int deviceId, HttpCallback<String> cb) {
        String url = "orgs/" + orgId + "/devicestate/" + deviceId;
        HttpManager.getInstance().get(url, cb, String.class);
    }

    /**
     * 获取聊天记录
     */
    public static void getChatHisData(int page, int orgId, String deviceNum, HttpCallback<MessageListBean> cb) {
        String url = "orgs/" + orgId + "/devicepacket/" + deviceNum;
        HashMap<String, Object> queryMap = new HashMap<>();
        queryMap.put("page", String.valueOf(page));
        queryMap.put("limit", 30);
        HttpManager.getInstance().get(url, cb, MessageListBean.class, queryMap);
    }

}
