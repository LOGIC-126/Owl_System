# 有关无人机多方面的讨论：入门无人机



## 引言：

​	实际上我也不知道为啥要写这个玩意儿，莫名奇妙的，本来就是想整理一下资料，规划一下下一步需要做的无人机，但是整理着整理着就整理多了，多了多了吧，那就整理的完善一点，也并非什么坏处。以后要是想给别人看就给别人看，也不失为一件好事。故写吧。多写点。

[TOC]

## 一：机体篇

### 1 、机架: 

#### ① 轴距: 

​	对角线两电机中心的距离 （四轴），而严格的定义通常为电机轴心轴围成的外接圆周的直径，单位毫米mm，对称多旋翼无人机中轴距即为对角线上的两个电机轴心的距离，轴距的大小确定了螺旋桨的尺寸上限，从而限定了螺旋桨能产生的最大拉力，也就是直接影响到无人机的载重能力（ 比赛对此参数往往有要求，电赛，Cuadc中 450 mm与 550 mm便是此参数 )

在此附上轴距公式，在动力系统相关会详细讨论。
$$
R = \frac { r _ { m a x } } { \sin \frac { \theta } { 2 } } = \frac { r _ { m a x } } { \sin \frac { \pi } { n } }
$$

$$
其中： R为机架半径，\frac{1}{2}轴距
，r _ { m a x }为桨的最大半径，\theta为轴臂夹角，n为轴臂数量
$$

#### ② 材质:

​	机架的材质关乎机体的重量， 常见材料有塑料， 碳纤维，一些小型无人机也会采用PCB(印刷电路板 )作为机架 ，也有用铝合金作为机架的案例 。*（笔者曾使用铝型材设计过一代机架，有着拔群的重量）*
​	而在结构连接件上， 常采用螺丝， 五金件， 3D打印结构件等。

------

##### 有关螺丝的讨论:

​	桨叶高速运动会导致螺丝松动，故会采用如自锁螺丝，黏着剂等方式固定螺丝。

------



------

##### 有关机架材质的讨论:

​	在此我们只讨论塑料与碳纤维。

​	***塑料***: 重量较轻， 强度和刚度不大，在螺丝固定情况下有松动可能 。成本低， 在 3D打印技术如此成熟的今天， 采用 3D打印的塑料机架不失为一种好选择 。

​	***碳纤维***: 密度低， 强度刚度高， 有一定的减振效果 。但成本相对较高， 我们所用的无人机往往是采用碳管作为
机臂， 这会带来电机难以对正的问题。

------

#### ③ 重量:

