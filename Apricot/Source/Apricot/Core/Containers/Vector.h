#pragma once

#include "Apricot/Core/Core.h"
#include "Apricot/Core/Assert.h"
#include "Apricot/Core/Memory.h"

#include "ArrayIterator.h"

namespace Apricot {

	/**
	* C++ Apricot Vector Container Implementation.
	* 
	* @tparam ElementType Type of the array elements.
	* @tparam AllocatorType Type of the allocator instance.
	*/
	template<typename ElementType, typename AllocatorType = HeapAllocator>
	class Vector
	{
	public:
		//// Iterator Typedefs ////
		using Iterator             = ArrayIterator<ElementType>;
		using ConstIterator        = ArrayIterator<const ElementType>;
		using ReverseIterator      = ArrayIterator<ElementType>;
		using ReverseConstIterator = ArrayIterator<const ElementType>;

		using PFN_Equal = bool(*)(const ElementType& E1, const ElementType& E2);

		/** Used in Find */
		static constexpr SizeType InvalidIndex = static_cast<SizeType>(-1);

	public:
		/**
		* Constructor, sets counter variables to 0.
		*/
		Vector()
			: m_Data(nullptr)
			, m_Capacity(0)
			, m_Size(0)
		{
		}

		/**
		* Copy constructor.
		* 
		* @param Other Vector to copy.
		*/
		Vector(const Vector& Other)
			: m_Data(nullptr)
			, m_Capacity(0)
			, m_Size(0)
		{
			Construct(Other.m_Data, Other.m_Size);
		}

		/**
		* Copy constructor. Different allocator types.
		* 
		* @param Other Vector to copy.
		*/
		template<typename OtherAllocatorType>
		Vector(const Vector<ElementType, OtherAllocatorType>& Other)
		{
			Construct(Other.m_Data, Other.m_Size);
		}

		/**
		* Move constructor.
		* 
		* @param Other Vector to move.
		*/
		Vector(Vector&& Other) noexcept
			: m_Data(nullptr)
			, m_Capacity(0)
			, m_Size(0)
		{
			MoveConstruct<AllocatorType>(AE::Move(Other));
		}

		/**
		* Move constructor. Different allocator types.
		* 
		* @param Other Vector to move.
		*/
		template<typename OtherAllocatorType>
		Vector(Vector<ElementType, OtherAllocatorType>&& Other) noexcept
			: m_Data(nullptr)
			, m_Capacity(0)
			, m_Size(0)
		{
			MoveConstruct<OtherAllocatorType>(Other);
		}

		/**
		* std::initializer_list constructor.
		* 
		* @param InitList IntializerList to copy.
		*/
		Vector(std::initializer_list<ElementType> InitList)
			: m_Data(nullptr)
			, m_Capacity(0)
			, m_Size(0)
		{
			const ElementType* OtherData = InitList.begin();
			SizeType OtherSize = InitList.end() - InitList.begin();

			Construct(OtherData, OtherSize);
		}

		/**
		* Construct from a raw array of elements.
		* 
		* @param Elements The raw array to copy.
		* 
		* @param Count The size of the raw array.
		*/
		Vector(const ElementType* Elements, SizeType Count)
			: m_Data(nullptr)
			, m_Capacity(0)
			, m_Size(0)
		{
			Construct(Elements, Count);
		}

		/**
		* Destructor.
		*/
		~Vector()
		{
			Clear();
			FreeData();
		}
	
	public:
		/**
		* Assignment operator.
		* 
		* @param Other Vector to copy.
		* 
		* @returns Reference to this.
		*/
		Vector& operator=(const Vector& Other)
		{
			Assign(Other.m_Data, Other.m_Size);
			return *this;
		}

		/**
		* Assignment operator. Different allocator types.
		* 
		* @param Other Vector to copy.
		* 
		* @returns Reference to this.
		*/
		template<typename OtherAllocatorType>
		Vector& operator=(const Vector<ElementType, OtherAllocatorType>& Other)
		{
			Assign(Other.m_Data, Other.m_Size);
			return *this;
		}

		/**
		* Move assignment operator.
		* 
		* @param Other Vector to move.
		* 
		* @returns Reference to this.
		*/
		Vector& operator=(Vector&& Other) noexcept
		{
			MoveAssign<AllocatorType>(AE::Move(Other));
			return *this;
		}

		/**
		* Move assignment operator. Different allocator types.
		* 
		* @param Other Vector to move.
		* 
		* @returns Reference to this.
		*/
		template<typename OtherAllocatorType>
		Vector& operator=(Vector<ElementType, OtherAllocatorType>&& Other) noexcept
		{
			MoveAssign<OtherAllocatorType>(Other);
			return *this;
		}

		/**
		* std::initializer_list assignment operator.
		* 
		* @param InitializerList to copy.
		* 
		* @returns Reference to this.
		*/
		Vector& operator=(std::initializer_list<ElementType> InitList)
		{
			const ElementType* OtherData = InitList.begin();
			SizeType OtherSize = InitList.end() - InitList.begin();

			Assign(OtherData, OtherSize);
			return *this;
		}

	public:
		/**
		* Array bracket operator. Returns a reference to the element at the given index.
		* 
		* @param Index Index of the element. Must be in range.
		* 
		* @returns Reference to the element at the given Index.
		*/
		ElementType& operator[](SizeType Index)
		{
			AE_CORE_ASSERT(IsValidIndex(Index));
			return m_Data[Index];
		}

		/**
		* Array bracket operator. Returns a reference to the element at the given index.
		* Const version of the above.
		* 
		* @param Index Index of the element. Must be in range.
		*
		* @returns Reference to the element at the given Index.
		*/
		const ElementType& operator[](SizeType Index) const
		{
			AE_CORE_ASSERT(IsValidIndex(Index));
			return m_Data[Index];
		}

