#include "ajpch.h"
#include "EditorLayer.h"

#include "Apricot/Core/Containers/ApricotString.h"

namespace Apricot {

	EditorLayer::EditorLayer()
		: Layer("EditorLayer")
	{
	}

	EditorLayer::~EditorLayer()
	{
	}

	void EditorLayer::OnAttached()
	{
		FString s = TEXT("hmmm");
	}

	void EditorLayer::OnDetached()
	{
		
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		
	}

}