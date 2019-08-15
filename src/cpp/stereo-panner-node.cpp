
#include "stereo-panner-node.hpp"

#include "common.hpp"


// ------ Constructor and Destructor

StereoPannerNode::StereoPannerNode() :
AudioNode() {
	
	_isDestroyed = false;
	
}


StereoPannerNode::~StereoPannerNode() {
	
	_destroy();
	
}


void StereoPannerNode::_destroy() { DES_CHECK;
	
	_isDestroyed = true;
	
	AudioNode::_destroy();
	
}


// ------ Methods and props



JS_GETTER(StereoPannerNode::panGetter) { THIS_CHECK;
	
	RET_VALUE(_pan.Value());
	
}


// ------ System methods and props for ObjectWrap

Napi::FunctionReference StereoPannerNode::_ctorStereoPannerNode;


void StereoPannerNode::init(Napi::Env env, Napi::Object exports) {
	
	Napi::Function ctor = DefineClass(env, "StereoPannerNode", {
		ACCESSOR_M(StereoPannerNode, destroy),
		ACCESSOR_R(StereoPannerNode, pan),
		ACCESSOR_R(StereoPannerNode, isDestroyed)
	});
	
	_ctorStereoPannerNode = Napi::Persistent(ctor);
	_ctorStereoPannerNode.SuppressDestruct();
	
	exports.Set("StereoPannerNode", ctor);
	
}


bool StereoPannerNode::isStereoPannerNode(Napi::Object obj) {
	return obj.InstanceOf(_ctorStereoPannerNode.Value());
}


Napi::Object StereoPannerNode::getNew() {
	
	Napi::Function ctor = Nan::New(_ctorStereoPannerNode);
	// Napi::Value argv[] = { /* arg1, arg2, ... */ };
	return Nan::NewInstance(ctor, 0/*argc*/, nullptr/*argv*/).ToLocalChecked();
	
}


StereoPannerNode::StereoPannerNode(const Napi::CallbackInfo &info): Napi::ObjectWrap<StereoPannerNode>(info) {
	
	CTOR_CHECK("StereoPannerNode");
	
	StereoPannerNode *stereoPannerNode = new StereoPannerNode();
	
}


JS_METHOD(StereoPannerNode::destroy) { THIS_CHECK;
	
	emit("destroy");
	
	_destroy();
	
}


JS_GETTER(StereoPannerNode::isDestroyedGetter) { NAPI_ENV;
	
	RET_BOOL(_isDestroyed);
	
}