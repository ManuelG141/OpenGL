#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <iostream>
#include <vector>
#include <functional>
#include <string>

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

	class TestMenu : public Test
	{
	public:
		TestMenu(Test*& currentTestPtr);

		void OnImGuiRender() override;

		template<typename T>
		void RegisterTest(const std::string& name)
		{
			std::cout << "Registering test " << name << std::endl;

			m_Tests.emplace_back(std::make_pair(name, []() { return new T(); }));
		}
	private:
		Test*& m_CurrentTest; // Reference of the Test Pointer
		std::vector<std::pair<std::string, std::function<Test* ()>>> m_Tests;
	};
}