package com.NC.o2ovender.Activity;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;

import com.NC.o2ovender.R;
import com.NC.o2ovender.Data.GoodsInfo;

public class BuyActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		Intent intent = this.getIntent();
		GoodsInfo goodsInfo = (GoodsInfo)intent.getSerializableExtra(GoodsListAdspter.GOODINFO_TAG);
		setContentView(R.layout.buy_activity);
	}

}
