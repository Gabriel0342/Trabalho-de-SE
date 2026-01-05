import os
import sys
import argparse
import time
import cv2
import numpy as np
import serial
import requests

from ultralytics import YOLO

# Define argumentos para iniciar o script
parser = argparse.ArgumentParser()
parser.add_argument('--model', help='Caminho do modelo', required=True)
parser.add_argument('--source', help='Dispositivo (usb0, picamera, http)', required=True)
parser.add_argument('--thresh', help='Certeza (0.01 a 0.99)', default=0.7)
parser.add_argument('--resolution', help='Resolucao (Ex: 1280x720)', default=None)

args = parser.parse_args()

model_path = args.model
source_img = args.source
min_thresh = float(args.thresh)
res_param = args.resolution

ser = None
try:
    ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)
    ser.reset_input_buffer()
    print("Conexão serial estabelecida com sucesso!")
except Exception as e:
    print(f"Aviso: não foi possivel conectar ao serial ({e}).")
    sys.exit(0)

if not os.path.exists(model_path):
    print("Erro: Modelo não encontrado.")
    sys.exit(0)

resize = False
resW, resH = None, None
if res_param:
    try:
        resW, resH = map(int, res_param.split("x"))
        resize = True
    except:
        print("Resolução inválida! Use LARGURAxALTURA.")
        sys.exit(0)

cap = None
source_type = ""

if source_img.startswith("usb"):
    if not source_img[3:].isdigit():
        print("Formato inválido! Use usb0, usb1...")
        sys.exit(0)
    cam_index = int(source_img.replace("usb", ""))
    if os.path.exists(f"/dev/video{cam_index}"):
        print("Camera USB encontrada!")
        source_type = "usb"
        cap = cv2.VideoCapture(cam_index)
        if not cap.isOpened():
            print("Erro ao abrir a câmera USB.")
            sys.exit(0)
        if resize:
            cap.set(cv2.CAP_PROP_FRAME_WIDTH, resW)
            cap.set(cv2.CAP_PROP_FRAME_HEIGHT, resH)
        cap.set(cv2.CAP_PROP_BUFFERSIZE, 1)
        cap.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter_fourcc('M', 'J', 'P', 'G'))
    else:
        print("Camera USB não encontrada!")
        sys.exit(0)

elif source_img.startswith("picamera"):
    from picamera2 import Picamera2
    source_type = "picamera"
    try:
        cap = Picamera2()
        if resize:
            config = cap.create_video_configuration(main={"format": 'RGB888', "size": (resW, resH)})
            cap.configure(config)
        cap.start()
        print("PiCamera encontrada!")
    except:
        print("PiCamera não encontrada!")
        sys.exit(0)

elif source_img.startswith("http"):
    source_type = "stream"
    cap = cv2.VideoCapture(source_img)
    if not cap.isOpened():
        print("Erro ao abrir a câmera HTTP.")
        sys.exit(0)
    cap.set(cv2.CAP_PROP_BUFFERSIZE, 1)
    print("HTTP camera encontrada!")
else:
    print("Fonte inválida.")
    sys.exit(0)

if not (0.0 <= min_thresh <= 1.0):
    print("Erro: thresh deve ser entre 0 e 1.")
    sys.exit(0)

model = YOLO(model_path, task='detect')
labels = model.names

def buscar_preco_api(nome_prod):
    url = f"http://localhost:4000/frutas/get/{nome_prod}"
    try:
        response = requests.get(url, timeout=2)
        if response.status_code == 200:
            dados_json = response.json()
            fruta = dados_json.get('fruta')
            preco = float(dados_json.get('preco', 0.0))
            return fruta, preco
        else:
            print(f"Erro API: {response.status_code}")
            return None, 0.0
    except Exception as e:
        print(f"Erro conexao API: {e}")
        return None, 0.0

print("Sistema Pronto! À espera de peso do ESP32...")

bbox_colors = np.random.uniform(0, 255, size=(len(labels), 3))

