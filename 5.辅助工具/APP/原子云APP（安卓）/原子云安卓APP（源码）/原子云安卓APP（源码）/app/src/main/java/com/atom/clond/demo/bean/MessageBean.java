package com.atom.clond.demo.bean;

import com.atom.clond.demo.utils.ByteUtils;

/**
 * 设备通讯消息
 *
 * @author bohan.chen
 */
public class MessageBean {

    private boolean isSelf;
    private String name;
    private String hex_packet;
    private String time;
    private byte[] dataByte;
    /**
     * 16进制数据
     */
    private String hexData;
    /**
     * 正常数据
     */
    private String normalData;

    public String getNormalData() {
        return normalData;
    }

    public void setNormalData(String normalData) {
        this.normalData = normalData;
    }

    public void setHexData(String hexData) {
        StringBuilder sb = new StringBuilder();
        hexData = hexData.replace(" ", "");
        int size = hexData.length();
        if (size % 2 == 1) {
            --size;
        }
        for (int i = 0; i < size; i += 2) {
            sb.append(hexData.charAt(i)).append(hexData.charAt(i + 1)).append(" ");
        }
        this.hexData = sb.toString();
    }

    public String getHexData() {
        return hexData;
    }

    public void setDataByte(byte[] dataByte) {
        this.dataByte = dataByte;
    }

    public byte[] getDataByte() {
        return dataByte;
    }

    public String getTime() {
        return time;
    }

    public void setTime(String time) {
        this.time = time;
    }

    public boolean isSelf() {
        return isSelf;
    }

    public void setSelf(boolean self) {
        isSelf = self;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getHex_packet() {
        return hex_packet;
    }

    public void setHex_packet(String hex_packet) {
        this.hex_packet = hex_packet;
    }
}
