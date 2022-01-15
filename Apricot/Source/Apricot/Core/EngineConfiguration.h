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

	private:
		EngineConfig() = default;
		EngineConfig(const EngineConfig&) = delete;
		EngineConfig& operator=(const EngineConfig&) = delete;

	public:
		Version EngineVersion = Version(0, 0, 1);
		Version GameVersion = Version(0, 0, 1);

		// NOTE: In order to not pollute a lot of engine code with rendering stuff, we can't include "Renderer.h".
		// So we have to store the rendering API like this.
		// This translates to RenderingAPI::None.
		uint8 RenderingAPI = 0;

		bool8 bStartWithConsole = true;

		friend class Engine;
	};

	class APRICOT_API ConfigSerializer
	{
	public:
		static bool8 ReadGlobalEngineConfig(const Filesystem::Path256& filepath);
		static bool8 WriteGlobalEngineConfig(const Filesystem::Path256& filepath);
	};


	//////////////////////////---- AVAILABLE FLAGS ----//////////////////////////

}