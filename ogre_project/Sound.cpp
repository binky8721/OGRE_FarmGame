#include "Sound.h"

CSound::CSound()
{
	
	Title_BGM = irrklang::createIrrKlangDevice();
	Title_BGM->setSoundVolume(0.65f);

	Stage1_BGM = irrklang::createIrrKlangDevice();
	Stage1_BGM->setSoundVolume(0.9f);

	Title_Button_Sound = irrklang::createIrrKlangDevice();
	Title_Button_Sound->setSoundVolume(0.9f);

	Plant_Sound = irrklang::createIrrKlangDevice();
	Plant_Sound->setSoundVolume(0.9f);

	Walk_Sound = irrklang::createIrrKlangDevice();
	Walk_Sound->setSoundVolume(2.0f);

	Gameover_Sound = irrklang::createIrrKlangDevice();
	Gameover_Sound->setSoundVolume(0.9f);
	
	Gameclear_Sound = irrklang::createIrrKlangDevice();
	Gameclear_Sound->setSoundVolume(0.9f);

	CropGet_Sound = irrklang::createIrrKlangDevice();
	CropGet_Sound->setSoundVolume(1.0f);

    GrowUp_Sound = irrklang::createIrrKlangDevice();
	GrowUp_Sound->setSoundVolume(1.0f);

	WaterCharge_Sound = irrklang::createIrrKlangDevice();
	WaterCharge_Sound->setSoundVolume(1.0f);

	WaterSpray_Sound = irrklang::createIrrKlangDevice();
	WaterSpray_Sound->setSoundVolume(1.0f);

	Morning_Sound = irrklang::createIrrKlangDevice();
	Morning_Sound->setSoundVolume(1.0f);
}

CSound::~CSound()
{
	if (Title_BGM)
	{
		Title_BGM->stopAllSounds();
		Title_BGM->drop();
	}

	if (Stage1_BGM)
	{
		Stage1_BGM->stopAllSounds();
		Stage1_BGM->drop();
	}
	if (Title_Button_Sound)
	{
		Title_Button_Sound->stopAllSounds();
		Title_Button_Sound->drop();
	}

	if (Plant_Sound)
	{
		Plant_Sound->stopAllSounds();
		Plant_Sound->drop();
	}

	if (Walk_Sound)
	{
		Walk_Sound->stopAllSounds();
		Walk_Sound->drop();
	}
	if (Gameover_Sound)
	{
		Gameover_Sound->stopAllSounds();
		Gameover_Sound->drop();
	}
	if (Gameclear_Sound)
	{
		Gameclear_Sound->stopAllSounds();
		Gameclear_Sound->drop();
	}
	if (GrowUp_Sound)
	{
		GrowUp_Sound->stopAllSounds();
		GrowUp_Sound->drop();
	}
	if (CropGet_Sound)
	{
		CropGet_Sound->stopAllSounds();
		CropGet_Sound->drop();
	}

	if (WaterCharge_Sound)
	{
		WaterCharge_Sound->stopAllSounds();
		WaterCharge_Sound->drop();
	}
	if (WaterSpray_Sound)
	{
		WaterSpray_Sound->stopAllSounds();
		WaterSpray_Sound->drop();
	}
	if (Morning_Sound)
	{
		Morning_Sound->stopAllSounds();
		Morning_Sound->drop();
	}
}


void CSound::PlayTitle_BGM()
{
	Title_BGM->play2D("sound/Title.mp3", true);
}

void CSound::PlayStage1_BGM()
{
	if(!Stage1_BGM->isCurrentlyPlaying("sound/FieldMap.mp3"))
		Stage1_BGM->play2D("sound/FieldMap.mp3", true);
}

void CSound::PlayStartButton_Sound()
{
	Title_Button_Sound->play2D("sound/GameStart.wav", false);
}

void CSound::PlayWalk_Sound()
{
	if(!Walk_Sound->isCurrentlyPlaying("sound/regular_walking.wav"))
		Walk_Sound->play2D("sound/regular_walking.wav", true);
}

void CSound::PlayPlant_Sound()
{
	Plant_Sound->play2D("sound/PlantSeed.mp3",false);
}

void CSound::PlayGameover_Sound()
{
	if(!Gameover_Sound->isCurrentlyPlaying("sound/GameOver.wav"))
		Gameover_Sound->play2D("sound/GameOver.wav", false);
}

void CSound::PlayGameclear_Sound()
{
	if(!Gameclear_Sound->isCurrentlyPlaying("sound/GameClear.wav"))
	Gameclear_Sound->play2D("sound/GameClear.wav", false);
}

void CSound::PlayWaterCharge_Sound()
{
	if(!WaterCharge_Sound->isCurrentlyPlaying("sound/InWater.wav"))
	WaterCharge_Sound->play2D("sound/InWater.wav", false);
}
void CSound::PlayWaterSpray_Sound()
{
	if(!WaterSpray_Sound->isCurrentlyPlaying("sound/SprayWater.wav"))
	WaterSpray_Sound->play2D("sound/SprayWater.wav", false);
}
void CSound::PlayMorning_Sound()
{
	if(!Morning_Sound->isCurrentlyPlaying("sound/Morning.wav"))
	Morning_Sound->play2D("sound/Morning.wav", false);
}

void CSound:: PlayGrowUp_Sound()
{
	if(!GrowUp_Sound->isCurrentlyPlaying("sound/GrowUp.mp3"))
		GrowUp_Sound->play2D("sound/GrowUp.mp3", false);
}
void CSound::PlayCropGet_Sound()
{
	if(!CropGet_Sound->isCurrentlyPlaying("sound/CropGet.wav"))
		CropGet_Sound->play2D("sound/CropGet.wav", false);
}

void CSound::Gameover_Stop()
{
	Gameover_Sound->stopAllSounds();
}

void CSound::Gameclear_Stop()
{
	Gameclear_Sound->stopAllSounds();
}

void CSound::Title_BGM_Stop()
{
	Title_BGM->drop();
}

void CSound::Stage1_BGM_Stop()
{
	Stage1_BGM->stopAllSounds();
}

void CSound::Walk_Sound_Stop()
{
	Walk_Sound->stopAllSounds();
}

