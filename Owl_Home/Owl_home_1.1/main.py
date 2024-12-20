import sys,threading,serial,json
import serial.tools.list_ports
from Qt_GUI import *

# 全局变量
lock = threading.Lock()
window = None
The_datas = []

#定义串口
ser = serial.Serial()
datas = []  #数据列表


# 数据获取线程
def dataget(datas):
    global window
    global RxState,pRxPacket,Serial_RxFlag,ser
    try:
        once = True # 保证串口在循环中只被打开一次
        Serial_RxFlag = False
        RxState = 0		#定义表示当前状态机状态的静态变量
        pRxPacket = 0	    #定义表示当前接收数据位置的静态变量
        while True:
            with lock:
                if window is not None and window.is_running:
                    if once:        #主窗口定时器开始运行时只打开串口一次
                        try:
                            setting = load_data(window.setfile)
                            ser_init(setting)
                            ser.open()
                            once = False
                        except serial.SerialException as e:
                            window.boot_text = f"串口未连接或打开失败: {e}"
                            continue  # 跳过当前循环的剩余部分，进行下一次循环
                        finally:
                            continue
                    
                    if window.if_settings_changed:
                        """检测标志，如是，更改设置"""
                        setting = load_data(window.setfile)
                        window.if_settings_changed = False

                    if window.if_send_data:
                        """检测标志,如是,则发送send_lineEdit里的数据"""
                        send_text = window.send_lineEdit.text().encode('ascii')
                        # 发送数据
                        ser.write(send_text)
                        window.if_send_data = False

                    data = ser.read()  # 读取数据
                    if data:
                        boot_data = int.from_bytes(data, byteorder='big',signed=True)   # 原始数据化整
                        datas = packetget(boot_data)    # 解包数据包
                        
                        # # 改变接收数据文本
                        # text_data = boot_textprint(data,setting["set_decode_mode"])
                        # window.boot_text += str(text_data) + " "
                        window.boot_text += printText(data,setting["set_decode_mode"])
                        if Serial_RxFlag:   # 判断数据包是否接收完信息
                            window.values.Pirth = datas[0]
                            window.values.GyroY = datas[1]
                            window.values.GyroZ = datas[2]
                            Serial_RxFlag = False
                    

                elif window is not None and not window.is_running:
                    ser.close()
                    once = True

    except KeyboardInterrupt:
        # 捕获用户中断（如按下 Ctrl+C）
        print("程序被手动中断")
    finally:
        # 确保串口被关闭
        if ser.is_open:
            ser.close()
            once = True
            print("串口已关闭")


# 主函数
def mainWindow():
    global window,available_ports
    available_ports = get_available_com_ports()
    key_port = []
    for port in available_ports:     #添加可用串口
        key_port.append(port['device'])
    modify_json_file(returnPATH(),{"set_allport":key_port})

    app = QApplication(sys.argv)
    window = MainWindow()
    sys.exit(app.exec_())

            
def packetget(Rxdata):
    global RxState, pRxPacket, Serial_RxFlag
    # 初始化 The_datas 作为一个全局列表，以便在函数调用之间持久化
    if 'The_datas' not in globals():
        globals()['The_datas'] = []
        
    if RxState == 0:  # 当前状态为0，接收数据包包头
        if Rxdata == -1:  # 如果数据确实是包头
            RxState = 1  # 置下一个状态
            The_datas.clear()  # type: ignore # 清空列表，准备接收新数据包
                
    elif RxState == 1:  # 当前状态为1，接收数据包数据
        The_datas.append(Rxdata)  # 将数据添加到 The_datas 列表中
        if len(The_datas) >= 11:  # 如果已经接收到3个数据字节
            RxState = 2  # 置下一个状态
                
    elif RxState == 2:  # 当前状态为2，接收数据包包尾
        if Rxdata == 0:  # 如果数据确实是包尾
            RxState = 0  # 状态归0
            Serial_RxFlag = True  # 设置接收数据包标志位，表示成功接收到一个数据包
            return The_datas  # 返回接收到的数据包

    return None  # 如果没有完成整个数据包的接收，返回 None        

def boot_textprint(text, mode):
    """
    接收模式，以此模式显示

    :param text: 要处理的字节数据
    :param mode: 显示模式，可选值为 'utf-8', 'ascii', 'gbk', 'hex', 'int'
    :return: 处理后的文本
    """
    if not isinstance(text, bytes):
        return "Error: 输入必须是字节类型"

    if mode in ['utf-8', 'ascii', 'gbk']:  # 字符编码模式
        try:
            text = text.decode(mode)
        except UnicodeDecodeError:
            text = "Error: 解码失败"
    elif mode == 'hex':  # 十六进制原始数据模式
        text = text.hex()  # 使用 hex() 方法转换为十六进制字符串
    elif mode == 'int':  # 整数/数字模式
        text = int.from_bytes(text, byteorder='big',signed= True)
    else:  # 报错
        text = "Error: 不支持的模式"

    return text

def ser_init(modifications):
    """
    初始化串口参数

    :param modifications: 包含串口设置的字典
    """
    global ser
    # 创建Serial对象
    ser = serial.Serial()

    # 设置串口参数
    ser.port = modifications["set_port"]  # 串口设备路径
    ser.baudrate = modifications["set_baudrate"]  # 波特率
    ser.bytesize = {
        "serial.FIVEBITS": serial.FIVEBITS,
        "serial.SIXBITS": serial.SIXBITS,
        "serial.SEVENBITS": serial.SEVENBITS,
        "serial.EIGHTBITS": serial.EIGHTBITS
    }[modifications["set_bytesize"]]  # 数据位长度
    ser.parity = {
        "serial.PARITY_NONE": serial.PARITY_NONE,
        "serial.PARITY_ODD": serial.PARITY_ODD,
        "serial.PARITY_EVEN": serial.PARITY_EVEN
    }[modifications["set_parity"]]  # 校验位
    ser.stopbits = {
        "serial.STOPBITS_ONE": serial.STOPBITS_ONE,
        "serial.STOPBITS_TWO": serial.STOPBITS_TWO
    }[modifications["set_stopbits"]]  # 停止位
    ser.timeout = modifications["set_timeout"]  # 读取超时时间

def get_available_com_ports():
    """
    获取可用的串口信息
    返回一个包含此信息的字典
    """
    # 获取所有可用的串口
    ports = serial.tools.list_ports.comports()
    
    # 存储可用串口的信息
    available_ports = []
    
    for port in ports:
        available_ports.append({
            'device': port.device,
            'name': port.name,
            'description': port.description,
            'hwid': port.hwid,
            'vid': port.vid,
            'pid': port.pid,
            'serial_number': port.serial_number,
            'location': port.location,
            'manufacturer': port.manufacturer,
            'product': port.product,
            'interface': port.interface
        })
    
    return available_ports

def printText(data,mode):
    """
    在相应文本框中参考模式打印文字

    :param data: 打印数据
    :param mode: 显示模式，可选值为 'utf-8', 'ascii', 'gbk', 'hex', 'int'
    :return: 无
    """
    text = boot_textprint(data,mode)
    if mode == 'hex' or mode == 'int':
        ptext = str(text) + " "
    else:
        ptext = str(text)

    return ptext

# 创建并启动数据获取线程
thread_data = threading.Thread(target=dataget, args=(datas,))
thread_data.daemon = True  # 设置为守护线程，确保主线程结束时子线程也结束
thread_data.start()

# 启动主窗口
mainWindow()










        