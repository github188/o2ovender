package com.NC.o2ovender.Data;

public class GoodsInfo {
	private int mGoodsID = 0;
	private String mGoodsIcon = null;
	private String mGoodsName = null;
	private double mGoodsPrice = 2;
	private double mGoodsOff = 0;
	public GoodsInfo(int mGoodsID, String mGoodsIcon, String mGoodsName,
			double mGoodsPrice, double mGoodsOff) {
		super();
		this.mGoodsID = mGoodsID;
		this.mGoodsIcon = mGoodsIcon;
		this.mGoodsName = mGoodsName;
		this.mGoodsPrice = mGoodsPrice;
		this.mGoodsOff = mGoodsOff;
	}	
}
