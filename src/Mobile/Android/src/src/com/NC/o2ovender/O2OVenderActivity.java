package com.NC.o2ovender;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;

public class O2OVenderActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_o2_ovender);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {

		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.o2_ovender, menu);
		return true;
	}
}
