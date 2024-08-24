package com.atom.clond.demo.activity;

import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.support.annotation.Nullable;
import android.support.v4.widget.SwipeRefreshLayout;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.text.InputFilter;
import android.text.Spanned;
import android.text.TextUtils;
import android.view.View;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.atom.clond.demo.R;
import com.atom.clond.demo.adapter.ChatAdapter;
import com.atom.clond.demo.bean.DeviceBean;
import com.atom.clond.demo.bean.MessageBean;
import com.atom.clond.demo.bean.MessageListBean;
import com.atom.clond.demo.common.Constants;
import com.atom.clond.demo.common.GlobalValue;
import com.atom.clond.demo.http.HttpCallback;
import com.atom.clond.demo.req.HttpRequest;
import com.atom.clond.demo.utils.ByteUtils;
import com.atom.clond.demo.utils.ToastUtils;
import com.atom.clond.demo.ws.WsEventListener;
import com.atom.clond.demo.ws.WsManager;

import java.util.ArrayList;
import java.util.UUID;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * 设备通讯页面
 *
 * @author bohan.chen
 */
public class DeviceChatActivity extends AppCompatActivity implements WsEventListener {

    private EditText etMsg;
    private ChatAdapter mAdapter;
    private SwipeRefreshLayout srl;
    private LinearLayout llChat;
    private LinearLayout llFun1;
    private LinearLayout llFun2;
    private TextView tvTitle;
    private RecyclerView rv;
    private Button btnHexShow;
    private Handler mMainHandler;

    private int pageNo = 1;

    private DeviceBean mDeviceBean;
    private WsManager wsManager;


    /**
     * 是否为16进制发送
     */
    private boolean isHexSend = false;