		/**
		* Equality operator.
		* 
		* @param Other Vector to compare.
		* 
		* @returns True if the vectors are the same. False otherwise.
		*/
		template<typename OtherAllocatorType>
		bool operator==(const Vector<ElementType, OtherAllocatorType>& Other) const
		{
			if (m_Size != Other.m_Size)
			{
				return false;
			}

			for (SizeType Index = 0; Index < m_Size; Index++)
			{
				if (m_Data[Index] != Other.m_Data[Index])
				{
					return false;
				}
			}

			return true;
		}

		/**
		* Inequality operator.
		* 
		* @param Other Vector to compare.
		*
		* @returns True if the vectors are NOT the same. False otherwise.
		*/
		template<typename OtherAllocatorType>
		bool operator!=(const Vector<ElementType, OtherAllocatorType>& Other) const
		{
			return !(*this == Other);
		}

		/**
		* Appends the specified vector to this array.
		* 
		* @param Other Vector to append.
		* 
		* @return Reference to this.
		*/
		Vector& operator+=(const Vector& Other)
		{
			Append(Other);
			return *this;
		}

		/**
		* Appends the specified vector to this array.
		* Move semantics version.
		* 
		* @param Other Vector to append.
		*
		* @return Reference to this.
		*/
		Vector& operator+=(Vector&& Other)
		{
			Append(AE::Move(Other));
			return *this;
		}

	public:
		/**
		* Gives a pointer to the first array element.
		* 
		* @returns Pointer to the first element in the array.
		*/
		ElementType* Data() const { return m_Data; }

		/**
		* Gives the number of elements that the vector can store without reallocating.
		* 
		* @returns The number of elements that the vector can store without reallocating.
		*/
		SizeType Capacity() const { return m_Capacity; }

		/**
		* Gives the current number of allocated elements.
		* 
		* @returns The current number of allocated elements in the array.
		*/
		SizeType Size() const { return m_Size; }

		/**
		* Checks if the vector doesn't have any valid elements.
		* 
		* @returns True if the vector has no allocated elements. False otherwise.
		*/
		bool IsEmpty() const { return m_Size == 0; }

		/**
		* Checks if the given Index is in vector's range.
		* 
		* @param Index The index to be checked.
		* 
		* @returns True if the Index is in range. False otherwise.
		*/
		bool IsValidIndex(SizeType Index) const { return Index < m_Size; }

		/**
		* Gives the current allocator instance.
		* 
		* @returns Reference to the allocator instance.
		*/
		AllocatorType& GetAllocatorInstance() { return m_Allocator; }

		/**
		* Gives the current allocator instance.
		* Const version of the above.
		* 
		* @returns Const reference to the allocator instance.
		*/
		const AllocatorType& GetAllocatorInstance() const { return m_Allocator; }

	public:
		/**
		* Adds an element to the end of the array. It will reallocate if needed.
		* 
		* @param Element Element to be added.
		* 
		* @returns The index where the element was placed. It will be the last valid index.
		*/
		SizeType Add(const ElementType& Element)
		{
			if (m_Size >= m_Capacity)
			{
				ReAllocate(m_Capacity + m_Capacity / 2 + 1);
			}

			new(&m_Data[m_Size++]) ElementType(Element);
			return m_Size - 1;
		}

		/**
		* Adds an element to the end of the array. It will reallocate if needed.
		*
		* @param Element Element to be added.
		*
		* @returns Reference to the newly added element.
		*/
		ElementType& AddGetRef(const ElementType& Element)
		{
			SizeType Index = Add(Element);
			return m_Data[Index];
		}

		/**
		* Adds an element to the end of the array. It will reallocate if needed.
		* Move semantics version.
		* 
		* @param Element Element to be added.
		*
		* @returns The index where the element was placed. It will be the last valid index.
		*/
		SizeType Add(ElementType&& Element)
		{
			if (m_Size >= m_Capacity)
			{
				ReAllocate(m_Capacity + m_Capacity / 2 + 1);
			}

			new(&m_Data[m_Size++]) ElementType(AE::Move(Element));
			return m_Size - 1;
		}

		/**
		* Adds an element to the end of the array. It will reallocate if needed.
		* Move semantics version.
		*
		* @param Element Element to be added.
		*
		* @returns Reference to the newly added element.
		*/
		ElementType& AddGetRef(ElementType&& Element)
		{
			SizeType Index = Add(AE::Move(Element));
			return m_Data[Index];
		}

		/**
		* Adds an element(or elements) to the end of the array. It will reallocate if needed.
		* The element(s) are initialized with the default constructor.
		* 
		* @params Count The number of elements to add.
		* 
		* @returns The index where the first element was placed.
		*/
		SizeType AddDefaulted(SizeType Count)
		{
			AE_CORE_ASSERT(Count > 0);

			if (m_Size + Count > m_Capacity)
			{
				SizeType NewCapacity = m_Capacity + m_Capacity / 2 + 1;
				ReAllocate(NewCapacity >= m_Size + Count ? NewCapacity : m_Size + Count);
			}

			for (SizeType Index = 0; Index < Count; Index++)
			{
				new(m_Data + m_Size + Index) ElementType();
			}

			m_Size += Count;
			return m_Size - Count;
		}

		/**
		* Adds an element(or elements) to the end of the array. It will reallocate if needed.
		* The element(s) are initialized with the default constructor.
		*
		* @returns Reference to the newly added element.
		*/
		ElementType& AddDefaultedGetRef()
		{
			SizeType Index = AddDefaulted(1);
			return m_Data[Index];
		}

		/**
		* Adds an element(or elements) to the end of the array. It will reallocate if needed.
		* Their memory is set to 0.
		* 
		* @params Count The number of elements to add.
		*
		* @returns The index where the first element was placed.
		*/
		SizeType AddZeroed(SizeType Count)
		{
			AE_CORE_ASSERT(Count > 0);

			if (m_Size + Count > m_Capacity)
			{
				SizeType NewCapacity = m_Capacity + m_Capacity / 2 + 1;
				ReAllocate(NewCapacity >= m_Size + Count ? NewCapacity : m_Size + Count);
			}

			MemSet(m_Data + m_Size, 0, Count * sizeof(ElementType));

			m_Size += Count;
			return m_Size - Count;
		}