avg_frame_rate = 0
frame_rate_buffer = []
fps_avg_len = 200

fruta_ja_processada = False
peso_recebido = 0.0

# LOOP PRINCIPAL
while True:
    t_start = time.perf_counter()
    
    frame = None
    if source_type in ["usb", 'stream']:
        ret, frame = cap.read()
        if frame is None or not ret:
            print("Erro ao ler frame USB.")
            break
    elif source_type == "picamera":
        frame = cap.capture_array()
        if frame is None:
            print("Erro ao ler frame PiCamera.")
            break

    if ser and ser.in_waiting > 0:
        try:
            linha = ser.readline().decode('utf-8', errors='ignore').strip()
            if linha:
                novo_peso = float(linha)
                peso_recebido = novo_peso
                print(f"Peso atualizado: {peso_recebido}")
        except:
            pass
    object_count = 0
    
    if peso_recebido > 0.02:
        # Só corre o YOLO se houver peso
        results = model(frame, imgsz=320, stream=True, verbose=False)
        
        melhor_deteccao = None
        maior_confianca = 0

        for r in results:
            for box in r.boxes:
                conf = box.conf.item()
                classidx = int(box.cls.item())

                if conf > min_thresh:
                    xyxy = box.xyxy.cpu().numpy().squeeze().astype(int)
                    classname = labels[classidx]
                    color = bbox_colors[classidx % len(bbox_colors)]
                    
                    cv2.rectangle(frame, (xyxy[0], xyxy[1]), (xyxy[2], xyxy[3]), color, 2)
                    label_txt = f'{classname} {int(conf*100)}%'
                    
                    labelSize, baseLine = cv2.getTextSize(label_txt, cv2.FONT_HERSHEY_SIMPLEX, 0.5, 1)
                    label_ymin = max(xyxy[1], labelSize[1] + 10)
                    cv2.rectangle(frame, (xyxy[0], label_ymin - labelSize[1] - 14), 
                                  (xyxy[0] + labelSize[0], label_ymin + baseLine - 10), color, cv2.FILLED)
                    cv2.putText(frame, label_txt, (xyxy[0], label_ymin - 7), 
                                cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0), 1)
                    
                    object_count += 1
                    
                    # Guarda a melhor deteccao
                    if conf > maior_confianca:
                        maior_confianca = conf
                        melhor_deteccao = classname


        if not fruta_ja_processada:
            if melhor_deteccao:
                nome, preco = buscar_preco_api(melhor_deteccao)
                if nome and preco > 0:
                    print(f"<-- VENDIDO: {nome} | {preco:.2f}$")
                    ser.write(f"{nome},{preco:.2f}\n".encode('utf-8'))
                    fruta_ja_processada = True
                else:
                    # Nao encontrou na API, mas encontrou visualmente. 
                    # Nao faz nada
		    pass
            else:
                print("Tem peso, mas sem fruta...")
                pass
    
    else:
        fruta_ja_processada = False
        cv2.putText(frame, "AGUARDANDO PESO...", (10, 70), 
                    cv2.FONT_HERSHEY_SIMPLEX,  0.7, (0, 255, 255), 2)

    #Interface e FPS
    cv2.putText(frame, f'FPS: {avg_frame_rate:.1f}', (10, 20), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 255), 2)
    cv2.putText(frame, f'Objs: {object_count}', (10, 45), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 255), 2)

    try:
        cv2.imshow('Smart Balance', frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    except:
        pass

    # Calculo dos FPS
    t_stop = time.perf_counter()
    fr = 1.0 / (t_stop - t_start) if (t_stop - t_start) > 0 else 0
    frame_rate_buffer.append(fr)
    if len(frame_rate_buffer) > fps_avg_len:
        frame_rate_buffer.pop(0)
    avg_frame_rate = np.mean(frame_rate_buffer)

print("A sair...")
if source_img.startswith("usb"):
    cap.release()
elif source_img.startswith("picamera"):
    cap.stop()
if ser:
    ser.close()
cv2.destroyAllWindows()
