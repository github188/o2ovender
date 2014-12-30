package com.NC.o2ovender;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;

public class UserInfoActivity extends Activity {
	public final static String TAG = "UserInfo";
	public static final String U2M_BUNDLE_KEY = "userinfo2main_bundle_key";
	
	private Button mActivity2MainBtn;
	private TextView mTextView;
	
	private void InitView() {
		mTextView = (TextView)findViewById(R.id.UserInfoTextView);
		Bundle bundle = this.getIntent().getExtras();
		if (null != bundle) {
			mTextView.setText(bundle.getString(O2OVenderActivity.M2U_BUNDLE_KEY));
		}
		
		mActivity2MainBtn = (Button)findViewById(R.id.activityo2oBtn);
		mActivity2MainBtn.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				Intent intent = new Intent();
				intent.setClass(UserInfoActivity.this, O2OVenderActivity.class);
				Bundle bundle = new Bundle();
				bundle.putString(U2M_BUNDLE_KEY, "userinfo to main");
				intent.putExtras(bundle);
				startActivity(intent);
			}
		});
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.userinfo_activity);
		InitView();
	}
}