		/**
		* Adds an element(or elements) to the end of the array. It will reallocate if needed.
		* Their memory is set to 0.
		*
		* @returns Reference to the newly added element.
		*/
		ElementType& AddZeroedGetRef()
		{
			SizeType Index = AddZeroed(1);
			return m_Data[Index];
		}

		/**
		* Adds an element(or elements) to the end of the array. It will reallocate if needed.
		* The element(s) are NOT initialized with anything.
		* 
		* @params Count The number of elements to add.
		*
		* @returns The index where the first element was placed.
		*/
		SizeType AddUninitialized(SizeType Count)
		{
			AE_CORE_ASSERT(Count > 0);

			if (m_Size + Count > m_Capacity)
			{
				SizeType NewCapacity = m_Capacity + m_Capacity / 2 + 1;
				ReAllocate(NewCapacity >= m_Size + Count ? NewCapacity : m_Size + Count);
			}

			m_Size += Count;
			return m_Size - Count;
		}

		/**
		* Adds an element(or elements) to the end of the array. It will reallocate if needed.
		* The element(s) are NOT initialized with anything.
		*
		* @returns Reference to the newly added element.
		*/
		ElementType& AddUninitializedGetRef()
		{
			SizeType Index = AddUninitialized(1);
			return m_Data[Index];
		}

		/**
		* Adds an element to the end of the array, ONLY if it doesn't already exist. It will reallocate if needed.
		* 
		* @param Element Element to be added.
		*
		* @returns The index where the element was placed, if it didn't exist. Otherwise, the index of the first element found to be equal.
		*/
		SizeType AddUnique(const ElementType& Element)
		{
			for (SizeType Index = 0; Index < m_Size; Index++)
			{
				if (m_Data[Index] == Element)
				{
					return Index;
				}
			}

			Add(Element);
			return m_Size - 1;
		}

		/**
		* Adds an element to the end of the array, ONLY if it doesn't already exist. It will reallocate if needed.
		*
		* @param Element Element to be added.
		*
		* @returns Reference to the newly added element, if it didn't exist. Otherwise, reference to the first element found to be equal.
		*/
		ElementType& AddUniqueGetRef(const ElementType& Element)
		{
			SizeType Index = AddUnique(Element);
			return m_Data[Index];
		}

		/**
		* Adds an element to the end of the array, ONLY if it doesn't already exist. It will reallocate if needed.
		* Move semantics version.
		* 
		* @param Element Element to be added.
		*
		* @returns The index where the element was placed, if it didn't exist. Otherwise, the index of the first element found to be equal.
		*/
		SizeType AddUnique(ElementType&& Element)
		{
			for (SizeType Index = 0; Index < m_Size; Index++)
			{
				if (m_Data[Index] == Element)
				{
					return Index;
				}
			}

			Add(AE::Move(Element));
			return m_Size - 1;
		}

		/**
		* Adds an element to the end of the array, ONLY if it doesn't already exist. It will reallocate if needed.
		* Move semantics version.
		*
		* @param Element Element to be added.
		*
		* @returns Reference to the newly added element, if it didn't exist. Otherwise, reference to the first element found to be equal.
		*/
		ElementType& AddUniqueGetRef(ElementType&& Element)
		{
			SizeType Index = AddUnique(AE::Move(Element));
			return m_Data[Index];
		}

		/**
		* Adds an element to the end of the array, ONLY if it doesn't already exist. It will reallocate if needed.
		* It uses user-defined equality function.
		*
		* @param Element Element to be added.
		* 
		* @param EqualFunc Function checking for equality.
		*
		* @returns The index where the element was placed, if it didn't exist. Otherwise, the index of the first element found to be equal.
		*/
		SizeType AddUnique(const ElementType& Element, PFN_Equal EqualFunc)
		{
			for (SizeType Index = 0; Index < m_Size; Index++)
			{
				if (EqualFunc(Element, m_Data[Index]))
				{
					return Index;
				}
			}

			Add(Element);
			return m_Size - 1;
		}

		/**
		* Adds an element to the end of the array, ONLY if it doesn't already exist. It will reallocate if needed.
		* It uses user-defined equality function.
		*
		* @param Element Element to be added.
		*
		* @param EqualFunc Function checking for equality.
		*
		* @returns Reference to the newly added element, if it didn't exist. Otherwise, reference to the first element found to be equal.
		*/
		ElementType& AddUniqueGetRef(const ElementType& Element, PFN_Equal EqualFunc)
		{
			SizeType Index = AddUnique(Element, EqualFunc);
			return m_Data[Index];
		}

		/**
		* Adds an element to the end of the array, ONLY if it doesn't already exist. It will reallocate if needed.
		* It uses user-defined equality function.
		* Move semantics version.
		*
		* @param Element Element to be added.
		* 
		* @param EqualFunc Function checking for equality.
		*
		* @returns The index where the element was placed, if it didn't exist. Otherwise, the index of the first element found to be equal.
		*/
		SizeType AddUnique(ElementType&& Element, PFN_Equal EqualFunc)
		{
			for (SizeType Index = 0; Index < m_Size; Index++)
			{
				if (EqualFunc(Element, m_Data[Index]))
				{
					return Index;
				}
			}

			Add(AE::Move(Element));
			return m_Size - 1;
		}

		/**
		* Adds an element to the end of the array, ONLY if it doesn't already exist. It will reallocate if needed.
		* It uses user-defined equality function.
		* Move semantics version.
		*
		* @param Element Element to be added.
		* 
		* @param EqualFunc Function checking for equality.
		*
		* @returns Reference to the newly added element, if it didn't exist. Otherwise, reference to the first element found to be equal.
		*/
		ElementType& AddUniqueGetRef(ElementType&& Element, PFN_Equal EqualFunc)
		{
			SizeType Index = AddUnique(AE::Move(Element), EqualFunc);
			return m_Data[Index];
		}

