g++ \
    source_files\/main.cpp \
    source_files\/Mesh.cpp \
    source_files\/Model.cpp \
    source_files\/TextureManager.cpp \
    source_files\/ShaderManager.cpp \
    source_files\/Player.cpp \
    source_files\/CollisionManager.cpp \
    source_files\/GUIManager.cpp \
    source_files\/GLCore.cpp \
    source_files\/stb_image.cpp \
    /usr/include/glad.c \
    -o icyTower3D \
    -Wall -lGL -lGLU -lGLEW -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor -lGLESv2 -lassimp -I/usr/include/freetype2 -I/usr/include/libpng16

./icyTower3D
