#pragma once

#include <utility>

#include "d3d.hpp"

namespace engine
{
	// A helping struct holding a raw pointer to a D3D resource

	template <typename T>
	struct DxResPtr
	{
		DxResPtr() { m_ptr = nullptr; }
		~DxResPtr() { release(); }

		DxResPtr(const DxResPtr& other) { *this = other; }
		DxResPtr(T* other) { T* m_ptr = other; }
		DxResPtr& operator=(const DxResPtr& other)
		{
			m_ptr = other.m_ptr;
			m_ptr->AddRef();
			return *this;
		}

		DxResPtr(DxResPtr&& other) noexcept { *this = std::move(other); }
		DxResPtr& operator=(DxResPtr&& other) noexcept
		{
			if (m_ptr) m_ptr->Release();
			m_ptr = other.m_ptr;
			other.m_ptr = nullptr;
			return *this;
		}


		T* ptr() const { return m_ptr; }
		T* ptr() { return m_ptr; }

		T* operator->() const { return m_ptr; }
		T* operator->() { return m_ptr; }

		operator T* () const { return m_ptr; }
		operator T* () { return m_ptr; }

		T** reset()
		{
			release();
			return &m_ptr;
		}

		void reset(T* ptr)
		{
			release();
			m_ptr = ptr;
		}

		T** access()
		{
			return &m_ptr;
		}

		void release()
		{
			if (m_ptr)
			{
				m_ptr->Release();
				m_ptr = nullptr;
			}
		}

		bool valid()
		{
			return m_ptr != nullptr;
		}

	protected:
		T* m_ptr;
	};
}