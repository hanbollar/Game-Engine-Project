#pragma once

// for reference: https://www.ambiera.com/irrklang/

#include <iostream>

#include "third_party/irrKlang/irrKlang.h"
#include "globals.h"

using namespace irrklang;

#pragma comment(lib, "../external/irrKlang/irrKlang.lib") // link with irrKlang.dll

class AudioHandler {
private:
    vec3df audio_source_;
    bool using_3D_audio_;

    float min_distance_;

    ISoundEngine* engine_;
    ISound* music_;

    bool playing_;

    const char* playing_filename_;
public:
    AudioHandler();
    AudioHandler(const char* audioFile, const float& minDistance, const glm::vec3& sourcePos);
    ~AudioHandler();

    void StartPlayingSound();
    void StopPlayingSound();
    void Update(const glm::vec3& listener_position);

    void SetAudioSource(const glm::vec3& source_position);
    void SetMinDistance(const float& min_dist);
    void SetAudioFile(const char* filename);
};
