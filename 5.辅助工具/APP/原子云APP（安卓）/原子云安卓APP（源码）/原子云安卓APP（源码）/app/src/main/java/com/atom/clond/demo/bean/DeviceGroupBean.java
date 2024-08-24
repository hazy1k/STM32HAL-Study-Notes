package com.atom.clond.demo.bean;

/**
 * 设备分组
 *
 * @author bohan.chen
 */
public class DeviceGroupBean {


    private int id;
    private int acc_id;
    private String acc_name;
    private int org_id;
    private String org_name;
    private String name;
    private String transfer_groups;
    private int count_device;
    private String created_at;
    private String operation;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getAcc_id() {
        return acc_id;
    }

    public void setAcc_id(int acc_id) {
        this.acc_id = acc_id;
    }

    public String getAcc_name() {
        return acc_name;
    }

    public void setAcc_name(String acc_name) {
        this.acc_name = acc_name;
    }

    public int getOrg_id() {
        return org_id;
    }

    public void setOrg_id(int org_id) {
        this.org_id = org_id;
    }

    public String getOrg_name() {
        return org_name;
    }

    public void setOrg_name(String org_name) {
        this.org_name = org_name;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getTransfer_groups() {
        return transfer_groups;
    }

    public void setTransfer_groups(String transfer_groups) {
        this.transfer_groups = transfer_groups;
    }

    public int getCount_device() {
        return count_device;
    }

    public void setCount_device(int count_device) {
        this.count_device = count_device;
    }

    public String getCreated_at() {
        return created_at;
    }

    public void setCreated_at(String created_at) {
        this.created_at = created_at;
    }

    public String getOperation() {
        return operation;
    }

    public void setOperation(String operation) {
        this.operation = operation;
    }
}
