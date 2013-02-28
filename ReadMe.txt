该项目是仿照 TSVNCache.exe 的启动方式来实现的，
TSVNCache.exe 是通过修改注册表的一些文件关联方式，
然后当资源管理器启动时由于会加载这些文件关联方式，从而间接将我们的 DLL 加载到 Explorer.exe 进程，
此时便会通过这个 DLL 来启动进程，从而间接达到进程自动启动的目的。
至于我们的 DLL 则需要放在此路径下：C:\\AutoStartDLL.dll。
同时我们的 DLL 会启动 C:\\AutoStartExe.exe。
至于项目中生成的 AutoStart.exe 则是用来注册或者取消注册自动启动的，
注册即是将 AutoStartDLL.dll 加入到需要关联的注册表中，
取消注册即是将 AutoStartDLL.dll 从管理的注册表中删除。