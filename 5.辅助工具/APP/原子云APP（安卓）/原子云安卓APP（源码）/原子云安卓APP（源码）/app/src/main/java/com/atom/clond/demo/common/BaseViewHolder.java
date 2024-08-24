package com.atom.clond.demo.common;

import android.content.Context;
import android.graphics.Bitmap;
import android.support.annotation.NonNull;
import android.support.v7.widget.RecyclerView;
import android.util.SparseArray;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.bumptech.glide.Glide;

import java.io.File;

/**
 * recyclerView 基holder
 *
 * @author bohan.chen
 */
public class BaseViewHolder extends RecyclerView.ViewHolder {

    private SparseArray<View> mViewArray;
    private Context mContext;

    public BaseViewHolder(Context context, @NonNull View itemView) {
        super(itemView);
        mViewArray = new SparseArray<>(8);
        this.mContext = context;
    }

    public Context getContext() {
        return mContext;
    }

    /**
     * 通过Id获取到view
     *
     * @param viewId
     * @param <T>
     * @return
     */
    public <T extends View> T getView(int viewId) {
        View view = mViewArray.get(viewId);
        if (view == null) {
            view = itemView.findViewById(viewId);
            mViewArray.put(viewId, view);
        }
        return (T) view;
    }

    /**
     * 返回viewHolder给adapter使用
     */
    public static BaseViewHolder getHolder(Context context, ViewGroup parent, int layoutId) {
        return new BaseViewHolder(context, LayoutInflater.from(context).inflate(layoutId, parent, false));
    }

    /**
     * textView设置值
     */
    public BaseViewHolder setText(int viewId, String text) {
        TextView view = getView(viewId);
        view.setText(text);
        return this;
    }

    /**
     * 设置字符串颜色
     */
    public BaseViewHolder setTextColor(int viewId, int textColor) {
        TextView view = getView(viewId);
        view.setTextColor(textColor);
        return this;
    }

    /**
     * 设置View是否被选中
     */
    public BaseViewHolder setViewChecked(int viewId, boolean isCheck) {
        View view = getView(viewId);
        view.setSelected(isCheck);
        return this;
    }

    /**
     * 设置view的点击事件
     *
     * @param viewId
     * @param listener
     * @return
     */
    public BaseViewHolder setClickListener(int viewId, View.OnClickListener listener) {
        getView(viewId).setOnClickListener(listener);
        return this;
    }

    /**
     * 设置是否可见
     *
     * @param viewId
     * @param visibility
     * @return
     */
    public BaseViewHolder setViewVisibility(int viewId, int visibility) {
        getView(viewId).setVisibility(visibility);
        return this;
    }

    /**
     * 设置View的背景颜色
     */
    public BaseViewHolder setViewBgColor(int viewId, int colorId) {
        getView(viewId).setBackgroundColor(colorId);
        return this;
    }

    /**
     * 设置View的背景图片
     */
    public BaseViewHolder setViewBgResource(int viewId, int resId) {
        getView(viewId).setBackgroundResource(resId);
        return this;
    }

    /**
     * 设置资源图片
     */
    public BaseViewHolder setImageResource(int viewId, int resId) {
        ImageView iv = getView(viewId);
        iv.setImageResource(resId);
        return this;
    }

    /**
     * 设置bitmap图片
     */
    public BaseViewHolder setImageBitmap(int viewId, Bitmap bitmap) {
        ImageView iv = getView(viewId);
        iv.setImageBitmap(bitmap);
        return this;
    }

    /**
     * 为linearLayout添加子View
     *
     * @param viewId
     * @param childView
     * @return
     */
    public BaseViewHolder addChildView(int viewId, View childView) {
        LinearLayout view = getView(viewId);
        view.addView(childView);
        return this;
    }

    /**
     * 设置网络图片并显示默认图片
     *
     * @param viewId
     * @param url
     * @param defaultImage
     * @return
     */
    public BaseViewHolder setImageByUrl(int viewId, String url, int defaultImage) {
        ImageView view = getView(viewId);
        Glide.with(mContext).load(url).placeholder(defaultImage).into(view);
        return this;
    }

    /**
     * 设置本地图片
     *
     * @param file 文件本地地址
     */
    public BaseViewHolder setImageByFile(int viewId, File file, int defaultImage) {
        ImageView view = getView(viewId);
        Glide.with(mContext).load(file).placeholder(defaultImage).into(view);
        return this;
    }

}
