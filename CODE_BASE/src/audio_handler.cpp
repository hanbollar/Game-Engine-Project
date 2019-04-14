#include "audio_handler.h"

AudioHandler::AudioHandler() : AudioHandler("resources/AUDIO/irrklang/ophelia.mp3", 5.f, glm::vec3(0.f)) {}

AudioHandler::AudioHandler(const char* audioFile, const float& minDistance, const glm::vec3& sourcePos)
        : engine_(createIrrKlangDevice()),
          audio_source_(vec3df(sourcePos.x, sourcePos.y, sourcePos.z)),
          music_(nullptr), min_distance_(minDistance),
          using_3D_audio_(true), playing_(false),
          playing_filename_(audioFile) {

    if (!engine_) {
        ErrorHandler::ThrowError("Sound Engine wasn't created properly");
    }
}

AudioHandler::~AudioHandler() {
    delete engine_;
    delete music_;
    delete playing_filename_;
}

void AudioHandler::StartPlayingSound() {
    if (using_3D_audio_) {
        music_ = engine_->play3D(playing_filename_, vec3df(0, 0, 0), true, false, true);

        if (!music_) {
            ErrorHandler::ThrowError("Music wasn't created properly");
        }

        playing_ = true;
        music_->setMinDistance(min_distance_);
    }
}

void AudioHandler::StopPlayingSound() {
    if (music_) {
        music_->drop();
    }
    engine_->drop();
}

void AudioHandler::Update(const glm::vec3& listener_pos) {
    if (!playing_) {
        return;
    }

    vec3df listener_position(listener_pos.x, listener_pos.y, listener_pos.z);
    vec3df listener_direction(0, 0, 1); // leaving as +z for now, might update later
    engine_->setListenerPosition(listener_position, listener_direction);

    if (music_) {
        music_->setPosition(audio_source_);
        music_->setMinDistance(min_distance_);
    }

    //sleepSomeTime();

    // Handle user input: Every time the user presses a key in the console,
    // play a random sound or exit the application if he pressed ESCAPE.

    /*const char* filename;
    if (rand() % 2)
        playing_filename_ = "src/resources/irrklang/bell.wav";
    else
        filename = "src/resources/irrklang/explosion.wav";

    if (!playing_filename_) {
        ErrorHandler::ThrowError("No audio file to play.");
    }
    engine_->play3D(playing_filename_, listener_position);*/
}

void AudioHandler::SetAudioSource(const glm::vec3& source_position) {
    audio_source_ = vec3df(source_position.x, source_position.y, source_position.z);
}

void AudioHandler::SetMinDistance(const float& min_dist) {
    min_distance_ = min_dist;
}

void AudioHandler::SetAudioFile(const char* filename) {
    playing_filename_ = filename;
}

