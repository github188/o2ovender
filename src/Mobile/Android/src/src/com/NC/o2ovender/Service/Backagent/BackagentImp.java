package com.NC.o2ovender.Service.Backagent;

import java.util.HashMap;
import java.util.Map;

import android.os.Handler;
import android.os.Looper;
import android.os.Message;

import com.NC.o2ovender.Service.Command.Mobile2Service.request;
import com.NC.o2ovender.Service.Command.Mobile2Service.response;
import com.NC.o2ovender.Service.ServiceMgrImp.ResponseCmdCallback;
import com.NC.o2ovender.Service.TransLayer.TransLayerImp;

public class BackagentImp extends Handler {
	private static final String TAG = "BackagentImp";
	private TransLayerImp mTransLayerImp = null;
		
	//消息ID
	public static final int MSG_TYPE_START = 0;
	public static final int MSG_TYPE_QUIT = 1;
	public static final int MSG_TYPE_SEND = 2;
	public static final int MSG_TYPE_RECV = 3;
	public static final int MSG_TYPE_REGIST = 4;
	public static final int MSG_TYPE_UNREGIST = 5;
	
	private Map<response.TYPE, ResponseCmdCallback> mCallbackMap = null;
	
	public BackagentImp(Looper looper) {
		super(looper);
	}
	
	private boolean init() {
		if (null == mTransLayerImp) {
			mTransLayerImp = new TransLayerImp();
			if (mTransLayerImp.init()) {
				mCallbackMap = new HashMap<response.TYPE, ResponseCmdCallback>();
				return true;
			} else {
				unInit();
				return false;
			}
		} else {
			return true;
		}
	}
	
	public void unInit() {
		if (null != mCallbackMap) {
			mCallbackMap = null;
		}
		if (null != mTransLayerImp) {
			mTransLayerImp.unInit();
			mTransLayerImp = null;
		}
	}

	@Override
	public void handleMessage(Message msg) {
		switch (msg.what) {
		case MSG_TYPE_START:
			handleStartMessage();
			break;
		case MSG_TYPE_QUIT:
			handleQuitMessage();
			break;
		case MSG_TYPE_SEND:
			handleSendDataMessage(msg);
			break;
		case MSG_TYPE_RECV:
			handleRecvDataMessage(msg);
			break;
		case MSG_TYPE_REGIST:
			handleRegisteMessage(msg);
			break;
		case MSG_TYPE_UNREGIST:
			handleUnregistMessage(msg);
			break;
		default:
			super.handleMessage(msg);
			break;
		}
	}
	
	private void handleSendDataMessage(Message msg) {
		request requestCmd = (request)msg.obj;
		mTransLayerImp.sendRequest(requestCmd);
	}
	
	private void handleRecvDataMessage(Message msg) {
		response requestCmd = (response)msg.obj;
		response.TYPE type = requestCmd.getType();
		if (null != mCallbackMap) {
			ResponseCmdCallback cmdCallback = mCallbackMap.get(type);
			if (null != cmdCallback) {
				cmdCallback.responseCallback(requestCmd);
			}
		}		
	}
	
	private void handleStartMessage() {
		init();
	}
	
	private void handleQuitMessage() {
		unInit();
		getLooper().quit();
	}
	
	private void handleRegisteMessage(Message msg) {
		ResponseCallbackUnit unit = (ResponseCallbackUnit)msg.obj;
		mCallbackMap.put(unit.getType(), unit.getCmdCallback());
	}
	
	private void handleUnregistMessage(Message msg) {
		ResponseCallbackUnit unit = (ResponseCallbackUnit)msg.obj;
		mCallbackMap.remove(unit.getType());
	}
}
