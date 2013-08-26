#include "Sound.h"
#include "Resources.h"

namespace TiGRE
{
	Sound::Sound(Resources* resources, std::string name) : Object(resources, "Sound", name)
	{

	}

	Sound::~Sound()
	{
		alSourceStop(alSource);
		alDeleteSources(1, &alSource);
		alDeleteBuffers(1, &alSampleSet);
	}
	
	void Sound::setState(bool on)
	{
		if(on)
			alSourcePlay(alSource);
		else
			alSourceStop(alSource);
	}
	
	bool Sound::isEnabled()
	{
		ALint state;
		alGetSourcei(alSource, AL_SOURCE_STATE, &state);
		return state == AL_PLAYING;
	}

	void Sound::setLooping(bool on)
	{
		alSourcei(alSource, AL_LOOPING, on ? AL_TRUE : AL_FALSE);
	}

	void Sound::setPitch(float pitch)
	{
		alSourcef(alSource, AL_PITCH, pitch);
	}

	void Sound::setGain(float gain)
	{
		alSourcef(alSource, AL_GAIN, gain);
	}

	void Sound::updateTransform()
	{
		Object::updateTransform();
		alSourcefv(alSource, AL_POSITION, glm::value_ptr((*transform())[3]));
	}

	void Sound::loadFromFile(std::string filepath)
	{
		//we do only accept *.wav files at the moment
		loadFromWav(filepath);
	}

	void Sound::loadFromWav(std::string filepath)
	{
		char* alBuffer;
		ALenum alFormatBuffer;
		ALsizei alFreqBuffer;
		long alBufferLen;
		ALboolean alLoop;
		ALenum error;
 
		alutLoadWAVFile((ALbyte*)filepath.c_str(), &alFormatBuffer, (void**)&alBuffer, (ALsizei*)&alBufferLen, &alFreqBuffer, &alLoop);
		if((error = alutGetError()) != ALUT_ERROR_NO_ERROR)
			throw new ResourceLoadException("error while loading \"" + filepath + "\": " + alutGetErrorString(error));
		alGenSources(1, &alSource);
		alGenBuffers(1, &alSampleSet);
		if((error = alGetError()) != AL_NO_ERROR)
			throw new ResourceLoadException("error creating openal buffers for \"" + filepath + "\"");
		alBufferData(alSampleSet, alFormatBuffer, alBuffer, alBufferLen, alFreqBuffer);
		alSourcei(alSource, AL_BUFFER, alSampleSet);
		alutUnloadWAV(alFormatBuffer, alBuffer, alBufferLen, alFreqBuffer);

		alSourcef(alSource, AL_PITCH, 1.0f);
		alSourcef(alSource, AL_GAIN, 2.0f);
		alSourcefv(alSource, AL_POSITION, glm::value_ptr(position));
		ALfloat velocity[] = { 0.0, 0.0, 0.0 };
		alSourcefv(alSource, AL_VELOCITY, velocity);
		alSourcei(alSource, AL_LOOPING, AL_TRUE);
		alSourcef(alSource, AL_MAX_DISTANCE, 100.0f);
		alSourcef(alSource, AL_ROLLOFF_FACTOR, 0.1f);
		//alSourcei(alSource, AL_SOURCE_RELATIVE, AL_TRUE);

		alSourcePlay(alSource);
	}

	void Sound::loadFromXml(pugi::xml_node node)
	{
		pugi::xml_node state = node.child("State");
		if(state)
			setState(state.attribute("enabled").as_bool());

		pugi::xml_node looping = node.child("Looping");
		if(looping)
			setLooping(looping.attribute("enabled").as_bool());

		pugi::xml_node pitch = node.child("Pitch");
		if(pitch)
			setPitch(pitch.attribute("value").as_float());

		pugi::xml_node gain = node.child("Gain");
		if(gain)
			setGain(gain.attribute("value").as_float());

		Object::loadFromXml(node);
	}
}
