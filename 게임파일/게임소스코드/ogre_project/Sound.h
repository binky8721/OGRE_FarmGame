#pragma once
#include"irrklangInclude\irrKlang.h"
//#include <irrKlang.h>
#pragma comment (lib, "irrklang.lib")
using namespace irrklang;

class CSound
{
private:

	//------------------------------------------------------
	irrklang::ISoundEngine* Title_BGM;
	irrklang::ISoundEngine* Stage1_BGM;
	irrklang::ISoundEngine* Title_Button_Sound;
	irrklang::ISoundEngine* Plant_Sound;
	irrklang::ISoundEngine* Walk_Sound;
	irrklang::ISoundEngine* Gameover_Sound;
	irrklang::ISoundEngine* Gameclear_Sound;
	irrklang::ISoundEngine* CropGet_Sound;
	irrklang::ISoundEngine* GrowUp_Sound;
	irrklang::ISoundEngine* WaterCharge_Sound;
	irrklang::ISoundEngine* WaterSpray_Sound;
	irrklang::ISoundEngine* Morning_Sound;
	//------------------------------------------------------
public:
	CSound();
	~CSound();
	//-------------------------------------------
	void PlayTitle_BGM();
	void PlayStage1_BGM();
	void PlayStartButton_Sound();
	void PlayWalk_Sound();
	void PlayGameover_Sound();
	void PlayGameclear_Sound();
	void PlayPlant_Sound();
	void PlayGrowUp_Sound();
	void PlayCropGet_Sound();

	void PlayWaterCharge_Sound();
	void PlayWaterSpray_Sound();
	void PlayMorning_Sound();

	void Gameover_Stop();
	void Gameclear_Stop();
	void Title_BGM_Stop();
	void Stage1_BGM_Stop();
	void Walk_Sound_Stop();
	//void GrowUp_Stop();
	//void CropGet_Stop();
};