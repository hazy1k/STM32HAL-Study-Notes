package com.atom.clond.demo.bean;

import java.util.List;

/**
 * 消息列表
 * @author bohan.chen
 */
public class MessageListBean {

    private int current_page;
    private int total_page;
    private int total_item;
    private int page_limit;
    private List<MessageBean> items;

    public int getCurrent_page() {
        return current_page;
    }

    public void setCurrent_page(int current_page) {
        this.current_page = current_page;
    }

    public int getTotal_page() {
        return total_page;
    }

    public void setTotal_page(int total_page) {
        this.total_page = total_page;
    }

    public int getTotal_item() {
        return total_item;
    }

    public void setTotal_item(int total_item) {
        this.total_item = total_item;
    }

    public int getPage_limit() {
        return page_limit;
    }

    public void setPage_limit(int page_limit) {
        this.page_limit = page_limit;
    }

    public List<MessageBean> getItems() {
        return items;
    }

    public void setItems(List<MessageBean> items) {
        this.items = items;
    }
}
