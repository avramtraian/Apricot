// Part of Apricot Engine. 2022-2022.

#pragma once

#include "Base.h"
#include "Types.h"

#include "Memory/Allocator.h"

namespace Apricot {
	
	/*
	* Apricot Engine
	* 
	* C++ Core engine architecture. The base class for all engine objects.
	* 
	* All engine classes must publicly inherit from this.
	*/
	class APRICOT_API AObject
	{
	public:
		/*
		* Get & Set the allocator used for allocating / freeing the object.
		* These should never be called outside the smart pointers.
		*/
		AAllocatorBase* GetAllocator() const { return m_Allocator; }
		void SetAllocator(AAllocatorBase* Allocator) { m_Allocator = Allocator; }

		/*
		* Get the object's reference count.
		*/
		uint64 GetReferenceCount() const { return m_ReferenceCount; }

		/*
		* Used for manipulating the object's reference count.
		* These should never be called outside the smart pointers.
		*/
		void IncreaseReferenceCount() { m_ReferenceCount++; }
		void DecreaseReferenceCount() { m_ReferenceCount--; }

	private:
		/*
		* Object's reference count. When it hits 0, the smart pointers will free the object.
		*/
		uint64 m_ReferenceCount = 0;

		/*
		* Allocator used for allocating the object's block of memory. Must be kept in order to free the memory on deletion of the object.
		*/
		AAllocatorBase* m_Allocator = nullptr;

		/* TODO: Should not be needed anymore? */
		template<typename T>
		friend class TSharedPtr;
		template<typename T>
		friend class TSharedRef;
	};

}