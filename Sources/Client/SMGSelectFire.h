#pragma once

#include "Player.h"

namespace spades {
	namespace SMGSF {

		enum SMGMode { FULL_AUTO, SLOW_AUTO, BURST, SINGLE };

		class SMGFireSelector {
			private:
				SMGMode currentFireMode;
				float lastShotTime;
				bool singleFired;

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

				void FireInterval(spades::client::Player * player, float time,
						spades::client::WeaponInput& winp);
				void PlaySwitchSound();


		};
	}
}
