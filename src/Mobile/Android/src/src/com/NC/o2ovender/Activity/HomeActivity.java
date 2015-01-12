package com.NC.o2ovender.Activity;

import java.util.ArrayList;
import java.util.List;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.ImageView;
import android.widget.ListView;

import com.NC.o2ovender.R;
import com.NC.o2ovender.Data.GoodsInfo;

public class HomeActivity extends Activity {
	private static final String TAG = "HomeActivity";
	private ImageView mLocation = null;
	private ImageView mUserInfo = null;
	private ListView mGoodsListView = null;
	public static HomeActivity sHomeActivity = null;
	
	public class GoodsItemData {
		private GoodsInfo mLeftData = null;
		private GoodsInfo mRightData = null;
		public GoodsItemData(GoodsInfo leftData, GoodsInfo rightData) {
			super();
			this.mLeftData = leftData;
			this.mRightData = rightData;
		}
		public GoodsInfo getLeftData() {
			return mLeftData;
		}
		public GoodsInfo getRightData() {
			return mRightData;
		}		
	}

	private void initTitle() {
		mLocation = (ImageView)findViewById(R.id.location);
		
		mUserInfo = (ImageView)findViewById(R.id.userInfo);
		mUserInfo.setOnClickListener(new OnClickListener() {			
			@Override
			public void onClick(View v) {
				startActivity(new Intent(HomeActivity.this, LoginActivity.class));				
			}
		});
	}
	
	private void initGoodsListView() {
		mGoodsListView = (ListView)findViewById(R.id.GoodsListView);
		List<GoodsItemData> goodsListData = getData();
		mGoodsListView.setAdapter(new GoodsListAdspter(this, goodsListData));
	}
	
	private List<GoodsItemData> getData() {
		List<GoodsItemData> list = new ArrayList<GoodsItemData>();
		
		String leftName = "left_goods++++++++统一冰红茶";
		String rightName = "right_goods++++++++统一冰红茶";
		list.add(new GoodsItemData(new GoodsInfo(0, R.drawable.bskltest, leftName, "¥ 4.80", 0), new GoodsInfo(0, R.drawable.bskl300ml, rightName, "¥ 6.50", 0)));
		list.add(new GoodsItemData(new GoodsInfo(0, R.drawable.bskl600ml, leftName, "¥ 5.80", 0), new GoodsInfo(0, R.drawable.bskl300ml, rightName, "¥ 1.80", 0)));
		list.add(new GoodsItemData(new GoodsInfo(0, R.drawable.bskl300ml, rightName, "¥ 1.80", 0), null));
		return list;
	}
	
	private void initView() {
		sHomeActivity = this;
		initTitle();
		initGoodsListView();
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.home_activity);
		initView();
	}

}
