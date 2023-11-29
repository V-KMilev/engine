#include "ui.h"

#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui.h>

#include "camera.h"
#include "perspective_camera.h"

#include "object.h"
#include "quad.h"

#include "gl_error_handle.h"

const ImGuiWindowFlags staticWindow = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoFocusOnAppearing;
namespace Engine {
	uiData::uiData(
		unsigned int width,
		unsigned int height,
		bool isActive
	) : width(width),
		height(height),
		isActive(isActive) {}

	UI::UI(
		GLFWwindow* window,
		const char* gl_version,
		unsigned int width,
		unsigned int height
	) : _mData(uiData(width, height, true))
	{
		if (_mData.isActive) {
			setViewPort();
		}

		IMGUI_CHECKVERSION();

		ImGui::CreateContext();
		ImGuiIO _mIO = ImGui::GetIO();
		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(gl_version);

		setStyle();
	}

	UI::~UI() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	const uiData& UI::getData() const {
		return _mData;
	}

	uiData& UI::getData() {
		return _mData;
	}

	void UI::newFrame() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void UI::render() {
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void UI::ui(
		std::vector<std::shared_ptr<Object>>& objects,
		std::vector<std::shared_ptr<Camera>>& cameras
	) {
		ImGui::ShowDemoWindow();
		mainManu();

		leftPanel(objects, cameras);
		rightPanel(objects, cameras);

		lowLeftPanel(objects, cameras);
		LowRightPanel(objects, cameras);

		topPanel(objects, cameras);
		botPanel(objects, cameras);
	}

	void UI::showUI() {
		if (!_mData.isActive) {
			_mData.isActive = true;

			setViewPort();
		}
		else {
			_mData.isActive = false;

			MY_GL_CHECK(glViewport(0, 0, _mData.width, _mData.height));
		}
	}

	void UI::mainManu() {
		if (ImGui::BeginMainMenuBar()) {
			ImGui::TextColored(ImVec4(0.75f, 0.25f, 0.25f, 1.0f), "VKM ENGINE");

			if (ImGui::BeginMenu("Test0")) {
			}
			if (ImGui::BeginMenu("Test1")) {
			}
			if (ImGui::BeginMenu("Test2")) {
			}
			if (ImGui::BeginMenu("Test3")) {
			}
			ImGui::EndMainMenuBar();
		}
	}

	void UI::leftPanel(
		std::vector<std::shared_ptr<Object>>& objects,
		std::vector<std::shared_ptr<Camera>>& cameras
	) {
		// Object Data
		ImGui::SetNextWindowPos(ImVec2(0, MainBarWidth), ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2(_mData.width * 1 / 4, _mData.height - MainBarWidth - (_mData.height * 1 / 4 + MainBarWidth * 4)), ImGuiCond_Once);
		ImGui::SetNextWindowBgAlpha(1.0f);

		ImGui::Begin("##Left", nullptr, staticWindow);

		for (std::shared_ptr<Object>& object : objects) {
			if (object->getUseData().isSelected) {
				object->UIWorld();
			}
		}

		ImGui::End();
	}

	void UI::rightPanel(
		std::vector<std::shared_ptr<Object>>& objects,
		std::vector<std::shared_ptr<Camera>>& cameras
	) {
		// Object Params
		ImGui::SetNextWindowPos(ImVec2(_mData.width * 3 / 4, MainBarWidth), ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2(_mData.width * 1 / 4, _mData.height - MainBarWidth - (_mData.height * 1 / 4 + MainBarWidth * 4)), ImGuiCond_Once);
		ImGui::SetNextWindowBgAlpha(1.0f);

		ImGui::Begin("##Right", nullptr, staticWindow);

		for (std::shared_ptr<Camera>& camera : cameras) {
			camera->UIWorld();
		}

		ImGui::End();
	}

	void UI::lowLeftPanel(
		std::vector<std::shared_ptr<Object>>& objects,
		std::vector<std::shared_ptr<Camera>>& cameras
	) {
		// Object Data
		ImGui::SetNextWindowPos(ImVec2(0, _mData.height * 3 / 4 - MainBarWidth * 4), ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2(_mData.width * 1 / 4, _mData.height * 1 / 4 + MainBarWidth * 4), ImGuiCond_Once);
		ImGui::SetNextWindowBgAlpha(1.0f);

		ImGui::Begin("##LowLeft", nullptr, staticWindow);

		if (ImGui::BeginTabBar("##lowLeftTabBar")) {
			if (ImGui::BeginTabItem("test1")) {
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("test2")) {
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("test3")) {
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}

		for (std::shared_ptr<Object>& object : objects) {
			if (object->getUseData().isSelected) {
				object->UIMeshsWorld();
			}
		}

		ImGui::End();
	}

	void UI::LowRightPanel(
		std::vector<std::shared_ptr<Object>>& objects,
		std::vector<std::shared_ptr<Camera>>& cameras
	) {
		// Object Data
		ImGui::SetNextWindowPos(ImVec2(_mData.width * 3 / 4, _mData.height * 3 / 4 - MainBarWidth * 4), ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2(_mData.width * 1 / 4, _mData.height * 1 / 4 + MainBarWidth * 4), ImGuiCond_Once);
		ImGui::SetNextWindowBgAlpha(1.0f);

		ImGui::Begin("##LowRight", nullptr, staticWindow);

		if (ImGui::BeginTabBar("##lowRightTabBar")) {
			if (ImGui::BeginTabItem("test1")) {
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("test2")) {
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("test3")) {
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}

		for (std::shared_ptr<Object>& object : objects) {
			if (object->getUseData().isSelected) {
				object->UIMeshsMaterialCoefficients();
			}
		}

		ImGui::End();
	}

	void UI::topPanel(
		std::vector<std::shared_ptr<Object>>& objects,
		std::vector<std::shared_ptr<Camera>>& cameras
	) {
		// Activity information
		ImGui::SetNextWindowPos(ImVec2(_mData.width * 1 / 4,  MainBarWidth), ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2(_mData.width * 2 / 4, ActivityBarWidth * 2), ImGuiCond_Once);
		ImGui::SetNextWindowBgAlpha(1.0f);

		ImGui::Begin("##Top", nullptr, staticWindow | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollWithMouse);

		for (std::shared_ptr<Camera>& camera : cameras) {
			if (camera->getUseData().isActive) {
				ImGui::BulletText("Camera");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(0.50f, 0.50f, 0.50f, 1.0f), "%u", camera->getID());
			}
		}

		ImGui::BulletText("FPS");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.50f, 0.50f, 0.50f, 1.0f), "%f", ImGui::GetIO().Framerate);

		ImGui::End();
	}

