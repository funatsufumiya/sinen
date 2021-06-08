﻿#include <Engine.hpp>
#include <SDL_log.h>
#include <vector>
#include <Scene.hpp>
#include <Actor/include/Actor.h>
#include <SDL_mixer.h>
namespace nen
{
	Vector3f Calc(const Quaternion &r);
	namespace detail
	{
		static int check_openal_error(const char *where)
		{
			const ALenum err = alGetError();
			if (err != AL_NONE)
			{
				printf("OpenAL Error at %s! %s (%u)\n", where, alGetString(err), (unsigned int)err);
				return 1;
			}
			return 0;
		}
		static ALenum get_openal_format(const SDL_AudioSpec *spec)
		{
			if ((spec->channels == 1) && (spec->format == AUDIO_U8))
			{
				return AL_FORMAT_MONO8;
			}
			else if ((spec->channels == 1) && (spec->format == AUDIO_S16SYS))
			{
				return AL_FORMAT_MONO16;
			}
			else if ((spec->channels == 2) && (spec->format == AUDIO_U8))
			{
				return AL_FORMAT_STEREO8;
			}
			else if ((spec->channels == 2) && (spec->format == AUDIO_S16SYS))
			{
				return AL_FORMAT_STEREO16;
			}
			else if ((spec->channels == 1) && (spec->format == AUDIO_F32SYS))
			{
				return alIsExtensionPresent("AL_EXT_FLOAT32") ? alGetEnumValue("AL_FORMAT_MONO_FLOAT32") : AL_NONE;
			}
			else if ((spec->channels == 2) && (spec->format == AUDIO_F32SYS))
			{
				return alIsExtensionPresent("AL_EXT_FLOAT32") ? alGetEnumValue("AL_FORMAT_STEREO_FLOAT32") : AL_NONE;
			}
			return AL_NONE;
		}
	}
	AudioSystem::AudioSystem(std::shared_ptr<Scene> scene)
		: mScene(scene)
	{
	}

	AudioSystem::~AudioSystem()
	{
		alcMakeContextCurrent(NULL);
		alcDestroyContext(context);
		alcCloseDevice(device);
	}

	bool AudioSystem::Initialize()
	{
		device = alcOpenDevice(NULL);
		if (!device)
		{
			printf("Couldn't open OpenAL default device.\n");
			return -1;
		}
		context = alcCreateContext(device, NULL);
		if (!context)
		{
			printf("Couldn't create OpenAL context.\n");
			alcCloseDevice(device);
			return -1;
		}

		if (!alcMakeContextCurrent(context))
		{
			printf("Couldn't create OpenAL context.\n");
			alcMakeContextCurrent(NULL);
			alcCloseDevice(device);
		}
		return true;
	}

	void AudioSystem::Shutdown()
	{
	}

	void AudioSystem::Update(float deltaTime)
	{
	}

	AudioEvent AudioSystem::PlayEvent(std::string_view name, AudioType ae)
	{
		AudioEvent e(shared_from_this(), ae, name);
		return e;
	}

	void AudioSystem::SetListener(const Vector3f &pos, const Quaternion &quat)
	{
		alListener3f(AL_POSITION, pos.x, pos.z, pos.y);
		auto at = Vector3f::Transform(Vector3f::UnitX, quat);
		auto up = Vector3f::Transform(Vector3f::UnitZ, quat);
		float ori[6] = {at.x, at.z, at.y, up.x, up.z, up.y};
		alListenerfv(AL_ORIENTATION, ori);
	}

	void AudioSystem::LoadAudioFile(std::string_view fileName, AudioType type)
	{
		if (type == AudioType::Music)
		{
		}
		else
		{
			SDL_AudioSpec spec;
			ALenum alfmt = AL_NONE;
			Uint8 *buffer = NULL;
			Uint32 buffer_length = 0;
			ALuint sid = 0;
			ALuint bid = 0;

			if (!SDL_LoadWAV(fileName.data(), &spec, &buffer, &buffer_length))
			{
				printf("Loading '%s' failed! %s\n", fileName.data(), SDL_GetError());
				return;
			}

			alGenSources(1, &sid);
			alGenBuffers(1, &bid);

			alBufferData(bid, AL_FORMAT_MONO16, buffer, buffer_length, spec.freq);
			alSourcei(sid, AL_BUFFER, bid);
			SoundParameters param;
			param.buffer_id = bid;
			param.source_id = sid;
			sounds.emplace(fileName.data(), param);
			SDL_FreeWAV(buffer);
		}
	}

	void AudioSystem::UnloadAudioFile(std::string_view fileName)
	{
		std::string name = fileName.data();
		if (sounds.contains(name))
		{
			alDeleteSources(1, &sounds[name].source_id);
			detail::check_openal_error("alDeleteSources");
			alDeleteBuffers(1, &sounds[name].buffer_id);
			detail::check_openal_error("alDeleteBuffers");
			sounds.erase(name);
		}
	}
	Vector3f Calc(const Quaternion &r)
	{
		float x = r.x;
		float y = r.y;
		float z = r.z;
		float w = r.w;

		float x2 = x * x;
		float y2 = y * y;
		float z2 = z * z;

		float xy = x * y;
		float xz = x * z;
		float yz = y * z;
		float wx = w * x;
		float wy = w * y;
		float wz = w * z;

		// 1 - 2y^2 - 2z^2
		float m00 = 1.f - (2.f * y2) - (2.f * z2);

		// 2xy + 2wz
		float m01 = (2.f * xy) + (2.f * wz);

		// 2xy - 2wz
		float m10 = (2.f * xy) - (2.f * wz);

		// 1 - 2x^2 - 2z^2
		float m11 = 1.f - (2.f * x2) - (2.f * z2);

		// 2xz + 2wy
		float m20 = (2.f * xz) + (2.f * wy);

		// 2yz+2wx
		float m21 = (2.f * yz) - (2.f * wx);

		// 1 - 2x^2 - 2y^2
		float m22 = 1.f - (2.f * x2) - (2.f * y2);

		float tx, ty, tz;

		if (m21 >= 0.99 && m21 <= 1.01)
		{
			tx = Math::Pi / 2.f;
			ty = 0;
			tz = Math::Atan2(m10, m00);
		}
		else if (m21 >= -1.01f && m21 <= -0.99f)
		{
			tx = -Math::Pi / 2.f;
			ty = 0;
			tz = Math::Atan2(m10, m00);
		}
		else
		{
			tx = std::asin(-m21);
			ty = Math::Atan2(m20, m22);
			tz = Math::Atan2(m01, m11);
		}

		return Vector3f(Math::ToDegrees(tx), Math::ToDegrees(ty), Math::ToDegrees(tz));
	}
}