		/**
		* Inserts an element to a given Index. It will reallocate if needed.
		* 
		* @param Element Element to be inserted.
		* 
		* @param Index Index where the element will be placed.
		*			It can't be bigger than the vector number of allocated elements.
		* 
		* @returns The index where the element was placed. It will be the same as the parameter Index.
		*/
		SizeType Insert(const ElementType& Element, SizeType Index)
		{
			AE_CORE_ASSERT(Index <= m_Size);
			ShiftElementsRight(Index, 1);
			m_Data[Index] = Element;
			return Index;
		}

		/**
		* Inserts an element to a given Index. It will reallocate if needed.
		*
		* @param Element Element to be inserted.
		*
		* @param Index Index where the element will be placed.
		*			It can't be bigger than the vector number of allocated elements.
		*
		* @returns Reference to the newly inserted element.
		*/
		ElementType& InsertGetRef(const ElementType& Element, SizeType Index)
		{
			SizeType Idx = Insert(Element, Index);
			return m_Data[Idx];
		}

		/**
		* Inserts an element to a given Index. It will reallocate if needed.
		* Move semantics version.
		* 
		* @param Element Element to be inserted.
		*
		* @param Index Index where the element will be placed.
		*			It can't be bigger than the vector number of allocated elements.
		*
		* @returns The index where the element was placed. It will be the same as the parameter Index.
		*/
		SizeType Insert(ElementType&& Element, SizeType Index)
		{
			AE_CORE_ASSERT(Index <= m_Size);
			ShiftElementsRight(Index, 1);
			m_Data[Index] = AE::Move(Element);
			return Index;
		}

		/**
		* Inserts an element to a given Index. It will reallocate if needed.
		* Move semantics version.
		*
		* @param Element Element to be inserted.
		*
		* @param Index Index where the element will be placed.
		*			It can't be bigger than the vector number of allocated elements.
		*
		* @returns Reference to the newly inserted element.
		*/
		ElementType& InsertGetRef(ElementType&& Element, SizeType Index)
		{
			SizeType Idx = Insert(AE::Move(Element), Index);
			return m_Data[Idx];
		}

		/**
		* Inserts an element(or elements) to a given Index. It will reallocate if needed.
		* The element(s) are initialized with the default constructor.
		* 
		* @param Count The number of elements(s) to be inserted. Must be greater than 0.
		* 
		* @param Index Index where the element will be placed.
		*			It can't be bigger than the vector number of allocated elements.
		* 
		* @returns The index where the first element was placed. It will be the same as the parameter Index.
		*/
		SizeType InsertDefaulted(SizeType Count, SizeType Index)
		{
			AE_CORE_ASSERT(Count > 0);
			ShiftElementsRight(Index, Count);

			for (SizeType Idx = Index; Idx < Index + Count; Idx++)
			{
				m_Data[Idx].~ElementType();
				new(m_Data + Idx) ElementType();
			}

			return Index;
		}

		/**
		* Inserts an element(or elements) to a given Index. It will reallocate if needed.
		* The element(s) are initialized with the default constructor.
		*
		* @param Index Index where the element will be placed.
		*			It can't be bigger than the vector number of allocated elements.
		*
		* @returns Reference to the newly inserted element.
		*/
		ElementType& InsertDefaultedGetRef(SizeType Index)
		{
			SizeType Idx = InsertDefaulted(1, Index);
			return m_Data[Idx];
		}

		/**
		* Inserts an element(or elements) to a given Index. It will reallocate if needed.
		* Their memory is set to 0.
		* 
		* @param Count The number of elements to be inserted. Must be greater than 0.
		*
		* @param Index Index where the element(s) will be placed.
		*			It can't be bigger than the vector number of allocated elements.
		*
		* @returns The index where the first element was placed. It will be the same as the parameter Index.
		*/
		SizeType InsertZeroed(SizeType Count, SizeType Index)
		{
			AE_CORE_ASSERT(Count > 0);
			ShiftElementsRight(Index, Count);

			for (SizeType Idx = Index; Idx < Index + Count; Idx++)
			{
				m_Data[Idx].~ElementType();
			}
			MemZero(m_Data + Index, Count * sizeof(ElementType));

			return Index;
		}

		/**
		* Inserts an element(or elements) to a given Index. It will reallocate if needed.
		* Their memory is set to 0.
		*
		* @param Index Index where the element will be placed.
		*			It can't be bigger than the vector number of allocated elements.
		*
		* @returns Reference to the newly inserted element.
		*/
		ElementType& InsertZeroedGetRef(SizeType Index)
		{
			SizeType Idx = InsertZeroed(1, Index);
			return m_Data[Idx];
		}

		/**
		* Inserts an element(or elements) to a given Index. It will reallocate if needed.
		* The element(s) are NOT initialized with anything.
		* 
		* @param Count The number of elements to be inserted. Must be greater than 0.
		*
		* @param Index Index where the element(s) will be placed.
		*			It can't be bigger than the vector number of allocated elements.
		*
		* @returns The index where the first element was placed. It will be the same as the parameter Index.
		*/
		SizeType InsertUninitialized(SizeType Count, SizeType Index)
		{
			AE_CORE_ASSERT(Count > 0);
			ShiftElementsRight(Index, Count);

			for (SizeType Idx = Index; Idx < Index + Count; Idx++)
			{
				m_Data[Idx].~ElementType();
			}

			return Index;
		}

		/**
		* Inserts an element(or elements) to a given Index. It will reallocate if needed.
		* The element(s) are NOT initialized with anything.
		* 
		* @param Index Index where the element will be placed.
		*			It can't be bigger than the vector number of allocated elements.
		* 
		* @returns Reference to the newly inserted element.
		*/
		ElementType& InsertUninitializedGetRef(SizeType Index)
		{
			SizeType Idx = InsertUninitialized(1, Index);
			return m_Data[Idx];
		}

