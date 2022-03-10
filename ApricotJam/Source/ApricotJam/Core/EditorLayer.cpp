// Part of Apricot Engine. 2022-2022.
// Module: EditorCore

#include "ajpch.h"
#include "EditorLayer.h"

namespace Apricot {

	EditorLayer::EditorLayer()
		: Layer(TEXT("Editor Layer"))
	{
	}

	EditorLayer::~EditorLayer()
	{
	}

	void EditorLayer::OnAttach()
	{
		AE_CORE_INFO(TEXT("EditorLayer attached!"));
	}

	void EditorLayer::OnDetach()
	{
		AE_CORE_INFO(TEXT("EditorLayer detached!"));
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		
	}

	void EditorLayer::OnEvent(AEvent* ev)
	{
		
	}

}