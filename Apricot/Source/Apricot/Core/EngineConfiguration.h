// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Base.h"
#include "Apricot/Filesystem/Path.h"

namespace Apricot {
	
	struct Version
	{
		Version() = default;
		Version(uint32 major, uint32 minor, uint32 patch)
			: Major(major), Minor(minor), Patch(patch) {}

		uint32 Major = 0;
		uint32 Minor = 0;
		uint32 Patch = 0;
	};

	class APRICOT_API EngineConfig
	{
	public:
		static void Init(const char8* commandLine);
		static void Destroy();

		static void Flush();

	private:
		EngineConfig() = default;
		EngineConfig(const EngineConfig&) = delete;
		EngineConfig& operator=(const EngineConfig&) = delete;

	public:
		Version EngineVersion = Version(0, 0, 1);
		Version GameVersion = Version(0, 0, 1);

		/*
		* NOTE: In order to not pollute a lot of core-engine code with rendering stuff, we can't include "Renderer.h".
		* So we have to store the rendering API as an uint8.
		* The default value translates to RenderingAPI::None.
		*/
		uint8 RenderingAPI = 0;
		bool8 bHasChanged_RenderingAPI = true;

		/*
		* This flag is evaluated only on the initialization of the engine.
		* If AE_ENABLE_CONSOLE isn't defined, this does nothing at all.
		*/
		bool8 bStartWithConsole = true;
		bool8 bHasChanged_bStartWithConsole = true;

		friend class Engine;
	};

	//////////////////////////---- AVAILABLE FLAGS ----//////////////////////////

}