		/**
		* Emplaces an element at the end of the array. It will reallocate if needed.
		* The element will be constructed with the passed arguments.
		*
		* @param args Templated argument pack. Will be forwarded to the ElementType constructor.
		*
		* @returns Index of the newly emplaced element. It will be the last valid index.
		*/
		template<typename... Args>
		SizeType Emplace(Args&&... args)
		{
			if (m_Size >= m_Capacity)
			{
				ReAllocate(m_Capacity + m_Capacity / 2 + 1);
			}

			new(&m_Data[m_Size++]) ElementType(AE::Forward<Args>(args)...);
			return m_Size - 1;
		}

		/**
		* Emplaces an element at the end of the array. It will reallocate if needed.
		* The element will be constructed with the passed arguments.
		* 
		* @param args Templated argument pack. Will be forwarded to the ElementType constructor.
		* 
		* @returns Reference to the newly emplaced element.
		*/
		template<typename... Args>
		ElementType& EmplaceGetRef(Args&&... args)
		{
			SizeType Index = Emplace(AE::Forward<Args>(args)...);
			return m_Data[Index];
		}

		/**
		* Appends the specified vector to this array.
		* 
		* @param Source Vector to append.
		*/
		template<typename OtherAllocatorType>
		void Append(const Vector<ElementType, OtherAllocatorType>& Source)
		{
			// Cannot append to self.
			AE_CORE_ASSERT(m_Data != Source.m_Data);

			if (Source.IsEmpty())
			{
				return;
			}

			if (m_Size + Source.m_Size > m_Capacity)
			{
				ReAllocate(m_Size + Source.m_Size);
			}

			for (SizeType Index = 0; Index < Source.m_Size; Index++)
			{
				new(m_Data + m_Size + Index) ElementType(Source.m_Data[Index]);
			}

			m_Size += Source.m_Size;
		}

		/**
		* Appends the specified vector to this array.
		* Move semantics version.
		* 
		* @param Source Vector to append.
		*/
		template<typename OtherAllocatorType>
		void Append(Vector<ElementType, OtherAllocatorType>&& Source)
		{
			// Cannot append to self.
			AE_CORE_ASSERT(m_Data != Source.m_Data);

			if (Source.IsEmpty())
			{
				return;
			}

			if (m_Size + Source.m_Size > m_Capacity)
			{
				ReAllocate(m_Size + Source.m_Size);
			}

			for (SizeType Index = 0; Index < Source.m_Size; Index++)
			{
				new(m_Data + m_Size + Index) ElementType(AE::Move(Source.m_Data[Index]));
			}

			m_Size += Source.m_Size;
			Source.Clear();
		}

		/**
		* Appends a raw array to this array.
		* 
		* @param Elements The raw array to append.
		* 
		* @param Count The raw array size.
		*/
		void Append(const ElementType* Elements, SizeType Count)
		{
			// Cannot append to self.
			AE_CORE_ASSERT(m_Data != Elements);

			if (Count == 0)
			{
				return;
			}

			if (m_Size + Count > m_Capacity)
			{
				ReAllocate(m_Size + Count);
			}

			for (SizeType Index = 0; Index < Count; Index++)
			{
				new(m_Data + m_Size + Index) ElementType(Elements[Index]);
			}

			m_Size += Count;
		}

		/**
		* Appends an std::initializer_list to this array.
		* 
		* @param InitList InitializerList to append.
		*/
		void Append(std::initializer_list<ElementType> InitList)
		{
			ElementType* InitListData = InitList.begin();
			SizeType InitListSize = InitList.end() - InitList.begin();

			Append(InitListData, InitListSize);
		}

		/**
		* Removes the element(s) at the given Index. It can shrink the array.
		* 
		* @param Index Index of the first element to remove.
		* 
		* @param Count The number of elements to remove.
		* 
		* @param bAllowShrinking Whether or not the array should shrink if the number of allocated
		*			elements is much smaller than the number of elements the array can store.
		*			Default value is set to false.
		*/
		void RemoveAt(SizeType Index, SizeType Count, bool bAllowShrinking = false)
		{
			ShiftElementsLeft(Index, Count);
			if (bAllowShrinking && m_Size < m_Capacity / 2)
			{
				ReAllocate(m_Size + m_Size / 3);
			}
		}

		/**
		* Removes the element at the given Index.
		* 
		* @param Index Index of the element to remove.
		*/
		void RemoveAt(SizeType Index)
		{
			RemoveAt(Index, 1, false);
		}

		/**
		* Pops the last element(s). It can shrink the array.
		* 
		* @param Count The number of elements to pop. Default value set to 1.
		* 
		* @param bAllowShrinking Whether or not the array should shrink if the number of allocated
		*			elements is much smaller than the number of elements the array can store.
		*			Default value is set to false.
		*/
		void Pop(SizeType Count = 1, bool bAllowShrinking = false)
		{
			AE_CORE_ASSERT(Count <= m_Size);

			for (SizeType Index = 0; Index < Count; Index++)
			{
				m_Data[m_Size - Count + Index].~ElementType();
			}
			m_Size -= Count;

			if (bAllowShrinking && m_Size < m_Capacity / 2)
			{
				ReAllocate(m_Size + m_Size / 3);
			}
		}

		/**
		* Gives the first element in the array.
		* 
		* @returns Reference to the first element in the array.
		*/
		ElementType& Front()
		{
			AE_CORE_ASSERT(m_Size > 0);
			return m_Data[0];
		}

		/**
		* Gives the first element in the array.
		* Const version of the above.
		* 
		* @returns Const reference to the first element in the array.
		*/
		const ElementType& Front() const
		{
			AE_CORE_ASSERT(m_Size > 0);
			return m_Data[0];
		}

