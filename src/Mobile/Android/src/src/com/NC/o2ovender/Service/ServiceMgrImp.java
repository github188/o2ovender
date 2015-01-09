package com.NC.o2ovender.Service;

import android.os.HandlerThread;
import android.os.Message;
import android.os.Process;
import android.util.Log;

import com.NC.o2ovender.Service.Backagent.BackagentImp;
import com.NC.o2ovender.Service.Backagent.ResponseCallbackUnit;
import com.NC.o2ovender.Service.Command.Mobile2Service.request;
import com.NC.o2ovender.Service.Command.Mobile2Service.response;

public class ServiceMgrImp extends HandlerThread {
	public interface ResponseCmdCallback {
		public void responseCallback(response responseCmd);
	}
	
	private static final String TAG = "ServiceMgrImp";
	private BackagentImp mBackagentImp = null;	
	private static ServiceMgrImp sInstance = null;

	public static ServiceMgrImp GetServiceMgrImp() {
		if (null == sInstance) {
			sInstance = new ServiceMgrImp();
			sInstance.init();
		}
		return sInstance;
	}
	
	public static void ReleaseServiceMgrImp() {
		if (null != sInstance) {
			sInstance.unInit();
			sInstance = null;
		}
	}
	
	private ServiceMgrImp() {
		super(TAG, Process.THREAD_PRIORITY_FOREGROUND);
	}
	
	private void init() {
		super.start();
		if (null == mBackagentImp) {
			mBackagentImp = new BackagentImp(getLooper());
			Message msg = Message.obtain(mBackagentImp, BackagentImp.MSG_TYPE_START);
			msg.sendToTarget();
		}
	}
	
	private void unInit() {
		if (null != mBackagentImp) {
			Message msg = Message.obtain(mBackagentImp, BackagentImp.MSG_TYPE_QUIT);
			msg.sendToTarget();
			mBackagentImp = null;
		}		
	}
	
	public boolean send(request requestCmd) {
		if (null == mBackagentImp) {
			Log.e(TAG, "backagent need init");
			return false;
		}
		Message msg = Message.obtain(mBackagentImp, BackagentImp.MSG_TYPE_SEND, requestCmd);
		msg.sendToTarget();
		return true;
	}
	
	public void recv(response responseCmd) {
		if (null == mBackagentImp) {
			Log.e(TAG, "backagent is uninit");
		}
		Message msg = Message.obtain(mBackagentImp, BackagentImp.MSG_TYPE_SEND, responseCmd);
		msg.sendToTarget();
	}
	
	public void Register(response.TYPE type, ResponseCmdCallback cmdCallback) {
		if (null != mBackagentImp) {
			ResponseCallbackUnit unit = new ResponseCallbackUnit(type, cmdCallback);
			if (null != unit) {
				Message msg = Message.obtain(mBackagentImp, BackagentImp.MSG_TYPE_REGIST, unit);
				msg.sendToTarget();
			}
		}
	}
	
	public void UnRegister(response.TYPE type, ResponseCmdCallback cmdCallback) {
		if (null != mBackagentImp) {
			ResponseCallbackUnit unit = new ResponseCallbackUnit(type, cmdCallback);
			if (null != unit) {
				Message msg = Message.obtain(mBackagentImp, BackagentImp.MSG_TYPE_UNREGIST, unit);
				msg.sendToTarget();
			}
		}
	}
}
