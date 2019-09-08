#ifndef _AUDIO_SCHEDULED_SOURCE_NODE_HPP_
#define _AUDIO_SCHEDULED_SOURCE_NODE_HPP_

#include "common.hpp"


class AudioScheduledSourceNode : public CommonNode {
DECLARE_ES5_CLASS(AudioScheduledSourceNode, AudioScheduledSourceNode);
	
public:
	static void init(Napi::Env env, Napi::Object exports);
	
	explicit AudioScheduledSourceNode(const Napi::CallbackInfo &info);
	~AudioScheduledSourceNode();
	
	void _destroy();
	
private:
	void onEnded();
	
	JS_DECLARE_METHOD(AudioScheduledSourceNode, destroy);
	
	JS_DECLARE_METHOD(AudioScheduledSourceNode, start);
	JS_DECLARE_METHOD(AudioScheduledSourceNode, stop);
	
};


#endif // _AUDIO_SCHEDULED_SOURCE_NODE_HPP_
