// Copyright 2014 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#pragma once

#include <d3d11.h>
#include <memory>

namespace DX11
{
	namespace D3D
	{

		// use as a deleter in std::unique_ptr
		struct IUnknownDeleter {
			void operator() (IUnknown * ptr) const {
				ptr->Release();
			}
		};

		// a simple alias to std::unique_ptr, add RAII semantic to a D3D pointer.
		template <typename T>
		using UniquePtr = std::unique_ptr<T, IUnknownDeleter>;

		// helper class to pass a UniquePtr to the various CreateSomething( T**/void** result )
		// use with the following function
		template <typename T>
		struct ToAddrImpl {
			ToAddrImpl(UniquePtr<T> & ptr) : ptr_{ ptr } {}
			ToAddrImpl(ToAddrImpl const &) = delete;
			ToAddrImpl& operator=(ToAddrImpl const &) = delete;

			~ToAddrImpl() {
				ptr_.reset(temp_);
			}

			operator void**() { return (void**)&temp_; }
			operator T**() { return &temp_; }
		private:
			UniquePtr<T> & ptr_;
			T* temp_{};
		};

		// usage example : device->CreateBuffer( bla, ToAddr( m_myBuffer ) );
		template <typename T>
		ToAddrImpl<T> ToAddr(UniquePtr<T>& ptr) {
			return{ ptr };
		}

	}  // namespace D3D
}  // namespace DX11
