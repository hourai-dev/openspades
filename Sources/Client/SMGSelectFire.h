#pragma once

#include <string>
#include <vector>

#include "Player.h"
#include "IAudioDevice.h"

namespace spades {
	namespace SMGSF {

		enum SMGMode { FULL_AUTO = 0, SLOW_AUTO = 1, BURST = 2, SINGLE = 3 };

		class SMGFireSelector {
			private:
				SMGMode currentFireMode;
				float lastShotTime;
				bool singleFired;
				const std::vector<std::string> modeStr = {"FULL", "SLOW", "BURST", "SINGLE"};

			public:

				SMGFireSelector() :
					currentFireMode(FULL_AUTO),
					lastShotTime(0.f),
					singleFired(false) {}

				float GetInterval();
				float GetDelay();
				SMGMode GetMode();
				void SetMode(const SMGMode newMode);
				void NextMode();
				std::string GetModeStr();

				void FireInterval(spades::client::Player * player, float time,
						spades::client::WeaponInput& winp);
				void PlaySwitchSound(spades::client::IAudioDevice * audioDevice);


		};
	}
}
