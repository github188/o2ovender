package com.NC.o2ovender.Activity;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnFocusChangeListener;
import android.widget.Button;
import android.widget.EditText;

import com.NC.o2ovender.R;
import com.NC.o2ovender.Service.ServiceMgrImp;
import com.NC.o2ovender.Service.ServiceMgrImp.ResponseCmdCallback;
import com.NC.o2ovender.Service.Command.Mobile2Service.get_identifying_code_req;
import com.NC.o2ovender.Service.Command.Mobile2Service.register_req;
import com.NC.o2ovender.Service.Command.Mobile2Service.register_resp;
import com.NC.o2ovender.Service.Command.Mobile2Service.request;
import com.NC.o2ovender.Service.Command.Mobile2Service.response;

public class RegisterActivity extends Activity implements ResponseCmdCallback {
	private Button mAccBtn = null;
	private Button mRegistBtn = null;
	private EditText mUserIDEditText = null;
	private EditText mUserPasswordEditText = null;
	private EditText mAccountEditText = null;
	
	private String mStrUserID = null;
	
	private void initView() {
		mUserIDEditText = (EditText)findViewById(R.id.registerUserID);
		mUserIDEditText.setOnFocusChangeListener(new OnFocusChangeListener() {
			
			@Override
			public void onFocusChange(View v, boolean hasFocus) {
				if (false == hasFocus) {
					mStrUserID = mUserIDEditText.getText().toString();
					mAccBtn.setEnabled(null != mStrUserID);
				}
			}
		});
		mUserPasswordEditText = (EditText)findViewById(R.id.registerUserPassword);
		mAccountEditText = (EditText)findViewById(R.id.verificationEdit);
		
		mAccBtn = (Button)findViewById(R.id.verificationBtn);
		mAccBtn.setEnabled(null != mStrUserID);
		mAccBtn.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				assert(null != mStrUserID);
				ServiceMgrImp.GetServiceMgrImp().send(EncodeVerificationRequestCmd(mStrUserID));
			}
		});
		
		mRegistBtn = (Button)findViewById(R.id.RegisterBtn);
		mRegistBtn.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				ServiceMgrImp.GetServiceMgrImp().send(
						EncodeRegisterRequestCmd(mStrUserID, 
								mUserPasswordEditText.getText().toString(), 
								mAccountEditText.getText().toString()));
				}
		});
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.register_activity);
		initView();
		ServiceMgrImp.GetServiceMgrImp().Register(response.TYPE.REGISTER, this);
	}

	@Override
	protected void onDestroy() {
		ServiceMgrImp.GetServiceMgrImp().UnRegister(response.TYPE.REGISTER, this);
		super.onDestroy();		
	}

	@Override
	public void responseCallback(response responseCmd) {
		response.TYPE respondCmdType = responseCmd.getType();
		if (response.TYPE.REGISTER == respondCmdType) {
			DecodeRegisterRequestCmd(responseCmd);
		} else {
			assert(false);
		}
	}
	
	private request EncodeVerificationRequestCmd(String strPhoneNum) {
		get_identifying_code_req getIdentifying = get_identifying_code_req.newBuilder().setUid(strPhoneNum).build();
				
		request.Builder builder = request.newBuilder();
		builder.setType(request.TYPE.GET_IDENTIFYING_CODE);
		builder.setGetIdentifyingCodeReq(getIdentifying);
		
		request request = builder.build();
		return request;
	}
	
	private request EncodeRegisterRequestCmd(String strPhoneNum, String strPassword, String strVerification) {
		register_req.Builder registerCmd = register_req.newBuilder();
		registerCmd.setUid(strPhoneNum);
		registerCmd.setPassWord(strPassword);
		registerCmd.setIdentifyingCode(strVerification);
		
		request.Builder builder = request.newBuilder();
		builder.setType(request.TYPE.REGISTER);
		builder.setRegisterReq(registerCmd);
		
		request request = builder.build();
		return request;
	}
	
	private void DecodeRegisterRequestCmd(response responseCmd) {
		register_resp registerCmd = responseCmd.getRegisterResp();
		int iResult = registerCmd.getResult();
		String strErrorMsg = registerCmd.getMsg();
	}
}
