package com.NC.o2ovender.Activity;

import java.util.List;

import android.content.Context;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.NC.o2ovender.R;
import com.NC.o2ovender.Activity.HomeActivity.GoodsItemData;
import com.NC.o2ovender.Data.GoodsInfo;

public class GoodsListAdspter extends BaseAdapter {
	public static final String TAG = "GoodsListAdspter";
	public static final String GOODINFO_TAG = "goodsinfo_tag";
	
	private List<GoodsItemData> mGoodsList = null;
	private LayoutInflater mLayoutInflater = null;
	private Context mContext = null;

	public GoodsListAdspter(Context context, List<GoodsItemData> goodsList) {
		super();
		this.mContext = context;
		this.mGoodsList = goodsList;
		this.mLayoutInflater = LayoutInflater.from(context);
	}
	
	public final class GoodsListItem {
		public ImageView mLeftGoodsImgView;
		public TextView mLeftGoodsName;
		public TextView mLeftGoodsPrice;
		public ImageView mRightGoodsImgView;
		public TextView mRightGoodsName;
		public TextView mRightGoodsPrice;
	}

	@Override
	public int getCount() {
		return mGoodsList.size();
	}

	@Override
	public Object getItem(int position) {
		return mGoodsList.get(position);
	}

	@Override
	public long getItemId(int position) {
		return position;
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		
		GoodsListItem goodsListItem = null;
		GoodsItemData goodsItemData = mGoodsList.get(position);
		GoodsInfo leftGoodsItemData = goodsItemData.getLeftData();
		GoodsInfo rightGoodsItemData = goodsItemData.getRightData();
		if (null == convertView) {
			goodsListItem = new GoodsListItem();
			convertView = mLayoutInflater.inflate(R.layout.goods_list, null);
			if (null != leftGoodsItemData) {
				goodsListItem.mLeftGoodsImgView = (ImageView)convertView.findViewById(R.id.goodIcon1);
				goodsListItem.mLeftGoodsName = (TextView)convertView.findViewById(R.id.goodName1);
				goodsListItem.mLeftGoodsPrice = (TextView)convertView.findViewById(R.id.goodPrice1);
			}
			if (null != rightGoodsItemData) {
				goodsListItem.mRightGoodsImgView = (ImageView)convertView.findViewById(R.id.goodIcon2);
				goodsListItem.mRightGoodsName = (TextView)convertView.findViewById(R.id.goodName2);
				goodsListItem.mRightGoodsPrice = (TextView)convertView.findViewById(R.id.goodPrice2);
			}
			convertView.setTag(goodsListItem);
		} else {
			goodsListItem = (GoodsListItem)convertView.getTag();
		}
				
		if (null != leftGoodsItemData) {
			goodsListItem.mLeftGoodsImgView.setImageResource(leftGoodsItemData.getGoodsIcon());
			goodsListItem.mLeftGoodsName.setText((String)leftGoodsItemData.getGoodsName());
			goodsListItem.mLeftGoodsPrice.setText((String)leftGoodsItemData.getGoodsPrice());
			goodsListItem.mLeftGoodsImgView.setOnClickListener(new ItemClickListener(leftGoodsItemData));
			goodsListItem.mLeftGoodsPrice.setOnClickListener(new ItemClickListener(leftGoodsItemData));
			goodsListItem.mLeftGoodsName.setOnClickListener(new ItemClickListener(leftGoodsItemData));
		} else {
			RelativeLayout leftLayout = (RelativeLayout)convertView.findViewById(R.id.goodsUnit1);
			leftLayout.setBackgroundColor(Color.TRANSPARENT);
		}
		
		if (null != rightGoodsItemData) {
			goodsListItem.mRightGoodsImgView.setImageResource(rightGoodsItemData.getGoodsIcon());
			goodsListItem.mRightGoodsName.setText((String)rightGoodsItemData.getGoodsName());
			goodsListItem.mRightGoodsPrice.setText((String)rightGoodsItemData.getGoodsPrice());
			goodsListItem.mRightGoodsImgView.setOnClickListener(new ItemClickListener(rightGoodsItemData));
			goodsListItem.mRightGoodsPrice.setOnClickListener(new ItemClickListener(rightGoodsItemData));
			goodsListItem.mRightGoodsName.setOnClickListener(new ItemClickListener(rightGoodsItemData));
		} else {
			RelativeLayout rightLayout = (RelativeLayout)convertView.findViewById(R.id.goodsUnit2);
			rightLayout.setBackgroundColor(Color.TRANSPARENT);
		}
				
		return convertView;
	}
	
	class ItemClickListener implements OnClickListener {
		
		private GoodsInfo mGoodsInfo = null;
		
		public ItemClickListener(GoodsInfo goodsInfo) {
			this.mGoodsInfo = goodsInfo;
		}

		@Override
		public void onClick(View v) {
			Intent intent = new Intent(HomeActivity.sHomeActivity, BuyActivity.class);
			Bundle bundle = new Bundle();
			bundle.putSerializable(GOODINFO_TAG, mGoodsInfo);
			intent.putExtras(bundle);
			HomeActivity.sHomeActivity.startActivity(intent);
		}		
	}

}
