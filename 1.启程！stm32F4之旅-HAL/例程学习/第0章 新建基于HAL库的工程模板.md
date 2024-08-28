# 第0章 新建基于HAL库的工程模板

## 1. 新建工程模板

在建立工程之前，我们建议用户在电脑的某个目录下面建立一个文件夹，后面所建立的工程都可以放在这个文件夹下面，这里我们建立一个文件夹为 Template。这是工程的根目录文件夹。然后为了方便我们存放工程需要的一些其他文件，这里我们还新建下面 4 个子文件夹： CORE ， HALLIB， OBJ 和 USER。至于这些文件夹名字，实际上是可以任取的，我们这样取名只是为了方便识别。对于这些文件夹用来存放什么文件，我们后面的步骤会一一提到。新建好的目录结构如下图：

![](C:\Users\qiu\AppData\Roaming\marktext\images\2024-08-28-16-35-49-image.png)

接下来，打开 MDK，点击菜单 Project –>New Uvision Project ，然后将目录定位到刚才建立的文件夹 Template 之下的 USER 子目录， 工程取名为 Template 之后点击保存， 工程文件就都保存到 USER 文件夹下面。

![](C:\Users\qiu\AppData\Roaming\marktext\images\2024-08-28-16-35-24-image.png)

接下来会出现一个选择 Device 的界面，就是选择我们的芯片型号，这里我们定位到STMicroelectronics下面的STM32F407ZGT6 (针对我们的正点原子探索者STM32F4板子是这个型号)。 这里我们选择 STMicroelectronics→STM32F4 Series→STM32F407→STM32F07ZG（如果使用的是其他系列的芯片，选择相应的型号就可以了， 例如我们的精英 STM32 开发板是STM32F103ZE。 特别注意：一定要安装对应的器件 pack 才会显示这些内容）。

![](C:\Users\qiu\AppData\Roaming\marktext\images\2024-08-28-16-36-33-image.png)

点击 OK， MDK 会弹出 Manage Run-Time Environment 对话框

![](C:\Users\qiu\AppData\Roaming\marktext\images\2024-08-28-16-37-06-image.png)

这是 MDK5 新增的一个功能，在这个界面，我们可以添加自己需要的组件，从而方便构建开发环境，不过这里我们不做介绍，我们直接点击 Cancel即可。

![](C:\Users\qiu\AppData\Roaming\marktext\images\2024-08-28-16-41-00-image.png)

现在我们看看 USER 目录下面内容：

![](C:\Users\qiu\AppData\Roaming\marktext\images\2024-08-28-16-41-50-image.png)

这里我们说明一下， Template.uvprojx 是工程文件，非常关键，不能轻易删除， MDK5.20生成的工程文件是以.uvprojx 为后缀。 DebugConfig， Listings 和 Objects 三个文件夹是 MDK 自动生成的文件夹。其中 DebugConfig 文件夹用于存储一些调试配置文件， Listings 和 Objects 文件夹用来存储 MDK 编译过程的一些中间文件。这里，我们把 Listings 和 Objects 文件夹删除，我们会在下一步骤中新建一个 OBJ 文件夹，用来存放编译中间文件。当然，我们不删除这两个文件夹也没有关系，只是我们不用它而已。

接下来我们将从官方 stm32cubeF4 包里面复制一些我们新建工程需要的关键文件到我们的工程目录中。首先，我们要将 STM32CubeF4 包里的源码文件复制到我们的工程目录文件夹下面。 打开官方 STM32CubeF4 包，定位到我们之前准备好的 HAL 库包的目录: \STM32Cube_FW_F4_V1.24.1\Drivers\STM32F4xx_HAL_Driver 下面，将目录下面的 Src,Inc 文件夹复制到我们刚才建立的 HALLIB 文件夹下面。 Src 存放的是 HAL 库的.c 文件， Inc 存放的是对应的.h 文件。

![](C:\Users\qiu\AppData\Roaming\marktext\images\2024-08-28-16-42-42-image.png)

接下来，我们要将 STM32CubeF4 包里面相关的启动文件以及一些关键头文件复制到我们的工程目录 CORE 之下。打开 STM32CubeF4 包，定位到目录\ STM32Cube_FW_F4_V1.24.1\Drivers\CMSIS\Device\ST\STM32F4xx\Source\Templates\arm 下面，将 文 件 startup_stm32f407xx.s 复 制 到 CORE 目 录 下 面 。 然 后 定 位 到 目 录 \ STM32Cube_FW_F4_V1.24.1\Drivers\CMSIS\Include，将里面的几个头文件： cmsis_armcc.h， cmsis_armclang.h， cmsis_compiler.h， cmsis_version.h， mpu_armv7.h， core_cm4.h 同样复制到 CORE 目录下面。

![](C:\Users\qiu\AppData\Roaming\marktext\images\2024-08-28-16-43-28-image.png)

接下来我们要复制工程模板需要的一些其他头文件和源文件到我们工程。首先定位到目录：\ STM32Cube_FW_F4_V1.24.1\Drivers\CMSIS\Device\ST\STM32F4xx\Include 将里面的 3 个文件stm32f4xx.h， system_stm32f4xx.h 和 stm32f407xx.h 复制到 USER 目录之下。 这三个头文件是STM32F4 工程非常关键的头文件，

然后进入目录\STM32Cube_FW_F4_V1.24.1\Projects\STM32F4-Discovery\Templates 目录下，这个目录下面有好几个文件夹，我们需要从 Src 和 Inc 文件夹下面复制我们需要的文件到 USER 目录。

![](C:\Users\qiu\AppData\Roaming\marktext\images\2024-08-28-16-44-54-image.png)

首先我们打开Inc目录，将目录下面的3个头文件stm32f4xx_it.h， stm32f4xx_hal_conf.h 和main.h全部复制到USER 目录下面。然后我们打开 Src 目录，将下面的四个源文件 system_stm32f4xx.c， stm32f4xx_it.c, stm32f4xx_hal_msp.c 和 main.c 同样全部复制到 USER 目录下面。相关文件复制到 USER 目录之后 USER 目录文件如下图：

![](C:\Users\qiu\AppData\Roaming\marktext\images\2024-08-28-16-46-10-image.png)

接下来，我们还需要复制 ALIENTEK 编写的 SYSTEM 文件夹内容到工程目录中。首先，我们需要解释一下，这个SYSTEM 文件夹内容是 ALIENTEK 为开发板用户编写的一套非常实用的函数库，比如系统时钟初始化，串口打印，延时函数等，这些函数被很多工程师运用到自己的工程项目中。当然，大家也可以根据自己需求决定是否需要 SYSTEM 文件夹，对于 STM32F407 的工程模板，如果没有加入 SYSTEM 文件夹，那么大家需要自己定义系统时钟初始化。

![](C:\Users\qiu\AppData\Roaming\marktext\images\2024-08-28-16-47-31-image.png)

到这里，工程模板所需要的所有文件都已经复制进去。接下来，我们将在 MDK 中将这些文件添加到工程。
