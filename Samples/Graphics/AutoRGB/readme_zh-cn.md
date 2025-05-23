![](./media/image1.png)

# AutoRGB 样本

*此样本与 Microsoft 游戏开发工具包兼容 (2023 年 3 月 QFE1)*

# 说明

此示例演示了如何从给定场景中提取具有代表性的单一环境颜色，并使用它从连接设备 (HID) 点亮灯。使用 LampArray API 与设备通信，因此此样本**需要** 2023 年 3 月 QFE 1 GDK 或更高版本。

![](./media/readme/image0.png)

> **请注意:** 自 2023 年 3 月 QFE1 版本起，GDK LampArray API 仅支持主机上的以下设备。 将来的恢复版本会添加对其他设备的支持。
> - 适用于 Xbox One 的 Razer Turret（键盘和鼠标）
> - Razer BlackWidow Tournament Edition Chroma V2

# 生成示例

如果使用 Xbox One 开发工具包，请将活动解决方案平台设置为 `Gaming.Xbox.XboxOne.x64`。

如果使用 Xbox Series X|S，请将活动解决方案平台设置为 `Gaming.Xbox.Scarlett.x64`。

*有关详细信息，请参阅* *GDK 文档*中的__运行示例__。

# 控件

| 操作 | 游戏板 |
|---|---|
| 切换启发式方法 | A |
| 切换场景色调 | B |
| 切换模式 | Y |
| 切换灯(样本模式) | RB |
| 切换直方图 UI | LB |
| 修改颜色切换速度 | 右/左方向键 |
| 修改基础光强度(样本模式) | 上/下方向键 |
| 移动到下一个图像(屏幕截图模式) | RT |
| 移动到上一个图像(屏幕截图模式) | LT |
| 设置相机周围模式(样本模式) | 用于向上移动的 LT-RT，向下向右摇杆旋转视图左摇杆以移动翻译 |
| 退出示例。 | &ldquo;视图&rdquo;按钮 |

# 实现说明

该样本演示了两种模式。

## 样本模式

![](./media/readme/image1.png)

第一个(默认)是常规样本，可以在其中在点亮的场景周围自由移动相机。 该场景将围绕其呈现一个颜色带，表示当前正在提取的颜色。 相机可以在场景中自由移动，并且颜色将实时更新(它是根据上一帧的信息计算得出的，因此延迟为 1 帧)。

控件允许通过以下方式修改场景。 可以打开和关闭指示灯(以测试不同亮度以及提取的颜色的外观)。 此外，可以控制**基础光强度**级别以上升或下降。 如果设置为零，表示当灯处于关闭状态时，场景将变为黑色。 此样本还允许更改代表性颜色的速度。 这可以像 1 帧一样快，但也可以通过降低**颜色切换速度**来使它更加流畅。

最后，通过按 B，示例将对整个场景应用红色、绿色或蓝色色调。 这样可以轻松测试转换速度是否合适，或者所使用的启发式方法是否正确处理了基本情况(下文将详细介绍启发式方法)。 按 B 时，示例将在无色调、红色、蓝色和绿色之间旋转(然后返回到无色调)。 这仅适用于样本模式。

## 屏幕截图模式

![](./media/readme/image2.png)

按 Y 时，样本会在模式之间切换。 屏幕截图模式将显示游戏的静态图像，周围具有相同的颜色带。 游戏手柄触发器允许导航屏幕截图。 这旨在显示提取的颜色与生产级别游戏中的场景的匹配程度。 在此模式下，相机不能四处移动，也不能修改色调或基础光强度。

# UI

该样本通过 (PIX 中的直方图)对当前场景(示例背景或当前选择的屏幕截图)进行一些图像处理，并获得 4 个直方图，如下所示:

![](./media/readme/image3.png)

下图将红色色调应用于场景。 直方图 UI 也会相应改变，在强度较高的分区中显示更多红色。![](./media/readme/image4.png)

前三个直方图分别对应场景中的红色、绿色和蓝色，以及它们在强度上的分布情况。 每个直方图分为 8 个直条。 由于每种颜色的强度从 0 到 255，每个直条包含 32 个强度值，从低强度(左)到高强度(右)。 每个直条的高度将对应于该强度范围内的像素数。 第四个直方图将显示 luma 分量，并且对于获取场景亮度的度量值非常有用。

> 某些设备只允许设置其颜色，但其它设备也允许修改强度。

# 启发式

样本以直方图的形式获取颜色分布信息后，可以使用它来获取具有代表性的环境颜色。如果要执行此操作，需要使用**启发式**。 目前，此样本仅用于获取整个场景的单种颜色(因此我们未获得有关颜色位置的信息)。 这意味着启发式需要处理三种成分(红、绿、蓝)平均分布的情况，而不是返回灰色，而是得出哪种颜色对场景的贡献更大。

该样本有三个启发式，可以使用 A 按钮进行更改。 下面是这三项的简要说明:

1. Top X Buckets：: 此方法首先将仅查看三个组件中的最高直条。 如果第一个箱所覆盖的像素量超过设置的阈值，仅使用此直条来计算颜色。 否则，将添加下一个直条并重复此过程。 这种启发式的理念是为具有较高强度像素的颜色赋予更多权重，同时自适应地更改直条数以考虑像素计数。

2. 平均值: 这仅采用所有直方图的加权平均值，然后规范化结果。 它为光线充足的场景提供了类似的结果，但也倾向于在更暗、更暗的场景中产生颜色。

3. Buckets x AVG (**默认**): 如名称所示，此启发式方法将插入前两个值的结果，并提供介于两者之间的值。

# 说明

# 更新历史记录

2023 年 4 月 8 日 -- 移植样本。

# 隐私声明

在编译和运行示例时，将向 Microsoft 发送示例可执行文件的文件名以帮助跟踪示例使用情况。 若要选择退出此数据收集，你可以删除 Main.cpp 中标记为&ldquo;示例使用遥测&rdquo;的代码块。

有关 Microsoft 的一般隐私策略的详细信息，请参阅 [Microsoft 隐私声明](https://privacy.microsoft.com/en-us/privacystatement/)。


