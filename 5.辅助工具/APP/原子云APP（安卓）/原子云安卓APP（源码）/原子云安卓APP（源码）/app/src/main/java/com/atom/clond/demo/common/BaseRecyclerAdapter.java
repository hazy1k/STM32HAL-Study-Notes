package com.atom.clond.demo.common;

import android.content.Context;
import android.support.annotation.NonNull;
import android.support.v7.widget.RecyclerView;
import android.view.View;
import android.view.ViewGroup;

import java.util.List;

/**
 * recycler adapter基类，单布局可以直接调用BaseRecyclerAdapter使用
 *
 * @author bohan.chen
 */
public abstract class BaseRecyclerAdapter<T> extends RecyclerView.Adapter<BaseViewHolder> {

    protected Context mContext;
    private List<T> mDataList;
    private OnItemClickListener<T> onItemClickListener;
    private OnItemLongClickListener<T> onItemLongClickListener;
    private int[] layoutIds;

    public void setOnItemClickListener(OnItemClickListener<T> onItemClickListener) {
        this.onItemClickListener = onItemClickListener;
    }

    public void setOnItemLongClickListener(OnItemLongClickListener<T> onItemLongClickListener) {
        this.onItemLongClickListener = onItemLongClickListener;
    }

    /**
     * 多布局时使用
     */
    public BaseRecyclerAdapter(Context mContext, List<T> mDataList, int[] layoutIds) {
        this.mContext = mContext;
        this.mDataList = mDataList;
        this.layoutIds = layoutIds;
    }

    /**
     * 单布局时使用
     */
    public BaseRecyclerAdapter(Context mContext, List<T> mDataList, int layoutId) {
        this.mContext = mContext;
        this.mDataList = mDataList;
        this.layoutIds = new int[]{layoutId};
    }

    public List<T> getDataList() {
        return mDataList;
    }

    public void setDataList(List<T> dataList) {
        this.mDataList = dataList;
    }

    @NonNull
    @Override
    public BaseViewHolder onCreateViewHolder(@NonNull ViewGroup viewGroup, int viewType) {
        return BaseViewHolder.getHolder(mContext, viewGroup, layoutIds[viewType]);
    }

    @Override
    public final void onBindViewHolder(@NonNull BaseViewHolder baseViewHolder, int position) {
        setListener(baseViewHolder, mDataList.get(position), position);
        covert(baseViewHolder, mDataList.get(position), position);
    }

    @Override
    public int getItemCount() {
        return mDataList == null ? 0 : mDataList.size();
    }

    /**
     * 设置点击事件
     */
    private void setListener(final BaseViewHolder holder, final T itemData, final int position) {
        holder.itemView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (onItemClickListener != null) {
                    onItemClickListener.onItemClick(holder, itemData, position);
                }
            }
        });
        holder.itemView.setOnLongClickListener(new View.OnLongClickListener() {
            @Override
            public boolean onLongClick(View v) {
                if (onItemLongClickListener != null) {
                    onItemLongClickListener.onItemLongClick(holder, itemData, position);
                    return true;
                }
                return false;
            }
        });
    }

    /**
     * 插入条目
     */
    public void insertItem(T itemData, int position) {
        mDataList.add(itemData);
        notifyItemInserted(position);
        notifyItemRangeChanged(position, getItemCount());
    }

    /**
     * 插入数据到最后面
     */
    public void insertItemToLast(T data) {
        insertItem(data, getItemCount());
    }

    /**
     * 插入条目
     */
    public void insertItem(T itemData) {
        insertItem(itemData, mDataList.size());
    }

    /**
     * 删除条目
     */
    public void removeItem(int position) {
        mDataList.remove(position);
        notifyItemRemoved(position);
        notifyItemRangeChanged(position, mDataList.size());
    }

    /**
     * 删除条目
     */
    public boolean removeItem(T data) {
        boolean isContains = mDataList.contains(data);
        if (isContains) {
            removeItem(mDataList.indexOf(data));
        }
        return isContains;
    }

    /**
     * 刷新条目
     */
    public void refreshList(List<T> data) {
        if (data != null) {
            this.mDataList = data;
            notifyDataSetChanged();
        }
    }

    /**
     * 刷新条目
     *
     * @param position
     */
    public void refreshItem(int position) {
        notifyItemChanged(position);
    }

    /**
     * 刷新条目数据
     */
    public void refreshItemData(T data, int position) {
        mDataList.set(position, data);
        notifyItemChanged(position);
    }

    /**
     * 批量添加条目
     */
    public void insertItems(List<T> list) {
        if (!list.isEmpty()) {
            int size = getItemCount();
            mDataList.addAll(list);
            notifyItemRangeChanged(size, list.size());
        }
    }

    /**
     * 获取最后一个条目的数据
     */
    public T getLastItemData() {
        if (!mDataList.isEmpty()) {
            return mDataList.get(mDataList.size() - 1);
        } else {
            return null;
        }
    }

    /**
     * 清楚数据
     */
    public void clearList() {
        mDataList.clear();
        notifyDataSetChanged();
    }

    protected abstract void covert(BaseViewHolder holder, T itemData, int position);


    public interface OnItemClickListener<T> {

        /**
         * item点击事件
         */
        void onItemClick(BaseViewHolder holder, T itemData, int position);

    }

    public interface OnItemLongClickListener<T> {
        /**
         * 长按点击事件
         */
        void onItemLongClick(BaseViewHolder holder, T itemData, int position);
    }

}
