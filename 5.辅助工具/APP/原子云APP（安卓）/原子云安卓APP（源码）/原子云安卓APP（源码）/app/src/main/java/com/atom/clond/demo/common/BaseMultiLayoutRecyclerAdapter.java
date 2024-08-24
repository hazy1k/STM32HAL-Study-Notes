package com.atom.clond.demo.common;

import android.content.Context;

import java.util.List;

/**
 * 多布局界面适配器
 *
 * @author bohan.chen
 */
public abstract class BaseMultiLayoutRecyclerAdapter<T> extends BaseRecyclerAdapter<T> {

    public BaseMultiLayoutRecyclerAdapter(Context mContext, List<T> dataList, int[] layoutIds) {
        super(mContext, dataList, layoutIds);
    }

    @Override
    protected final void covert(BaseViewHolder holder, T itemData, int position) {
        coverts(holder, itemData, position, getItemViewType(position));
    }


    @Override
    public int getItemViewType(int position) {
        return getItemType(position, getDataList().get(position));
    }

    /**
     * 获取当前View的type，根据type值得不同，加载不同得布局
     */
    protected abstract int getItemType(int position, T t);

    protected abstract void coverts(BaseViewHolder holder, T itemData, int position, int itemType);

}
