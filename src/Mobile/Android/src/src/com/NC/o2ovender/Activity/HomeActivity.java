package com.NC.o2ovender.Activity;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.ImageView;
import android.widget.ListView;

import com.NC.o2ovender.R;

public class HomeActivity extends Activity {
	
	private ImageView mLocation = null;
	private ImageView mUserInfo = null;
	private ListView mGoodsListView = null;

	private void initTitle() {
		mLocation = (ImageView)findViewById(R.id.location);
		mLocation.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				
			}
		});
		
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
		List<Map<String, Object>> goodsListData = getData();
		mGoodsListView.setAdapter(new GoodsListAdspter(this, goodsListData));
	}
	
	private List<Map<String, Object>> getData() {
	List<Map<String, Object>> list = new ArrayList<Map<String, Object>>();
	
	Map<String, Object> map = new HashMap<String, Object>();
	map.put(GoodsListAdspter.LEFT_PICID, R.drawable.icon_test);
	map.put(GoodsListAdspter.LEFT_NAME, "left_goods++++++++统一冰红茶");
	map.put(GoodsListAdspter.LEFT_PRICE, "¥ 2.80");
	map.put(GoodsListAdspter.RIGHT_PICID, R.drawable.icon_test);
	map.put(GoodsListAdspter.RIGHT_NAME, "right_goods++++++++统一冰红茶");
	map.put(GoodsListAdspter.RIGHT_PRICE, "¥ 6.50");
	
	list.add(map);
	
	map = new HashMap<String, Object>();
	map.put(GoodsListAdspter.LEFT_PICID, R.drawable.icon_test);
	map.put(GoodsListAdspter.LEFT_NAME, "left_goods++++++++统一冰红茶");
	map.put(GoodsListAdspter.LEFT_PRICE, "¥ 3.80");
	map.put(GoodsListAdspter.RIGHT_PICID, R.drawable.icon_test);
	map.put(GoodsListAdspter.RIGHT_NAME, "right_goods++++++++统一冰红茶");
	map.put(GoodsListAdspter.RIGHT_PRICE, "¥ 7.50");
	
	list.add(map);
	
	map = new HashMap<String, Object>();
	map.put(GoodsListAdspter.LEFT_PICID, R.drawable.icon_test);
	map.put(GoodsListAdspter.LEFT_NAME, "left_goods++++++++统一冰红茶");
	map.put(GoodsListAdspter.LEFT_PRICE, "¥ 3.80");
	map.put(GoodsListAdspter.RIGHT_PICID, R.drawable.icon_test);
	map.put(GoodsListAdspter.RIGHT_NAME, "right_goods++++++++统一冰红茶");
	map.put(GoodsListAdspter.RIGHT_PRICE, "¥ 7.50");
	
	list.add(map);
	
	map = new HashMap<String, Object>();
	map.put(GoodsListAdspter.LEFT_PICID, R.drawable.icon_test);
	map.put(GoodsListAdspter.LEFT_NAME, "left_goods++++++++统一冰红茶");
	map.put(GoodsListAdspter.LEFT_PRICE, "¥ 3.80");
	map.put(GoodsListAdspter.RIGHT_PICID, R.drawable.icon_test);
	map.put(GoodsListAdspter.RIGHT_NAME, "right_goods++++++++统一冰红茶");
	map.put(GoodsListAdspter.RIGHT_PRICE, "¥ 7.50");
	
	list.add(map);
	
	map = new HashMap<String, Object>();
	map.put(GoodsListAdspter.LEFT_PICID, R.drawable.icon_test);
	map.put(GoodsListAdspter.LEFT_NAME, "left_goods");
	map.put(GoodsListAdspter.LEFT_PRICE, "¥ 3.80");
	map.put(GoodsListAdspter.RIGHT_PICID, R.drawable.icon_test);
	map.put(GoodsListAdspter.RIGHT_NAME, "right_goods");
	map.put(GoodsListAdspter.RIGHT_PRICE, "¥ 7.50");
	
	list.add(map);
	
	map = new HashMap<String, Object>();
	map.put(GoodsListAdspter.LEFT_PICID, R.drawable.icon_test);
	map.put(GoodsListAdspter.LEFT_NAME, "left_goods");
	map.put(GoodsListAdspter.LEFT_PRICE, "¥ 3.80");
	map.put(GoodsListAdspter.RIGHT_PICID, R.drawable.icon_test);
	map.put(GoodsListAdspter.RIGHT_NAME, "right_goods");
	map.put(GoodsListAdspter.RIGHT_PRICE, "¥ 7.50");
	
	list.add(map);
	
	return list;
}
	
	private void initView() {
		initTitle();
		initGoodsListView();
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.home_activity);
		initView();
	}

}
