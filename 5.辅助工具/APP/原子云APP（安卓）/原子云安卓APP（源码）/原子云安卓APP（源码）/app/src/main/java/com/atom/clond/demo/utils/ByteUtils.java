package com.atom.clond.demo.utils;

/**
 * Byte转换工具
 *
 * @author yangle
 */
public class ByteUtils {

    /**
     * 16进制表示的字符串转换为字节数组
     *
     * @param hexString 16进制表示的字符串
     * @return byte[] 字节数组
     */
    public static byte[] hexStringToByteArray(String hexString) {
        hexString = hexString.replaceAll(" ", "");
        int len = hexString.length();
        if (len % 2 == 1) {
            hexString = hexString.substring(0, len - 1);
            len--;
        }
        byte[] bytes = new byte[len / 2];
        for (int i = 0; i < len; i += 2) {
            // 两位一组，表示一个字节,把这样表示的16进制字符串，还原成一个字节
            bytes[i / 2] = (byte) ((Character.digit(hexString.charAt(i), 16) << 4) + Character
                    .digit(hexString.charAt(i + 1), 16));
        }
        return bytes;
    }

    /**
     * 字符串转换成十六进制字符串
     *
     * @return String 每个Byte之间空格分隔，如: [61 6C 6B]
     */
    public static String str2HexStr(String str) {
        char[] chars = "0123456789ABCDEF".toCharArray();
        StringBuilder sb = new StringBuilder();
        byte[] bs = str.getBytes();
        int bit;
        for (int i = 0; i < bs.length; i++) {
            bit = (bs[i] & 0xf0) >> 4;
            sb.append(chars[bit]);
            bit = bs[i] & 0x0f;
            sb.append(chars[bit]);
            sb.append(' ');
        }
        return sb.toString().trim();
    }

    /**
     * 打印byte数组
     */
    public static String printByteArr(byte[] bytes) {
        StringBuilder sb = new StringBuilder();
        for (byte b : bytes) {
            sb.append(b & 0xFF).append(" ");
        }
        return sb.toString();
    }

    /**
     * byte[]数组转换为16进制的字符串
     *
     * @param bytes 要转换的字节数组
     * @return 转换后的结果
     */
    public static String bytesToHexString(byte[] bytes) {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < bytes.length; i++) {
            String hex = Integer.toHexString(0xFF & bytes[i]);
            if (hex.length() == 1) {
                sb.append('0');
            }
            sb.append(hex);
        }
        return sb.toString();
    }

    public static String showHexByte(String data) {
        StringBuilder sb = new StringBuilder();
        byte[] bytes = data.replace(" ", "").getBytes();
        for (byte b : bytes) {
            String str = Integer.toHexString(b);
            if (str.length() == 1) {
                str = "0" + str;
            }
            sb.append(str).append(" ");
        }
        return sb.toString();
    }

    public static String toHexShow(String str) {
        char[] chars = "0123456789ABCDEF".toCharArray();
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < str.length(); i += 2) {
            int i1 = Integer.parseInt(str.substring(i, i + 1), 16);
            int i2 = Integer.parseInt(str.substring(i + 1, i + 2), 16);
            int bit = (i1 & 0x0f);
            sb.append(chars[bit]);
            bit = i2 & 0x0f;
            sb.append(chars[bit]);
            sb.append(' ');
        }
        return sb.toString();
    }

    public static byte[] toByteData(String data) {
        StringBuilder sb = new StringBuilder();
        if (data.length() % 2 == 1) {
            data = data.substring(0, data.length() - 1);
        }
        byte[] byteArr = new byte[data.length() / 2];
        for (int i = 0; i < data.length(); i += 2) {
            byte i1 = (byte) (Integer.parseInt(data.substring(i, i + 1), 16) & 0xf0);
            byte i2 = (byte) (Integer.parseInt(data.substring(i + 1, i + 2), 16) & 0x0f);
            byte result = (byte) (0xff & i1 & i2);
            byteArr[i / 2] = result;
        }
        return byteArr;
    }



}