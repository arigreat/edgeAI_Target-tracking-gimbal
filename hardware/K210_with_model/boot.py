import sensor
import image
import lcd
import KPU as kpu
from fpioa_manager import fm
from machine import UART
fm.register(9,fm.fpioa.UART1_TX)
fm.register(10,fm.fpioa.UART1_RX)
uart = UART(UART.UART1, 9600, 8, None, 1, timeout=1000, read_buf_len=4096)
lcd.init()
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_windowing((224, 224))
sensor.set_hmirror(0)
sensor.run(1)
model_labels = ['green']
model_anchors = [2.75, 2.72, 1.62, 1.53, 2.47, 2.34, 2.0, 1.91, 4.44, 4.06]
task = kpu.load("/sd/greenlight.kmodel")
anchor=model_anchors
a = kpu.init_yolo2(task, 0.6, 0.3, 5, anchor)
labels = model_labels
while(True):
	img = sensor.snapshot()
	code = kpu.run_yolo2(task, img)
	if code:
		for i in code:
			a = img.draw_rectangle(i.rect(),(0,255,0),2)
			a = lcd.display(img)
			for i in code:
				x = int(i.x())
				y = int(i.y())
				lcd.draw_string(x+45, y-5, labels[i.classid()]+" "+'%.2f'%i.value(), lcd.WHITE,lcd.GREEN)
				hex_data = bytearray([0xFF,y,x,0xFE])
				uart.write(hex_data)
	else:
		a = lcd.display(img)
a = kpu.deinit(task)
