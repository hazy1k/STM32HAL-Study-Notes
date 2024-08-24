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
import com.atom.clond.demo.bean.DeviceBean;
import com.atom.clond.demo.common.BaseRecyclerAdapter;
import com.atom.clond.demo.common.BaseViewHolder;
import com.atom.clond.demo.common.Constants;
import com.atom.clond.demo.common.GlobalValue;
import com.atom.clond.demo.common.ItemDecorationWithMargin;
import com.atom.clond.demo.http.HttpCallback;
import com.atom.clond.demo.req.HttpRequest;
import com.atom.clond.demo.utils.ToastUtils;

import java.util.ArrayList;
import java.util.List;

/**
 * 设备列表
 *
 * @author bohan.chen
 */
public class DeviceActivity extends AppCompatActivity {

    private BaseRecyclerAdapter<DeviceBean> mAdapter;
    private RecyclerView rv;
    private LinearLayoutManager linearLayoutManager;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_common_list);

        initView();
        initData();
    }

    private void initView() {
        findViewById(R.id.ivBack).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                finish();
            }
        });

        rv = findViewById(R.id.rv);
        linearLayoutManager = new LinearLayoutManager((this));
        rv.setLayoutManager(linearLayoutManager);
        rv.addItemDecoration(new ItemDecorationWithMargin());
        mAdapter = new BaseRecyclerAdapter<DeviceBean>(this, new ArrayList<DeviceBean>(0), R.layout.item_device) {
            @Override
            protected void covert(BaseViewHolder holder, DeviceBean itemData, int position) {
                holder.setText(R.id.tvName, "设备名称：" + itemData.getName());
                holder.setText(R.id.tvNum, "设备编号：" + itemData.getNumber());
                getDeviceStatus((TextView) holder.getView(R.id.tvStatus), itemData, position);
            }
        };
        mAdapter.setOnItemClickListener(new BaseRecyclerAdapter.OnItemClickListener<DeviceBean>() {
            @Override
            public void onItemClick(BaseViewHolder holder, DeviceBean itemData, int position) {
                Intent intent = new Intent(getApplicationContext(), DeviceChatActivity.class);
                intent.putExtra(ActivityKey.DEVICE_BEAN, itemData);
                startActivity(intent);
            }
        });
        rv.setAdapter(mAdapter);
    }

    private void getDeviceStatus(final TextView tv, final DeviceBean itemData, final int position) {
        HttpRequest.getDeviceConnectStaatus(GlobalValue.orgId, itemData.getId(), new HttpCallback<String>() {
            @Override
            public void onSuccess(String data) {
                int firstVisibleItemPosition = linearLayoutManager.findFirstVisibleItemPosition();
                int lastVisibleItemPosition = linearLayoutManager.findLastVisibleItemPosition();
                if (firstVisibleItemPosition <= position && lastVisibleItemPosition >= position) {
                }
                tv.setText("设备状态：" + parseStatusData(data, itemData));
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

    /**
     * 解析状态数据
     */
    private String parseStatusData(String data, DeviceBean itemData) {
//        if (Constants.DEVICE_WIFI.equals(itemData.getDevice_type()) ||
//                Constants.DEVICE_DTU.equals(itemData.getDevice_type())) {
        if (Constants.DEVICE_STATUS_DISCONNECTED.equals(data)) {
            return "断开";
        } else if (Constants.DEVICE_STATUS_CONNECTED.equals(data)) {
            return "已连接";
        }
//        } else if (Constants.DEVICE_NBIOT.equals(itemData.getDevice_type())) {
        if (Constants.DEVICE_STATUS_BIND.equals(data)) {
            return "已绑定";
        } else if (Constants.DEVICE_STATUS_UN_BIND.equals(data)) {
            return "未绑定";
        }
//        }
        return "";
    }

    private void initData() {
        String title = getIntent().getStringExtra(ActivityKey.TITLE);
        ((TextView) findViewById(R.id.tvHeader)).setText(title);

        int groupId = getIntent().getIntExtra(ActivityKey.GROUP_ID, -1);
        HttpRequest.getDeviceList(GlobalValue.orgId, groupId, new HttpCallback<List<DeviceBean>>() {
            @Override
            public void onSuccess(List<DeviceBean> data) {
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
