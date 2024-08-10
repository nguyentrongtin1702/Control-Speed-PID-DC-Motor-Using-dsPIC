import serial
import time
import schedule
import logging
from logging import Formatter
from logging.handlers import RotatingFileHandler
from decimal import Decimal

logger=logging.getLogger('RotatingFileHandler')
logger.setLevel(logging.DEBUG)

handler=RotatingFileHandler('pymod_report.log',maxBytes=10240,backupCount=1)
formatter=Formatter('%(asctime)s-%(levelname)s-%(message)s')
handler.setFormatter(formatter)

logger.addHandler(handler)

# Khởi tạo cổng serial
ser = serial.Serial("COM3", 9600,bytesize=8,parity='N',stopbits=1, timeout = 1)

if ser.isOpen():
    print(f"Đã mở cổng . Bắt đầu nhập và gửi dữ liệu (nhấn Enter để gửi)...")
    
    try:
        while True:
            # Nhập dữ liệu từ máy tính
            data_to_send = input("Nhập dữ liệu: ")
            logger.debug(f'{data_to_send}')
            if data_to_send == '#chaythuan':
                data_to_send_new = '0'
                print(f"DANG CHAY THUAN")
            elif data_to_send == '#chaynghich':
                data_to_send_new = '1'
                print(f"DANG CHAY NGHICH")
            elif data_to_send == '#dung':
                data_to_send_new = '2'
                print(f"DANG DUNG")
            elif data_to_send == 'chaycap1':
                data_to_send_new = '3'
                print(f"DANG CHAY CAP 1")
            elif data_to_send == 'chaycap2':
                data_to_send_new = '4'
                print(f"DANG CHAY CAP 2")
            elif data_to_send == 'chaycap3':
                data_to_send_new = '5'
                print(f"DANG CHAY CAP 3")
            elif data_to_send == 'chaycap4':
                data_to_send_new = '6'
                print(f"DANG CHAY CAP 4")
            elif data_to_send == 'chaycap5':
                data_to_send_new = '7'
                print(f"DANG CHAY CAP 5")
            elif data_to_send == 'chaycap6':
                data_to_send_new = '8'
                print(f"DANG CHAY CAP 6")
            elif data_to_send == 'chaycap7':
                data_to_send_new = '9'
                print(f"DANG CHAY CAP 7")
            elif data_to_send == 'chaycap8':
                data_to_send_new = 'A'
                print(f"DANG CHAY CAP 8")
            elif data_to_send == 'chaycap9':
                data_to_send_new = 'B'
                print("DANG CHAY CAP 9")
            elif data_to_send == 'chaycap10':
                data_to_send_new ='C'
                print("DANG CHAY CAP 10")
            

            # Gửi dữ liệu nếu có dữ liệu nhập vào
            if data_to_send_new:#dung
                ser.write(data_to_send_new.encode('utf-8'))
                #print(f"Đã gửi: {data_to_send_new}")
                
                
            # if data_to_send == "#chaythuan" or data_to_send == "#chaynghich":
            #     start_time = time.time_ns()
            #     if ser.readline() :
            #         end_time = time.time_ns()
            #         print(f"thời gian thực hiện lệnh điều khiển xong là : {(end_time - start_time)/1000} us")
            
            data_read = ser.read()
            print(f"toc do nhan duoc la : {data_read}")
            logger.debug(data_read)
                    

            time.sleep(0.1)

    except KeyboardInterrupt:
        print("\nĐã ngắt nhập dữ liệu.")
        pass
    finally:
        # Đóng cổng serial khi kết thúc
        ser.close()
        print(f"Đã đóng cổng COM3 .")
else:
    print(f"Không thể mở cổng COM3.")