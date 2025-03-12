#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

namespace test {

	class Test
	{
	public:
		Test() {};
		virtual ~Test() {};

		virtual void OnUpdate(float deltaTime) {};
		virtual void OnRender() {};
		virtual void OnImGuiRender() {};
	};
}