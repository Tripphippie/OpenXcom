/*
 * Copyright 2010 OpenXcom Developers.
 *
 * This file is part of OpenXcom.
 *
 * OpenXcom is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenXcom is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenXcom.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef OPENXCOM_BATTLESTATE_H
#define OPENXCOM_BATTLESTATE_H

#include <string>

namespace OpenXcom
{

class BattlescapeState;

/**
 * This class sets the battlescape in a certain sub-state.
 * These states can be triggered by the player or the AI.
 */
class BattleState
{
protected:
	BattlescapeState *_parent;
	std::string _result;
public:
	/// Creates a new BattleState linked to the game.
	BattleState(BattlescapeState *parent);
	/// Cleans up the BattleState.
	virtual ~BattleState();
	/// Initializes the state. Returns false when finished.
	virtual bool init();
	/// Handles a cancels request.
	virtual void cancel();
	/// Runs state functionality every cycle. Returns false when finished.
	virtual bool think();
	/// Get the result of the state.
	std::string getResult() const;
};

}

#endif
