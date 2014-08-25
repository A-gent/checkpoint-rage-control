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
#include "saferoom_check_patch.h"
#include "codepatch/patchexceptions.h"
#include "misc_asm.h"

                                 // xor eax,eax; NOP_3;
const BYTE CHECK_REPLACEMENT[] = {0x31, 0xC0, 0x0f,0x1f,0x00};

BYTE * FindTarget(IGameConfig * gameConf)
{
	BYTE * addr;
	if(!gameConf->GetMemSig("CTerrorPlayer::UpdateZombieFrustration", (void**)&addr))
	{
		throw new PatchException("Couldn't find UpdateZombieFrustration in memory!");
	}

	int offset;
	if(!gameConf->GetOffset("UpdateZombieFrustration_SaferoomCheck", &offset))
	{
		throw new PatchException("Couldn't read Saferoom Check Offset!");
	}

	if(*addr != JMP_8_OPCODE)
	{
		throw new PatchException("Saferoom Check offset seems to be incorrect");
	}

	return addr;
}

SaferoomCheckPatch::SaferoomCheckPatch(IGameConfig * gameConf) 
	: m_Patch(FindTarget(gameConf), CHECK_REPLACEMENT)
{
}

void SaferoomCheckPatch::Patch()
{
	m_Patch.Patch();
}

void SaferoomCheckPatch::Unpatch()
{
	m_Patch.Unpatch();
}