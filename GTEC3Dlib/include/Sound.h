
#include "GTEC.h"

class CSound
{

public:

	// ----  Constructors ------	
  CSound() { pAudioChunk = NULL; Channel = 0; Volume = 100;}

  // -----  Destructor --------
  ~CSound() { if (pAudioChunk != NULL) { Mix_HaltChannel( Channel); Mix_FreeChunk( pAudioChunk); }}

private:
  Mix_Chunk *pAudioChunk;  // pointer to audio data
  int Channel;			   // audio channel
  int Volume;

public:
  // Load audio from a .wav file
  bool LoadAudioFile( string filename);

  // Play a certain number of loops
  void Play(int loops);

  // play once
  void Play() 	{ Play(1); }

  // play continuously
  void PlayContinuously() { Play(0); }

  // set volume in %
  void SetVolume( int volume);

  
  void Pause();

  void Resume();
  
  void Clear();
	
};
