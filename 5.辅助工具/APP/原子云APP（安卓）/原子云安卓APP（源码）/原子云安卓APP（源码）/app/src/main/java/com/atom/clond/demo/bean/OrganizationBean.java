package com.atom.clond.demo.bean;

/**
 * 机构信息
 *
 * @author bohan.chen
 */
public class OrganizationBean {


    private int id;
    private String name;
    private int acc_id;
    private int device_limit;
    private int device_counter;
    private String created_at;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getAcc_id() {
        return acc_id;
    }

    public void setAcc_id(int acc_id) {
        this.acc_id = acc_id;
    }

    public int getDevice_limit() {
        return device_limit;
    }

    public void setDevice_limit(int device_limit) {
        this.device_limit = device_limit;
    }

    public int getDevice_counter() {
        return device_counter;
    }

    public void setDevice_counter(int device_counter) {
        this.device_counter = device_counter;
    }

    public String getCreated_at() {
        return created_at;
    }

    public void setCreated_at(String created_at) {
        this.created_at = created_at;
    }
}
