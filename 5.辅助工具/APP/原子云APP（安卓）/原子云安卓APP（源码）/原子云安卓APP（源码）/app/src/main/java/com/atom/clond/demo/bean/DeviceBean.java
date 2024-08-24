package com.atom.clond.demo.bean;

import android.os.Parcel;
import android.os.Parcelable;

/**
 * 设备信息
 *
 * @author bohan.chen
 */
public class DeviceBean implements Parcelable {

    private int id;
    private String show_id;
    private String device_type;
    private String name;
    private String number;
    private String hexData;

    public DeviceBean() {
    }

    protected DeviceBean(Parcel in) {
        id = in.readInt();
        show_id = in.readString();
        device_type = in.readString();
        hexData = in.readString();
        name = in.readString();
        number = in.readString();
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeInt(id);
        dest.writeString(show_id);
        dest.writeString(device_type);
        dest.writeString(hexData);
        dest.writeString(name);
        dest.writeString(number);
    }

    @Override
    public int describeContents() {
        return 0;
    }

    public static final Creator<DeviceBean> CREATOR = new Creator<DeviceBean>() {
        @Override
        public DeviceBean createFromParcel(Parcel in) {
            return new DeviceBean(in);
        }

        @Override
        public DeviceBean[] newArray(int size) {
            return new DeviceBean[size];
        }
    };

    public String getHexData() {
        return hexData;
    }

    public void setHexData(String hexData) {
        this.hexData = hexData;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getShow_id() {
        return show_id;
    }

    public void setShow_id(String show_id) {
        this.show_id = show_id;
    }

    public String getDevice_type() {
        return device_type;
    }

    public void setDevice_type(String device_type) {
        this.device_type = device_type;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getNumber() {
        return number;
    }

    public void setNumber(String number) {
        this.number = number;
    }
}