    /**
     * 是否16进制显示
     */
    private boolean isHexShow = false;
    /**
     * 随机生成的uuid
     */
    private String uuid;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_device_chat);
        initView();

        getHistoryRecord();
        connectDevice();
    }

    private void initView() {
        findViewById(R.id.ivBack).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                finish();
            }
        });
        llChat = findViewById(R.id.llChat);
        llFun1 = findViewById(R.id.llFun1);
        llFun2 = findViewById(R.id.llFun2);
        tvTitle = findViewById(R.id.tvHeader);
        etMsg = findViewById(R.id.etMessage);
        srl = findViewById(R.id.srl);
        srl.setOnRefreshListener(new SwipeRefreshLayout.OnRefreshListener() {
            @Override
            public void onRefresh() {
                getHistoryRecord();
            }
        });
        rv = findViewById(R.id.rv);
        rv.setLayoutManager(new LinearLayoutManager(this));
        mAdapter = new ChatAdapter(this, new ArrayList<MessageBean>(0));

        btnHexShow = findViewById(R.id.btnShowHex);
        btnHexShow.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                changeHexShowBtnStatus(!isHexShow);
            }
        });

        CheckBox cb = findViewById(R.id.cbHexSend);
        cb.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                isHexSend = isChecked;
                etMsg.setText("");
            }
        });
        etMsg.setFilters(new InputFilter[]{hexFilter});

        findViewById(R.id.btnSend).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendMessage(etMsg.getText().toString());
            }
        });
        findViewById(R.id.btnSub).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                showAlert(wsManager.subDevice(), "订阅");
            }
        });
        findViewById(R.id.btnUnSub).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                showAlert(wsManager.unSubDevice(), "取消订阅");
            }
        });
        findViewById(R.id.btnClean).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mAdapter.refreshList(new ArrayList<MessageBean>(0));
            }
        });
        rv.setAdapter(mAdapter);

        mMainHandler = new Handler(Looper.getMainLooper());
    }

    /**
     * 改变16进制显示
     */
    private void changeHexShowBtnStatus(boolean isHexShow) {
        this.isHexShow = isHexShow;
        if (this.isHexShow) {
            btnHexShow.setText("ASCII码显示");
        } else {
            btnHexShow.setText("十六进制显示");
        }
        mAdapter.setShowHex(this.isHexShow);
    }

    /**
     * 16进制数过滤器
     */
    private InputFilter hexFilter = new InputFilter() {
        @Override
        public CharSequence filter(CharSequence source, int start, int end, Spanned dest, int dstart, int dend) {
            if (!isHexSend) {
                return null;
            }
            String upperCase = source.toString().toUpperCase();
            String regex = "^[A-Fa-f0-9]";
            Pattern pattern = Pattern.compile(regex);
            Matcher matcher = pattern.matcher(upperCase);
            if (!matcher.find()) return "";
            else return null;
        }
    };

    /**
     * 弹出提示
     */
    private void showAlert(boolean isSuccess, String actionName) {
        if (isSuccess) {
            ToastUtils.showShort(actionName + "成功");
        } else {
            ToastUtils.showShort(actionName + "失败");
        }
    }

    /**
     * 获取聊天记录
     */
    private void getHistoryRecord() {
        srl.setRefreshing(true);
        mDeviceBean = getIntent().getParcelableExtra(ActivityKey.DEVICE_BEAN);
        tvTitle.setText(mDeviceBean.getName());
        HttpRequest.getChatHisData(pageNo, GlobalValue.orgId, mDeviceBean.getNumber(), new HttpCallback<MessageListBean>() {

            @Override
            public void onSuccess(MessageListBean data) {
                if (data != null) {
                    for (MessageBean item : data.getItems()) {
                        item.setName("old");
                        item.setHex_packet(item.getHex_packet().replace(mDeviceBean.getNumber(), ""));
                        item.setNormalData(item.getHex_packet());
                        item.setHexData(ByteUtils.str2HexStr(item.getHex_packet()));
                    }
                    pageNo++;
                    mAdapter.insertItems(data.getItems());
                }
            }

            @Override
            public void onError(int code, String errMsg) {
                ToastUtils.showShort(errMsg);
            }

            @Override
            public void onFinish() {
                srl.setRefreshing(false);
            }
        });
    }

    /**
     * 连接设备
     */
    private void connectDevice() {
        wsManager = new WsManager();
        uuid = UUID.randomUUID().toString();
        String url = Constants.WS_SERVER_ADDRESS + Constants.HTTP_TOKEN + "/org/" + GlobalValue.orgId + "?token=" + uuid;
        wsManager.init(url, mDeviceBean, this);
    }

    /**
     * 发送数据
     */
    private void sendMessage(String msg) {
        if (!TextUtils.isEmpty(msg)) {
            byte[] dataByte;
            etMsg.setText("");
            MessageBean messageBean = new MessageBean();
            messageBean.setName("TX");
            if (isHexSend) {
                dataByte = ByteUtils.hexStringToByteArray(msg);
                messageBean.setHexData(msg);
                messageBean.setNormalData(new String(dataByte));
            } else {
                dataByte = msg.getBytes();
                messageBean.setNormalData(msg);
                messageBean.setHexData(ByteUtils.str2HexStr(msg));
            }
            messageBean.setDataByte(dataByte);
            if (wsManager.sendMsg(dataByte)) {
                insertMessage(messageBean);
            } else {
                ToastUtils.showShort("发送失败");
            }
        }
    }

    /**
     * 将消息添加到列表
     */
    private void insertMessage(MessageBean messageBean) {
        mAdapter.insertItem(messageBean);
        rv.smoothScrollToPosition(mAdapter.getItemCount() - 1);
    }

    @Override
    public void onFailed(final String errMsg) {
        mMainHandler.post(new Runnable() {
            @Override
            public void run() {
                llChat.setVisibility(View.GONE);
                ToastUtils.showShort(errMsg);
            }
        });
    }

    @Override
    public void onRecMessage(final MessageBean data) {
        mMainHandler.post(new Runnable() {
            @Override
            public void run() {
                insertMessage(data);
            }
        });
    }

    @Override
    public void onDisconnect() {
        mMainHandler.post(new Runnable() {
            @Override
            public void run() {
                llChat.setVisibility(View.GONE);
                llFun1.setVisibility(View.GONE);
                llFun2.setVisibility(View.GONE);
                ToastUtils.showShort("设备已断开");
            }
        });
    }

    @Override
    public void onConnect() {
        mMainHandler.post(new Runnable() {
            @Override
            public void run() {
                llChat.setVisibility(View.VISIBLE);
                llFun1.setVisibility(View.VISIBLE);
                llFun2.setVisibility(View.VISIBLE);
                ToastUtils.showShort("设备已连接");
            }
        });
    }

    @Override
    protected void onDestroy() {
        wsManager.close();
        super.onDestroy();
    }
}
