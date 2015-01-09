package com.NC.o2ovender.Service.Backagent;

import com.NC.o2ovender.Service.Command.Mobile2Service.response;
import com.NC.o2ovender.Service.Command.Mobile2Service.response.TYPE;
import com.NC.o2ovender.Service.ServiceMgrImp.ResponseCmdCallback;

public class ResponseCallbackUnit {
	private response.TYPE mType = null;
	private ResponseCmdCallback mCmdCallback = null;
	public ResponseCallbackUnit(TYPE type, ResponseCmdCallback cmdCallback) {
		super();
		this.mType = type;
		this.mCmdCallback = cmdCallback;
	}
	public response.TYPE getType() {
		return mType;
	}
	public ResponseCmdCallback getCmdCallback() {
		return mCmdCallback;
	}	
}
