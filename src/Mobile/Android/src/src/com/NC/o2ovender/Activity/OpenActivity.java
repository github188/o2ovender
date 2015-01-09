package com.NC.o2ovender.Activity;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.view.Window;
import android.view.WindowManager;

import com.NC.o2ovender.R;

public class OpenActivity extends Activity 	{
	private static final int SHOW_TIME = 3000;//ms

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,  WindowManager.LayoutParams.FLAG_FULLSCREEN);
		setContentView(R.layout.open_activity);
		
		new Handler().postDelayed(new Runnable() {			
			@Override
			public void run() {
				startActivity(new Intent(OpenActivity.this, HomeActivity.class));
				OpenActivity.this.finish();
			}
		}, SHOW_TIME);
	}
}
