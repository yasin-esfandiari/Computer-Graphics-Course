cmake_minimum_required(VERSION 3.0)

set(MY_ADDITIONAL_SOURCES
    # Please add your source files here
)

set(ASSIGNMENT_SOURCES
    main/a_render_chessking.cpp
)

# file(COPY
#     models/stones_bump.png
#     models/stones_spec.png
#     DESTINATION ${CMAKE_BINARY_DIR}/models
# )

file(COPY
    models/pawn.mtl
    models/cow.obj
    models/pawn.obj
    models/cow.mtl
    models/rc_pawn.obj
    models/rc_pawn.mtl
    models/rc_rook.obj
    models/rc_rook.mtl
    models/rc_queen.obj
    models/rc_queen.mtl
    models/rc_king.obj
    models/rc_king.mtl
    models/rc_plane.obj
    models/rc_plane.mtl
    # models/12937_WoodenChessPawnSideB_diffuse.jpg
    models/12937_WoodenChessPawnSideB_diffuse.png
    models/Bark_007_BaseColor.png
    models/stones_diffuse.png
    DESTINATION ${CMAKE_BINARY_DIR}/models
)
