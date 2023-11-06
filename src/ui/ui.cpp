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
			MY_GL_CHECK(glViewport(
				_mData.width  * 1 / 4,
				_mData.height * 1 / 4 + ActivityBarWidth * 4,
				_mData.width  * 2 / 4,
				_mData.height * 2 / 4
			));
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

		topPanel(cameras);
		botPanel();
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
		ImGui::SetNextWindowSize(ImVec2(_mData.width * 1 / 4, _mData.height - MainBarWidth), ImGuiCond_Once);
		ImGui::SetNextWindowBgAlpha(1.0f);

		ImGui::Begin("##Left", nullptr, staticWindow);

		// Debug only
		for(std::shared_ptr<Object>& object : objects) {
			std::string objectID = std::to_string(object->getID());

			ImGui::BulletText("Object:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.50f, 0.50f, 0.50f, 1.0f), "%u", object->getID());

			std::string position = "Position##" + objectID;
			std::string rotation = "Rotation##" + objectID;
			std::string scale    = "Scale##"    + objectID;
	
			if(ImGui::DragFloat3(position.c_str(), &object->getWorldData().position[0], 1)) {
				object->getWorldData().hasUpdate = true;
			}
			if(ImGui::DragFloat3(rotation.c_str(), &object->getWorldData().rotation[0], 1)) {
				object->getWorldData().hasUpdate = true;
			}
			if(ImGui::DragFloat3(scale.c_str(), &object->getWorldData().scale[0], 1)) {
				object->getWorldData().hasUpdate = true;
			}

			ImGui::Separator();
		}

		for(std::shared_ptr<Camera>& camera : cameras) {
			if (camera->getUseData().isActive) {
				ImGui::BulletText("Main Camera:");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(0.50f, 0.50f, 0.50f, 1.0f), "%u", camera->getID());
			}

			std::string cameraID = std::to_string(camera->getID());

			std::string sposition = "Position##" + cameraID;
			std::string starget   = "Target##" + cameraID;
			std::string sfar      = "Far##" + cameraID;
			std::string snear     = "Near##" + cameraID;
			std::string sfov      = "FOV##" + cameraID;
			std::string swidth    = "Width##" + cameraID;
			std::string sheight   = "Height##" + cameraID;

			if(ImGui::DragFloat3(sposition.c_str(), &camera->getWorldData().position[0], 1)) {
				camera->getUseData().hasUpdate = true;
			}
			if(ImGui::DragFloat3(starget.c_str(), &camera->getWorldData().target[0], 1)) {
				camera->getUseData().hasUpdate = true;
			}
			if(ImGui::DragFloat(sfar.c_str(), &camera->getWorldData().c_far, 1, 0, FLT_MAX)) {
				camera->getUseData().hasUpdate = true;
			}
			if(ImGui::DragFloat(snear.c_str(), &camera->getWorldData().c_near, 1, 0, FLT_MAX)) {
				camera->getUseData().hasUpdate = true;
			}

			if(camera->getTpye() == CameraType::PERSPECTIVE) {
				PerspectiveCamera* pCamera = static_cast<PerspectiveCamera*>(camera.get());

				if(ImGui::DragFloat(sfov.c_str(), &pCamera->getFov(), 1)) {
					camera->getUseData().hasUpdate = true;
				}

				int width  = pCamera->getWidth();
				int height = pCamera->getHeight();

				if(ImGui::DragInt(swidth.c_str(), &width, 1, 0, INT_MAX)) {
					pCamera->getWidth() = width;
					camera->getUseData().hasUpdate = true;
				}

				if(ImGui::DragInt(sheight.c_str(), &height, 1, 0, INT_MAX)) {
					pCamera->getHeight() = height;
					camera->getUseData().hasUpdate = true;
				}
			}
		}

		// ImGuiStyle* style = &ImGui::GetStyle();
		// ImVec4* colors = style->Colors;

		// for (int i = 0; i < ImGuiCol_COUNT; i++) {
		// 	ImGui::ColorEdit4(ImGui::GetStyleColorName(i), &colors[i].x);
		// }

		ImGui::End();
	}

	void UI::rightPanel(
		std::vector<std::shared_ptr<Object>>& objects,
		std::vector<std::shared_ptr<Camera>>& cameras
	) {
		// Object Params
		ImGui::SetNextWindowPos(ImVec2(_mData.width * 3 / 4 , MainBarWidth), ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2(_mData.width * 1 / 4, _mData.height - MainBarWidth), ImGuiCond_Once);
		ImGui::SetNextWindowBgAlpha(1.0f);

		ImGui::Begin("##Right", nullptr, staticWindow);
		ImGui::End();
	}

	void UI::topPanel(std::vector<std::shared_ptr<Camera>>& cameras) {
		// Activity information
		ImGui::SetNextWindowPos(ImVec2(_mData.width * 1 / 4,  MainBarWidth), ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2(_mData.width * 2 / 4, ActivityBarWidth * 2), ImGuiCond_Once);
		ImGui::SetNextWindowBgAlpha(1.0f);

		ImGui::Begin("##Top", nullptr, staticWindow | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollWithMouse);

		for(std::shared_ptr<Camera>& camera : cameras) {
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

	void UI::botPanel() {
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
		ImGui::End();
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