#define AL_LIBTYPE_STATIC
#include <AL/al.h>
#include <AL/alc.h>

#include <audio/music_system.hpp>
#include <audio/sound_event.hpp>
#include <audio/sound_system.hpp>
#include <manager/manager.hpp>
namespace nen {
sound::sound() {}
void sound::load(std::string_view file_name) {
  get_sound().LoadAudioFile(file_name);
  auto sourceID = get_sound().NewSource(file_name);
  ALint buf;
  alGetSourcei(sourceID, AL_BUFFER, &buf);
  param.source_id = sourceID;
  param.buffer_id = buf;
  mName = file_name.data();
}
void sound::play() { alSourcePlay(param.source_id); }
void sound::new_source() {
  auto sourceID = get_sound().NewSource(mName);
  ALint buf;
  alGetSourcei(sourceID, AL_BUFFER, &buf);
  param.source_id = sourceID;
  param.buffer_id = buf;
}
void sound::set_listener(vector3 pos, vector3 rotation) {
  quaternion q(vector3::neg_unit_z, rotation.z);
  q = quaternion::Concatenate(q, quaternion(vector3::unit_y, rotation.y));
  q = quaternion::Concatenate(q, quaternion(vector3::unit_x, rotation.x));
  get_sound().SetListener(pos, q);
}
void sound::delete_source() { get_sound().DeleteSource(param.source_id); }
bool sound::IsValid() {
  return get_sound().get_buffers().contains(mName.data());
}

void sound::Restart() {
  isPlaying = true;
  Stop();
  alSourcePlay(param.source_id);
}

void sound::Stop(bool allowFadeOut /* true */) {
  isPlaying = false;
  alSourceStop(param.source_id);
}

void sound::SetPaused(bool pause) {
  isPaused = pause;
  alSourcePause(param.source_id);
}

void sound::SetVolume(float value) {
  volume = value;
  alSourcef(param.source_id, AL_GAIN, value);
}

void sound::SetPitch(float value) {
  pitch = value;
  alSourcef(param.source_id, AL_PITCH, value);
}

void sound::SetPosition(vector3 pos) {
  this->pos = pos;
  alSource3f(param.source_id, AL_POSITION, pos.x, pos.y, pos.z);
}

bool sound::GetPaused() { return isPaused; }

float sound::GetVolume() { return volume; }

float sound::GetPitch() { return pitch; }

const vector3 &sound::GetPosition() { return pos; }

std::string sound::GetName() { return mName; }

} // namespace nen