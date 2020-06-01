#include "Client.h"
#include "SMGSelectFire.h"
#include "Player.h"
#include "World.h"
#include "Weapon.h"
#include "IAudioDevice.h"
#include <Core/Debug.h>
#include <Core/Math.h>

float spades::SMGSF::SMGFireSelector::GetInterval()
{
	switch (currentFireMode) {
		case FULL_AUTO:
			return 0.1f;
			break;
		case SLOW_AUTO:
			return 0.16f;
			break;
		case BURST:
			return .50f;
			break;
		case SINGLE:
			return .2f;
			break;
		default:
			return 0.1f;
			SPLog("Falling back to FULL_AUTO fire interval.");
			break;
	}
}

float spades::SMGSF::SMGFireSelector::GetDelay()
{
	switch (currentFireMode) {
		case FULL_AUTO:
			return 0.0f;
			break;
		case SLOW_AUTO:
			return 0.05f;
			break;
		case BURST:
			return 0.295f;
			break;
		case SINGLE:
			return 0.12f;
			break;
		default:
			return 0.0f;
			SPLog("Falling back to FULL_AUTO fire delay.");
			break;
	}
}
spades::SMGSF::SMGMode spades::SMGSF::SMGFireSelector::GetMode()
{
	return currentFireMode;
}

void spades::SMGSF::SMGFireSelector::SetMode(const SMGMode newMode)
{
	currentFireMode = newMode;
}

void spades::SMGSF::SMGFireSelector::NextMode()
{
	switch(currentFireMode) {
		case FULL_AUTO:
			currentFireMode = SLOW_AUTO;
			break;
		case SLOW_AUTO:
			currentFireMode = BURST;
			break;
		case BURST:
			SPLog("SINGLE");
			currentFireMode = SINGLE;
			break;
		case SINGLE:
			currentFireMode = FULL_AUTO;
			break;
	}
}

std::string spades::SMGSF::SMGFireSelector::GetModeStr()
{
	return modeStr[currentFireMode];
}

void spades::SMGSF::SMGFireSelector::
FireInterval(spades::client::Player * player, float time, spades::client::WeaponInput& winp)
{
	if (currentFireMode == FULL_AUTO)
		return;

	float delta = time - lastShotTime;
	if (currentFireMode == SLOW_AUTO || currentFireMode == BURST) {
		if (winp.primary && delta >= GetInterval()) {
			winp.primary = true;
			lastShotTime = time;
			SPLog("%f", delta);
		} else if (winp.primary && delta <= GetInterval() && delta <= GetDelay()) {
			winp.primary = true;
		} else {
			winp.primary = false;
		}
	}

	if (currentFireMode == BURST && !winp.primary && delta <= GetDelay()) {
		winp.primary = true;
	}

	if (currentFireMode == SINGLE) {
		if (winp.primary && !singleFired) {
			winp.primary = true;
			singleFired = true;
		} else if (winp.primary && singleFired)
			winp.primary = false;
		else if (!winp.primary && singleFired) {
			winp.primary = false;
			singleFired = false;
		}
		
		if (!winp.primary && delta <= GetDelay())
			winp.primary = true;
	}

}

void spades::SMGSF::SMGFireSelector::PlaySwitchSound(spades::client::IAudioDevice * audioDevice)
{
	auto c = audioDevice->RegisterSound("Sounds/Weapons/SMG/RaiseLocal.opus");
	audioDevice->PlayLocal(c, MakeVector3(.4f, -.3f, .5f), spades::client::AudioParam());
}
