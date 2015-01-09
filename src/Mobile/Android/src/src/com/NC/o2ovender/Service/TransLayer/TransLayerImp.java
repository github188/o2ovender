package com.NC.o2ovender.Service.TransLayer;

import java.net.InetSocketAddress;
import java.util.concurrent.Executors;

import org.jboss.netty.bootstrap.ClientBootstrap;
import org.jboss.netty.channel.Channel;
import org.jboss.netty.channel.ChannelFuture;
import org.jboss.netty.channel.ChannelPipelineFactory;
import org.jboss.netty.channel.socket.nio.NioClientSocketChannelFactory;

import com.NC.o2ovender.Service.Command.Mobile2Service.request;

public class TransLayerImp {
	private ClientBootstrap mClientBootstrap = null;
	private Channel mChannel = null;
	private ChannelPipelineFactory mChannelPipelineFactory = null;
	private boolean mInit = false;
	public boolean init() {
		mClientBootstrap = new ClientBootstrap(
				new NioClientSocketChannelFactory(
						Executors.newCachedThreadPool(),
						Executors.newCachedThreadPool()));
		
		mChannelPipelineFactory = new TransLayerPipelineFactory();
		mClientBootstrap.setPipelineFactory(mChannelPipelineFactory);
		
		ChannelFuture channelFuture = mClientBootstrap.connect(new InetSocketAddress("58.61.39.245", 80));
		mChannel = channelFuture.awaitUninterruptibly().getChannel();
		if ((null != mChannel) && (null != mChannelPipelineFactory) && (null != mClientBootstrap)) {
			mInit = true;
		}
		return mInit;
	}
	
	public void unInit() {
		if (null != mChannel) {
			mChannel.close().awaitUninterruptibly();
			mChannel = null;
		}
		if (null != mClientBootstrap) {
			mClientBootstrap.releaseExternalResources();
			mClientBootstrap = null;
		}
		mInit = false;
	}
	
	public void sendRequest(request requestCmd) {
		if (null != mChannel) {
			mChannel.write(requestCmd);
		}
	}
}
