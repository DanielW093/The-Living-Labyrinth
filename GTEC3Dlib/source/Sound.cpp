
#include "Sound.h"


bool CSound::LoadAudioFile(string filename)
{
    cout << "loading " << filename;
	// delete current data if it exists
    if (pAudioChunk != NULL) { Mix_HaltChannel( Channel); Mix_FreeChunk( pAudioChunk); }
    // load new wav data from file
	pAudioChunk = Mix_LoadWAV( filename.c_str());

	if (pAudioChunk == NULL) { cout << "..FAIL" << endl; Channel=0; return false; } 
	else cout << "..ok" << endl;
	return true;
}

void CSound::Play(int loops)
{
  if (loops < 0) return;
  if (pAudioChunk != NULL)  Channel = Mix_PlayChannel(-1, pAudioChunk, loops-1);
  else return;
  Mix_Volume( Channel, Volume); // set volume of current channel
   
}

void CSound::SetVolume( int volume)  
{ 
   if (volume < 0 || volume > 100) return;
   Volume = volume;
   Mix_Volume( Channel, Volume); // set volume of current channel
}


void CSound::Clear()
{
     if (pAudioChunk != NULL) Mix_FreeChunk( pAudioChunk);
	 pAudioChunk = NULL; Channel = 0;
}


void CSound::Pause()
{
  Mix_Pause( Channel);
}

void CSound::Resume()
{
  Mix_Resume( Channel);
}



	
