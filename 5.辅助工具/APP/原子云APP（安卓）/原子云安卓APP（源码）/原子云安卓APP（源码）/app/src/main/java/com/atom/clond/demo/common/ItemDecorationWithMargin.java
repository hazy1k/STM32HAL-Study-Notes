package com.atom.clond.demo.common;

import android.content.res.Resources;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Rect;
import android.graphics.drawable.ColorDrawable;
import android.support.annotation.NonNull;
import android.support.v7.widget.RecyclerView;
import android.view.View;

/**
 * 带左右margin的分隔线
 *
 * @author cbh
 */
public class ItemDecorationWithMargin extends RecyclerView.ItemDecoration {

    private int mMarginLeft = dip2px(15);
    private int mMarginRight = dip2px(15);
    private ColorDrawable mDivider;
    private final Rect mBounds = new Rect();
    private int mDividerHeight = dip2px(0.5f);

    public ItemDecorationWithMargin(int color) {
        mDivider = new ColorDrawable();
        mDivider.setColor(color);
    }

    public ItemDecorationWithMargin(float diverHeight){
        mDividerHeight = (int) diverHeight;
    }

    public ItemDecorationWithMargin() {
        this(Color.parseColor("#EEEEEE"));
    }

    public void setMarginLeft(int mMarginLeft) {
        this.mMarginLeft = mMarginLeft;
    }

    public void setMarginRight(int mMarginRight) {
        this.mMarginRight = mMarginRight;
    }

    public void setDrawable(ColorDrawable drawable) {
        if (drawable == null) {
            throw new IllegalArgumentException("Drawable cannot be null.");
        } else {
            this.mDivider = drawable;
        }
    }

    /**
     * 设置分隔线高度
     */
    public void setDividerHei(int height) {
        this.mDividerHeight = height;
    }

    @Override
    public void onDraw(Canvas c, @NonNull RecyclerView parent, @NonNull RecyclerView.State state) {
        if (parent.getLayoutManager() != null && this.mDivider != null) {
            this.drawVertical(c, parent);
        }
    }

    private void drawVertical(Canvas canvas, RecyclerView parent) {
        canvas.save();
        int left;
        int right;
        if (parent.getClipToPadding()) {
            left = parent.getPaddingLeft() + mMarginLeft;
            right = parent.getWidth() - parent.getPaddingRight() - mMarginRight;
            canvas.clipRect(left, parent.getPaddingTop(), right, parent.getHeight() - parent.getPaddingBottom());
        } else {
            left = mMarginLeft;
            right = parent.getWidth() - mMarginRight;
        }

        int childCount = parent.getChildCount() - 1;

        for (int i = 0; i < childCount; ++i) {
            View child = parent.getChildAt(i);
            parent.getDecoratedBoundsWithMargins(child, this.mBounds);
            int bottom = this.mBounds.bottom + Math.round(child.getTranslationY());
            int top = bottom - mDividerHeight;
            this.mDivider.setBounds(left, top, right, bottom);
            this.mDivider.draw(canvas);
        }
        canvas.restore();
    }


    @Override
    public void getItemOffsets(Rect outRect, View view, RecyclerView parent, RecyclerView.State state) {
        if (this.mDivider == null) {
            outRect.set(0, 0, 0, 0);
        } else {
            outRect.set(0, 0, 0, mDividerHeight);
        }
    }

    private static int dip2px(float dipValue) {
        float scale = Resources.getSystem().getDisplayMetrics().density;
        return (int) (dipValue * scale + 0.5f);
    }
    
}
