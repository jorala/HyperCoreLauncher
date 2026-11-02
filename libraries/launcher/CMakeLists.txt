cmake_minimum_required(VERSION 3.15)
project(launcher Java)
find_package(Java 1.7 REQUIRED COMPONENTS Development)

include(UseJava)
set(CMAKE_JAVA_JAR_ENTRY_POINT org.hypercorelauncher.EntryPoint)
set(CMAKE_JAVA_COMPILE_FLAGS -target 7 -source 7)

set(SRC
    org/hypercorelauncher/EntryPoint.java
    org/hypercorelauncher/launcher/Launcher.java
    org/hypercorelauncher/launcher/impl/AbstractLauncher.java
    org/hypercorelauncher/launcher/impl/StandardLauncher.java
    org/hypercorelauncher/exception/ParameterNotFoundException.java
    org/hypercorelauncher/exception/ParseException.java
    org/hypercorelauncher/utils/Parameters.java
    org/hypercorelauncher/utils/ReflectionUtils.java
    org/hypercorelauncher/utils/logging/Level.java
    org/hypercorelauncher/utils/logging/Log.java
    org/hypercorelauncher/legacy/LegacyProxy.java
)

set(LEGACY_SRC
    legacy/org/hypercorelauncher/legacy/LegacyFrame.java
    legacy/org/hypercorelauncher/legacy/LegacyLauncher.java
    legacy/org/hypercorelauncher/legacy/fix/online/Handler.java
    legacy/org/hypercorelauncher/legacy/fix/online/OnlineFixes.java
    legacy/org/hypercorelauncher/legacy/fix/online/OnlineModeFix.java
    legacy/org/hypercorelauncher/legacy/fix/online/SkinFix.java
    legacy/org/hypercorelauncher/legacy/utils/Base64.java
    legacy/org/hypercorelauncher/legacy/utils/api/MojangApi.java
    legacy/org/hypercorelauncher/legacy/utils/api/Texture.java
    legacy/org/hypercorelauncher/legacy/utils/json/JsonParseException.java
    legacy/org/hypercorelauncher/legacy/utils/json/JsonParser.java
    legacy/org/hypercorelauncher/legacy/utils/url/ByteArrayUrlConnection.java
    legacy/org/hypercorelauncher/legacy/utils/url/UrlUtils.java
    legacy/net/minecraft/Launcher.java
    legacy/org/hypercorelauncher/legacy/LegacyProxy.java
)

add_jar(NewLaunch ${SRC})
add_jar(NewLaunchLegacy ${LEGACY_SRC} INCLUDE_JARS NewLaunch)
install_jar(NewLaunch "${JARS_DEST_DIR}")
install_jar(NewLaunchLegacy "${JARS_DEST_DIR}")
