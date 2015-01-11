package com.NC.o2ovender.Activity;

import java.util.List;
import java.util.Map;

import android.R.string;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import com.NC.o2ovender.R;
import com.NC.o2ovender.Data.GoodsInfo;

public class GoodsListAdspter extends BaseAdapter {
	public static final String TAG = "GoodsListAdspter";
//	public static final String LEFT_PICID = "left_PicID";
//	public static final String LEFT_NAME = "left_Name";
//	public static final String LEFT_PRICE = "left_Prices";
//	public static final String RIGHT_PICID = "right_PicID";
//	public static final String RIGHT_NAME = "right_Name";
//	public static final String RIGHT_PRICE = "right_Prices";
	public static final String GOODINFO_TAG = "goodsinfo_tag";
	
//	private List<Map<String, Object>> mGoodsList = null;
	private List<GoodsInfo> mGoodsList = null;
	private LayoutInflater mLayoutInflater = null;
	private Context mContext = null;

	public GoodsListAdspter(Context context, List<GoodsInfo> goodsList) {
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
		int iLength = mGoodsList.size();		
		int iLeftIndex = position*2;
		int iRightIndex = iLeftIndex+1;
		
		GoodsListItem goodsListItem = null;
		if (null == convertView) {
			goodsListItem = new GoodsListItem();
			convertView = mLayoutInflater.inflate(R.layout.goods_list, null);
			
//			goodsListItem.mLeftGoodsImgView = (ImageView)convertView.findViewById(R.id.goodIcon1);
//			goodsListItem.mLeftGoodsName = (TextView)convertView.findViewById(R.id.goodName1);
//			goodsListItem.mLeftGoodsPrice = (TextView)convertView.findViewById(R.id.goodPrice1);
//			goodsListItem.mRightGoodsImgView = (ImageView)convertView.findViewById(R.id.goodIcon2);
//			goodsListItem.mRightGoodsName = (TextView)convertView.findViewById(R.id.goodName2);
//			goodsListItem.mRightGoodsPrice = (TextView)convertView.findViewById(R.id.goodPrice2);
			if (iLeftIndex < iLength) {
				goodsListItem.mLeftGoodsImgView = (ImageView)convertView.findViewById(R.id.goodIcon1);
				goodsListItem.mLeftGoodsName = (TextView)convertView.findViewById(R.id.goodName1);
				goodsListItem.mLeftGoodsPrice = (TextView)convertView.findViewById(R.id.goodPrice1);
			}
			
			if (iRightIndex < iLength) {
				goodsListItem.mRightGoodsImgView = (ImageView)convertView.findViewById(R.id.goodIcon2);
				goodsListItem.mRightGoodsName = (TextView)convertView.findViewById(R.id.goodName2);
				goodsListItem.mRightGoodsPrice = (TextView)convertView.findViewById(R.id.goodPrice2);
			}
			
			convertView.setTag(goodsListItem);
		} else {
			goodsListItem = (GoodsListItem)convertView.getTag();
		}
		
//		goodsListItem.mLeftGoodsImgView.setBackgroundResource((Integer)mGoodsList.get(position).get(LEFT_PICID));
//		goodsListItem.mLeftGoodsName.setText((String)mGoodsList.get(position).get(LEFT_NAME));
//		goodsListItem.mLeftGoodsPrice.setText((String)mGoodsList.get(position).get(LEFT_PRICE));
//		goodsListItem.mRightGoodsImgView.setBackgroundResource((Integer)mGoodsList.get(position).get(RIGHT_PICID));
//		goodsListItem.mRightGoodsName.setText((String)mGoodsList.get(position).get(RIGHT_NAME));
//		goodsListItem.mRightGoodsPrice.setText((String)mGoodsList.get(position).get(RIGHT_PRICE));
		
//		goodsListItem.mLeftGoodsImgView.setOnClickListener(new ItemClickListener(position, true));
//		goodsListItem.mLeftGoodsPrice.setOnClickListener(new ItemClickListener(position, true));
//		goodsListItem.mLeftGoodsName.setOnClickListener(new ItemClickListener(position, true));
//		goodsListItem.mRightGoodsImgView.setOnClickListener(new ItemClickListener(position, false));
//		goodsListItem.mRightGoodsPrice.setOnClickListener(new ItemClickListener(position, false));
//		goodsListItem.mRightGoodsName.setOnClickListener(new ItemClickListener(position, false));
		
		if (iLeftIndex < iLength) {
			goodsListItem.mLeftGoodsImgView.setImageResource(mGoodsList.get(iLeftIndex).getGoodsIcon());
			goodsListItem.mLeftGoodsName.setText((String)mGoodsList.get(iLeftIndex).getGoodsName());
			goodsListItem.mLeftGoodsPrice.setText((String)mGoodsList.get(iLeftIndex).getGoodsPrice());
			goodsListItem.mLeftGoodsImgView.setOnClickListener(new ItemClickListener(mGoodsList.get(iLeftIndex)));
			goodsListItem.mLeftGoodsPrice.setOnClickListener(new ItemClickListener(mGoodsList.get(iLeftIndex)));
			goodsListItem.mLeftGoodsName.setOnClickListener(new ItemClickListener(mGoodsList.get(iLeftIndex)));
		}
		
		if (iRightIndex < iLength) {
			goodsListItem.mLeftGoodsImgView.setImageResource(mGoodsList.get(iRightIndex).getGoodsIcon());
			goodsListItem.mLeftGoodsName.setText((String)mGoodsList.get(iRightIndex).getGoodsName());
			goodsListItem.mLeftGoodsPrice.setText((String)mGoodsList.get(iRightIndex).getGoodsPrice());
			goodsListItem.mLeftGoodsImgView.setOnClickListener(new ItemClickListener(mGoodsList.get(iRightIndex)));
			goodsListItem.mLeftGoodsPrice.setOnClickListener(new ItemClickListener(mGoodsList.get(iRightIndex)));
			goodsListItem.mLeftGoodsName.setOnClickListener(new ItemClickListener(mGoodsList.get(iRightIndex)));
		}
				
		return convertView;
	}
	
	class ItemClickListener implements OnClickListener {
		
//		private int mPostion = -1;
//		private boolean mLeft = true;
		
//		public ItemClickListener(int iPostion, boolean bLeft) {
//			super();
//			this.mPostion = iPostion;
//			this.mLeft = bLeft;
//		}
		
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