		/**
		* Gives the last element in the array.
		* 
		* @returns Reference to the last element in the array.
		*/
		ElementType& Back()
		{
			AE_CORE_ASSERT(m_Size > 0);
			return m_Data[m_Size - 1];
		}

		/**
		* Gives the last element in the array.
		* Const version of the above.
		* 
		* @returns Const reference to the last element in the array.
		*/
		const ElementType& Back() const
		{
			AE_CORE_ASSERT(m_Size > 0);
			return m_Data[m_Size - 1];
		}

		/**
		* Gives the first equal element index. It returns InvalidIndex (-1) if the Element is not found.
		* 
		* @param Element Element to search for.
		* 
		* @returns The index of the first found element, or InvalidIndex if none is found.
		*/
		SizeType Find(const ElementType& Element) const
		{
			for (SizeType Index = 0; Index < m_Size; Index++)
			{
				if (m_Data[Index] == Element)
				{
					return Index;
				}
			}

			return InvalidIndex;
		}

		/**
		* Gives the first equal element index. It returns InvalidIndex (-1) if the Element is not found.
		* It uses a user-defined equality function.
		* 
		* @param Element Element to search for
		* 
		* @param EqualFunc Function checking for equality.
		* 
		* @returns The index of the last found element, or InvalidIndex if none is found.
		*/
		SizeType Find(const ElementType& Element, PFN_Equal EqualFunc)
		{
			for (SizeType Index = 0; Index < m_Size; Index++)
			{
				if (EqualFunc(Element, m_Data[Index]))
				{
					return Index;
				}
			}

			return InvalidIndex;
		}

		/**
		* Gives the last equal element index. It returns InvalidIndex (-1) if the Element is not found.
		* 
		* @param Element Element to search for.
		* 
		* @returns The index of the last found element, or InvalidIndex if none is found.
		*/
		SizeType FindLast(const ElementType& Element) const
		{
			for (SizeType Index = m_Size; Index > 0; Index--)
			{
				if (m_Data[(Index - 1)] == Element)
				{
					return (Index - 1);
				}
			}

			return InvalidIndex;
		}

		/**
		* Gives the last equal element index. It returns InvalidIndex (-1) if the Element is not found.
		* It uses a user-defined equality function.
		* 
		* @param Element Element to search for
		* 
		* @param EqualFunc Function checking for equality.
		* 
		* @returns The index of the last found element, or InvalidIndex if none is found.
		*/
		SizeType FindLast(const ElementType& Element, PFN_Equal EqualFunc)
		{
			for (SizeType Index = m_Size; Index > 0; Index--)
			{
				if (EqualFunc(Element, m_Data[Index]))
				{
					return Index;
				}
			}

			return InvalidIndex;
		}

		/**
		* Checks if the Element exists in the array.
		* 
		* @param Element Element to search for.
		* 
		* @return True if the element is found. False otherwise.
		*/
		bool Contains(const ElementType& Element) const
		{
			return (Find(Element) != InvalidIndex);
		}

		/**
		* Checks if the Element at AtIndex is equal to Element.
		* 
		* @param AtIndex Index to be checked. Will return false is index is out of range.
		* 
		* @parma Element Element to be compared with.
		* 
		* @returns True if the elements are equal. False otherwise.
		*/
		bool Find(SizeType AtIndex, const ElementType& Element) const
		{
			if (!IsValidIndex(AtIndex))
			{
				return false;
			}

			return (m_Data[AtIndex] == Element);
		}

		/**
		* Resizes to the specified NewSize. It will reallocate if needed. It can shrink the array.
		* If the NewSize is bigger than the current one, it will initialize the newly created elements with the default constructor.
		* 
		* @param NewSize Size to be set.
		* 
		* @param bAllowShrinking Whether or not the array should shrink if the number of allocated
		*			elements is much smaller than the number of elements the array can store.
		*			Default value is set to true.
		*/
		void SetSize(SizeType NewSize, bool bAllowShrinking = true)
		{
			if (NewSize > m_Capacity)
			{
				ReAllocate(NewSize);
			}
			else if (NewSize < m_Size)
			{
				if (NewSize < m_Capacity / 2)
				{
					ReAllocate(NewSize + NewSize / 3);
				}
			}

			for (SizeType Index = m_Size; Index < NewSize; Index++)
			{
				new(m_Data + Index) ElementType();
			}
			for (SizeType Index = NewSize; Index < m_Size; Index++)
			{
				m_Data[Index].~ElementType();
			}

			m_Size = NewSize;
		}

		/**
		* Resizes to the specified NewSize. It will reallocate if needed. It can shrink the array.
		* If the NewSize is bigger than the current one, it will initialize the newly created elements with 0.
		* 
		* @param NewSize Size to be set.
		*
		* @param bAllowShrinking Whether or not the array should shrink if the number of allocated
		*			elements is much smaller than the number of elements the array can store.
		*			Default value is set to true.
		*/
		void SetSizeZeroed(SizeType NewSize, bool bAllowShrinking = true)
		{
			if (NewSize > m_Capacity)
			{
				ReAllocate(NewSize);
			}
			else if (NewSize < m_Size)
			{
				if (NewSize < m_Capacity / 2)
				{
					ReAllocate(NewSize);
				}
			}

			if (m_Size < NewSize)
			{
				MemZero(m_Data + m_Size, (NewSize - m_Size) * sizeof(ElementType));
			}
			else
			{
				for (SizeType Index = NewSize; Index < m_Size; Index++)
				{
					m_Data[Index].~ElementType();
				}
			}

			m_Size = NewSize;
		}

