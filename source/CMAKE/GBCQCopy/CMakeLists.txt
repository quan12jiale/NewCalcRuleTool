##设置库名称
set(LIBRARY_TARGET_NAME GBCQCopy)

##设置生成目标
add_library(${LIBRARY_TARGET_NAME} )

##打开qt特性配置
set_target_properties(${LIBRARY_TARGET_NAME} PROPERTIES AUTOMOC ON)
set_target_properties(${LIBRARY_TARGET_NAME} PROPERTIES AUTOUIC ON)
set_target_properties(${LIBRARY_TARGET_NAME} PROPERTIES AUTORCC ON)

set(Dest_BIN_DIR "${PROJECT_SOURCE_DIR}/../build/bin/${PROJECT_PLATFORM}/$<CONFIG>")
set(Dest_LIB_DIR "${PROJECT_SOURCE_DIR}/../build/lib/${PROJECT_PLATFORM}/$<CONFIG>")
set(GTJ_BUILD_BIN_DIR "${GTJ_BULID_DIR}/bin/${PROJECT_PLATFORM}/$(Configuration)")

add_custom_command(TARGET ${LIBRARY_TARGET_NAME} POST_BUILD
	COMMAND cmd /c ${PROJECT_SOURCE_DIR}/GBCQCopy/CopyAllReleaseDebug32.cmd "${QT_PREFIX_PATH}/bin" "${EXTERNAL_DIR}" "${GTJ_BUILD_BIN_DIR}" "${Dest_BIN_DIR}" "${Dest_LIB_DIR}"

)
