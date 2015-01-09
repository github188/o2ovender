package com.NC.o2ovender.Service.TransLayer;

import org.jboss.netty.channel.ChannelPipeline;
import org.jboss.netty.channel.ChannelPipelineFactory;
import org.jboss.netty.channel.Channels;
import org.jboss.netty.handler.codec.http.HttpRequestEncoder;
import org.jboss.netty.handler.codec.http.HttpResponseDecoder;
import org.jboss.netty.handler.codec.protobuf.ProtobufDecoder;
import org.jboss.netty.handler.codec.protobuf.ProtobufEncoder;

import com.NC.o2ovender.Service.Command.Mobile2Service.response;

public class TransLayerPipelineFactory implements ChannelPipelineFactory {
	private static final String HTTP_REQUEST_ENCODE = "httpRequestEncode";
	private static final String HTTP_RESPONSE_DECODE = "httpResponseDecode";
	private static final String PROTOBUF_ENCODE = "protobufEncode";
	private static final String PROTOBUF_DECODE = "protobufDecode";
	private static final String CLIENT_HANDLE = "clientHandler";
	
	@Override
	public ChannelPipeline getPipeline() throws Exception {
		ChannelPipeline pipeline = Channels.pipeline();
		pipeline.addLast(HTTP_RESPONSE_DECODE, new HttpResponseDecoder());
//		pipeline.addLast(PROTOBUF_DECODE, new ProtobufDecoder(response.getDefaultInstance()));
		pipeline.addLast(HTTP_REQUEST_ENCODE, new HttpRequestEncoder());
//		pipeline.addLast(PROTOBUF_ENCODE, new ProtobufEncoder());
		pipeline.addLast(CLIENT_HANDLE, new TransLayerClientHandler());
		return pipeline;
	}
}
