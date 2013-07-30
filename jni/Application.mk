#APP_PLATFORM := android-8     //对应2.2
#APP_PLATFORM := android-9     //对应2.3.1
APP_STL := stlport_static
#APP_STL参数
#system - 使用默认最小的C++运行库，这样生成的应用体积小，内存占用小，但部分功能将无法支持
#stlport_static - 使用STLport作为静态库，这项是Android开发网极力推荐的
#stlport_shared - STLport 作为动态库，这个可能产生兼容性和部分低版本的Android固件，目前不推荐使用。
#gnustl_static  - 使用 GNU libstdc++ 作为静态库
#APP_MODULES := libbstruct 编辑.a使用
APP_MODULES := libbstruct
