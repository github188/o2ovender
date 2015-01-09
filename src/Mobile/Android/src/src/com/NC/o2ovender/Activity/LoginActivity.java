package com.NC.o2ovender.Activity;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.ImageView;
import android.widget.TextView;

import com.NC.o2ovender.R;
import com.NC.o2ovender.Service.ServiceMgrImp;
import com.NC.o2ovender.Service.Command.Mobile2Service.response;
import com.NC.o2ovender.Service.ServiceMgrImp.ResponseCmdCallback;

public class LoginActivity extends Activity implements ResponseCmdCallback {
	
	private ImageView mHomeBtn = null;
	private TextView mRegistView = null;

	private void initView() {
		mHomeBtn = (ImageView)findViewById(R.id.loginBackHome);
		mHomeBtn.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				startActivity(new Intent(LoginActivity.this, HomeActivity.class));
			}
		});
		
		mRegistView = (TextView)findViewById(R.id.registerID);
		mRegistView.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				startActivity(new Intent(LoginActivity.this, RegisterActivity.class));
			}
		});
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.login_activity);
		initView();
		ServiceMgrImp.GetServiceMgrImp().Register(response.TYPE.LOGIN, this);
	}

	@Override
	protected void onDestroy() {
		ServiceMgrImp.GetServiceMgrImp().UnRegister(response.TYPE.LOGIN, this);
		super.onDestroy();
	}

	@Override
	public void responseCallback(response responseCmd) {
		// TODO Auto-generated method stub		
	}
	
}
