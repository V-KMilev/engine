set(ENGINE_SRC_DIR ${PROJECT_SOURCE_DIR}/src)

set(SOURCES
	${ENGINE_SRC_DIR}/common/input/input_manager.cpp
	${ENGINE_SRC_DIR}/common/input/input_key.cpp
	${ENGINE_SRC_DIR}/common/input/input_key_combination.cpp

	${ENGINE_SRC_DIR}/common/logger/logger.cpp

	${ENGINE_SRC_DIR}/common/profiler/tracer.cpp

	${ENGINE_SRC_DIR}/component/component.cpp
	${ENGINE_SRC_DIR}/component/emit/emit.cpp
	${ENGINE_SRC_DIR}/component/material/material.cpp
	${ENGINE_SRC_DIR}/component/view/view.cpp

	${ENGINE_SRC_DIR}/entity/camera/camera.cpp
	${ENGINE_SRC_DIR}/entity/camera/perspective/perspective_camera.cpp

	${ENGINE_SRC_DIR}/entity/light/light.cpp

	${ENGINE_SRC_DIR}/entity/mesh/mesh.cpp

	${ENGINE_SRC_DIR}/entity/object/object.cpp
	${ENGINE_SRC_DIR}/entity/object/triangle/triangle.cpp
	${ENGINE_SRC_DIR}/entity/object/cube/cube.cpp
	${ENGINE_SRC_DIR}/entity/object/quad/quad.cpp
	${ENGINE_SRC_DIR}/entity/object/sphere/sphere.cpp
	${ENGINE_SRC_DIR}/entity/object/model/model.cpp

	${ENGINE_SRC_DIR}/entity/entity_id.cpp
	${ENGINE_SRC_DIR}/entity/entity.cpp

	${ENGINE_SRC_DIR}/event/event_manager.cpp

	${ENGINE_SRC_DIR}/scene/system/scene_manager.cpp
	${ENGINE_SRC_DIR}/scene/orientation/orientation.cpp
	${ENGINE_SRC_DIR}/scene/gizmo/gizmo.cpp
	${ENGINE_SRC_DIR}/scene/grid/grid.cpp
	${ENGINE_SRC_DIR}/scene/scene.cpp

	${ENGINE_SRC_DIR}/ui/ui.cpp

	${ENGINE_SRC_DIR}/utils/utils.cpp

	${ENGINE_SRC_DIR}/window/window.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
	${ENGINE_SRC_DIR}/common/input
	${ENGINE_SRC_DIR}/common/logger
	${ENGINE_SRC_DIR}/common/profiler
	
	${ENGINE_SRC_DIR}/component
	${ENGINE_SRC_DIR}/component/emit
	${ENGINE_SRC_DIR}/component/material
	${ENGINE_SRC_DIR}/component/view

	${ENGINE_SRC_DIR}/entity

	${ENGINE_SRC_DIR}/entity/camera
	${ENGINE_SRC_DIR}/entity/camera/perspective
	
	${ENGINE_SRC_DIR}/entity/light

	${ENGINE_SRC_DIR}/entity/mesh

	${ENGINE_SRC_DIR}/entity/object
	${ENGINE_SRC_DIR}/entity/object/triangle
	${ENGINE_SRC_DIR}/entity/object/cube
	${ENGINE_SRC_DIR}/entity/object/quad
	${ENGINE_SRC_DIR}/entity/object/sphere
	${ENGINE_SRC_DIR}/entity/object/model
	
	${ENGINE_SRC_DIR}/event
	
	${ENGINE_SRC_DIR}/scene/system
	${ENGINE_SRC_DIR}/scene/orientation
	${ENGINE_SRC_DIR}/scene/gizmo
	${ENGINE_SRC_DIR}/scene/grid
	${ENGINE_SRC_DIR}/scene
	
	${ENGINE_SRC_DIR}/ui

	${ENGINE_SRC_DIR}/window

	${ENGINE_SRC_DIR}/utils
)

target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})