		/**
		* Resizes to the specified NewSize. It will reallocate if needed. It can shrink the array.
		* If the NewSize is bigger than the current one, it will NOT initialize the newly created elements.
		* 
		* @param NewSize Size to be set.
		*
		* @param bAllowShrinking Whether or not the array should shrink if the number of allocated
		*			elements is much smaller than the number of elements the array can store.
		*			Default value is set to true.
		*/
		void SetSizeUninitialized(SizeType NewSize, bool bAllowShrinking = true)
		{
			if (NewSize > m_Capacity)
			{
				ReAllocate(NewSize);
			}
			else if (NewSize < m_Size)
			{
				if (NewSize < m_Capacity / 2)
				{
					ReAllocate(NewSize);
				}
			}

			for (SizeType Index = NewSize; Index < m_Size; Index++)
			{
				m_Data[Index].~ElementType();
			}

			m_Size = NewSize;
		}

		/**
		* Just sets the internal m_Size variable to NewSize. It is unsafe.
		* 
		* @param NewSize Size to be set.
		*/
		void SetSizeInternal(SizeType NewSize)
		{
			m_Size = NewSize;
		}

		/**
		* Reallocates the internal array.
		* 
		* @param NewCapacity The capacity that the array will be reallocated to.
		*/
		void SetCapacity(SizeType NewCapacity)
		{
			if (NewCapacity == m_Capacity)
			{
				return;
			}

			ReAllocate(NewCapacity);
		}

		/**
		* Will destruct all elements. It will not shrink the array.
		*/
		void Clear()
		{
			for (SizeType Index = 0; Index < m_Size; Index++)
			{
				m_Data[Index].~ElementType();
			}
			m_Size = 0;
		}

		/**
		* Shrinks the array to the smallest possible value so the elements fit in.
		*/
		void Shrink()
		{
			if (m_Size == m_Capacity)
			{
				return;
			}

			ReAllocate(m_Size);
		}

		/**
		* Shift the all the elements after StartingIndex with Offset positions to the right.
		* Elements between StartingIndex and StartingIndex + Offset will be left in a valid state.
		* 
		* @param StartingIndex Index where the shift begins.
		*
		* @param Offset How much elements will be offsetted.
		*/
		void ShiftElementsRight(SizeType StartingIndex, SizeType Offset)
		{
			AE_CORE_ASSERT(StartingIndex <= m_Size);

			if ((Offset == 0) || (StartingIndex == m_Size))
			{
				return;
			}

			if (m_Size + Offset > m_Capacity)
			{
				ReAllocate(m_Size + Offset);
			}

			// NOTE (Avr): Think about using only constructors (and deconstructors).
			for (SizeType Index = m_Size; Index < m_Size + Offset; Index++)
			{
				new(&m_Data[Index]) ElementType(AE::Move(m_Data[Index - Offset]));
			}
			for (SizeType Index = m_Size - 1; Index >= StartingIndex + Offset; Index--)
			{
				m_Data[Index] = AE::Move(m_Data[Index - Offset]);
			}

			// Just in case
			// for (usize Index = StartingIndex; Index < StartingIndex + Offset; Index++)
			// {
			//	m_Data[Index].~ElementType();
			// }

			m_Size += Offset;
		}

		/**
		* Shift the all the elements after StartingIndex + Offset with Offset positions to the left.
		* Last Offset elements will be destructed.
		* 
		* @param StartingIndex Index where the shift begins.
		* 
		* @param Offset How much elements will be offsetted.
		*/
		void ShiftElementsLeft(SizeType StartingIndex, SizeType Offset)
		{
			AE_CORE_ASSERT(StartingIndex <= m_Size);

			if ((Offset == 0) || (StartingIndex == m_Size))
			{
				return;
			}

			for (SizeType Index = StartingIndex; Index < m_Size - Offset; Index++)
			{
				m_Data[Index] = AE::Move(m_Data[Index + Offset]);
			}
			for (SizeType Index = 0; Index < Offset; Index++)
			{
				m_Data[(m_Size - 1) - Index].~ElementType();
			}

			m_Size -= Offset;
		}

		/**
		* Sets the allocator instance. It will cause a reallocation if the new and old allocators are not compatible.
		* 
		* @param NewAllocator New allocator instance.
		*/
		void SetAllocatorInstance(const Allocator& NewAllocator)
		{
			if (!CompareAllocators(m_Allocator, NewAllocator))
			{
				ElementType* NewBlock = (ElementType*)NewAllocator.Allocate(m_Capacity * sizeof(ElementType));
				
				for (SizeType Index = 0; Index < m_Size; Index++)
				{
					new(NewBlock + Index) ElementType(AE::Move(m_Data[Index]));
					m_Data[Index].~ElementType();
				}

				m_Allocator.Free(m_Data, m_Capacity * sizeof(ElementType));
				m_Data = NewBlock;
			}
			m_Allocator = NewAllocator;
		}

	public:
		/**
		* Creates a new vector, with the same m_Size and m_Capacity.
		* Its values are the same, casted to the required type.
		* Currently, it doesn't perform a type-safe cast!
		* 
		* @returns The casted vector.
		*/
		template<typename AsT, typename AsAllocatorType>
		Vector<AsT, AsAllocatorType> As() const
		{
			Vector<AsT, AsAllocatorType> Result;
			Result.ReAllocate(m_Capacity);
			Result.SetSizeInternal(m_Size);

			for (SizeType Index = 0; Index < m_Size; Index++)
			{
				new(Result.m_Data + Index) AsT((const AsT&)(m_Data[Index]));
			}

			return Result;
		}

		/**
		* Creates a new vector, with the same m_Size and m_Capacity.
		* Same as As(), but the AllocatorType is the same.
		* 
		* @returns The casted vector.
		*/
		template<typename AsT>
		Vector<AsT, AllocatorType> AsType() const
		{
			return As<AsT, AllocatorType>();
		}

		/**
		* Creates a new vector, with the same m_Size and m_Capacity.
		* Same as As(), but the ElementType is the same.
		* 
		* @returns The casted vector.
		*/
		template<typename AsAllocatorType>
		Vector<ElementType, AsAllocatorType> AsAllocator() const
		{
			return As<ElementType, AsAllocatorType>();
		}
		
