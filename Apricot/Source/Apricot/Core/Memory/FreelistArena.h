// Part of Apricot Engine. 2022-2022.
// Module: Memory

#pragma once

#include "ApricotMemory.h"
#include "Apricot/Core/AClass.h"

namespace Apricot {
	
	/**
	* C++ Core Engine Architecture
	* 
	* Freelist Arena implementation
	*/
	class APRICOT_API AFreelistArena : public AMemoryArena
	{
		ACLASS_CORE()

	public:
		NODISCARD static TSharedPtr<AFreelistArena> Create();
	
	/* Constructors & Deconstructor */
	private:
		AFreelistArena();
		virtual ~AFreelistArena() override;

		AFreelistArena(const AFreelistArena&) = delete;
		AFreelistArena&operator=(const AFreelistArena&) = delete;
		AFreelistArena(AFreelistArena&&) = delete;
		AFreelistArena& operator=(AFreelistArena&&) = delete;
	
	/* Typedefs */
	public:
		

	/* API */
	public:
		

	/* Getters & Setters */
	public:
		

	/* Member variables */
	private:
		

	/* Friends */
	private:
		template<typename T, typename... Args>
		friend constexpr T* MemConstruct(void*, Args&&...);
	};

}