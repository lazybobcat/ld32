#include <resources/musicplayer.h>

MusicPlayer::MusicPlayer() :
    mMusic(),
    mFilenames(),
    mVolume(100.f)
{
    // Fill mFilenames table here :
    mFilenames[Musics::MainThemeMusical] = "assets/musics/ld32-maintheme-musical.wav";
    mFilenames[Musics::MainThemeSinging] = "assets/musics/ld32-maintheme-singing.wav";
}

void MusicPlayer::play(Musics::ID song)
{
    std::string filename = mFilenames[song];

    if (!mMusic.openFromFile(filename))
        throw std::runtime_error("Music " + filename + " could not be loaded.");

    mMusic.setVolume(mVolume);
    //mMusic.setLoop(true);
    mMusic.play();
}

void MusicPlayer::stop()
{
    mMusic.stop();
}

void MusicPlayer::setLoop(bool b)
{
    mMusic.setLoop(b);
}

void MusicPlayer::setVolume(float volume)
{
    mVolume = volume;
    mMusic.setVolume(volume);
}

void MusicPlayer::pause(bool flag)
{
    if (flag)
        mMusic.pause();
    else
        mMusic.play();
}
