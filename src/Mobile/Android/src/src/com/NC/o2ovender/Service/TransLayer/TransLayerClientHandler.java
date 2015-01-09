package com.NC.o2ovender.Service.TransLayer;

import org.jboss.netty.buffer.ChannelBuffer;
import org.jboss.netty.channel.Channel;
import org.jboss.netty.channel.ChannelHandlerContext;
import org.jboss.netty.channel.ChannelStateEvent;
import org.jboss.netty.channel.ExceptionEvent;
import org.jboss.netty.channel.MessageEvent;
import org.jboss.netty.channel.SimpleChannelUpstreamHandler;
import org.jboss.netty.handler.codec.http.HttpResponse;

import android.util.Log;

import com.NC.o2ovender.Service.ServiceMgrImp;
import com.NC.o2ovender.Service.Command.Mobile2Service.response;

public class TransLayerClientHandler extends SimpleChannelUpstreamHandler {
	private static final String TAG = "TransLayerClientHandler";
	private volatile Channel mChannel = null;

	@Override
	public void channelConnected(ChannelHandlerContext ctx, ChannelStateEvent e)
			throws Exception {
		Log.i(TAG, "channelConnected");
		super.channelConnected(ctx, e);
		ServiceMgrImp.GetServiceMgrImp().connected();
	}

	@Override
	public void channelDisconnected(ChannelHandlerContext ctx,
			ChannelStateEvent e) throws Exception {
		Log.i(TAG, "channelDisconnected");
		super.channelDisconnected(ctx, e);
		ServiceMgrImp.GetServiceMgrImp().disconnected();
	}

	@Override
	public void channelOpen(ChannelHandlerContext ctx, ChannelStateEvent e)
			throws Exception {		
		mChannel = e.getChannel();
		Log.i(TAG, "channelOpen mChannel =" + mChannel);
		super.channelOpen(ctx, e);	}

	@Override
	public void exceptionCaught(ChannelHandlerContext arg0, ExceptionEvent arg1)
			throws Exception {
		Log.e(TAG, "exceptionCaught:" + arg1.getCause());
		super.exceptionCaught(arg0, arg1);
		arg1.getChannel().close();
	}

	@Override
	public void messageReceived(ChannelHandlerContext ctx, MessageEvent e)
			throws Exception {
		HttpResponse httpResponse = (HttpResponse)e.getMessage();
		
		ChannelBuffer contentBuffer = httpResponse.getContent();
		byte[] bytes = contentBuffer.array();
		
		response responseCmd = response.parseFrom(bytes);
		ServiceMgrImp.GetServiceMgrImp().recv(responseCmd);
	}	
}
