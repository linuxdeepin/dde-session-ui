## 配置文件说明

优先级遵循这样的模式：

* 最高优先级的配置文件，所有的模块都优先使用这项： `/etc/deepin/dde-session-ui.conf`

* 以模块命名的配置文件，如: `/etc/deepin/dde-shutdown.conf`

* 模块自带的配置文件，如： `/usr/share/dde-session-ui/dde-shutdown.conf`

* dde-session-ui自带的配置文件，存储的是通用的配置： `/usr/share/dde-session-ui/dde-session-ui.conf`

配置文件中采用了ini的组，会根据组/键来说明，如: `Shutdown/enableSystemMonitor`。

## 锁屏和Greeter
定义在dde-session-ui.conf中。

* `General/loginPromptAvatar` 设置是否启用本地用户列表，默认为true。
* `General/loginPromptInput` 设置是否启用LDAP登录，默认为false。
* `Lock/showSwitchUserButton` 设置是否显示用户切换按钮，值固定三项： `[always | ondemand | disabled]`，默认为ondemand。
* `Power/sleep` 设置是否启用锁屏、登录界面和关机界面的待机按钮。
* `Power/hibernate` 设置是否启用锁屏、登录界面和关机界面的休眠按钮。
* `OS/isDeepin` 设置是否启用锁屏的指纹和密码同步验证机制，在Deepin发行版为true，其他发行版为false。

## dde-shutdown
定义在dde-shutdown.conf中。
* `Shutdown/enableSystemMonitor` 设置是否启用关机界面的系统监视器按钮，默认为true。
