#include <LabSound/LabSound.h>

#include "audio-listener.hpp"
#include "audio-context.hpp"
#include "audio-param.hpp"


IMPLEMENT_ES5_CLASS(AudioListener);

void AudioListener::init(Napi::Env env, Napi::Object exports) {
	
	Napi::Function ctor = wrap(env);
	JS_ASSIGN_METHOD(setOrientation);
	JS_ASSIGN_METHOD(setPosition);
	JS_ASSIGN_GETTER(upZ);
	JS_ASSIGN_GETTER(upY);
	JS_ASSIGN_GETTER(upX);
	JS_ASSIGN_GETTER(forwardZ);
	JS_ASSIGN_GETTER(forwardY);
	JS_ASSIGN_GETTER(forwardX);
	JS_ASSIGN_GETTER(positionZ);
	JS_ASSIGN_GETTER(positionY);
	JS_ASSIGN_GETTER(positionX);
	JS_ASSIGN_METHOD(destroy);
	
	exports.Set("AudioListener", ctor);
	
}


Napi::Object AudioListener::create(Napi::Env env, Napi::Object context, ListenerPtr listener) {
	Napi::Function ctor = _ctorEs5.Value().As<Napi::Function>();
	std::vector<napi_value> args;
	args.push_back(context);
	args.push_back(JS_EXT(&listener));
	return ctor.New(args);
}


AudioListener::AudioListener(const Napi::CallbackInfo &info):
CommonListener(info.This(), "AudioListener") { NAPI_ENV;
	
	super(info);
	
	REQ_OBJ_ARG(0, context);
	REQ_EXT_ARG(1, extListener);
	
	ListenerPtr *param = reinterpret_cast<ListenerPtr *>(extListener.Data());
	
	reset(context, *param);
	
	_positionX.Reset(AudioParam::create(env, context, _impl->positionX()));
	_positionY.Reset(AudioParam::create(env, context, _impl->positionY()));
	_positionZ.Reset(AudioParam::create(env, context, _impl->positionZ()));
	_forwardX.Reset(AudioParam::create(env, context, _impl->forwardX()));
	_forwardY.Reset(AudioParam::create(env, context, _impl->forwardY()));
	_forwardZ.Reset(AudioParam::create(env, context, _impl->forwardZ()));
	_upX.Reset(AudioParam::create(env, context, _impl->upX()));
	_upY.Reset(AudioParam::create(env, context, _impl->upY()));
	_upZ.Reset(AudioParam::create(env, context, _impl->upZ()));
	
}


AudioListener::~AudioListener() {
	_destroy();
}


void AudioListener::_destroy() { DES_CHECK;
	CommonListener::_destroy();
}


// ------ Methods and props


JS_IMPLEMENT_METHOD(AudioListener, setPosition) { THIS_CHECK;
	
	REQ_FLOAT_ARG(0, x);
	REQ_FLOAT_ARG(1, y);
	REQ_FLOAT_ARG(2, z);
	
	_impl->setPosition(x, y, z);
	RET_UNDEFINED;
	
}


JS_IMPLEMENT_METHOD(AudioListener, setOrientation) { THIS_CHECK;
	
	REQ_FLOAT_ARG(0, x);
	REQ_FLOAT_ARG(1, y);
	REQ_FLOAT_ARG(2, z);
	REQ_FLOAT_ARG(3, xUp);
	REQ_FLOAT_ARG(4, yUp);
	REQ_FLOAT_ARG(5, zUp);
	
	_impl->setOrientation(x, y, z, xUp, yUp, zUp);
	RET_UNDEFINED;
	
}


PARAM_GETTER(AudioListener, positionX);
PARAM_GETTER(AudioListener, positionY);
PARAM_GETTER(AudioListener, positionZ);
PARAM_GETTER(AudioListener, forwardX);
PARAM_GETTER(AudioListener, forwardY);
PARAM_GETTER(AudioListener, forwardZ);
PARAM_GETTER(AudioListener, upX);
PARAM_GETTER(AudioListener, upY);
PARAM_GETTER(AudioListener, upZ);


JS_IMPLEMENT_METHOD(AudioListener, destroy) { THIS_CHECK;
	
	emit("destroy");
	
	_destroy();
	RET_UNDEFINED;
	
}
