package com.NC.o2ovender.Activity;

import java.util.List;
import java.util.Map;

import com.NC.o2ovender.R;
import com.NC.o2ovender.Data.GoodsInfo;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

public class GoodsListAdspter extends BaseAdapter {
	public static final String LEFT_PICID = "left_PicID";
	public static final String LEFT_NAME = "left_Name";
	public static final String LEFT_PRICE = "left_Prices";
	public static final String RIGHT_PICID = "right_PicID";
	public static final String RIGHT_NAME = "right_Name";
	public static final String RIGHT_PRICE = "right_Prices";
	
	private List<Map<String, Object>> mGoodsList = null;
	private LayoutInflater mLayoutInflater = null;
	private Context mContext = null;

	public GoodsListAdspter(Context context, List<Map<String, Object>> goodsList) {
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
		if (null == goodsListItem) {
			goodsListItem = new GoodsListItem();
			convertView = mLayoutInflater.inflate(R.layout.goods_list, null);
			goodsListItem.mLeftGoodsImgView = (ImageView)convertView.findViewById(R.id.goodIcon1);
			goodsListItem.mLeftGoodsName = (TextView)convertView.findViewById(R.id.goodName1);
			goodsListItem.mLeftGoodsPrice = (TextView)convertView.findViewById(R.id.goodPrice1);
			goodsListItem.mRightGoodsImgView = (ImageView)convertView.findViewById(R.id.goodIcon2);
			goodsListItem.mRightGoodsName = (TextView)convertView.findViewById(R.id.goodName2);
			goodsListItem.mRightGoodsPrice = (TextView)convertView.findViewById(R.id.goodPrice2);
			convertView.setTag(goodsListItem);
		} else {
			goodsListItem = (GoodsListItem)convertView.getTag();
		}
		
		goodsListItem.mLeftGoodsImgView.setBackgroundResource((Integer)mGoodsList.get(position).get(LEFT_PICID));
		goodsListItem.mLeftGoodsName.setText((String)mGoodsList.get(position).get(LEFT_NAME));
		goodsListItem.mLeftGoodsPrice.setText((String)mGoodsList.get(position).get(LEFT_PRICE));
		goodsListItem.mRightGoodsImgView.setBackgroundResource((Integer)mGoodsList.get(position).get(RIGHT_PICID));
		goodsListItem.mRightGoodsName.setText((String)mGoodsList.get(position).get(RIGHT_NAME));
		goodsListItem.mRightGoodsPrice.setText((String)mGoodsList.get(position).get(RIGHT_PRICE));
		return convertView;
	}

}