​	机架最重要的参数，在设计时应使其尽可能轻使飞机有更多空余负载 。
同时对于重量有一定相关法规，参考 2024 年 1 月 1 日施行的 [《无人驾驶航空器飞行管理暂行条例》](https://www.gov.cn/zhengce/content/202306/content_6888799.htm)

```
（二）微型无人驾驶航空器，是指空机重量小于0.25千克，最大飞行真高不超过50米，最大平飞速度不超过40千米/小时，无线电发射设备符合微功率短距离技术要求，全程可以随时人工介入操控的无人驾驶航空器。
（三）轻型无人驾驶航空器，是指空机重量不超过4千克且最大起飞重量不超过7千克，最大平飞速度不超过100千米/小时，具备符合空域管理要求的空域保持能力和可靠被监视能力，全程可以随时人工介入操控的无人驾驶航空器，但不包括微型无人驾驶航空器。
（四）小型无人驾驶航空器，是指空机重量不超过15千克且最大起飞重量不超过25千克，具备符合空域管理要求的空域保持能力和可靠被监视能力，全程可以随时人工介入操控的无人驾驶航空器，但不包括微型、轻型无人驾驶航空器。
（五）中型无人驾驶航空器，是指最大起飞重量不超过150千克的无人驾驶航空器，但不包括微型、轻型、小型无人驾驶航空器。
（六）大型无人驾驶航空器，是指最大起飞重量超过150千克的无人驾驶航空器。
```

④ **起落架**: 

​	保证了机身与地面有一定安全距离， 在飞机降落时有一定缓冲，减小起飞降落时气流与地面的干扰 。加大重量的同时也使重心下移， 更有利于飞行时姿态的稳定 。

**同时附上设计准则：**


```
机体的基本设计原则:
(1)刚度、强度满足无人机的负载要求，机体不会发生晃动、弯曲。
(2)在满足其他设计的原则下，质量越轻越好。
(3)合适的长宽高比，各轴间距、结构布局适宜。
(4)飞行过程中，在满足其他设计的原则下，保证机体振动越小越好。
(5)美观耐用。
```

###  2、动力系统：

​	无人机的动力系统包含电机， 电调，正反桨， 电池 。

#### ① 电机: 

​	电机分为有刷无刷两种(以有无碳刷区分 )，一些小型无人机可能会采用无刷电机: 空心杯电机 。而我们现在重点着色于电调驱动的无刷电机 。

##### 电机型号:

​	往往为 4 位数字， 如 2212， 2216 等。这四位数字前后分割为两个两位数字， 前 2 位为转子直径， 后 2 位为转子高度 。																

<img src="..\..\image\6.jpg" alt="动子转子" style="zoom:25%;" />

​	电机越宽，产生扭矩越大。电机越高越窄，响应和速度都更快，更灵敏。

##### 电机KV值:

​	每个无刷电机都会标明电机KV值，这个KV值便是每增加1V电压所增加的每分钟空转转速例如: 我们有1000KV值的电机，电压提高 1 V， 在无桨叶的情况下，转速提升 1000 r/min 。

#### ②电调：

​	驱动电机用的调速器。无刷电机的驱动相对来说较为复杂，需要**单独的MCU**控制三相桥驱动。有刷电机一般用一个功率器件去驱动，通过调节PWM信号的脉冲宽度来控制各个电机的转速。有刷电机的电调通常为一个功率管，通过飞控的PWM输出来驱动。

##### 为什么需要电调?

​	电调的作用就是将飞控板的控制信号转变为电流的大小，以控制电机的转速。因为电机的电流是很大的，通常每个电机正常工作时平均有3A左右的电流，如果没有电调的存在，飞控板I/O口根本无法承受这样大的电流。

​	电调都会标上多少A(如20A、40A)，这个数字就是电调能够提供的电流。大电流的电调可以兼容用在小电流的地方。小电流电调不能超标使用。

```
根据简单测试，常见的新西达2212加1045浆最大电机电流有可能达到5A。为了保险起见，建议这样的配置用30A或40A电调(用20A电调的也比较多)，买大一点以后还可以用到其他地方去。
												——————《四轴飞行器 DIY--基于 STM32 微控制器》
```

##### 什么是四轴专用电调?

​	因为四轴飞行要求电调快速响应，而电调有快速响应和慢速响应的区别，所以需要快速响应的电调。其实大多数常见电调是可以通过编程来设置响应速度的，所以其实并不存在“专用”一说。

​	但是电调可接收的PWM信号频率是有区别的。笔者如再次选型再详细介绍。

##### 什么是电调编程?

​	首先要说明电调是有很多功能模式的，选择不同的功能就需要对电调编程编程的途径可以是直接将电调连接至遥控接收机的油门输出通道(通常是3道)，按照说明书在遥控器上通过扳动摇杆进行设置。这个方法比较麻烦，但成本较低。另外，还可以通过厂家的编程卡来进行设置(需要单独购买)，方法简单，不需接遥控器为了保险起见，一定要将购买的电调设置为一致，否则难以控制。如果电调的启动模式不一样，那么有些都转得很快了，有些还很慢，这就有问题了。

​	*注:通过遥控器进行电调设置，一定要接上电机，因为说明书上说的“滴滴”的声音是通过电机发出来的*

​	具体使用应参照说明书对电调进行解锁等系列操作

#### ③正反桨：

​	为了抵消螺旋桨的自旋，相隔的桨旋转方向是不一样的，所以需要正反桨。正反桨的风都向下吹。适合顺时针旋转的叫正浆、适合逆时针旋转的是反浆。安装的时候，一定记得无论正反桨，有字的一面是向上的（桨叶圆润的一面要和电机旋转方向一致）。
​	可放在电机上旋转测试，观察风向。 

##### 桨的型号含义

​	同电机类似，桨也有啥1045，7040这些4位数字，前面2位代表桨的直径（单位：英寸 1英寸=254毫米）后面2位是桨的角度。

##### 电机与螺旋桨的搭配

​	这是非常复杂的问题，所以建议读者采用常见的配置，这里简单阐述一下其原理。螺旋桨越大，升力就越大，但对应需要更大的力量来驱动;螺旋桨转速越高，升力越大;电机的kV越小，转动力量就越大;综上所述，大螺旋桨就需要用低kV电机，小螺旋桨就需要高kV电机(因为需要用转速来弥补升力的不足)。

​	同样，高kv值也带来更高的操作精度。
​	低kv值带来更高的载重。

​	如果高kV带大桨，力量不够，那么就很困难，实际还是低速运转，电机和电调很容易烧掉。
​	如果低kV带小桨，完全没有问题，但升力不够，可能会造成无法起飞。
​	例如:常用1000kV电机，配10英寸左右的桨。

#### ④电池：

​	常用的无人机动力电池有锂电池、镍电池等类型。重量轻、容量大的电源，动力型锂电池是首选。

##### 为什么要选锂电池?

​	同样的电池容量，锂电池最轻，起飞效率最高。

##### 电池参数含义

​	电池上标注的mAh表示电池容量，如2200mAh电池，如果以2200mA放电，可持续放电1h。如果以4400mA放电，可以持续放电0.5h。

​	电池后面2S、3S、4S代表锂电池的节数。锂电池1节的标准电压为3.7V，2S代表有2个3.7V电池在里面，电压为 7.4V。

​	电池后面多少C代表电池放电能力。这是普通锂电池和动力锂电池最重要的区别，动力锂电池需要很大的电流放电，这个放电能力就是用C来表示的。如2200mAh电池标准为25C，那么用25x2200mAh得出电池可以以55A的电流放电。这很重要，如果用低C的电池，大电流放电会导致电池损坏，甚至自燃。

##### 多少C快充是什么意思?

​	这个与上面的C一样，只是将放电变成了充电。如2200mAh电池，2C快充，代表可以用4400mA的电流来充电。千万不要图快而贸然用大电流，超过规定参数充电，电池很容易损坏。

##### 平衡充电器

​	平衡充电器是指能为串联电池充电的充电器，能使串联的各节电池达到彼此之间的相对平衡。使用平衡充电器，能使锂电池的寿命延长，并能在使用过程中增用电时间。因其在锂电池充电领域的安全性与电压精度的准确性，所以被大量应用与串联锂电池，特别是航模领域。

​	如3s电池，内部是3个锂电池，因为制造工艺原因，没办法保证每个电池完全一致，充电放电特性都有差异，电池串联的情况下，就容易照常某些放电过度或充电过度，充电不饱满等，所以解决办法是分别对内部单节电池充电。动力锂电都有2组线，1组是输出线（2根），1组是单节锂电引出线（与s数有关），充电时按说明书，都插入充电器内，就可以进行平衡充电了。

##### 怎么配电池?

​	这与选择的电机、螺旋桨，想要的飞行时间相关，容量越大，C越高，S越多，电池就越重。基本原则是用大桨，因为整体搭配下来功率高，自身升力大。为了保证飞行时间，可选高容量、高C、3S以上的电池。我们可以参照数据手册得知，电机所需的最大电流，电机可用的电压范围。以此来进行选型。



------

#### 有关动力系统的大讨论：

​	首先我们应确定无人机的整机重量（所有重量，包含所有的外接设备，负载重量，10%-20￥额外配重），整机总重量被每个电机均分。因此我们可以由此获取单电机所需升力（我们的油门以40%为参考）。

​	参照此，参照需求，结合上述设备的特性阐述，我们可以对动力系统的设备进行选型：

比如参照TI电赛的要求：

​	我们需要一台室内运行的无人机，轴距小于450mm，需要较高的操作精度，不需要太大的载重，

------



### 3、控制系统：

​	控制系统包含飞控，遥控器。

#### ①遥控器：

​	主要用于发送各种控制命令。

##### 通道

​	通道就是可以用遥控器控制的动作路数，比如遥控器只能控制四轴上下飞，那么就是1个通道。但四轴在控制过程中需要控制的动作路数有上下、左右、前后、旋转，所以起码得是4通道遥控器。

##### 接收机

​	接收机是用来接收遥控的信号并将信号通过自身的端口输出的模块。有关接收机的详细介绍，将在**二，飞控篇**完成。

##### 什么是日本手、美国手?

​	遥控器上油门的位置在右边的是日本手，在左边的是美国手。遥控器飞行器当中控制供电电流大小。电流大则电机转得快，飞得高，力量大，反之同理，遥控器的油门很简单，遥控器2个摇杆当中上下扳动后不自动回到中间那个就是油门摇杆。

#### ②飞控：

​	飞控为飞行器的核心部件，在此我不展开详细描述，留到飞控篇。
