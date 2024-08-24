package com.atom.clond.demo.activity;

import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.View;
import android.widget.TextView;

import com.atom.clond.demo.R;
import com.atom.clond.demo.bean.DeviceGroupBean;
import com.atom.clond.demo.common.BaseRecyclerAdapter;
import com.atom.clond.demo.common.BaseViewHolder;
import com.atom.clond.demo.common.GlobalValue;
import com.atom.clond.demo.common.ItemDecorationWithMargin;
import com.atom.clond.demo.http.HttpCallback;
import com.atom.clond.demo.req.HttpRequest;
import com.atom.clond.demo.utils.ToastUtils;

import java.util.ArrayList;
import java.util.List;

/**
 * 设备分组
 *
 * @author bohan.chen
 */
public class DeviceGroupActivity extends AppCompatActivity {

    private BaseRecyclerAdapter<DeviceGroupBean> mAdapter;
    private RecyclerView rv;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_common_list);

        initView();
        initData();
    }

    private void initView() {
        findViewById(R.id.ivBack).setVisibility(View.INVISIBLE);

        rv = findViewById(R.id.rv);
        rv.setLayoutManager(new LinearLayoutManager((this)));
        rv.addItemDecoration(new ItemDecorationWithMargin());
        mAdapter = new BaseRecyclerAdapter<DeviceGroupBean>(this, new ArrayList<DeviceGroupBean>(0), R.layout.item_device_group) {
            @Override
            protected void covert(BaseViewHolder holder, DeviceGroupBean itemData, int position) {
                holder.setText(R.id.tvName, "分组名称：" + itemData.getName());
                holder.setText(R.id.tvDes, "设备数量：" + itemData.getCount_device());
            }
        };
        mAdapter.setOnItemClickListener(new BaseRecyclerAdapter.OnItemClickListener<DeviceGroupBean>() {
            @Override
            public void onItemClick(BaseViewHolder holder, DeviceGroupBean itemData, int position) {
                Intent intent = new Intent(getApplicationContext(), DeviceActivity.class);
                intent.putExtra(ActivityKey.TITLE,itemData.getName());
                intent.putExtra(ActivityKey.GROUP_ID, itemData.getId());
                startActivity(intent);
            }
        });
        rv.setAdapter(mAdapter);
    }

    private void initData() {
        String title = getIntent().getStringExtra(ActivityKey.TITLE);
        ((TextView) findViewById(R.id.tvHeader)).setText(title + "：设备分组列表");

        HttpRequest.getDeviceGroupList(GlobalValue.orgId, new HttpCallback<List<DeviceGroupBean>>() {
            @Override
            public void onSuccess(List<DeviceGroupBean> data) {
                mAdapter.refreshList(data);
            }

            @Override
            public void onError(int code, String errMsg) {
                ToastUtils.showShort(errMsg);
            }

            @Override
            public void onFinish() {

            }
        });
    }
}
