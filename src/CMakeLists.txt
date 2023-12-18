set(ENGINE_SRC_DIR ${PROJECT_SOURCE_DIR}/src)

set(SOURCES
	${ENGINE_SRC_DIR}/common/input/input_manager.cpp
	${ENGINE_SRC_DIR}/common/input/input_key.cpp

	${ENGINE_SRC_DIR}/common/logger/logger.cpp

	${ENGINE_SRC_DIR}/common/profiler/tracer.cpp

	${ENGINE_SRC_DIR}/entity/camera/camera.cpp
	${ENGINE_SRC_DIR}/entity/camera/perspective_camera.cpp
	
	${ENGINE_SRC_DIR}/entity/object/object.cpp
	${ENGINE_SRC_DIR}/entity/object/basic_geometry/cube.cpp
	${ENGINE_SRC_DIR}/entity/object/basic_geometry/quad.cpp
	${ENGINE_SRC_DIR}/entity/object/basic_geometry/triangle.cpp
	${ENGINE_SRC_DIR}/entity/object/mesh/mesh.cpp
	${ENGINE_SRC_DIR}/entity/object/model/model.cpp
	${ENGINE_SRC_DIR}/entity/component/component.cpp
	${ENGINE_SRC_DIR}/entity/entity_id.cpp
	${ENGINE_SRC_DIR}/entity/entity.cpp

	${ENGINE_SRC_DIR}/event/event_manager.cpp

	${ENGINE_SRC_DIR}/material/material.cpp

	${ENGINE_SRC_DIR}/scene/system/scene_manager.cpp
	${ENGINE_SRC_DIR}/scene/orientation/orientation.cpp
	${ENGINE_SRC_DIR}/scene/gizmo/gizmo.cpp
	${ENGINE_SRC_DIR}/scene/grid/grid.cpp
	${ENGINE_SRC_DIR}/scene/scene.cpp

	${ENGINE_SRC_DIR}/ui/ui.cpp

	${ENGINE_SRC_DIR}/window/window.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
	${ENGINE_SRC_DIR}/common/input

	${ENGINE_SRC_DIR}/common/logger

	${ENGINE_SRC_DIR}/common/profiler

	${ENGINE_SRC_DIR}/common/utils

	${ENGINE_SRC_DIR}/entity/camera
	${ENGINE_SRC_DIR}/entity/object
	${ENGINE_SRC_DIR}/entity/object/basic_geometry
	${ENGINE_SRC_DIR}/entity/object/mesh
	${ENGINE_SRC_DIR}/entity/object/model
	${ENGINE_SRC_DIR}/entity/component
	${ENGINE_SRC_DIR}/entity

	${ENGINE_SRC_DIR}/event

	${ENGINE_SRC_DIR}/material

	${ENGINE_SRC_DIR}/scene/system
	${ENGINE_SRC_DIR}/scene/orientation
	${ENGINE_SRC_DIR}/scene/gizmo
	${ENGINE_SRC_DIR}/scene/grid
	${ENGINE_SRC_DIR}/scene

	${ENGINE_SRC_DIR}/ui
	${ENGINE_SRC_DIR}/window
)

target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})
