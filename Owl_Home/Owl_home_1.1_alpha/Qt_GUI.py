import sys,json
from PyQt5.QtWidgets import (
    QApplication, QMainWindow,
    QPushButton,QPlainTextEdit,
    QScrollBar,QWidget,
    QVBoxLayout,QLineEdit,
    QRadioButton,QComboBox,
    QSpinBox,QDoubleSpinBox,
    QCheckBox
)
from PyQt5.QtCore import QTimer, Qt
from PyQt5 import uic
from PyQt5.QtGui import QIcon
import pyqtgraph as pg
import warnings

# 忽略 DeprecationWarning
warnings.filterwarnings("ignore", category=DeprecationWarning)

class Outdatas:
    """外部传输数据"""
    def __init__(self):
        #定义数据
        self.Pirth = 0
        self.Roll = 0
        self.Yaw = 0
        self.AccX = 0
        self.AccY = 0
        self.AccZ = 2048
        self.GyroX = 0
        self.GyroY = 0
        self.GyroZ = 0
        


class MainWindow(QMainWindow):
    """我的窗口"""

    def __init__(self):
        super().__init__()
        # 加载外部文件
        self.values = Outdatas()
        # 运行标志变量
        self.is_running = False
        # 串口接收数据标志
        self.boot_text = ""
        self.boot_data_flag = False
        # 串口发送数据标志变量
        self.if_send_data = False

        self.init_ui()
        self.init_setting()
          
        

    def init_ui(self):
        # 加载UI文件
        self.ui = uic.loadUi("./ui/Owl_1.1.ui")

        """提取要操作的控件"""
        ###数据接收区###
        self.clear_button = self.ui.findChild(QPushButton, "clear")
        self.save_button = self.ui.findChild(QPushButton, "save")
        self.start_button = self.ui.findChild(QPushButton,"start")
        self.boot_data = self.ui.findChild(QPlainTextEdit, "data_get")

        ###数据发送区###
        self.send_button = self.ui.findChild(QPushButton,"send_button")
        self.send_clear_button = self.ui.findChild(QPushButton,"send_clear_button")
        self.send_load_button = self.ui.findChild(QPushButton,"send_load_button")
        self.his_send = self.ui.findChild(QPlainTextEdit, "his_send")
        self.send_lineEdit = self.ui.findChild(QLineEdit, "send_lineEdit")

        ###欧拉角区###
        self.data_Pirth = self.ui.findChild(QLineEdit,"data_Pirth")
        self.data_Roll = self.ui.findChild(QLineEdit,"data_Roll")
        self.data_Yaw = self.ui.findChild(QLineEdit,"data_Yaw")

        ###六轴数据区###
        self.data_AccX = self.ui.findChild(QLineEdit,"data_AccX")
        self.data_AccY = self.ui.findChild(QLineEdit,"data_AccY")
        self.data_AccZ = self.ui.findChild(QLineEdit,"data_AccZ")
        self.data_GyroX = self.ui.findChild(QLineEdit,"data_GyroX")
        self.data_GyroY = self.ui.findChild(QLineEdit,"data_GyroY")
        self.data_GyroZ = self.ui.findChild(QLineEdit,"data_GyroZ")

        ###图表显示区###
        self.scroll_bar = self.ui.findChild(QScrollBar, "horizontalScrollBar")
        self.Roll_radioButton = self.ui.findChild(QRadioButton, "Roll_radioButton")
        self.Pirth_radioButton = self.ui.findChild(QRadioButton,"Pirth_radioButton")
        self.Yaw_radioButton = self.ui.findChild(QRadioButton,"Yaw_radioButton")
        self.AccX_radioButton = self.ui.findChild(QRadioButton,"AccX_radioButton")
        self.AccY_radioButton = self.ui.findChild(QRadioButton,"AccY_radioButton")
        self.AccZ_radioButton = self.ui.findChild(QRadioButton,"AccZ_radioButton")
        self.Gyrox_radioButton = self.ui.findChild(QRadioButton,"GyroX_radioButton")
        self.GyroY_radioButton = self.ui.findChild(QRadioButton,"GyroY_radioButton")
        self.GyroZ_radioButton = self.ui.findChild(QRadioButton,"GyroZ_radioButton")

        ###设置区###
        self.Apply_pushButton = self.ui.findChild(QPushButton,"Apply_pushButton")
        self.default_pushButton = self.ui.findChild(QPushButton,"default_pushButton")
        
        ###串口设置区###
        self.port_comboBox = self.ui.findChild(QComboBox,"port_comboBox")
        self.parity_comboBox = self.ui.findChild(QComboBox,"parity_comboBox")
        self.stopbits_1_radioButton = self.ui.findChild(QRadioButton,"stopbits_1_radioButton")
        self.stopbits_2_radioButton = self.ui.findChild(QRadioButton,"stopbits_2_radioButton")
        self.baudrate_lineEdit = self.ui.findChild(QLineEdit,"baudrate_lineEdit")
        self.bytesize_spinBox = self.ui.findChild(QSpinBox,"bytesize_spinBox")
        self.timeout_doubleSpinBox = self.ui.findChild(QDoubleSpinBox,"timeout_doubleSpinBox")

        ###数据接收设置区###
        self.UTF8_radioButton = self.ui.findChild(QRadioButton,"UTF8_radioButton")
        self.ASCII_radioButton = self.ui.findChild(QRadioButton,"ASCII_radioButton")
        self.GBK_radioButton = self.ui.findChild(QRadioButton,"GBK_radioButton")
        self.HEX_radioButton = self.ui.findChild(QRadioButton,"HEX_radioButton")
        self.INT_radioButton = self.ui.findChild(QRadioButton,"INT_radioButton")
        self.STOPview_checkBox = self.ui.findChild(QCheckBox,"STOPview_checkBox")
        self.Daily_checkBox = self.ui.findChild(QCheckBox,"Daily_checkBox")

        ###PID设置区###
        self.KP_1lineEdit = self.ui.findChild(QLineEdit,"KP_1lineEdit")
        self.KI_1lineEdit = self.ui.findChild(QLineEdit,"KI_1lineEdit")
        self.KD_1lineEdit = self.ui.findChild(QLineEdit,"KD_1lineEdit")

        self.KP_2lineEdit = self.ui.findChild(QLineEdit,"KP_2lineEdit")
        self.KI_2lineEdit = self.ui.findChild(QLineEdit,"KI_2lineEdit")
        self.KD_2lineEdit = self.ui.findChild(QLineEdit,"KD_2lineEdit")
        
        self.KP_3lineEdit = self.ui.findChild(QLineEdit,"KP_3lineEdit")
        self.KI_3lineEdit = self.ui.findChild(QLineEdit,"KI_3lineEdit")
        self.KD_3lineEdit = self.ui.findChild(QLineEdit,"KD_3lineEdit")

        self.KP_4lineEdit = self.ui.findChild(QLineEdit,"KP_4lineEdit")
        self.KI_4lineEdit = self.ui.findChild(QLineEdit,"KI_4lineEdit")
        self.KD_4lineEdit = self.ui.findChild(QLineEdit,"KD_4lineEdit")

        # 查找用于放置 PlotWidget 的 QWidget
        widget = self.ui.findChild(QWidget, "XYView")
        
        # 创建 PlotWidget
        plot_widget = pg.PlotWidget()
            
        # 尝试设置 PlotWidget 为控件的布局
        if isinstance(widget, QWidget):
            # 使用 QVBoxLayout 来容纳 PlotWidget
            layout = QVBoxLayout()
            layout.addWidget(plot_widget)
            widget.setLayout(layout)  # 设置布局
        else:
            widget.setWidget(plot_widget)  # 如果是单个子控件，则直接设置
        
        self.graphics_view = plot_widget

        # 绑定信号与槽函数
        self.clear_button.clicked.connect(self.on_clear_clicked)
        self.save_button.clicked.connect(self.on_save_clicked)
        self.start_button.clicked.connect(self.on_start_clicked)
        self.scroll_bar.valueChanged.connect(self.on_scroll_bar_changed)
        self.send_button.clicked.connect(self.on_send_button_clicked)
        self.send_clear_button.clicked.connect(self.on_send_clear_clicked)
        self.send_load_button.clicked.connect(self.on_send_load_clicked)
        self.Roll_radioButton.clicked.connect(self.on_Roll_Button_clicked)
        self.Pirth_radioButton.clicked.connect(self.on_Pirth_Button_clicked)
        self.Yaw_radioButton.clicked.connect(self.on_Yaw_Button_clicked)
        self.AccX_radioButton.clicked.connect(self.on_AccX_Button_clicked)
        self.AccY_radioButton.clicked.connect(self.on_AccY_Button_clicked)
        self.AccZ_radioButton.clicked.connect(self.on_AccZ_Button_clicked)
        self.Gyrox_radioButton.clicked.connect(self.on_Gyrox_Button_clicked)
        self.GyroY_radioButton.clicked.connect(self.on_GyroY_Button_clicked)
        self.GyroZ_radioButton.clicked.connect(self.on_GyroZ_Button_clicked)
        self.send_lineEdit.returnPressed.connect(self.handle_return_pressed)
        self.Apply_pushButton.clicked.connect(self.on_Apply_pushButton_clicked)
        # self.default_pushButton.clicked.connect(self.default_pushButton_clicked)

        # 输出文字
        self.set_boot_text("以下是收到数据：")
        
        # 图表初始化
        
        self.view_data = []
        self.pen_colors = [
            (255, 0, 0),     # 红色
            (0, 255, 0),     # 绿色
            (0, 0, 255),     # 蓝色
            (255, 255, 0),   # 黄色
            (255, 0, 255),   # 品红
            (128, 0, 0),     # 棕色
            (0, 128, 0),     # 深绿色
            (0, 0, 128),     # 深蓝色
            (128, 128, 0),   # 橄榄绿
            (128, 0, 128),   # 紫色
            (0, 128, 128),   # 青色
            (255, 165, 0),   # 橙色
            (255, 192, 203), # 粉红色
            (139, 69, 19),   # 巧克力色
            (128, 128, 128), # 灰色
            (192, 192, 192), # 浅灰色
            (255, 255, 255), # 白色
            (0, 0, 0)        # 黑色
        ]
        self.init_plot(self.view_data,self.pen_colors)
        
        #设置定时器
        self.timer = QTimer(self)
        
        # 展示窗口
        self.setGeometry(100, 100, 800, 600)  # (x, y, width, height)
        self.ui.show()
    
    def init_setting(self):
        """初始化设置选项"""
        # 加载设置文件
        self.setfile = './setting.json'

        self.setting = self.load_data(self.setfile)
        self.set_port = self.setting["set_port"]  
        self.set_baudrate = self.setting['set_baudrate']
        self.set_bytesize = self.setting['set_bytesize']
        self.set_parity = self.setting['set_parity']
        self.set_stopbits = self.setting['set_stopbits']
        self.set_timeout = self.setting['set_timeout']

        self.add_port_comboBox_item()
        self.set_parity_comboBox_items()
        self.get_set_baudrate_lineEdit()
        self.get_set_stopbits_radioButton()
        self.get_set_bytesize_spinBox()
        self.get_set_timeout_doubleSpinBox()
        self.get_set_decode_mode()
        self.get_set_IF_notview()
        self.get_set_IF_add_daily()
        self.get_set_PIDvalues()
    """
    ############################## strat:通用控件函数 ##################################
    """

    def set_lineda(self,name,text):
        """显示短数据并清除"""
        text = str(text)
        name.clear()
        name.setText(text)

    def load_data(self,The_file):
        """载入数据"""
        try:
            with open(The_file, 'r') as file:
                jsondata = json.load(file)
                return jsondata
        except FileNotFoundError:
            with open(The_file, 'w') as file:
                json.dump({}, file)
            jsondata = {}
            print('Not find')
        except json.JSONDecodeError as e:
            pass
            print(f"JSON 解析错误: {e}")
            jsondata = {}

    def modify_json_file(self, modifications):
        """
        修改JSON文件中的内容。

        :param file_path: JSON文件的路径
        :param modifications: 包含要修改的键值对的字典
        """
        # 步骤1: 读取JSON文件
        with open(self.setfile, 'r', encoding='utf-8') as file:
            data = json.load(file)

        # 步骤2: 修改数据
        for key, value in modifications.items():
            if key in data:
                data[key] = value
            else:
                print(f"警告：键 '{key}' 不存在于JSON文件中,无法修改。")

        # 步骤3: 写回JSON文件
        with open(self.setfile, 'w', encoding='utf-8') as file:
            json.dump(data, file, ensure_ascii=False, indent=4)

    def Refreshdata(self,value):
        """重获取一次数据"""    
        self.values = value
        
    """
    ############################## ending:通用控件函数 #################################
    """    
    
    """
    ############################## strat:图表区控件函数 ##################################
    """   
    def init_plot(self, labels, colors):
        """初始化图表"""
        self.graphics_view.setBackground('w')
        self.x = []
        self.curves = {}
        self.y_data = {label: [] for label in labels}
        
        for label, color in zip(labels, colors):
            pen = pg.mkPen(color=color, width=5)
            self.curves[label] = self.graphics_view.plot(self.x, self.y_data[label], pen=pen, name=label)

        self.graphics_view.setLabel('left', 'Value')
        self.graphics_view.setLabel('bottom', 'Time')
        self.graphics_view.addLegend()

    def on_scroll_bar_changed(self, value):
        """滑块值变化时更新图表显示范围"""
        start = value
        end = value + self.longX/20
        self.graphics_view.setXRange(start, end, padding=0)


    def view_plot(self):
        """在图表上显示数据"""
        self.longX = 3000
        # 更新图表数据
        if self.y_data:
            self.x.append(len(self.x))
            # self.graphics_view.setXRange(0,50, padding=0)
        
            for label in self.y_data.keys():
                self.y_data[label].append(getattr(self.values, label))
                self.curves[label].setData(self.x, self.y_data[label])

        # 更新滑块的最大值
        self.scroll_bar.setMaximum(len(self.x) - 10)
        self.scroll_bar.setValue(len(self.x) - 10)

        # 设置 X 轴和 Y 轴的比例尺
        self.set_scale(x_range=(max(0, len(self.x) - self.longX), len(self.x)))

    def clear_plot(self):
        """清除图表上的所有数据"""
        # 清空 x 轴数据
        self.x.clear()

        # 清空 y 轴数据
        for label in self.y_data.keys():
            self.y_data[label].clear()
            self.curves[label].clear()

        # 重置滑块的最大值和当前值
        self.scroll_bar.setMaximum(0)
        self.scroll_bar.setValue(0)

        # 可选：重新设置图表的标签和图例
        self.graphics_view.clear()  # 清除所有绘图项
        self.graphics_view.setLabel('left', 'Value')
        self.graphics_view.setLabel('bottom', 'Time')
        self.graphics_view.addLegend()
    
    def set_scale(self, x_range=None, y_range=None):
        """设置图表的比例尺"""
        if x_range is not None:
            self.graphics_view.setXRange(*x_range, padding=0)
        if y_range is not None:
            self.graphics_view.setYRange(*y_range, padding=0)
    """
    ############################## enging:图表区控件函数 #################################
    """

    """
    ############################## strat:接收区控件函数 ##################################
    """
    
    def on_clear_clicked(self):
        """事件清除按钮被点击"""
        self.clear_boot_text()
        self.set_lineda(self.data_Pirth,"")
        self.set_lineda(self.data_Roll,"")
        self.set_lineda(self.data_Yaw,"")
        self.x = []
        self.clear_plot()

        self.scroll_bar.setValue(0)
        self.scroll_bar.setMaximum(0)
        self.graphics_view.setXRange(0, self.longX, padding=0)  # 设置 x 轴范围

        
    def on_save_clicked(self):
        """事件保存按钮被点击"""
        self.set_text(self.boot_data,"保存完毕，路径为：")

    def on_start_clicked(self):
        """开始/暂停按钮被点击"""
        if self.is_running:
            # 当前是运行状态，切换为暂停状态
            self.start_button.setText("开始")
            self.timer.stop()
            self.is_running = False
        else:
            # 当前是暂停状态，切换为运行状态
            self.start_button.setText("暂停")
            self.timer.timeout.connect(self.timer_open)
            self.timer.start(20)
            self.clear_plot()
            self.init_plot(self.view_data,self.pen_colors)
            self.is_running = True
    
    def set_boot_text(self, text):
        """原始数据框文字直接设置"""
        # 直接设置文本
        self.boot_data.setPlainText(text)

    def add_boot_text(self, text):
        """原始数据框文字后续添加"""
        # 追加文本
        self.boot_data.appendPlainText(text)

    def clear_boot_text(self):
        """清除原始数据框里数据"""
        self.boot_data.clear()
        

    def put_boot_text(self):
        if self.boot_text:
            if self.is_running:
                self.add_boot_text(self.boot_text)
                self.boot_text = ""
            else:
                 self.boot_text = ""
    
    """
    ############################## ending:接收区控件函数 #################################
    """
    
    """
    ############################## start:发送区控件函数 ##################################
    """
    
    def on_send_button_clicked(self):
        """发送按钮被点击"""
        self.if_send_data = True
        self.send_lineEdit_send()
        

    def on_send_clear_clicked(self):
        """发送区清除按钮被点击"""
        self.his_send.clear()

    def on_send_load_clicked(self):
        """用于加载发送文件数据"""
        self.his_send.setPlainText("已加载")

    def handle_return_pressed(self):
        """如果send_lineEdit回车"""
        self.if_send_data = True
        self.send_lineEdit_send()
        

    def send_lineEdit_send(self):
        """发送输入行"""
        self.his_send.appendPlainText(self.send_lineEdit.text())
        # self.send_lineEdit.clear()
        
        

    """
    ############################## ending:发送区控件函数 #################################
    """

    """
    ############################## start:数显区控件函数 #################################
    """
    def IFclicked(self, radioButton, label):
        """ 
        通用函数：检查 radioButton 是否被选中
        且在可视列表中增加或减少值
        """
        # 检查 radioButton 是否被选中
        if radioButton.isChecked():
            # 如果被选中，从 view_data 中移除 label
            if label not in self.view_data:
                self.view_data.append(label)
        else:
            # 如果未被选中，将 label 添加到 view_data
            if label in self.view_data:
                self.view_data.remove(label)

    def on_Roll_Button_clicked(self):
        """当Roll按钮被点击"""
        self.IFclicked(self.Roll_radioButton,'Roll')
     
    def on_Pirth_Button_clicked(self):
        """当Pirth按钮被点击"""
        self.IFclicked(self.Pirth_radioButton,'Pirth')

    def on_Yaw_Button_clicked(self):
        """当Yaw按钮被点击"""
        self.IFclicked(self.Yaw_radioButton,'Yaw')

    def on_AccX_Button_clicked(self):
        """当AccX按钮被点击"""
        self.IFclicked(self.AccX_radioButton,'AccX')
    
    def on_AccY_Button_clicked(self):
        """当AccY按钮被点击"""
        self.IFclicked(self.AccY_radioButton,'AccY')

    def on_AccZ_Button_clicked(self):
        """当AccZ按钮被点击"""
        self.IFclicked(self.AccZ_radioButton,'AccZ')

    def on_Gyrox_Button_clicked(self):
        """当Gyrox按钮被点击"""
        self.IFclicked(self.Gyrox_radioButton,'Gyrox')

    def on_GyroY_Button_clicked(self):
        """当GyroY按钮被点击"""
        self.IFclicked(self.GyroY_radioButton,'GyroY')

    def on_GyroZ_Button_clicked(self):
        """当GyroZ按钮被点击"""
        self.IFclicked(self.GyroZ_radioButton,'GyroZ')

    def angleview(self):
        """显示角度值"""
        self.set_lineda(self.data_Pirth,self.values.Pirth)
        self.set_lineda(self.data_Roll,self.values.Roll)
        self.set_lineda(self.data_Yaw,self.values.Yaw)
        
    def acveview(self):
        """六轴数据显示"""
        self.set_lineda(self.data_AccX ,self.values.AccX)
        self.set_lineda(self.data_AccY ,self.values.AccY)
        self.set_lineda(self.data_AccZ ,self.values.AccZ)
        self.set_lineda(self.data_GyroX ,self.values.GyroX)
        self.set_lineda(self.data_GyroY ,self.values.GyroY)
        self.set_lineda(self.data_GyroZ ,self.values.GyroZ)

    """
    ############################## ending:数显区控件函数 #################################
    """
    """
    ============================== start:设置区控件函数 ==================================
    """
    def add_port_comboBox_item(self):
        """
        对port_comboBox里添加串口号
        添加第一个默认值还有可选的其他串口
        """
        items = []
        items.append(self.setting['set_port'])
        items = items + self.setting['set_allport']
        self.port_comboBox.addItems(items)

    def set_parity_comboBox_items(self):
        """
        添加已知的parity_comboBox设置,
        并将其设置为显示
        """
        items = ["无","奇","偶"]
        self.parity_comboBox.addItems(items)
        if self.setting['set_parity'] == "serial.PARITY_NONE":
            self.parity_comboBox.setCurrentText("无")
        elif self.setting['set_parity'] == "serial.PARITY_ODD":
            self.parity_comboBox.setCurrentText("奇")
        elif self.setting['set_parity'] == "serial.PARITY_EVEN":
            self.parity_comboBox.setCurrentText("偶")

    def get_set_baudrate_lineEdit(self):
        """
        读取json值
        并将baudrate设置为显示
        """
        self.baudrate_lineEdit.setText(str(self.setting["set_baudrate"]))
    
    def get_set_stopbits_radioButton(self):
        """
        读取json值
        并将stopbits设置为显示
        """
        if self.setting['set_stopbits'] == "serial.STOPBITS_ONE":
            self.stopbits_1_radioButton.setChecked(True)
            self.stopbits_2_radioButton.setChecked(False)
        elif self.setting['set_stopbits'] == "serial.STOPBITS_TWO":
            self.stopbits_1_radioButton.setChecked(False)
            self.stopbits_2_radioButton.setChecked(True)

    def get_set_bytesize_spinBox(self):
        """
        设置最值
        显示bytesize设置值
        """
        self.bytesize_spinBox.setMinimum(5)
        self.bytesize_spinBox.setMaximum(8)
        if self.setting['set_bytesize'] == "serial.EIGHTBITS":
            self.bytesize_spinBox.setValue(8)
        elif self.setting['set_bytesize'] == "serial.SEVENBITS":
             self.bytesize_spinBox.setValue(7)
        elif self.setting['set_bytesize'] == "serial.SIXBITS":
             self.bytesize_spinBox.setValue(6)
        elif self.setting['set_bytesize'] == "serial.FIVEBITS":
             self.bytesize_spinBox.setValue(5)

    def get_set_timeout_doubleSpinBox(self):
        """
        读取显示timeout值
        """
        self.timeout_doubleSpinBox.setValue(self.setting["set_timeout"])

    def get_set_decode_mode(self):
        """
        读取显示选择的解码模式
        """
        if self.setting["set_decode_mode"] == "int":
            self.INT_radioButton.setChecked(True)
        elif self.setting["set_decode_mode"] == "utf-8":
            self.UTF8_radioButton.setChecked(True)
        elif self.setting["set_decode_mode"] == "ascii":
            self.ASCII_radioButton.setChecked(True)
        elif self.setting["set_decode_mode"] == "gbk":
            self.GBK_radioButton.setChecked(True)
        elif self.setting["set_decode_mode"] == "hex":
            self.HEX_radioButton.setChecked(True)

    def get_set_IF_notview(self):
        """
        读取配置STOPview_checkBox是否被点击
        """
        self.STOPview_checkBox.setChecked(self.setting["set_IF_notview"])

    def get_set_IF_add_daily(self):
        """
        读取配置STOPview_checkBox是否被点击
        """
        self.Daily_checkBox.setChecked(self.setting["set_IF_add_daily"])

    def get_set_PIDvalues(self):
        """
        读取配置PID值
        """
        self.KP_1lineEdit.setText(str(self.setting["set_K1"][0]))
        self.KI_1lineEdit.setText(str(self.setting["set_K1"][1]))
        self.KD_1lineEdit.setText(str(self.setting["set_K1"][2]))
        self.KP_2lineEdit.setText(str(self.setting["set_K2"][0]))
        self.KI_2lineEdit.setText(str(self.setting["set_K2"][1]))
        self.KD_2lineEdit.setText(str(self.setting["set_K2"][2]))      
        self.KP_3lineEdit.setText(str(self.setting["set_K3"][0]))
        self.KI_3lineEdit.setText(str(self.setting["set_K3"][1]))
        self.KD_3lineEdit.setText(str(self.setting["set_K3"][2]))
        self.KP_4lineEdit.setText(str(self.setting["set_K4"][0]))
        self.KI_4lineEdit.setText(str(self.setting["set_K4"][1]))
        self.KD_4lineEdit.setText(str(self.setting["set_K4"][2]))

    def get_all_set(self):
        """
        获取窗口中所有设置值

        返回值：包含所有设置值的字典
        """
        settings = {}

        # 获取串口号
        settings['set_port'] = self.port_comboBox.currentText()
        # settings['set_allport'] = [self.port_comboBox.itemText(i) for i in range(self.port_comboBox.count())]

        # 获取校验位
        settings['set_parity'] = {
            "无": "serial.PARITY_NONE",
            "奇": "serial.PARITY_ODD",
            "偶": "serial.PARITY_EVEN"
        }.get(self.parity_comboBox.currentText(), "serial.PARITY_NONE")

        # 获取波特率
        settings['set_baudrate'] = int(self.baudrate_lineEdit.text())

        # 获取停止位
        settings['set_stopbits'] = "serial.STOPBITS_ONE" if self.stopbits_1_radioButton.isChecked() else "serial.STOPBITS_TWO"

        # 获取数据位
        settings['set_bytesize'] = {
            5: "serial.FIVEBITS",
            6: "serial.SIXBITS",
            7: "serial.SEVENBITS",
            8: "serial.EIGHTBITS"
        }.get(self.bytesize_spinBox.value(), "serial.EIGHTBITS")

        # 获取超时时间
        settings['set_timeout'] = self.timeout_doubleSpinBox.value()

        # 获取解码模式
        settings['set_decode_mode'] = {
            True: "int",
            False: "utf-8"
        }.get(self.INT_radioButton.isChecked(), "utf-8") if self.INT_radioButton.isChecked() else \
        ("ascii" if self.ASCII_radioButton.isChecked() else \
        ("gbk" if self.GBK_radioButton.isChecked() else \
        ("hex" if self.HEX_radioButton.isChecked() else "utf-8")))

        # 获取STOPview_checkBox状态
        settings['set_IF_notview'] = self.STOPview_checkBox.isChecked()

        # 获取Daily_checkBox状态
        settings['set_IF_add_daily'] = self.Daily_checkBox.isChecked()

        # 获取PID值
        settings['set_K1'] = [float(self.KP_1lineEdit.text()), float(self.KI_1lineEdit.text()), float(self.KD_1lineEdit.text())]
        settings['set_K2'] = [float(self.KP_2lineEdit.text()), float(self.KI_2lineEdit.text()), float(self.KD_2lineEdit.text())]
        settings['set_K3'] = [float(self.KP_3lineEdit.text()), float(self.KI_3lineEdit.text()), float(self.KD_3lineEdit.text())]
        settings['set_K4'] = [float(self.KP_4lineEdit.text()), float(self.KI_4lineEdit.text()), float(self.KD_4lineEdit.text())]

        return settings

    
    def on_Apply_pushButton_clicked(self):
        """
        当应用按钮被点击时：存储窗口中所选中的设置
        """
        # 获取所有设置值
        settings = self.get_all_set()

        # 将设置值写入JSON文件
        self.modify_json_file(settings)


    """
    ============================== ending:设置区控件函数 ==================================
    """
    def timer_open(self):
        """定时器被启动后执行"""
        self.angleview()
        self.view_plot()
        self.acveview()
        self.put_boot_text()


if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = MainWindow()
    sys.exit(app.exec_())