	void UI::botPanel(
		std::vector<std::shared_ptr<Object>>& objects,
		std::vector<std::shared_ptr<Camera>>& cameras
	) {
		// Assets
		ImGui::SetNextWindowPos(ImVec2(_mData.width * 1 / 4,  _mData.height * 3 / 4 - MainBarWidth * 4), ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2(_mData.width * 2 / 4, _mData.height * 1 / 4 + MainBarWidth * 4), ImGuiCond_Once);
		ImGui::SetNextWindowBgAlpha(1.0f);

		ImGui::Begin("##Bot", nullptr, staticWindow | ImGuiWindowFlags_MenuBar);

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("test0")) {
			}
			if (ImGui::BeginMenu("test1")) {
			}
			if (ImGui::BeginMenu("test2")) {
			}
			if (ImGui::BeginMenu("test3")) {
			}
			ImGui::EndMenuBar();
		}

		for (std::shared_ptr<Object>& object : objects) {
			if (object->getUseData().isSelected) {
				object->UIMeshsMaterialTextures();
			}
		}

		// Debug
		// ImGuiStyle* style = &ImGui::GetStyle();
		// ImVec4* colors = style->Colors;

		// for (int i = 0; i < ImGuiCol_COUNT; i++) {
		// 	ImGui::ColorEdit4(ImGui::GetStyleColorName(i), &colors[i].x);
		// }

		ImGui::End();
	}

	void UI::setViewPort() {
		MY_GL_CHECK(
			glViewport(
				_mData.width  * 1 / 4,
				_mData.height * 1 / 4 + ActivityBarWidth * 4,
				_mData.width  * 2 / 4,
				_mData.height * 2 / 4
			)
		);
	}

	void UI::setStyle() {
		ImGuiStyle* style = &ImGui::GetStyle();
		ImVec4* colors = style->Colors;

		colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);

		colors[ImGuiCol_WindowBg]               = ImVec4(0.075f, 0.075f, 0.075f, 1.00f);

		colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_PopupBg]                = ImVec4(0.15f, 0.15f, 0.15f, 0.90f);

		colors[ImGuiCol_Border]                 = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

		colors[ImGuiCol_FrameBg]                = ImVec4(0.48f, 0.16f, 0.16f, 0.60f);
		colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.98f, 0.26f, 0.26f, 0.50f);
		colors[ImGuiCol_FrameBgActive]          = ImVec4(0.98f, 0.26f, 0.26f, 0.70f);

		colors[ImGuiCol_TitleBg]                = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
		colors[ImGuiCol_TitleBgActive]          = ImVec4(0.48f, 0.16f, 0.16f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);

		colors[ImGuiCol_MenuBarBg]              = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);

		colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.05f, 0.05f, 0.05f, 0.50f);
		colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.48f, 0.16f, 0.16f, 0.60f);
		colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.98f, 0.26f, 0.26f, 0.50f);
		colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.98f, 0.26f, 0.26f, 0.70f);

		colors[ImGuiCol_CheckMark]              = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);

		colors[ImGuiCol_SliderGrab]             = ImVec4(0.88f, 0.24f, 0.24f, 1.00f);
		colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);

		colors[ImGuiCol_Button]                 = ImVec4(0.98f, 0.26f, 0.26f, 0.40f);
		colors[ImGuiCol_ButtonHovered]          = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);
		colors[ImGuiCol_ButtonActive]           = ImVec4(0.98f, 0.06f, 0.06f, 1.00f);

		colors[ImGuiCol_Header]                 = ImVec4(0.98f, 0.26f, 0.26f, 0.30f);
		colors[ImGuiCol_HeaderHovered]          = ImVec4(0.98f, 0.26f, 0.26f, 0.80f);
		colors[ImGuiCol_HeaderActive]           = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);

		colors[ImGuiCol_Separator]              = ImVec4(0.75f, 0.10f, 0.10f, 0.50f);
		colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.75f, 0.10f, 0.10f, 0.80f);
		colors[ImGuiCol_SeparatorActive]        = ImVec4(0.75f, 0.10f, 0.10f, 1.00f);

		colors[ImGuiCol_ResizeGrip]             = ImVec4(0.98f, 0.26f, 0.26f, 0.20f);
		colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.98f, 0.26f, 0.26f, 0.70f);
		colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.98f, 0.26f, 0.26f, 0.90f);

		colors[ImGuiCol_Tab]                    = ImVec4(0.60f, 0.18f, 0.18f, 0.80f);
		colors[ImGuiCol_TabHovered]             = ImVec4(0.98f, 0.26f, 0.26f, 0.90f);
		colors[ImGuiCol_TabActive]              = ImVec4(0.70f, 0.20f, 0.20f, 1.00f);
		colors[ImGuiCol_TabUnfocused]           = ImVec4(0.15f, 0.07f, 0.07f, 1.00f);
		colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.43f, 0.14f, 0.14f, 1.00f);

		colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);

		colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
		colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
		colors[ImGuiCol_TableBorderLight]       = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
		colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);

		colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.98f, 0.26f, 0.26f, 0.35f);

		colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);

		colors[ImGuiCol_NavHighlight]           = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	}
};
