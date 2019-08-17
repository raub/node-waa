#ifndef _GAIN_NODE_HPP_
#define _GAIN_NODE_HPP_

#include "common.hpp"

#include "audio-node.hpp"


class GainNode : public Napi::ObjectWrap<GainNode>, private CommonNode {
	
public:
	
	static void init(Napi::Env env, Napi::Object exports);
	static GainNode *create();
	
	explicit GainNode(const Napi::CallbackInfo &info);
	~GainNode();
	
	void _destroy();
	
private:
	
	static Napi::FunctionReference _constructor;
	
	Napi::ObjectReference _gain;
	
	JS_GETTER(gainGetter);
	
};


#endif // _GAIN_NODE_HPP_