	private:
		/**
		* Reallocates the internal array.
		* It supports both growing and shrinking.
		* 
		* @param NewCapacity The new capacity of the internal array.
		*/
		void ReAllocate(SizeType NewCapacity)
		{
			ElementType* NewBlock = (ElementType*)m_Allocator.Allocate(NewCapacity * sizeof(ElementType));
			SizeType NewSize = NewCapacity > m_Size ? m_Size : NewCapacity;

			for (SizeType Index = 0; Index < NewSize; Index++)
			{
				new (NewBlock + Index) ElementType(AE::Move(m_Data[Index]));
			}

			Clear();
			FreeData();

			m_Data = NewBlock;
			m_Capacity = NewCapacity;
			m_Size = NewSize;
		}

		/**
		* Deletes the current data.
		*/
		void FreeData()
		{
			m_Allocator.Free(m_Data, m_Capacity * sizeof(ElementType));
		}

		/**
		* Constructs a Vector. Used to avoid code duplication in the constructors.
		* 
		* @param Elements Raw array to the elements to copy.
		* 
		* @param Count Size of the raw array.
		*/
		void Construct(const ElementType* Elements, SizeType Count)
		{
			if (Count == 0)
			{
				return;
			}

			ReAllocate(Count);
			m_Size = Count;

			for (SizeType Index = 0; Index < Count; Index++)
			{
				new(m_Data + Index) ElementType(Elements[Index]);
			}
		}

		/**
		* Constructs a Vector. Used to avoid code duplication in the constructors.
		* Move semantics version.
		* 
		* @param Other Vector to move.
		*/
		template<typename OtherAllocatorType>
		void MoveConstruct(Vector<ElementType, OtherAllocatorType>&& Other)
		{
			if (CompareAllocators(m_Allocator, Other.m_Allocator))
			{
				m_Data = Other.m_Data;
				m_Capacity = Other.m_Capacity;
				m_Size = Other.m_Size;

				Other.m_Data = nullptr;
				Other.m_Capacity = 0;
				Other.m_Size = 0;
			}
			else if (!Other.IsEmpty())
			{
				ReAllocate(Other.m_Size);
				m_Size = Other.m_Size;

				for (SizeType Index = 0; Index < Other.m_Size; Index++)
				{
					new(m_Data + Index) ElementType(AE::Move(Other.m_Data[Index]));
				}

				Other.Clear();
			}
		}

		/**
		* Assigns a Vector. Used to avoid code duplication in the assignment operators.
		* 
		* @param Elements Raw array to the elements to copy.
		*
		* @param Count Size of the raw array.
		*/
		void Assign(const ElementType* Elements, SizeType Count)
		{
			Clear();

			if (Count == 0)
			{
				return;
			}

			if (Count > m_Capacity)
			{
				ReAllocate(Count);
			}
			m_Size = Count;

			for (SizeType Index = 0; Index < Count; Index++)
			{
				new(m_Data + Index) ElementType(Elements[Index]);
			}
		}

		/**
		* Assigns a Vector. Used to avoid code duplication in the assignment operators.
		* Move semantics version.
		* 
		* @param Other Vector to move.
		*/
		template<typename OtherAllocatorType>
		void MoveAssign(Vector<ElementType, OtherAllocatorType>&& Other)
		{
			Clear();

			if (CompareAllocators(m_Allocator, Other.m_Allocator))
			{
				ElementType* TempData = m_Data;
				SizeType TempCapacity = m_Capacity;

				m_Data = Other.m_Data;
				m_Capacity = Other.m_Capacity;
				m_Size = Other.m_Size;

				Other.m_Data = TempData;
				Other.m_Capacity = TempCapacity;
				Other.m_Size = 0;
			}
			else if (!Other.IsEmpty())
			{
				if (Other.m_Size > m_Capacity)
				{
					ReAllocate(Other.m_Size);
				}
				m_Size = Other.m_Size;

				for (SizeType Index = 0; Index < Other.m_Size; Index++)
				{
					new(m_Data + Index) ElementType(AE::Move(Other.m_Data[Index]));
				}

				Other.Clear();
			}
		}

	public:
		/**
		* Creates an iterator at the given index.
		* 
		* @param Index Index where to create the iterator.
		* 
		* @returns The iterator at the given index.
		*/
		Iterator CreateIterator(SizeType Index)
		{
			return Iterator(m_Data + Index);
		}

		/**
		* Creates an iterator at the given index.
		* Const version of the above.
		*
		* @param Index Index where to create the iterator.
		*
		* @returns The iterator at the given index.
		*/
		ConstIterator CreateIterator(SizeType Index) const
		{
			return ConstIterator(m_Data + Index);
		}

		//// Iterators ////

		Iterator begin()
		{
			return CreateIterator(0);
		}

		Iterator end()
		{
			return CreateIterator(m_Size);
		}

		ConstIterator begin() const
		{
			return CreateIterator(0);
		}

		ConstIterator end() const
		{
			return CreateIterator(m_Size);
		}

		ReverseIterator rbegin()
		{
			return ReverseIterator(m_Data + m_Size - 1);
		}

		ReverseIterator rend()
		{
			return ReverseIterator(m_Data - 1);
		}

		ReverseConstIterator rbegin() const
		{
			return ReverseConstIterator(m_Data + m_Size - 1);
		}

		ReverseConstIterator rend() const
		{
			return ReverseConstIterator(m_Data - 1);
		}

	private:
		/**
		* Pointer to the first element in the array.
		*/
		ElementType* m_Data;

		/**
		* Number of elements that the vector can store without reallocating.
		*/
		SizeType m_Capacity;

		/**
		* Number of valid elements.
		*/
		SizeType m_Size;

		/**
		* Allocator instance.
		*/
		AllocatorType m_Allocator;

		template<typename FriendElementType, typename FriendAllocatorType>
		friend class Vector;

		friend class FString;
	};

}