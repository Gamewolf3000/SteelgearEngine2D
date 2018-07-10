#pragma once

#include <vector>

namespace SG
{
	template<typename T>
	class SGVector
	{
	private:

		struct StorageData
		{
			T element;
			unsigned int refCount;

			StorageData(T toStore) : element(toStore), refCount(0)
			{

			}
		};

		std::vector<StorageData> elements;
		std::vector<unsigned int> freeSpots;
		unsigned int resizeAmount;

	public:
		SGVector<T>(unsigned int startSize = 0, unsigned int resizeSize = 1);
		~SGVector();

		unsigned int Add(T& element);
		unsigned int AddToBack(T& element);
		void Remove(unsigned int spot);
		void FreeSpot(unsigned int spot);

		T& operator[](unsigned int toGet);
		int GetReferenceCountOfElement(unsigned int toGet);
		void IncrementReferenceCountOfElement(unsigned int index);
		void DecrementReferenceCountOfElement(unsigned int index);

		int Find(const T& toFind) const;
		inline int Size() const
		{
			return elements.size();
		}

	};

	template<typename T>
	inline SGVector<T>::SGVector(unsigned int startSize, unsigned int resizeSize) : resizeAmount(resizeSize)
	{
		elements.reserve(startSize);
	}


	template<typename T>
	inline SGVector<T>::~SGVector()
	{
		// Empty
	}

	template<typename T>
	inline unsigned int SGVector<T>::Add(T& element)
	{
		for (unsigned int i = 0; i < elements.size(); i++)
		{
			if (elements[i].element == element) // If the element already exists then we find it, increment its refCount and return the position
			{
				if (elements[i].refCount == 0) // If the element has been removed then we cannot reliably reuse it since data might have been deleted or such
				{
					continue;
				}

				elements[i].refCount++;
				return i;
			}
		}

		if (freeSpots.size())
		{
			unsigned int position = freeSpots.back();
			freeSpots.pop_back();
			elements[position] = element;
			return position;
		}

		if (elements.size() == elements.capacity())
		{
			elements.reserve(resizeAmount);
		}

		elements.push_back(element);
		return elements.size() - 1;
	}

	template<typename T>
	inline unsigned int SGVector<T>::AddToBack(T & element)
	{
		elements.push_back(element);
		return elements.size() - 1;
	}

	template<typename T>
	inline void SGVector<T>::Remove(unsigned int spot)
	{
		if (elements[spot].refCount == 1)
		{
			freeSpots.push_back(spot);
		}

		elements[spot].refCount--;
	}

	template<typename T>
	inline void SGVector<T>::FreeSpot(unsigned int spot)
	{
		freeSpots.push_back(spot);
	}

	template<typename T>
	inline T & SGVector<T>::operator[](unsigned int toGet)
	{
		return elements[toGet].element;
	}

	template<typename T>
	inline int SGVector<T>::GetReferenceCountOfElement(unsigned int toGet)
	{
		return elements[toGet].refCount;
	}

	template<typename T>
	inline void SGVector<T>::IncrementReferenceCountOfElement(unsigned int index)
	{
		elements[index].refCount++;
	}

	template<typename T>
	inline void SGVector<T>::DecrementReferenceCountOfElement(unsigned int index)
	{
		elements[index].refCount--;
	}

	template<typename T>
	inline int SGVector<T>::Find(const T & toFind) const
	{
		for (int i = 0; i < elements.size(); i++)
		{
			if (elements[i].element == toFind)
				return i;
		}

		return -1;
	}

}