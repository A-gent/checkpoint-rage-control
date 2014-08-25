/**
 * vim: set ts=4 :
 * =============================================================================
 * Checkpoint Rage Control
 * Copyright (C) 2014 Michael Busby  All rights reserved.
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * As a special exception, AlliedModders LLC gives you permission to link the
 * code of this program (as well as its derivative works) to "Half-Life 2," the
 * "Source Engine," the "SourcePawn JIT," and any Game MODs that run on software
 * by the Valve Corporation.  You must obey the GNU General Public License in
 * all respects for all other code used.  Additionally, AlliedModders LLC grants
 * this exception to all derivative works.  AlliedModders LLC defines further
 * exceptions, found in LICENSE.txt (as of this writing, version JULY-31-2007),
 * or <http://www.sourcemod.net/license.php>.
 *
 * Version: $Id$
 */

#include "extension.h"
#include "codepatch/patchmanager.h"
#include "codepatch/autopatch.h"
#include "saferoom_check_patch.h"
#include "codepatch/patchexceptions.h"

using SourceMod::IGameConfig;

CheckpointRageControl g_CheckpointRageControl;		/**< Global singleton for extension's main interface */

SMEXT_LINK(&g_CheckpointRageControl);

PatchManager s_PatchManager;

bool CheckpointRageControl::SDK_OnLoad(char *error, size_t maxlength, bool late)
{
	//load sigscans and offsets, etc from our gamedata file
	IGameConfig * gameConf;
	char conf_error[255] = "";
	if (!gameconfs->LoadGameConfigFile("checkpoint-rage-control", &gameConf, conf_error, sizeof(conf_error)))
	{
		if (conf_error[0])
		{
			snprintf(error, maxlength, "Could not read gamedata checkpoint-rage-control.txt: %s", conf_error);
		}
		return false;
	}

	try
	{
		s_PatchManager.Register(new SaferoomCheckPatch(gameConf));
		s_PatchManager.PatchAll();
	}
	catch(PatchException & e)
	{
		snprintf(error, maxlength, "Could not successfully apply all patches. Failure: %s", e.GetDescription());

		s_PatchManager.UnpatchAll();
		return false;
	}

	return true;
}

void CheckpointRageControl::SDK_OnUnload()
{
	s_PatchManager.UnpatchAll();
	s_PatchManager.UnregisterAll();
}