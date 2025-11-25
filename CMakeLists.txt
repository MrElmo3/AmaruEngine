cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(AmaruEngine VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BUILD_DIR ${CMAKE_SOURCE_DIR}/build/Assets)
set(PRIVATE_DIR ${CMAKE_SOURCE_DIR}/src/private)
set(PUBLIC_DIR ${CMAKE_SOURCE_DIR}/src/public)
set(ASSETS_SRC_DIR ${CMAKE_SOURCE_DIR}/Assets)

# Input components Source
set(COMPONENTS_INPUT
	${PRIVATE_DIR}/Core/Components/Input/FollowMouseComponent.cpp
	${PRIVATE_DIR}/Core/Components/Input/InputComponent.cpp
)

# Physcis components source
set(COMPONENTS_PHYSICS
	${PRIVATE_DIR}/Core/Components/Physics/AColliderComponent.cpp
	${PRIVATE_DIR}/Core/Components/Physics/2D/ACollider2DComponent.cpp
	${PRIVATE_DIR}/Core/Components/Physics/2D/Rigidbody2DComponent.cpp
	${PRIVATE_DIR}/Core/Components/Physics/2D/SquareColliderComponent.cpp
	${PRIVATE_DIR}/Core/Components/Physics/3D/ACollider3DComponent.cpp
	${PRIVATE_DIR}/Core/Components/Physics/3D/BoxColliderComponent.cpp
	${PRIVATE_DIR}/Core/Components/Physics/3D/Rigidbody3DComponent.cpp
)

# Render components source
set(COMPONENTS_RENDER
	${PRIVATE_DIR}/Core/Components/Render/CameraComponent.cpp
	${PRIVATE_DIR}/Core/Components/Render/RenderCubeComponent.cpp
	${PRIVATE_DIR}/Core/Components/Render/RenderQuadComponent.cpp
	${PRIVATE_DIR}/Core/Components/Render/Lights/LightComponent.cpp
)

# COmponents source
set(COMPONENTS
	${COMPONENTS_INPUT}
	${COMPONENTS_PHYSICS}
	${COMPONENTS_RENDER}
)

#Materials source
set(MATERIALS
	${PRIVATE_DIR}/Core/Materials/AMaterial.cpp
	${PRIVATE_DIR}/Core/Materials/LitMaterial.cpp
	${PRIVATE_DIR}/Core/Materials/UnlitMaterial.cpp
	${PRIVATE_DIR}/Core/Materials/ColorMaterial.cpp
)

#Objects source
set(OBJECTS
	${PRIVATE_DIR}/Core/Objects/AObject.cpp
	${PRIVATE_DIR}/Core/Objects/EmptyObject.cpp
	${PRIVATE_DIR}/Core/Objects/General/CameraObject.cpp
	${PRIVATE_DIR}/Core/Objects/3D/Cube.cpp
	${PRIVATE_DIR}/Core/Objects/2D/Square.cpp
	${PRIVATE_DIR}/Core/Objects/Light/PointLight.cpp
	${PRIVATE_DIR}/Core/Objects/Light/DirectionalLight.cpp
	${PRIVATE_DIR}/Core/Objects/Light/SpotLight.cpp
)

#Physics engine source
set(PHYSICS
	${PRIVATE_DIR}/Core/Physics/PhysicsEngine2D.cpp
	${PRIVATE_DIR}/Core/Physics/PhysicsEngine3D.cpp
)

#Render engine source
set(RENDER
	${PRIVATE_DIR}/Core/Render/Color.cpp
	${PRIVATE_DIR}/Core/Render/Render.cpp
	${PRIVATE_DIR}/Core/Render/Shader.cpp
)

#Scene mannager source
set(SCENES
	${PRIVATE_DIR}/Core/Scenes/ASceneController.cpp
	${PRIVATE_DIR}/Core/Scenes/TestCollisionScene.cpp
	${PRIVATE_DIR}/Core/Scenes/TestScene.cpp
	${PRIVATE_DIR}/Core/Scenes/TestPhongLightScene.cpp
)

#Core source
set(CORE
	${PRIVATE_DIR}/Core/Global.cpp
	${PRIVATE_DIR}/Core/Window.cpp

	${COMPONENTS}
	${MATERIALS}
	${OBJECTS}
	${PHYSICS}
	${RENDER}
	${SCENES}
)

#Util Source Files
set(UTIL
	${PRIVATE_DIR}/Util/Graph.cpp
	${PRIVATE_DIR}/Util/Logger.cpp
	${PRIVATE_DIR}/Util/Singleton.cpp
	${PRIVATE_DIR}/Util/Utility.cpp
)

#Game Source Files
Set(GAME_SCENES
	${PRIVATE_DIR}/Game/Scenes/Maze3DScene.cpp
)

set(GAME_OBJECTS
	${PRIVATE_DIR}/Game/Objects/Player.cpp
	${PRIVATE_DIR}/Game/Objects/Maze.cpp
)

set(GAME_COMPONENTS
	${PRIVATE_DIR}/Game/Components/MovementComponent.cpp
	${PRIVATE_DIR}/Game/Components/MazeGeneratorComponent.cpp
)

set(GAME
	${GAME_SCENES}
	${GAME_OBJECTS}
	${GAME_COMPONENTS}
)

set(SOURCES
	src/Main.cpp
	src/glad.c

	${CORE}
	${UTIL}
	${GAME}
)

add_executable(
	AmaruEngine
	${SOURCES}
)

target_include_directories(AmaruEngine PUBLIC
	includes
	src/public)

add_subdirectory(libs/glfw-3.4)
target_link_libraries(AmaruEngine glfw)
find_package(OpenGL REQUIRED)
target_link_libraries(AmaruEngine OpenGL::GL)

add_custom_target(copy_assets ALL
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${CMAKE_SOURCE_DIR}/Assets"
	"$<TARGET_FILE_DIR:AmaruEngine>/Assets"
	COMMENT "Updating Assets folder..."
)