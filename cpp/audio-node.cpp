#include <cstdlib>
//#include <iostream> // -> std::cout << "..." << std::endl;


#include <LabSound/core/AudioContext.h>
#include <LabSound/core/AudioNode.h>

#include "audio-node.hpp"


using namespace v8;
using namespace node;
using namespace std;


// ------ Aux macros

#define THIS_AUDIO_NODE                                                    \
	AudioNode *audioNode = ObjectWrap::Unwrap<AudioNode>(info.This());

#define THIS_CHECK                                                            \
	if (audioNode->_isDestroyed) return;

#define CACHE_CAS(CACHE, V)                                                   \
	if (audioNode->CACHE == V) {                                           \
		return;                                                               \
	}                                                                         \
	audioNode->CACHE = V;


inline std::string fromChannelCountMode(ChannelCountMode mode) {
	if (mode == ClampedMax) {
		return "clamped-max";
	} else if (mode == Explicit) {
		return "explicit";
	} else {
		return "max";
	}
}

inline std::string toChannelCountMode(const std::string &mode) {
	if (mode == "clamped-max") {
		return ClampedMax;
	} else if (mode == "explicit") {
		return Explicit;
	} else {
		return Max;
	}
}

inline std::string fromChannelInterpretation(ChannelInterpretation io) {
	if (io == Discrete) {
		return "discrete";
	} else {
		return "speakers";
	}
}

inline std::string toChannelInterpretation(const std::string &io) {
	if (io == "discrete") {
		return Discrete;
	} else {
		return Speakers;
	}
}


// ------ Constructor and Destructor

AudioNode::AudioNode(<v8::Object> context, lab::AudioNode *node) : EventEmitter() {
	
	_isDestroyed = false;
	
	_context.reset(context);
	_impl.reset(node);
	
	_channelCount = node.channelCount()
	_channelCountMode = node.channelCountMode()
	_channelInterpretation = node.channelInterpretation()
	
}


AudioNode::~AudioNode() {
	
	_destroy();
	
}


void AudioNode::_destroy() { DES_CHECK;
	
	_isDestroyed = true;
	
	EventEmitter::_destroy();
	
}


// ------ Methods and props


NAN_METHOD(AudioNode::connect) { THIS_AUDIO_NODE; THIS_CHECK;
	
	REQ_OBJ_ARG(0, destination);
	REQ_INT32_ARG(1, output);
	REQ_INT32_ARG(2, input);
	
	// TODO: do something?
	
}


NAN_METHOD(AudioNode::disconnect) { THIS_AUDIO_NODE; THIS_CHECK;
	
	REQ_OBJ_ARG(0, destination);
	REQ_INT32_ARG(1, output);
	REQ_INT32_ARG(2, input);
	
	// TODO: do something?
	
}


NAN_GETTER(AudioNode::contextGetter) { THIS_AUDIO_NODE; THIS_CHECK;
	
	RET_VALUE(JS_OBJ(audioNode->_context));
	
}


NAN_GETTER(AudioNode::numberOfInputsGetter) { THIS_AUDIO_NODE; THIS_CHECK;
	
	RET_VALUE(JS_UINT32(audioNode->_impl->numberOfInputs()));
	
}


NAN_GETTER(AudioNode::numberOfOutputsGetter) { THIS_AUDIO_NODE; THIS_CHECK;
	
	RET_VALUE(JS_UINT32(audioNode->_impl->numberOfOutputs()));
	
}


NAN_GETTER(AudioNode::channelCountGetter) { THIS_AUDIO_NODE; THIS_CHECK;
	
	RET_VALUE(JS_UINT32(audioNode->_impl->channelCount()));
	
}

NAN_SETTER(AudioNode::channelCountSetter) { THIS_AUDIO_NODE; THIS_CHECK; SETTER_UINT32_ARG;
	
	CACHE_CAS(_channelCount, v);
	
	// TODO: may be additional actions on change?
	
	audioNode->emit("channelCount", 1, &value);
	
}


NAN_GETTER(AudioNode::channelCountModeGetter) { THIS_AUDIO_NODE; THIS_CHECK;
	
	RET_VALUE(JS_UTF8(audioNode->_channelCountMode));
	
}

NAN_SETTER(AudioNode::channelCountModeSetter) { THIS_AUDIO_NODE; THIS_CHECK; SETTER_UTF8_ARG;
	
	if (audioNode->_channelCountMode == *v) {
		return;
	}
	audioNode->_channelCountMode = *v;
	
	// TODO: may be additional actions on change?
	
	audioNode->emit("channelCountMode", 1, &value);
	
}


NAN_GETTER(AudioNode::channelInterpretationGetter) { THIS_AUDIO_NODE; THIS_CHECK;
	
	RET_VALUE(JS_UTF8(audioNode->_channelInterpretation));
	
}

NAN_SETTER(AudioNode::channelInterpretationSetter) { THIS_AUDIO_NODE; THIS_CHECK; SETTER_UTF8_ARG;
	
	if (audioNode->_channelInterpretation == *v) {
		return;
	}
	audioNode->_channelInterpretation = *v;
	
	// TODO: may be additional actions on change?
	
	audioNode->emit("channelInterpretation", 1, &value);
	
}


// ------ System methods and props for ObjectWrap

Nan::Persistent<v8::FunctionTemplate> AudioNode::_protoAudioNode;
Nan::Persistent<v8::Function> AudioNode::_ctorAudioNode;


void AudioNode::init(Local<Object> target) {
	
	Local<FunctionTemplate> proto = Nan::New<FunctionTemplate>(newCtor);
	
	// class AudioNode inherits EventEmitter
	Local<FunctionTemplate> parent = Nan::New(EventEmitter::_protoEventEmitter);
	proto->Inherit(parent);
	
	proto->InstanceTemplate()->SetInternalFieldCount(1);
	proto->SetClassName(JS_STR("AudioNode"));
	
	
	// Accessors
	Local<ObjectTemplate> obj = proto->PrototypeTemplate();
	ACCESSOR_R(obj, isDestroyed);
	
	ACCESSOR_R(obj, context);
	ACCESSOR_R(obj, numberOfInputs);
	ACCESSOR_R(obj, numberOfOutputs);
	ACCESSOR_RW(obj, channelCount);
	ACCESSOR_RW(obj, channelCountMode);
	ACCESSOR_RW(obj, channelInterpretation);
	
	// -------- dynamic
	
	Nan::SetPrototypeMethod(proto, "destroy", destroy);
	
	Nan::SetPrototypeMethod(proto, "connect", connect);
	Nan::SetPrototypeMethod(proto, "disconnect", disconnect);
	
	// -------- static
	
	Local<Function> ctor = Nan::GetFunction(proto).ToLocalChecked();
	
	_protoAudioNode.Reset(proto);
	_ctorAudioNode.Reset(ctor);
	
	Nan::Set(target, JS_STR("AudioNode"), ctor);
	
	
}


NAN_METHOD(AudioNode::newCtor) {
	
	Nan::ThrowTypeError("'AudioNode' is abstract, use a specific node class.");
	
}


NAN_METHOD(AudioNode::destroy) { THIS_AUDIO_NODE; THIS_CHECK;
	
	audioNode->_destroy();
	
}


NAN_GETTER(AudioNode::isDestroyedGetter) { THIS_AUDIO_NODE;
	
	RET_VALUE(JS_BOOL(audioNode->_isDestroyed));
	
}
