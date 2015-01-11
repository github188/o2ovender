package com.NC.o2ovender.Data;

import java.io.Serializable;

public class GoodsInfo implements Serializable {
	private int mGoodsID = 0;
	private int mGoodsIcon = 0;
	private String mGoodsName = null;
	private String mGoodsPrice = null;
	private double mGoodsOff = 0;
	public GoodsInfo(int mGoodsID, int mGoodsIcon, String mGoodsName,
			String mGoodsPrice, double mGoodsOff) {
		super();
		this.mGoodsID = mGoodsID;
		this.mGoodsIcon = mGoodsIcon;
		this.mGoodsName = mGoodsName;
		this.mGoodsPrice = mGoodsPrice;
		this.mGoodsOff = mGoodsOff;
	}
	public int getGoodsID() {
		return mGoodsID;
	}
	public int getGoodsIcon() {
		return mGoodsIcon;
	}
	public String getGoodsName() {
		return mGoodsName;
	}
	public String getGoodsPrice() {
		return mGoodsPrice;
	}
	public double getGoodsOff() {
		return mGoodsOff;
	}	
}
