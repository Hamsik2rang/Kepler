#pragma once


#include "Core/Base.h"

namespace kepler {

	// (Im Yongsik)정의된 컴포넌트 개수를 어떻게 식별할 것인가??
	// 매직 넘버로 사용하는건 좋지 않은듯함(컴포넌트 하나 새로 정의할 때마다 수 고치고... X)
	// 추후 새로운 방법을 찾을 때까지만 임시로 매직 넘버 상수를 사용합니다.
	namespace component {
		extern const uint16_t componentCount;

		void InitComponentSystem();
	}

	enum class eComponentType
	{
		IComponent = 0,
		TagComponent,
		TransformComponent,

		MeshRendererComponent,
		SpriteRendererComponent,

		InvalidComponent // (Im Yongsik) 타입 개수 표시자의 역할이므로 type enum의 가장 마지막에 위치해야 합니다.
	};

	enum eComponentIndex
	{
		TagIndex = 0,
		TransformIndex,
		MeshRendererIndex,
		SpriteRendererIndex,
	};

	enum eComponentFlag : uint64_t
	{
		BaseFlag = 0,
		TagFlag = BIT_UINT64(0),
		TransformFlag = BIT_UINT64(1),
		MeshRendererFlag = BIT_UINT64(2),
		SpriteRendererFlag = BIT_UINT64(3),
	};


#define COMPONENT_CLASS_TYPE(type) \
	static eComponentType GetStaticType() { return eComponentType::##type; } \
	virtual eComponentType GetType() const override { return GetStaticType(); }

#define COMPONENT_CLASS_INDEX(index) \
	static eComponentIndex GetStaticIndex() { return eComponentIndex::##index; } \
	virtual eComponentIndex GetIndex() const override { return GetStaticIndex(); }

#define COMPONENT_CLASS_FLAG(flag) \
	static eComponentFlag GetStaticFlag() { return eComponentFlag::##flag; } \
	virtual eComponentFlag GetFlag() const override { return GetStaticFlag(); }

	template <typename ... TComponent>
	struct TypeToFlag;

	template <>
	struct TypeToFlag<>
	{
		uint64_t  operator()() const
		{
			return 0;
		}
	};

	template <typename TComponent, typename ... TOther>
	struct TypeToFlag<TComponent, TOther...>
	{
		uint64_t operator()() const
		{
			uint64_t bitset = (uint64_t)TComponent::GetStaticFlag() | (uint64_t)TypeToFlag<TOther...>()();
			return bitset;
		}
	};

	template <typename ... TComponent>
	eComponentFlag ConvertTypeToFlag()
	{
		return (eComponentFlag)TypeToFlag<TComponent...>()();
	}
}