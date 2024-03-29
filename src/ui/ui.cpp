#include "ui.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui.h>

#include "gl_error_handle.h"

#include "utils.h"

#include "scene_manager.h"

#include "perspective_camera.h"

#include "light.h"

#include "object.h"

#include "triangle.h"
#include "quad.h"
#include "cube.h"
#include "sphere.h"
#include "model.h"

#include "mesh.h"

#include "component.h"

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
		Scene& scene,
		GLFWwindow* window,
		const char* gl_version,
		unsigned int width,
		unsigned int height
	) : _mScene(scene) {
		_mData = uiData(width, height, false);

		showUI();

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

	void UI::ui() {
		ImGui::ShowDemoWindow();
		mainManu();

		leftPanel();
		rightPanel();

		lowLeftPanel();
		LowRightPanel();

		topPanel();
		botPanel();

		addOrRemoveToScene();
		saveOrLoadScene();
	}

	void UI::fullscreen() {
		mainManu();

		ImGuiWindowFlags window_flags = 
			ImGuiWindowFlags_NoDecoration    | ImGuiWindowFlags_AlwaysAutoResize   |
			ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
			ImGuiWindowFlags_NoNav;

		const float PAD = 10.0f;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
		ImVec2 work_size = viewport->WorkSize;
		ImVec2 window_pos, window_pos_pivot;

		window_pos.x = (2 & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
		window_pos.y = (2 & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
		window_pos_pivot.x = (2 & 1) ? 1.0f : 0.0f;
		window_pos_pivot.y = (2 & 2) ? 1.0f : 0.0f;

		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);

		ImGui::SetNextWindowBgAlpha(0.35f);
		if(ImGui::Begin("Info overlay", NULL, window_flags)) {
				ImGui::Text("VKM - Build v0.01");
				ImGui::Separator();
				ImGui::Text("Render: %.3f ms | %d FPS", 1000.0f / Utils::Time::FPS, Utils::Time::FPS);
				ImGui::Text("UI:     %.3f ms | %d FPS", 1000.0f / ImGui::GetIO().Framerate, (int)ImGui::GetIO().Framerate);
			ImGui::End();
		}
	}

	void UI::showUI() {
		if (!_mData.isActive) {
			_mData.isActive = true;

			// TODO: Write why * 2 / 4 and s.o
			_mScene.setSceneViewPort(
				_mData.width  * 1 / 4,
				_mData.height * 1 / 4 + ActivityBarWidth * 4,
				_mData.width  * 2 / 4,
				_mData.height * 2 / 4
			);
		}
		else {
			_mData.isActive = false;

			_mScene.setSceneViewPort(0, 0, _mData.width, _mData.height);
		}
	}

	void UI::mainManu() {
		if (ImGui::BeginMainMenuBar()) {
			ImGui::TextColored(ImVec4(0.75f, 0.25f, 0.25f, 1.0f), "VKM ENGINE");

			if (ImGui::BeginMenu("File")) {
				ImGui::MenuItem("Save", "Ctrl + S", &_mCallSave);
				ImGui::MenuItem("Load", "Ctrl + L", &_mCallLoad);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit")) {
				ImGui::MenuItem("Undo", "Ctrl + Z", &_mCallUndo);
				ImGui::MenuItem("Redo", "Ctrl + Y", &_mCallRedo);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("View")) {
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Scene")) {
				if (ImGui::BeginMenu("Add Entity")) {

					if (ImGui::BeginMenu("Cameras")) {
						ImGui::MenuItem("Perspective", nullptr, &_mCallAddPerspective);
						ImGui::EndMenu();
					}

					if (ImGui::BeginMenu("Objects")) {
						ImGui::MenuItem("Triangle", nullptr, &_mCallAddTriangle);
						ImGui::MenuItem("Quad", nullptr, &_mCallAddQuad);
						ImGui::MenuItem("Cube", nullptr, &_mCallAddCube);
						ImGui::MenuItem("Sphere", nullptr, &_mCallAddSphere);
						ImGui::MenuItem("Model", nullptr, &_mCallAddModel);
						ImGui::EndMenu();
					}

					if (ImGui::BeginMenu("Lights")) {
						ImGui::MenuItem("Light", nullptr, &_mCallAddLight);
						ImGui::EndMenu();
					}
					ImGui::EndMenu();
				}

				ImGui::MenuItem("Remove Entity","Ctrl + R", &_mCallRemove);
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}

	void UI::leftPanel() {
		ImGui::SetNextWindowPos(ImVec2(0, MainBarWidth), ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2(_mData.width * 1 / 4, _mData.height - MainBarWidth - (_mData.height * 1 / 4 + MainBarWidth * 4)), ImGuiCond_Once);
		ImGui::SetNextWindowBgAlpha(1.0f);

		ImGui::Begin("##Left", nullptr, staticWindow);

		auto& entitys = _mScene.getEntitys();
		auto& objects = _mScene.getObjects();

		static std::string sEntityList = "Entity list";
		static std::string sMeshList   = "Mesh list";

		ImGui::BeginChild(sEntityList.c_str(), ImVec2(_mData.width * 1 / 9, 0), true);

		for (auto& entity : entitys) {
			EntityType type = entity->getType();

			static std::string labe = "Entity #" + std::to_string(entity->getID());

			if (type == EntityType::OBJECT) {
				labe = "Object #" + std::to_string(entity->getID());
			}
			else if (type == EntityType::CAMERA) {
				labe = "Camera #" + std::to_string(entity->getID());
			}
			else if (type == EntityType::LIGHT) {
				labe = "Light #" + std::to_string(entity->getID());
			}

			if (ImGui::Selectable(labe.c_str(), entity->isSelected())) {
				entity->setIsSelected(!entity->isSelected());
			}
		}

		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild(sMeshList.c_str(), ImVec2(_mData.width * 1 / 8, 0), true);

		for (auto& object : objects) {

			if (object->isSelected()) {

				for(auto& mesh : object->getMeshes()) {
					static std::string labe = "Mesh #" + std::to_string(mesh->getID());

					if (ImGui::Selectable(labe.c_str(), mesh->isSelected())) {
						mesh->setIsSelected(!mesh->isSelected());
					}
				}
			}
		}

		ImGui::EndChild();

		ImGui::End();
	}

	void UI::rightPanel() {
		ImGui::SetNextWindowPos(ImVec2(_mData.width * 3 / 4, MainBarWidth), ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2(_mData.width * 1 / 4, _mData.height - MainBarWidth - (_mData.height * 1 / 4 + MainBarWidth * 4)), ImGuiCond_Once);
		ImGui::SetNextWindowBgAlpha(1.0f);

		ImGui::Begin("##Right", nullptr, staticWindow);

		auto& entitys = _mScene.getEntitys();

		for (auto& entity : entitys) {
			if (entity->isSelected()) {
				entity->drawUI();
			}
		}

		ImGui::End();
	}

	void UI::lowLeftPanel() {
		ImGui::SetNextWindowPos(ImVec2(0, _mData.height * 3 / 4 - MainBarWidth * 4), ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2(_mData.width * 1 / 4, _mData.height * 1 / 4 + MainBarWidth * 4), ImGuiCond_Once);
		ImGui::SetNextWindowBgAlpha(1.0f);

		ImGui::Begin("##LowLeft", nullptr, staticWindow | ImGuiWindowFlags_MenuBar);

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("Soon 0")) {
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Soon 1")) {
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Soon 2")) {
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Soon 3")) {
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		auto& objects = _mScene.getObjects();

		for (auto& object : objects) {

			if (object->isSelected()) {

				for(auto& mesh : object->getMeshes()) {
					if (mesh->isSelected()) {
						// TODO:
					}
				}
			}
		}

		ImGui::End();
	}

	void UI::LowRightPanel() {
		ImGui::SetNextWindowPos(ImVec2(_mData.width * 3 / 4, _mData.height * 3 / 4 - MainBarWidth * 4), ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2(_mData.width * 1 / 4, _mData.height * 1 / 4 + MainBarWidth * 4), ImGuiCond_Once);
		ImGui::SetNextWindowBgAlpha(1.0f);

		ImGui::Begin("##LowRight", nullptr, staticWindow | ImGuiWindowFlags_MenuBar);

		static int option = 0;

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("Render")) {
				option = 0;
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Shaders")) {
				option = 1;
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Soon 2")) {
				option = 2;
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Soon 3")) {
				option = 3;
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		switch (option) {
			case 0: break;
			case 1: showShaders(); break;
			case 2: break;
			case 3: break;
			default: break;
		}

		ImGui::End();
	}

	void UI::topPanel() {
		ImGui::SetNextWindowPos(ImVec2(_mData.width * 1 / 4,  MainBarWidth), ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2(_mData.width * 2 / 4, ActivityBarWidth * 2), ImGuiCond_Once);
		ImGui::SetNextWindowBgAlpha(1.0f);

		static std::string sTop    = "##Top";
		static std::string sCamera = "Camera";
		static std::string sRender = "Render";
		static std::string sADS    = "ADS";
		static std::string sFPS    = "FPS";
		static std::string sUI     = "UI ";

		ImGui::Begin(sTop.c_str(), nullptr, staticWindow | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollWithMouse);

		auto& cameras = _mScene.getCameras();

		ImGui::BeginGroup();

		for (std::shared_ptr<Camera>& camera : cameras) {
			if (camera->getComponent<Activatable>()->isActive()) {
				Utils::UI::ColoredBulletText(sCamera, std::to_string(camera->getID()), ImVec4(0.75f, 0.30f, 0.30f, 1.0f));
			}
			else {
				Utils::UI::ColoredBulletText(sCamera, std::to_string(camera->getID()), ImVec4(0.50f, 0.50f, 0.50f, 1.0f));
			}
			ImGui::SameLine();
		}

		ImGui::NewLine();
		// TODO: Update this when we have more render types
		Utils::UI::ColoredBulletText(sRender, sADS, ImVec4(0.75f, 0.30f, 0.30f, 1.0f));

		ImGui::EndGroup();

		ImGui::SameLine(_mData.width * 2 / 4 - 85);

		ImVec4 fpsColor = ImVec4(0.75f, 0.30f, 0.30f, 1.0f);

		if (Utils::Time::FPS < 10) {
			fpsColor = ImVec4(0.30f, 0.30f, 0.30f, 1.0f);
		}
		else if (Utils::Time::FPS < 30) {
			fpsColor = ImVec4(0.40f, 0.30f, 0.30f, 1.0f);
		}
		else if (Utils::Time::FPS < 60) {
			fpsColor = ImVec4(0.55f, 0.30f, 0.30f, 1.0f);
		}

		ImGui::BeginGroup();

		Utils::UI::ColoredBulletText(sFPS, std::to_string(Utils::Time::FPS), fpsColor);

		Utils::UI::ColoredBulletText(sUI, std::to_string(int(ImGui::GetIO().Framerate)), fpsColor);

		ImGui::EndGroup();

		ImGui::End();
	}

	void UI::botPanel() {
		ImGui::SetNextWindowPos(ImVec2(_mData.width * 1 / 4,  _mData.height * 3 / 4 - MainBarWidth * 4), ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2(_mData.width * 2 / 4, _mData.height * 1 / 4 + MainBarWidth * 4), ImGuiCond_Once);
		ImGui::SetNextWindowBgAlpha(1.0f);

		static std::string sBot = "##Bot";

		ImGui::Begin(sBot.c_str(), nullptr, staticWindow | ImGuiWindowFlags_MenuBar);

		if (ImGui::BeginMenuBar()) {
			ImGui::EndMenuBar();
		}

		auto& objects = _mScene.getObjects();

		for (auto& object : objects) {

			if (object->isSelected()) {

				for(auto& mesh : object->getMeshes()) {
					if (mesh->isSelected()) {
						mesh->drawUI();
					}
				}
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

	void UI::addOrRemoveToScene() {
		if (_mCallAddPerspective) {
			_mScene.addCamera(std::make_shared<PerspectiveCamera>(_mData.width, _mData.height));
			_mCallAddPerspective = false;
		}

		if (_mCallAddTriangle) {
			_mScene.addObject(std::make_shared<Triangle>());
			_mCallAddTriangle = false;
		}

		if (_mCallAddQuad) {
			_mScene.addObject(std::make_shared<Quad>());
			_mCallAddQuad = false;
		}

		if (_mCallAddCube) {
			_mScene.addObject(std::make_shared<Cube>());
			_mCallAddCube = false;
		}

		if (_mCallAddSphere) {
			_mScene.addObject(std::make_shared<Sphere>());
			_mCallAddSphere = false;
		}

		if (_mCallAddModel) {
			ImGui::OpenPopup("Add Model");
			_mCallAddModel = false;
		}

		if (_mCallAddLight) {
			_mScene.addLight(std::make_shared<Light>());
			_mCallAddLight = false;
		}

		if (_mCallRemove) {
			if (_mScene.isAnythingSelected()) {
				ImGui::OpenPopup("Remove?");
			}
			_mCallRemove = false;
		}

		addModel();
		remove();
	}

	void UI::addModel() {
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

		if (ImGui::BeginPopupModal("Add Model", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			static char path[128] = "";
			ImGui::InputTextWithHint("Model path", "/home/models/model.obj", &path[0], 128);

			if (ImGui::Button("Add", ImVec2(120, 0))) {
				_mScene.addObject(std::make_shared<Model>(path));

				// Reset path
				memset(path, 0, sizeof(path));

				ImGui::CloseCurrentPopup();
			}

			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) {
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	void UI::remove() {
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

		if (ImGui::BeginPopupModal("Remove?", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text("Are you sure you want to remove:");

			auto& entitys = _mScene.getEntitys();

			for (auto& entity : entitys) {
				EntityType type = entity->getType();

				static std::string labe = "Entity #" + std::to_string(entity->getID());

				if (type == EntityType::OBJECT) {
					labe = "Object #" + std::to_string(entity->getID());
				}
				else if (type == EntityType::CAMERA) {
					labe = "Camera #" + std::to_string(entity->getID());
				}

				if (entity->isSelected()) {
					Utils::UI::ColoredBulletText("", labe, ImVec4(0.86f, 0.26f, 0.26f, 1.00f));
				}
			}

			if (ImGui::Button("Remove", ImVec2(120, 0))) {
				_mScene.removeSelectedEntitys();

				ImGui::CloseCurrentPopup();
			}

			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) {
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	void UI::saveOrLoadScene() {
		if (_mCallSave) {
			ImGui::OpenPopup("Save Scene");
			_mCallSave = false;
		}
		if (_mCallLoad) {
			ImGui::OpenPopup("Load Scene");
			_mCallLoad = false;
		}

		save();
		load();
	}

	void UI::save() {
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

		if (ImGui::BeginPopupModal("Save Scene", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			static char path[128] = "";
			ImGui::InputTextWithHint("Save path", "/home/models/Save.vkm", &path[0], 128);

			if (ImGui::Button("Save", ImVec2(120, 0))) {
				auto& ss = _mScene.getSceneManager();

				ss->save(path);

				// Reset path
				memset(path, 0, sizeof(path));

				ImGui::CloseCurrentPopup();
			}

			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) {
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	void UI::load() {
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

		if (ImGui::BeginPopupModal("Load Scene", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			static char path[128] = "";
			ImGui::InputTextWithHint("Scene path", "/home/models/Scene.vkm", &path[0], 128);

			if (ImGui::Button("Load", ImVec2(120, 0))) {
				auto& ss = _mScene.getSceneManager();

				ss->load(path);

				// Reset path
				memset(path, 0, sizeof(path));

				ImGui::CloseCurrentPopup();
			}

			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) {
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	void UI::showShaders() {
		auto& shaders = _mScene.getShader();

		ImGui::BeginGroup();
		for (auto& shader : shaders) {
			if (ImGui::Button(shader->getName().c_str())) {
				shader->reCompleShader();
			}
		}
		ImGui::EndGroup();
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
