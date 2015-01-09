package com.NC.o2ovender.Service.TransLayer;

import java.net.InetSocketAddress;
import java.net.URI;
import java.net.URISyntaxException;
import java.util.Map;
import java.util.concurrent.Executors;

import org.jboss.netty.bootstrap.ClientBootstrap;
import org.jboss.netty.buffer.ChannelBuffer;
import org.jboss.netty.buffer.ChannelBuffers;
import org.jboss.netty.channel.Channel;
import org.jboss.netty.channel.ChannelFuture;
import org.jboss.netty.channel.ChannelFutureListener;
import org.jboss.netty.channel.ChannelPipeline;
import org.jboss.netty.channel.ChannelPipelineFactory;
import org.jboss.netty.channel.group.ChannelGroup;
import org.jboss.netty.channel.group.DefaultChannelGroup;
import org.jboss.netty.channel.socket.nio.NioClientSocketChannelFactory;
import org.jboss.netty.handler.codec.http.CookieEncoder;
import org.jboss.netty.handler.codec.http.DefaultHttpRequest;
import org.jboss.netty.handler.codec.http.HttpHeaders;
import org.jboss.netty.handler.codec.http.HttpMethod;
import org.jboss.netty.handler.codec.http.HttpRequest;
import org.jboss.netty.handler.codec.http.HttpVersion;

import com.NC.o2ovender.Service.Command.Mobile2Service.request;

public class TransLayerImp {
	private static final String HTTP_SCHEME = "http";
	private static final String LOCAL_HOST = "localhost";
	private static final String HTTP_PROT_GET = "GET";
	private static final String HTTP_PROT_DELETE = "DELETE";
	private static final String HTTP_PROT_POST = "POST";
	private static final String SERVICE_IP = "http://58.61.39.245:80";
	private ClientBootstrap mClientBootstrap = null;
	private ChannelGroup mAllChannels = null;
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
		mAllChannels = new DefaultChannelGroup();
		
//		ChannelFuture channelFuture = mClientBootstrap.connect(new InetSocketAddress("58.61.39.245", 80));
//		mChannel = channelFuture.awaitUninterruptibly().getChannel();
//		if ((null != mChannel) && (null != mChannelPipelineFactory) && (null != mClientBootstrap)) {
//			mInit = true;
//		}
		
//		URI uri = new URI("http://58.61.39.245:80");
//		String scheme = uri.getScheme() == null ? HTTP_SCHEME : uri.getScheme();	
//		
//		HttpRequest request = new DefaultHttpRequest(
//				HttpVersion.HTTP_1_1, 
//				HttpMethod.valueOf(HTTP_PROT_POST), 
//				uri.toASCIIString());
//		
//		String host = uri.getHost() == null ? LOCAL_HOST : uri.getHost();
//		int port = uri.getPort() == -1? 80 : uri.getPort();
//		
//		request.headers().set(HttpHeaders.Names.HOST, host);
//		request.headers().set(HttpHeaders.Names.CONNECTION, HttpHeaders.Values.KEEP_ALIVE);	
//		ChannelFuture channelFuture = mClientBootstrap.connect(new InetSocketAddress(request.headers().get(HttpHeaders.Names.HOST) , port));
//		channelFuture.addListener(new ChannelFutureListener(){
//
//			@Override
//			public void operationComplete(ChannelFuture arg0) throws Exception {
//				if (!arg0.isSuccess()) {                                                  
//					arg0.getCause().printStackTrace();                                 
//		            return;                            
//		        }
//		        mChannel = arg0.getChannel();   
//			}
//			
//		});
//        mAllChannels.add(channelFuture.getChannel());
        
//		mChannel = channelFuture.awaitUninterruptibly().getChannel();
//		if ((null != mChannel) && (null != mChannelPipelineFactory) && (null != mClientBootstrap)) {
//			mInit = true;
//		}
		
//		return mInit;
		
		return true;
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
	
	public void sendRequest(request requestCmd) throws Exception {
		byte[] requestByte = requestCmd.toByteArray();
		ChannelBuffer channelBuffer = ChannelBuffers.buffer(requestByte.length);
		channelBuffer.writeBytes(requestByte);
//		if (null != mChannel) {
//			mChannel.write(channelBuffer);
//		}
		post(channelBuffer);
	}
	
	public void setOption(String key, Object value) {
		mClientBootstrap.setOption(key, value);
	}
	
	public ChannelPipeline get() throws Exception{
		return retrieve(HTTP_PROT_GET, SERVICE_IP);
	}
    	
    private ChannelPipeline delete() throws Exception{
    	return retrieve(HTTP_PROT_DELETE, SERVICE_IP);
    }
    
    private ChannelPipeline post(ChannelBuffer data) throws Exception{
    	return retrieve(HTTP_PROT_POST, SERVICE_IP, data);
    }	
    	
    private ChannelPipeline retrieve(String method, String url) throws Exception{
    	return retrieve(method, url, null, null);
    }
    	
   	private ChannelPipeline retrieve(String method, String url, ChannelBuffer data) throws Exception{
   		return retrieve(method, url, data, null);
    }
	
	private ChannelPipeline retrieve(String method, String url, ChannelBuffer data, Map<String, String> cookie) throws Exception {
		if (null == url) {
			throw new Exception("url is null");
		}
		URI uri = new URI(url);
		String scheme = uri.getScheme() == null ? HTTP_SCHEME : uri.getScheme();
		
		if (!scheme.equals(HTTP_SCHEME)) {
			throw new Exception("just support http protocol");
		}
		
		HttpRequest request = new DefaultHttpRequest(
				HttpVersion.HTTP_1_1, 
				HttpMethod.valueOf(method), 
				uri.toASCIIString());
		
		String host = uri.getHost() == null ? LOCAL_HOST : uri.getHost();
		request.headers().set(HttpHeaders.Names.HOST, host);
		request.headers().set(HttpHeaders.Names.CONNECTION, HttpHeaders.Values.KEEP_ALIVE);
		
        if(cookie != null){
        	CookieEncoder httpCookieEncoder = new CookieEncoder(false);  
	        for (Map.Entry<String, String> m : cookie.entrySet()) {
	        	httpCookieEncoder.addCookie(m.getKey(), m.getValue());
	        	request.headers().set(HttpHeaders.Names.COOKIE, httpCookieEncoder.encode());    
	        }
        }
        
        if (null != mChannel) {
        	request.setContent(data);
        	mChannel.write(request);
        	return mChannel.getPipeline();
        } else {
        	return retrieve(request);
        }
        
	}
	
	class ConnectOk implements ChannelFutureListener {
		private HttpRequest request=null;
		
		ConnectOk(HttpRequest req){
			this.request = req;
		}
		                                              
		public void operationComplete(ChannelFuture future){
	        if (!future.isSuccess()) {                                                  
	            future.getCause().printStackTrace();                                 
	            return;                                                                 
	        }                                                                   
	        mChannel = future.getChannel();                                                                            
	        mChannel.write(request);
	    }
	}
	
	public ChannelPipeline retrieve(HttpRequest request)throws Exception{
   		URI uri = new URI(request.getUri());          
        int port = uri.getPort() == -1? 80 : uri.getPort(); 
   		ChannelFuture future = mClientBootstrap.connect(new InetSocketAddress(request.headers().get(HttpHeaders.Names.HOST) , port));
        future.addListener(new ConnectOk(request));
        mAllChannels.add(future.getChannel());
        return   future.getChannel().getPipeline();
   	} 
}
