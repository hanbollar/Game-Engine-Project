#include "audio_handler.h"

char* SOUNDS[5]{ "resources/AUDIO/irrklang/bell.wav",
                  "resources/AUDIO/irrklang/ophelia.mp3",
                  "resources/AUDIO/Cat2.mp3",
                  "resources/AUDIO/kitten3.mp3",
                  "resources/AUDIO/kittycry.mp3"};

AudioHandler::AudioHandler() : AudioHandler(SOUNDS[AudioChoices::GENERAL], 5.f, glm::vec3(0.f)) {}

AudioHandler::AudioHandler(const char* audioFile, const float& minDistance, const glm::vec3& sourcePos)
        : engine_(createIrrKlangDevice()),
          audio_source_(vec3df(sourcePos.x, sourcePos.y, sourcePos.z)),
          music_3d_(nullptr), music_2d_(nullptr), min_distance_(minDistance),
          using_3D_audio_(true), playing_(false),
          playing_filename_(audioFile) {

    char* sound = SOUNDS[AudioChoices::GENERAL];
    if (!engine_) {
        ErrorHandler::ThrowError("Sound Engine wasn't created properly");
    }
}

AudioHandler::~AudioHandler() {
    delete engine_;
    delete music_3d_;
    delete music_2d_;
    delete playing_filename_;
}

void AudioHandler::StartPlayingSound(AudioChoices sound3d, AudioChoices sound2d) {
    music_3d_ = engine_->play3D(SOUNDS[sound3d], audio_source_, true, false/*!using_3D_audio_*/, true);
    if (!music_3d_) {
        ErrorHandler::ThrowError("Music wasn't created properly");
    }
    music_3d_->setMinDistance(min_distance_);

    music_2d_ = engine_->play2D(SOUNDS[sound2d], true, false /*using_3D_audio_*/, true);

    music_3d_->setIsPaused(!using_3D_audio_);
    music_2d_->setIsPaused(using_3D_audio_);

    playing_ = true;
}

void AudioHandler::PauseSound() {
    if (using_3D_audio_) {
        music_3d_->setIsPaused(true);
    } else {
        music_2d_->setIsPaused(true);
    }
}

void AudioHandler::UnpauseSound() {
    if (using_3D_audio_) {
        music_3d_->setIsPaused(false);
    } else {
        music_2d_->setIsPaused(false);
    }
}

void AudioHandler::StopPlayingSound() {
    if (music_3d_) {
        music_3d_->drop();
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

    if (music_3d_) {
        music_3d_->setPosition(audio_source_);
        music_3d_->setMinDistance(min_distance_);
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

void AudioHandler::SetAudioSourcePos(const glm::vec3& source_position) {
    audio_source_ = vec3df(source_position.x, source_position.y, source_position.z);
}

void AudioHandler::SetMinDistance(const float& min_dist) {
    min_distance_ = min_dist;
}

void AudioHandler::SetAudioFile(AudioChoices choice) {
    playing_filename_ = SOUNDS[choice];
}

void AudioHandler::PlaySingleSound(AudioChoices choice) {
    engine_->play2D(SOUNDS[choice]);
}

void AudioHandler::PlaySingleSound(AudioChoices choice, const glm::vec3& listener_pos) {
    if (using_3D_audio_) {
        vec3df listener_position(listener_pos.x, listener_pos.y, listener_pos.z);
        vec3df listener_direction(0, 0, 1); // leaving as +z for now, might update later
        engine_->setListenerPosition(listener_position, listener_direction);

        if (music_3d_) {
            music_3d_->setPosition(audio_source_);
            music_3d_->setMinDistance(min_distance_);
        }

        engine_->play3D(SOUNDS[choice], audio_source_);
    } else {
        engine_->play2D(SOUNDS[choice]);
    }
}
